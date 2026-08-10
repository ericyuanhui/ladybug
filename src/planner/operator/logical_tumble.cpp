#include "planner/operator/logical_tumble.h"

#include "planner/operator/factorization/flatten_resolver.h"

namespace lbug {
namespace planner {

f_group_pos_set LogicalTumble::getGroupsPosToFlatten() {
    return FlattenAll::getGroupsPosToFlatten(timeExpression, *children[0]->getSchema());
}

void LogicalTumble::computeFactorizedSchema() {
    copyChildSchema(0);
    auto analyzer = GroupDependencyAnalyzer(false, *children[0]->getSchema());
    analyzer.visit(timeExpression);
    const auto dependentGroups = analyzer.getDependentGroups();
    f_group_pos groupPos;
    if (dependentGroups.empty()) {
        groupPos = schema->createGroup();
        schema->setGroupAsSingleState(groupPos);
    } else {
        SchemaUtils::validateAtMostOneUnFlatGroup(dependentGroups, *children[0]->getSchema());
        groupPos = SchemaUtils::getLeadingGroupPos(dependentGroups, *children[0]->getSchema());
    }
    schema->insertToGroupAndScope(windowStartExpression, groupPos);
    schema->insertToGroupAndScope(windowEndExpression, groupPos);
}

void LogicalTumble::computeFlatSchema() {
    copyChildSchema(0);
    schema->insertToGroupAndScope(windowStartExpression, 0);
    schema->insertToGroupAndScope(windowEndExpression, 0);
}

} // namespace planner
} // namespace lbug
