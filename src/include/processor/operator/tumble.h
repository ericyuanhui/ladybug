#pragma once

#include "binder/expression/expression.h"
#include "expression_evaluator/expression_evaluator.h"
#include "processor/operator/physical_operator.h"

namespace lbug {
namespace processor {

struct TumblePrintInfo final : OPPrintInfo {
    std::shared_ptr<binder::Expression> timeExpression;
    int64_t durationMicros;

    TumblePrintInfo(std::shared_ptr<binder::Expression> timeExpression, int64_t durationMicros)
        : timeExpression{std::move(timeExpression)}, durationMicros{durationMicros} {}

    std::string toString() const override;

    std::unique_ptr<OPPrintInfo> copy() const override {
        return std::unique_ptr<TumblePrintInfo>(new TumblePrintInfo(*this));
    }

private:
    TumblePrintInfo(const TumblePrintInfo& other)
        : OPPrintInfo{other}, timeExpression{other.timeExpression},
          durationMicros{other.durationMicros} {}
};

class Tumble final : public PhysicalOperator {
    static constexpr PhysicalOperatorType type_ = PhysicalOperatorType::TUMBLE;

public:
    Tumble(std::unique_ptr<evaluator::ExpressionEvaluator> timeEvaluator, int64_t durationMicros,
        DataPos windowStartPos, DataPos windowEndPos, std::unique_ptr<PhysicalOperator> child,
        physical_op_id id, std::unique_ptr<OPPrintInfo> printInfo)
        : PhysicalOperator{type_, std::move(child), id, std::move(printInfo)},
          timeEvaluator{std::move(timeEvaluator)}, durationMicros{durationMicros},
          windowStartPos{windowStartPos}, windowEndPos{windowEndPos} {}

    void initLocalStateInternal(ResultSet* resultSet, ExecutionContext* context) override;
    bool getNextTuplesInternal(ExecutionContext* context) override;

    std::unique_ptr<PhysicalOperator> copy() override {
        return make_unique<Tumble>(timeEvaluator->copy(), durationMicros, windowStartPos,
            windowEndPos, children[0]->copy(), id, printInfo->copy());
    }

private:
    int64_t normalizeToMicros(const common::ValueVector& vector, uint32_t pos) const;
    void writeBounds(uint32_t pos, int64_t windowStartMicros, int64_t windowEndMicros) const;

    std::unique_ptr<evaluator::ExpressionEvaluator> timeEvaluator;
    int64_t durationMicros;
    DataPos windowStartPos;
    DataPos windowEndPos;
    std::shared_ptr<common::ValueVector> windowStartVector;
    std::shared_ptr<common::ValueVector> windowEndVector;
};

} // namespace processor
} // namespace lbug
