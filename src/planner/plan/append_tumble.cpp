#include "binder/query/reading_clause/bound_tumble_clause.h"
#include "planner/operator/logical_tumble.h"
#include "planner/planner.h"

using namespace lbug::binder;
using namespace lbug::common;

namespace lbug {
namespace planner {

void Planner::appendTumble(const BoundReadingClause& readingClause, LogicalPlan& plan) {
    const auto& tumbleClause = readingClause.constCast<BoundTumbleClause>();
    auto tumble = make_shared<LogicalTumble>(tumbleClause.getTimeExpression(),
        tumbleClause.getDurationMicros(), tumbleClause.getWindowStartExpression(),
        tumbleClause.getWindowEndExpression(), plan.getLastOperator());
    appendFlattens(tumble->getGroupsPosToFlatten(), plan);
    tumble->setChild(0, plan.getLastOperator());
    tumble->computeFactorizedSchema();
    plan.setLastOperator(std::move(tumble));
}

} // namespace planner
} // namespace lbug
