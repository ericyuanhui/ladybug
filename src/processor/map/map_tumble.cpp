#include "planner/operator/logical_tumble.h"
#include "processor/expression_mapper.h"
#include "processor/operator/tumble.h"
#include "processor/plan_mapper.h"

using namespace lbug::planner;

namespace lbug {
namespace processor {

std::unique_ptr<PhysicalOperator> PlanMapper::mapTumble(const LogicalOperator* logicalOperator) {
    const auto& tumble = logicalOperator->constCast<LogicalTumble>();
    const auto outSchema = tumble.getSchema();
    const auto inSchema = tumble.getChild(0)->getSchema();
    auto child = mapOperator(tumble.getChild(0).get());
    auto expressionMapper = ExpressionMapper(inSchema);
    auto evaluator = expressionMapper.getEvaluator(tumble.getTimeExpression());
    auto printInfo =
        std::make_unique<TumblePrintInfo>(tumble.getTimeExpression(), tumble.getDurationMicros());
    return std::make_unique<Tumble>(std::move(evaluator), tumble.getDurationMicros(),
        getDataPos(*tumble.getWindowStartExpression(), *outSchema),
        getDataPos(*tumble.getWindowEndExpression(), *outSchema), std::move(child), getOperatorID(),
        std::move(printInfo));
}

} // namespace processor
} // namespace lbug
