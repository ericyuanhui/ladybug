#pragma once

#include "binder/expression/expression.h"
#include "bound_reading_clause.h"

namespace lbug {
namespace binder {

class BoundTumbleClause final : public BoundReadingClause {
public:
    BoundTumbleClause(std::shared_ptr<Expression> timeExpression, int64_t durationMicros,
        std::shared_ptr<Expression> windowStartExpression,
        std::shared_ptr<Expression> windowEndExpression)
        : BoundReadingClause{common::ClauseType::TUMBLE}, timeExpression{std::move(timeExpression)},
          durationMicros{durationMicros}, windowStartExpression{std::move(windowStartExpression)},
          windowEndExpression{std::move(windowEndExpression)} {}

    std::shared_ptr<Expression> getTimeExpression() const { return timeExpression; }
    int64_t getDurationMicros() const { return durationMicros; }
    std::shared_ptr<Expression> getWindowStartExpression() const { return windowStartExpression; }
    std::shared_ptr<Expression> getWindowEndExpression() const { return windowEndExpression; }

private:
    std::shared_ptr<Expression> timeExpression;
    int64_t durationMicros;
    std::shared_ptr<Expression> windowStartExpression;
    std::shared_ptr<Expression> windowEndExpression;
};

} // namespace binder
} // namespace lbug
