#include <limits>

#include "binder/binder.h"
#include "binder/expression/literal_expression.h"
#include "binder/query/reading_clause/bound_tumble_clause.h"
#include "common/exception/binder.h"
#include "common/types/interval_t.h"
#include "parser/query/reading_clause/tumble_clause.h"

using namespace lbug::common;
using namespace lbug::parser;

namespace lbug {
namespace binder {

static bool isTumbleTimestampType(const LogicalTypeID typeID) {
    switch (typeID) {
    case LogicalTypeID::TIMESTAMP:
    case LogicalTypeID::TIMESTAMP_SEC:
    case LogicalTypeID::TIMESTAMP_MS:
    case LogicalTypeID::TIMESTAMP_NS:
    case LogicalTypeID::TIMESTAMP_TZ:
        return true;
    default:
        return false;
    }
}

static int64_t bindDurationMicros(const std::shared_ptr<Expression>& intervalExpression) {
    if (intervalExpression->getDataType().getLogicalTypeID() != LogicalTypeID::INTERVAL) {
        throw BinderException("TUMBLE EVERY expression must have data type INTERVAL.");
    }
    if (intervalExpression->expressionType != ExpressionType::LITERAL) {
        throw BinderException("TUMBLE EVERY expression must be a non-null constant INTERVAL.");
    }
    const auto value = intervalExpression->constCast<LiteralExpression>().getValue();
    if (value.isNull()) {
        throw BinderException("TUMBLE EVERY expression must be a non-null constant INTERVAL.");
    }
    const auto interval = value.getValue<interval_t>();
    if (interval.months != 0) {
        throw BinderException("TUMBLE EVERY does not support calendar month or year intervals.");
    }
    const auto duration = static_cast<__int128>(interval.days) * Interval::MICROS_PER_DAY +
                          static_cast<__int128>(interval.micros);
    if (duration <= 0 || duration > std::numeric_limits<int64_t>::max()) {
        throw BinderException("TUMBLE EVERY interval must be a positive fixed duration.");
    }
    return static_cast<int64_t>(duration);
}

static void validateDurationPrecision(LogicalTypeID typeID, int64_t durationMicros) {
    constexpr int64_t MICROS_PER_MILLISECOND = 1000;
    constexpr int64_t MICROS_PER_SECOND = 1000 * MICROS_PER_MILLISECOND;
    if ((typeID == LogicalTypeID::TIMESTAMP_SEC && durationMicros % MICROS_PER_SECOND != 0) ||
        (typeID == LogicalTypeID::TIMESTAMP_MS && durationMicros % MICROS_PER_MILLISECOND != 0)) {
        throw BinderException(
            "TUMBLE EVERY interval must align with the precision of the TUMBLE ON timestamp type.");
    }
}

std::unique_ptr<BoundReadingClause> Binder::bindTumbleClause(const ReadingClause& readingClause) {
    const auto& tumbleClause = readingClause.constCast<TumbleClause>();
    if (scope.empty()) {
        throw BinderException("TUMBLE must follow a reading clause that produces rows.");
    }
    if (tumbleClause.getWindowStartName() == tumbleClause.getWindowEndName() ||
        scope.contains(tumbleClause.getWindowStartName()) ||
        scope.contains(tumbleClause.getWindowEndName())) {
        throw BinderException("TUMBLE YIELD names must be distinct and not already in scope.");
    }
    auto timeExpression = expressionBinder.bindExpression(*tumbleClause.getTimeExpression());
    if (!isTumbleTimestampType(timeExpression->getDataType().getLogicalTypeID())) {
        throw BinderException("TUMBLE ON expression must have a timestamp data type.");
    }
    auto intervalExpression =
        expressionBinder.bindExpression(*tumbleClause.getIntervalExpression());
    const auto durationMicros = bindDurationMicros(intervalExpression);
    validateDurationPrecision(timeExpression->getDataType().getLogicalTypeID(), durationMicros);
    auto windowStartExpression =
        createVariable(tumbleClause.getWindowStartName(), timeExpression->getDataType());
    auto windowEndExpression =
        createVariable(tumbleClause.getWindowEndName(), timeExpression->getDataType());
    return std::make_unique<BoundTumbleClause>(std::move(timeExpression), durationMicros,
        std::move(windowStartExpression), std::move(windowEndExpression));
}

} // namespace binder
} // namespace lbug
