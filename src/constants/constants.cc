// constants.cc
#include "config/config.hh"

#include "constants.hh"

namespace Const {

// Definições de todas as variáveis (sem const)
QString Referee::ip_address;
int Referee::port;
int Referee::packet_size;

float Physics::ball_radius;
float Physics::robot_radius;
float Physics::stop_game_distance;
float Physics::off_field_tolerance;
float Physics::kicking_device_distance;
float Physics::minimum_ball_velocity_to_consider_movement;

int AI::role_swap_hysteresis;
float AI::distance_tolerance;
float AI::angle_tolerance;
float AI::min_velocity_threshold;

namespace Skills {
namespace Kicking {
float max_power;
float optimal_distance;
float enable_distance;
float max_velocity;
float min_velocity;
int enable_duration;
float min_angle_to_shoot_to_goal;
float min_angle_to_pass;
float deflect_angle;
float min_angle_to_deflect;
} // namespace Kicking

namespace Dribbling {
float max_push_distance;
float behind_ball_distance;
} // namespace Dribbling
} // namespace Skills

namespace Control {
namespace Movement {
float max_linear_speed;
float min_linear_speed;
float max_linear_accel;
float max_angular_speed;
float min_angular_speed;
float speed_boost;
} // namespace Movement

namespace PID {
Gains linear;
Gains angular;
} // namespace PID

namespace Timing {
float max_dt;
float replan_interval;
float kick_enabled_timeout;
} // namespace Timing
} // namespace Control

int System::thread_frequency;
int System::invalid_id;
float System::game_off_max_speed;
int System::max_robots;

// Path planner
namespace PathPlanner {
namespace PotentialField {
float katt;
float krep;
float min_rad;
float threshold;
float epsilon;
} // namespace PotentialField

namespace StarPotential {
float threshold;
}

namespace UnivectorField {
float de;
float kr;
float ko;
float dmin;
float delta;
int maxIts;
float step;
} // namespace UnivectorField

namespace PFLorinho {
float ball_repulsive;
float repulsive;
float area_repulsive;
float collision_check_radius;
float robot_collision_radius;
float ball_collision_radius;
float x_shift;
float y_shift;
float factor;
} // namespace PFLorinho
} // namespace PathPlanner

// Função que efetivamente busca todos os valores em Config e atribui
void initialize() {
    // Referee
    Referee::ip_address = Config::retrieve<QString>(Config::CONFIGS["referee"]["ip_address"]);
    Referee::port = Config::retrieve<int>(Config::CONFIGS["referee"]["port"]);
    Referee::packet_size = Config::retrieve<int>(Config::CONFIGS["referee"]["packet_size"]);

    // Physics
    Physics::ball_radius = Config::retrieve<float>(Config::CONFIGS["physics"]["ball_radius"]);
    Physics::robot_radius = Config::retrieve<float>(Config::CONFIGS["physics"]["robot_radius"]);
    Physics::stop_game_distance = Config::retrieve<float>(Config::CONFIGS["physics"]["stop_game_distance"]);
    Physics::off_field_tolerance = Config::retrieve<float>(Config::CONFIGS["physics"]["off_field_tolerance"]);
    Physics::kicking_device_distance = Config::retrieve<float>(Config::CONFIGS["physics"]["kicking_device_distance"]);
    Physics::minimum_ball_velocity_to_consider_movement =
        Config::retrieve<float>(Config::CONFIGS["physics"]["minimum_ball_velocity_to_consider_movement"]);

    // AI
    AI::role_swap_hysteresis = Config::retrieve<int>(Config::CONFIGS["ai"]["role_swap_hysteresis"]);
    AI::distance_tolerance = Config::retrieve<float>(Config::CONFIGS["ai"]["distance_tolerance"]);
    AI::angle_tolerance = Config::retrieve<float>(Config::CONFIGS["ai"]["angle_tolerance"]);
    AI::min_velocity_threshold = Config::retrieve<float>(Config::CONFIGS["ai"]["min_velocity_threshold"]);

    // Skills::Kicking
    Skills::Kicking::max_power = Config::retrieve<float>(Config::CONFIGS["skills"]["kicking"]["max_power"]);
    Skills::Kicking::optimal_distance =
        Config::retrieve<float>(Config::CONFIGS["skills"]["kicking"]["optimal_distance"]);
    Skills::Kicking::enable_distance = Config::retrieve<float>(Config::CONFIGS["skills"]["kicking"]["enable_distance"]);
    Skills::Kicking::max_velocity = Config::retrieve<float>(Config::CONFIGS["skills"]["kicking"]["max_velocity"]);
    Skills::Kicking::min_velocity = Config::retrieve<float>(Config::CONFIGS["skills"]["kicking"]["min_velocity"]);
    Skills::Kicking::enable_duration = Config::retrieve<int>(Config::CONFIGS["skills"]["kicking"]["enable_duration"]);
    Skills::Kicking::min_angle_to_shoot_to_goal =
        Config::retrieve<float>(Config::CONFIGS["skills"]["kicking"]["min_angle_to_shoot_to_goal"]);
    Skills::Kicking::min_angle_to_pass =
        Config::retrieve<float>(Config::CONFIGS["skills"]["kicking"]["min_angle_to_pass"]);
    Skills::Kicking::deflect_angle =
        Config::retrieve<float>(Config::CONFIGS["skills"]["kicking"]["deflect_angle"]);
    Skills::Kicking::min_angle_to_deflect =
        Config::retrieve<float>(Config::CONFIGS["skills"]["kicking"]["min_angle_to_deflect"]);

    // Skills::Dribbling
    Skills::Dribbling::max_push_distance =
        Config::retrieve<float>(Config::CONFIGS["skills"]["dribbling"]["max_push_distance"]);
    Skills::Dribbling::behind_ball_distance =
        Config::retrieve<float>(Config::CONFIGS["skills"]["dribbling"]["behind_ball_distance"]);

    // Control::Movement
    Control::Movement::max_linear_speed =
        Config::retrieve<float>(Config::CONFIGS["control"]["movement"]["max_linear_speed"]);
    Control::Movement::min_linear_speed =
        Config::retrieve<float>(Config::CONFIGS["control"]["movement"]["min_linear_speed"]);
    Control::Movement::max_linear_accel =
        Config::retrieve<float>(Config::CONFIGS["control"]["movement"]["max_linear_accel"]);
    Control::Movement::max_angular_speed =
        Config::retrieve<float>(Config::CONFIGS["control"]["movement"]["max_angular_speed"]);
    Control::Movement::min_angular_speed =
        Config::retrieve<float>(Config::CONFIGS["control"]["movement"]["min_angular_speed"]);
    Control::Movement::speed_boost =
        Config::retrieve<float>(Config::CONFIGS["control"]["movement"]["speed_boost"]);

    // Control::PID
    Control::PID::linear.kp = Config::retrieve<float>(Config::CONFIGS["control"]["pid"]["linear"]["kp"]);
    Control::PID::linear.ki = Config::retrieve<float>(Config::CONFIGS["control"]["pid"]["linear"]["ki"]);
    Control::PID::linear.kd = Config::retrieve<float>(Config::CONFIGS["control"]["pid"]["linear"]["kd"]);
    Control::PID::linear.max_integral =
        Config::retrieve<float>(Config::CONFIGS["control"]["pid"]["linear"]["max_integral"]);
    Control::PID::angular.kp = Config::retrieve<float>(Config::CONFIGS["control"]["pid"]["angular"]["kp"]);
    Control::PID::angular.ki = Config::retrieve<float>(Config::CONFIGS["control"]["pid"]["angular"]["ki"]);
    Control::PID::angular.kd = Config::retrieve<float>(Config::CONFIGS["control"]["pid"]["angular"]["kd"]);
    Control::PID::angular.max_integral =
        Config::retrieve<float>(Config::CONFIGS["control"]["pid"]["angular"]["max_integral"]);

    // Control::Timing
    Control::Timing::max_dt = Config::retrieve<float>(Config::CONFIGS["control"]["timing"]["max_dt"]);
    Control::Timing::replan_interval = Config::retrieve<float>(Config::CONFIGS["control"]["timing"]["replan_interval"]);
    Control::Timing::kick_enabled_timeout =
        Config::retrieve<float>(Config::CONFIGS["control"]["timing"]["kick_enabled_timeout"]);

    // System
    System::thread_frequency = Config::retrieve<int>(Config::CONFIGS["system"]["thread_frequency"]);
    System::invalid_id = Config::retrieve<int>(Config::CONFIGS["system"]["invalid_id"]);
    System::game_off_max_speed = Config::retrieve<float>(Config::CONFIGS["system"]["game_off_max_speed"]);
    System::max_robots = Config::retrieve<int>(Config::CONFIGS["system"]["max_robots"]);

    // Path planner: potential field
    PathPlanner::PotentialField::katt =
        Config::retrieve<float>(Config::CONFIGS["pathplanner"]["potentialfield"]["katt"]);
    PathPlanner::PotentialField::krep =
        Config::retrieve<float>(Config::CONFIGS["pathplanner"]["potentialfield"]["krep"]);
    PathPlanner::PotentialField::min_rad =
        Config::retrieve<float>(Config::CONFIGS["pathplanner"]["potentialfield"]["min_rad"]);
    PathPlanner::PotentialField::threshold =
        Config::retrieve<float>(Config::CONFIGS["pathplanner"]["potentialfield"]["threshold"]);
    PathPlanner::PotentialField::epsilon =
        Config::retrieve<float>(Config::CONFIGS["pathplanner"]["potentialfield"]["epsilon"]);

    // Path planner: star potential
    PathPlanner::StarPotential::threshold =
        Config::retrieve<float>(Config::CONFIGS["pathplanner"]["starpotential"]["threshold"]);

    // Path planner: univector field
    PathPlanner::UnivectorField::de = Config::retrieve<float>(Config::CONFIGS["pathplanner"]["univectorfield"]["de"]);
    PathPlanner::UnivectorField::kr = Config::retrieve<float>(Config::CONFIGS["pathplanner"]["univectorfield"]["kr"]);
    PathPlanner::UnivectorField::ko = Config::retrieve<float>(Config::CONFIGS["pathplanner"]["univectorfield"]["ko"]);
    PathPlanner::UnivectorField::dmin =
        Config::retrieve<float>(Config::CONFIGS["pathplanner"]["univectorfield"]["dmin"]);
    PathPlanner::UnivectorField::delta =
        Config::retrieve<float>(Config::CONFIGS["pathplanner"]["univectorfield"]["delta"]);
    PathPlanner::UnivectorField::maxIts =
        Config::retrieve<int>(Config::CONFIGS["pathplanner"]["univectorfield"]["maxIts"]);
    PathPlanner::UnivectorField::step =
        Config::retrieve<float>(Config::CONFIGS["pathplanner"]["univectorfield"]["step"]);

    // Path planner: PFLorinho
    PathPlanner::PFLorinho::ball_repulsive =
        Config::retrieve<float>(Config::CONFIGS["pathplanner"]["pflorinho"]["ball_repulsive"]);
    PathPlanner::PFLorinho::repulsive =
        Config::retrieve<float>(Config::CONFIGS["pathplanner"]["pflorinho"]["repulsive"]);
    PathPlanner::PFLorinho::area_repulsive =
        Config::retrieve<float>(Config::CONFIGS["pathplanner"]["pflorinho"]["area_repulsive"]);
    PathPlanner::PFLorinho::collision_check_radius =
        Config::retrieve<float>(Config::CONFIGS["pathplanner"]["pflorinho"]["collision_check_radius"]);
    PathPlanner::PFLorinho::robot_collision_radius =
        Config::retrieve<float>(Config::CONFIGS["pathplanner"]["pflorinho"]["robot_collision_radius"]);
    PathPlanner::PFLorinho::ball_collision_radius =
        Config::retrieve<float>(Config::CONFIGS["pathplanner"]["pflorinho"]["ball_collision_radius"]);
    PathPlanner::PFLorinho::x_shift = Config::retrieve<float>(Config::CONFIGS["pathplanner"]["pflorinho"]["x_shift"]);
    PathPlanner::PFLorinho::y_shift = Config::retrieve<float>(Config::CONFIGS["pathplanner"]["pflorinho"]["y_shift"]);
    PathPlanner::PFLorinho::factor = Config::retrieve<float>(Config::CONFIGS["pathplanner"]["pflorinho"]["factor"]);
    // Path planner: PFLorinho
}

} // namespace Const
