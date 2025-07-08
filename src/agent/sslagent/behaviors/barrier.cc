#include "barrier.hh"
#include "agent/sslagent/actions/actions.hh"
#include "algorithm/behaviortree/behaviortree.hh"
#include "constants/constants.hh"
#include "algorithm/pathplanner/pflorinho/pflorinho.hh"

Q_LOGGING_CATEGORY(BEHAVIOR_BARRIER, "BEHAVIOR_BARRIER")

Barrier::Barrier(const PlayerID &playerId,
               SSLController &controller,
               const World &worldRef)
  : SSLBehavior(playerId, controller, worldRef, "Barrier")
{
    auto goToPosition = BehaviorTree::Sequence(
        "GoTo",
        {
            action<GoToLookAt>()
                ->setGoal([this]() {
                    return block().getBarrierPosition();
                })
                ->setLookAt([this]() {
                    return world().ballPositionVec2();
                })
                ->setPathPlanner(new PFLorinho())
        }
    );
}