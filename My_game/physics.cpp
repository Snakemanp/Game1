#include"vec2.h"
#include<cmath>
#include"entity.h"
namespace physics {
	float distance(const Entity& a, const Entity& b) {
		vec2 acm, bcm;
		acm = a.obj_trans->pos;
		bcm = b.obj_trans->pos;
		acm = acm - bcm;
		float dist = 0;
		dist = (acm.x) * (acm.x) + (acm.y) * (acm.y);
		dist = std::sqrt(dist);
		return dist;
	}
	bool collision(const Entity& a, const Entity& b) {
		float dist = 0, sr = 0;
		sr = a.shape->getRadius() + b.shape->getRadius();
		dist = distance(a, b);
		if (dist > sr) return false;
		return true;
	}
	bool out_scope(const Entity& a, int x, int y) {
		vec2 temp;
		temp = a.obj_trans->pos;
		if (temp.x > x) return true;
		if (temp.x < 0)return true;
		if (temp.y > y)return true;
		if (temp.y < 0)return true;
		return false;
	}
	void bounce(const Entity& a,int x,int y) {
		vec2& temp = a.obj_trans->pos;
		vec2& vel = a.obj_trans->vel;
		float rad = a.shape->getRadius();
		if (temp.x > x-rad || temp.x < rad) vel.x = vel.x * -1;
		if (temp.y > y-rad || temp.y < rad) vel.y = vel.y * -1;
	}
	void clip(const Entity& a, int x, int y) {
		float rad = a.shape->getRadius();
		vec2& temp = a.obj_trans->pos;
		if (temp.x > x - rad) {
			temp.x = x - rad;
		}
		if (temp.x < rad) {
			temp.x = rad;
		}
		if (temp.y > y - rad) {
			temp.y = y - rad;
		}
		if (temp.y < rad) {
			temp.y = rad;
		}
	}
}
