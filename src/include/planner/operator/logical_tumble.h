#pragma once

#include "planner/operator/logical_operator.h"

namespace lbug {
namespace planner {

class LogicalTumble final : public LogicalOperator {
public:
    LogicalTumble(std::shared_ptr<binder::Expression> timeExpression, int64_t durationMicros,
        std::shared_ptr<binder::Expression> windowStartExpression,
        std::shared_ptr<binder::Expression> windowEndExpression,
        std::shared_ptr<LogicalOperator> child)
        : LogicalOperator{LogicalOperatorType::TUMBLE, std::move(child)},
          timeExpression{std::move(timeExpression)}, durationMicros{durationMicros},
          windowStartExpression{std::move(windowStartExpression)},
          windowEndExpression{std::move(windowEndExpression)} {}

    f_group_pos_set getGroupsPosToFlatten();
    void computeFactorizedSchema() override;
    void computeFlatSchema() override;

    std::shared_ptr<binder::Expression> getTimeExpression() const { return timeExpression; }
    int64_t getDurationMicros() const { return durationMicros; }
    std::shared_ptr<binder::Expression> getWindowStartExpression() const {
        return windowStartExpression;
    }
    std::shared_ptr<binder::Expression> getWindowEndExpression() const {
        return windowEndExpression;
    }

    std::string getExpressionsForPrinting() const override {
        return timeExpression->toString() + ", every " + std::to_string(durationMicros) + "us";
    }

    std::unique_ptr<LogicalOperator> copy() override {
        return make_unique<LogicalTumble>(timeExpression, durationMicros, windowStartExpression,
            windowEndExpression, children[0]->copy());
    }

private:
    std::shared_ptr<binder::Expression> timeExpression;
    int64_t durationMicros;
    std::shared_ptr<binder::Expression> windowStartExpression;
    std::shared_ptr<binder::Expression> windowEndExpression;
};

} // namespace planner
} // namespace lbug
