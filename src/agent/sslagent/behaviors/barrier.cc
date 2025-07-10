#include "barrier.hh"
#include "agent/sslagent/actions/actions.hh"
#include "algorithm/behaviortree/behaviortree.hh"
#include "constants/constants.hh"
#include "algorithm/pathplanner/pflorinho/pflorinho.hh"

#define POSITION_OFFSET Const::Physics::robot_radius * 1.5f 

Q_LOGGING_CATEGORY(BEHAVIOR_BARRIER, "BEHAVIOR_BARRIER")

/* TODOs:
    - Ajustar distância entre left e right barrier
    - Barreira precisa chutar bola quando está ao alcance dela
    - Abrir espaço quando bola está na nossa área de defesa
    - Adaptar posicionamento da barreira de acordo com posição do nosso goleiro?
*/
BarrierMiddle::BarrierMiddle(const PlayerID &playerId,
               SSLController &controller,
               const World &worldRef)
  : SSLBehavior(playerId, controller, worldRef, "BarrierMiddle")
{
    auto root = BehaviorTree::Sequence(
        "GoTo",
        {
            action<GoToLookAt>()
                ->setGoal([this]() {
                    return block().getBarrierPosition(0.0f);
                })
                ->setLookAt([this]() {
                    return world().ballPositionVec2();
                })
                ->setPathPlanner(new PFLorinho())
        }
    );
    setRoot(root);
}

BarrierLeft::BarrierLeft(const PlayerID &playerId,
               SSLController &controller,
               const World &worldRef)
  : SSLBehavior(playerId, controller, worldRef, "BarrierLeft")
{
    auto root = BehaviorTree::Sequence(
        "GoTo",
        {
            action<GoToLookAt>()
                ->setGoal([this]() {
                    return block().getBarrierPosition(POSITION_OFFSET);
                })
                ->setLookAt([this]() {
                    return world().ballPositionVec2();
                })
                ->setPathPlanner(new PFLorinho())
                ->avoidTeammates()
                ->avoidOurGoal()
        }
    );
    setRoot(root);
}

BarrierRight::BarrierRight(const PlayerID &playerId,
               SSLController &controller,
               const World &worldRef)
  : SSLBehavior(playerId, controller, worldRef, "BarrierRight")
{
    auto root = BehaviorTree::Sequence(
        "GoTo",
        {
            action<GoToLookAt>()
                ->setGoal([this]() {
                    return block().getBarrierPosition(-POSITION_OFFSET);
                })
                ->setLookAt([this]() {
                    return world().ballPositionVec2();
                })
                ->setPathPlanner(new PFLorinho())
        }
    );
    setRoot(root);
}