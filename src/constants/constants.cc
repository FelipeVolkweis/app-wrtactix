#include "config/config.hh"

#include "constants.hh"

namespace Const {
// Referee
const QString Referee::ip_address = Config::retrieve<QString>(Config::CONFIGS["referee"]["ip_address"]);
const int Referee::port = Config::retrieve<int>(Config::CONFIGS["referee"]["port"]);
const int Referee::packet_size = Config::retrieve<int>(Config::CONFIGS["referee"]["packet_size"]);

// Physics
const float Physics::ball_radius = Config::retrieve<float>(Config::CONFIGS["physics"]["ball_radius"]);
const float Physics::robot_radius = Config::retrieve<float>(Config::CONFIGS["physics"]["robot_radius"]);
const float Physics::stop_game_distance = Config::retrieve<float>(Config::CONFIGS["physics"]["stop_game_distance"]);
const float Physics::off_field_tolerance = Config::retrieve<float>(Config::CONFIGS["physics"]["off_field_tolerance"]);

// AI
const int AI::max_roles = Config::retrieve<int>(Config::CONFIGS["ai"]["max_roles"]);
const int AI::role_swap_hysteresis = Config::retrieve<int>(Config::CONFIGS["ai"]["role_swap_hysteresis"]);
const float AI::distance_tolerance = Config::retrieve<float>(Config::CONFIGS["ai"]["distance_tolerance"]);
const float AI::angle_tolerance = Config::retrieve<float>(Config::CONFIGS["ai"]["angle_tolerance"]);
const float AI::min_velocity_threshold = Config::retrieve<float>(Config::CONFIGS["ai"]["min_velocity_threshold"]);

// Skills
namespace Skills {
namespace Kicking {
const float max_power = Config::retrieve<float>(Config::CONFIGS["skills"]["kicking"]["max_power"]);
const float optimal_distance = Config::retrieve<float>(Config::CONFIGS["skills"]["kicking"]["optimal_distance"]);
const float enable_distance = Config::retrieve<float>(Config::CONFIGS["skills"]["kicking"]["enable_distance"]);
const float max_velocity = Config::retrieve<float>(Config::CONFIGS["skills"]["kicking"]["max_velocity"]);
const float min_velocity = Config::retrieve<float>(Config::CONFIGS["skills"]["kicking"]["min_velocity"]);
const int enable_duration = Config::retrieve<int>(Config::CONFIGS["skills"]["kicking"]["enable_duration"]);
} // namespace Kicking

namespace Dribbling {
const float max_push_distance = Config::retrieve<float>(Config::CONFIGS["skills"]["dribbling"]["max_push_distance"]);
const float barrier_distance = Config::retrieve<float>(Config::CONFIGS["skills"]["dribbling"]["barrier_distance"]);
const float behind_ball_distance =
    Config::retrieve<float>(Config::CONFIGS["skills"]["dribbling"]["behind_ball_distance"]);
const float minkowski_distance = Config::retrieve<float>(Config::CONFIGS["skills"]["dribbling"]["minkowski_distance"]);
} // namespace Dribbling
} // namespace Skills

// Control
namespace Control {
namespace Movement {
const float max_linear_speed = Config::retrieve<float>(Config::CONFIGS["control"]["movement"]["max_linear_speed"]);
const float min_linear_speed = Config::retrieve<float>(Config::CONFIGS["control"]["movement"]["min_linear_speed"]);
const float max_linear_accel = Config::retrieve<float>(Config::CONFIGS["control"]["movement"]["max_linear_accel"]);
const float max_angular_speed = Config::retrieve<float>(Config::CONFIGS["control"]["movement"]["max_angular_speed"]);
const float min_angular_speed = Config::retrieve<float>(Config::CONFIGS["control"]["movement"]["min_angular_speed"]);
const float linear_error = Config::retrieve<float>(Config::CONFIGS["control"]["movement"]["linear_error"]);
const float angular_error = Config::retrieve<float>(Config::CONFIGS["control"]["movement"]["angular_error"]);
} // namespace Movement

namespace PID {
const Gains linear = {.kp = Config::retrieve<float>(Config::CONFIGS["control"]["pid"]["linear"]["kp"]),
                      .ki = Config::retrieve<float>(Config::CONFIGS["control"]["pid"]["linear"]["ki"]),
                      .kd = Config::retrieve<float>(Config::CONFIGS["control"]["pid"]["linear"]["kd"]),
                      .max_integral =
                          Config::retrieve<float>(Config::CONFIGS["control"]["pid"]["linear"]["max_integral"])};

const Gains angular = {.kp = Config::retrieve<float>(Config::CONFIGS["control"]["pid"]["angular"]["kp"]),
                       .ki = Config::retrieve<float>(Config::CONFIGS["control"]["pid"]["angular"]["ki"]),
                       .kd = Config::retrieve<float>(Config::CONFIGS["control"]["pid"]["angular"]["kd"]),
                       .max_integral =
                           Config::retrieve<float>(Config::CONFIGS["control"]["pid"]["angular"]["max_integral"])};
} // namespace PID

namespace Timing {
const float max_dt = Config::retrieve<float>(Config::CONFIGS["control"]["timing"]["max_dt"]);
const float replan_interval = Config::retrieve<float>(Config::CONFIGS["control"]["timing"]["replan_interval"]);
} // namespace Timing
} // namespace Control

// System
const int System::thread_frequency = Config::retrieve<int>(Config::CONFIGS["system"]["thread_frequency"]);
const int System::invalid_id = Config::retrieve<int>(Config::CONFIGS["system"]["invalid_id"]);
const float System::game_off_max_speed = Config::retrieve<float>(Config::CONFIGS["system"]["game_off_max_speed"]);
const int System::max_robots = Config::retrieve<int>(Config::CONFIGS["system"]["max_robots"]);
const float System::bezier_control_point = Config::retrieve<float>(Config::CONFIGS["system"]["bezier_control_point"]);
} // namespace Const