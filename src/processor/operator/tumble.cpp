#include "processor/operator/tumble.h"

#include <limits>

#include "common/exception/overflow.h"
#include "common/types/interval_t.h"
#include "common/types/timestamp_t.h"
#include "processor/execution_context.h"

using namespace lbug::common;

namespace lbug {
namespace processor {

namespace {

int64_t floorWindowStart(int64_t value, int64_t duration) {
    auto quotient = value / duration;
    if (value % duration < 0) {
        --quotient;
    }
    const auto result = static_cast<__int128>(quotient) * duration;
    if (result < std::numeric_limits<int64_t>::min() ||
        result > std::numeric_limits<int64_t>::max()) {
        throw OverflowException("TUMBLE window start is out of TIMESTAMP range.");
    }
    return static_cast<int64_t>(result);
}

int64_t checkedWindowEnd(int64_t start, int64_t duration) {
    const auto result = static_cast<__int128>(start) + duration;
    if (result > std::numeric_limits<int64_t>::max()) {
        throw OverflowException("TUMBLE window end is out of TIMESTAMP range.");
    }
    return static_cast<int64_t>(result);
}

int64_t floorNanoSecondsToMicros(int64_t nanoseconds) {
    auto micros = nanoseconds / Interval::NANOS_PER_MICRO;
    if (nanoseconds % Interval::NANOS_PER_MICRO < 0) {
        --micros;
    }
    return micros;
}

} // namespace

std::string TumblePrintInfo::toString() const {
    return "Time: " + timeExpression->toString() + ", Every: " + std::to_string(durationMicros) +
           "us";
}

void Tumble::initLocalStateInternal(ResultSet* resultSet, ExecutionContext* context) {
    timeEvaluator->init(*resultSet, context->clientContext);
    windowStartVector = resultSet->getValueVector(windowStartPos);
    windowEndVector = resultSet->getValueVector(windowEndPos);
}

int64_t Tumble::normalizeToMicros(const ValueVector& vector, uint32_t pos) const {
    switch (vector.dataType.getLogicalTypeID()) {
    case LogicalTypeID::TIMESTAMP:
        return vector.getValue<timestamp_t>(pos).value;
    case LogicalTypeID::TIMESTAMP_SEC:
        return Timestamp::fromEpochSeconds(vector.getValue<timestamp_sec_t>(pos).value).value;
    case LogicalTypeID::TIMESTAMP_MS:
        return Timestamp::fromEpochMilliSeconds(vector.getValue<timestamp_ms_t>(pos).value).value;
    case LogicalTypeID::TIMESTAMP_NS:
        return floorNanoSecondsToMicros(vector.getValue<timestamp_ns_t>(pos).value);
    case LogicalTypeID::TIMESTAMP_TZ:
        return vector.getValue<timestamp_tz_t>(pos).value;
    default:
        throw OverflowException("TUMBLE received an unsupported timestamp type.");
    }
}

void Tumble::writeBounds(uint32_t pos, int64_t windowStartMicros, int64_t windowEndMicros) const {
    const auto typeID = windowStartVector->dataType.getLogicalTypeID();
    windowStartVector->setNull(pos, false);
    windowEndVector->setNull(pos, false);
    switch (typeID) {
    case LogicalTypeID::TIMESTAMP:
        windowStartVector->setValue(pos, timestamp_t{windowStartMicros});
        windowEndVector->setValue(pos, timestamp_t{windowEndMicros});
        return;
    case LogicalTypeID::TIMESTAMP_SEC:
        windowStartVector->setValue(pos,
            timestamp_sec_t{Timestamp::getEpochSeconds(timestamp_t{windowStartMicros})});
        windowEndVector->setValue(pos,
            timestamp_sec_t{Timestamp::getEpochSeconds(timestamp_t{windowEndMicros})});
        return;
    case LogicalTypeID::TIMESTAMP_MS:
        windowStartVector->setValue(pos,
            timestamp_ms_t{Timestamp::getEpochMilliSeconds(timestamp_t{windowStartMicros})});
        windowEndVector->setValue(pos,
            timestamp_ms_t{Timestamp::getEpochMilliSeconds(timestamp_t{windowEndMicros})});
        return;
    case LogicalTypeID::TIMESTAMP_NS:
        windowStartVector->setValue(pos,
            timestamp_ns_t{Timestamp::getEpochNanoSeconds(timestamp_t{windowStartMicros})});
        windowEndVector->setValue(pos,
            timestamp_ns_t{Timestamp::getEpochNanoSeconds(timestamp_t{windowEndMicros})});
        return;
    case LogicalTypeID::TIMESTAMP_TZ:
        windowStartVector->setValue(pos, timestamp_tz_t{windowStartMicros});
        windowEndVector->setValue(pos, timestamp_tz_t{windowEndMicros});
        return;
    default:
        throw OverflowException("TUMBLE produced an unsupported timestamp type.");
    }
}

bool Tumble::getNextTuplesInternal(ExecutionContext* context) {
    if (!children[0]->getNextTuple(context)) {
        return false;
    }
    timeEvaluator->evaluate();
    const auto& timeVector = *timeEvaluator->resultVector;
    const auto& selVector = timeVector.state->getSelVector();
    for (auto i = 0u; i < selVector.getSelSize(); ++i) {
        const auto pos = selVector[i];
        if (timeVector.isNull(pos)) {
            windowStartVector->setNull(pos, true);
            windowEndVector->setNull(pos, true);
            continue;
        }
        const auto windowStart =
            floorWindowStart(normalizeToMicros(timeVector, pos), durationMicros);
        writeBounds(pos, windowStart, checkedWindowEnd(windowStart, durationMicros));
    }
    metrics->numOutputTuple.increase(selVector.getSelSize());
    return true;
}

} // namespace processor
} // namespace lbug
