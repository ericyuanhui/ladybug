#pragma once

#include "parser/expression/parsed_expression.h"
#include "reading_clause.h"

namespace lbug {
namespace parser {

class TumbleClause final : public ReadingClause {
    static constexpr common::ClauseType clauseType_ = common::ClauseType::TUMBLE;

public:
    TumbleClause(std::unique_ptr<ParsedExpression> timeExpression,
        std::unique_ptr<ParsedExpression> intervalExpression, std::string windowStartName,
        std::string windowEndName)
        : ReadingClause{clauseType_}, timeExpression{std::move(timeExpression)},
          intervalExpression{std::move(intervalExpression)},
          windowStartName{std::move(windowStartName)}, windowEndName{std::move(windowEndName)} {}

    const ParsedExpression* getTimeExpression() const { return timeExpression.get(); }
    const ParsedExpression* getIntervalExpression() const { return intervalExpression.get(); }
    const std::string& getWindowStartName() const { return windowStartName; }
    const std::string& getWindowEndName() const { return windowEndName; }

private:
    std::unique_ptr<ParsedExpression> timeExpression;
    std::unique_ptr<ParsedExpression> intervalExpression;
    std::string windowStartName;
    std::string windowEndName;
};

} // namespace parser
} // namespace lbug
