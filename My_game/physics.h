#ifndef game_physics_
#define game_physics_
#include "vec2.h"
#include "entity.h"

namespace physics {
    float distance(const Entity& a, const Entity& b);

    bool collision(const Entity& a, const Entity& b);

    bool out_scope(const Entity& a, int x, int y);

    void bounce(const Entity& a, int x, int y);

    void clip(const Entity& a, int x, int y);
}

#endif // !game_logic_
