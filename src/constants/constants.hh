#ifndef CONSTANTS_HH
#define CONSTANTS_HH

#include <QString>

namespace Const {
// Referee communication
namespace Referee {
extern const QString ip_address;
extern const int port;
extern const int packet_size;
} // namespace Referee

// Physical properties
namespace Physics {
extern const float ball_radius;
extern const float robot_radius;
extern const float stop_game_distance;
extern const float off_field_tolerance;
} // namespace Physics

// AI configuration
namespace AI {
extern const int max_roles;
extern const int role_swap_hysteresis;
extern const float distance_tolerance;
extern const float angle_tolerance;
extern const float min_velocity_threshold;
} // namespace AI

// Robot skills
namespace Skills {
namespace Kicking {
extern const float max_power;
extern const float optimal_distance;
extern const float enable_distance;
extern const float max_velocity;
extern const float min_velocity;
extern const int enable_duration;
} // namespace Kicking

namespace Dribbling {
extern const float max_push_distance;
extern const float barrier_distance;
extern const float behind_ball_distance;
extern const float minkowski_distance;
} // namespace Dribbling
} // namespace Skills

// Control system
namespace Control {
namespace Movement {
extern const float max_linear_speed;
extern const float min_linear_speed;
extern const float max_linear_accel;
extern const float max_angular_speed;
extern const float min_angular_speed;
extern const float linear_error;
extern const float angular_error;
} // namespace Movement

namespace PID {
struct Gains {
    float kp;
    float ki;
    float kd;
    float max_integral;
};

extern const Gains linear;
extern const Gains angular;
} // namespace PID

namespace Timing {
extern const float max_dt;
extern const float replan_interval;
} // namespace Timing
} // namespace Control

// System configuration
namespace System {
extern const int thread_frequency;
extern const int invalid_id;
extern const float game_off_max_speed;
extern const int max_robots;
extern const float bezier_control_point;
} // namespace System
} // namespace Const
#endif