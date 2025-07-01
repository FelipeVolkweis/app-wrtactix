// constants.hh
#ifndef CONSTANTS_HH
#define CONSTANTS_HH

#include <QString>

namespace Const {

// Referee communication
namespace Referee {
extern QString ip_address;
extern int port;
extern int packet_size;
} // namespace Referee

// Physical properties
namespace Physics {
extern float ball_radius;
extern float robot_radius;
extern float stop_game_distance;
extern float off_field_tolerance;
} // namespace Physics

// AI configuration
namespace AI {
extern int max_roles;
extern int role_swap_hysteresis;
extern float distance_tolerance;
extern float angle_tolerance;
extern float min_velocity_threshold;
} // namespace AI

// Robot skills
namespace Skills {
namespace Kicking {
extern float max_power;
extern float optimal_distance;
extern float enable_distance;
extern float max_velocity;
extern float min_velocity;
extern int enable_duration;
} // namespace Kicking

namespace Dribbling {
extern float max_push_distance;
extern float barrier_distance;
extern float behind_ball_distance;
extern float minkowski_distance;
} // namespace Dribbling
} // namespace Skills

// Control system
namespace Control {
namespace Movement {
extern float max_linear_speed;
extern float min_linear_speed;
extern float max_linear_accel;
extern float max_angular_speed;
extern float min_angular_speed;
extern float linear_error;
extern float angular_error;
} // namespace Movement

namespace PID {
struct Gains {
    float kp;
    float ki;
    float kd;
    float max_integral;
};

extern Gains linear;
extern Gains angular;
} // namespace PID

namespace Timing {
extern float max_dt;
extern float replan_interval;
extern float kick_enabled_timeout;
} // namespace Timing
} // namespace Control

// System configuration
namespace System {
extern int thread_frequency;
extern int invalid_id;
extern float game_off_max_speed;
extern int max_robots;
extern float bezier_control_point;
} // namespace System

// Path planner
namespace PathPlanner {
namespace PotentialField {
extern float katt;
extern float krep;
extern float min_rad;
extern float threshold;
extern float epsilon;
} // namespace PotentialField

namespace StarPotential {
extern float threshold;
}

namespace UnivectorField {
extern float de;
extern float kr;
extern float ko;
extern float dmin;
extern float delta;
extern int maxIts;
extern float step;
} // namespace UnivectorField

} // namespace PathPlanner

/// Inicializa todas as constantes a partir de Config::retrieve<…>()
void initialize();

} // namespace Const

#endif // CONSTANTS_HH
