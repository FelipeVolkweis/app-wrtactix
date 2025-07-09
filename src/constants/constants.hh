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
extern float kicking_device_distance;
extern float minimum_ball_velocity_to_consider_movement;
} // namespace Physics

// AI configuration
namespace AI {
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
extern float min_angle_to_shoot_to_goal;
extern float min_angle_to_pass;
} // namespace Kicking

namespace Dribbling {
extern float max_push_distance;
extern float behind_ball_distance;
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
extern float speed_boost;
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

namespace PFLorinho {
extern float ball_repulsive;
extern float repulsive;
extern float area_repulsive;
extern float collision_check_radius;
extern float robot_collision_radius;
extern float ball_collision_radius;
extern float x_shift;
extern float y_shift;
extern float factor;
} // namespace PFLorinho

} // namespace PathPlanner

/// Inicializa todas as constantes a partir de Config::retrieve<…>()
void initialize();

} // namespace Const

#endif // CONSTANTS_HH
