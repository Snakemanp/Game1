#ifndef classes_game
#define classes_game
#include"vec2.h"
#include<ctime>
class objtrans {
public:
	vec2 pos;
	vec2 vel;
	float angle;
	float rspeed;
	objtrans(const vec2 p, const vec2 v = vec2(0,0), const float r = 0, const float a = 0) :pos(p), vel(v), angle(a), rspeed(r) {}
	void change_pos(const vec2 pos_) {
		pos = pos_;
	}
	void upd_pos() {
		pos = pos + vel;
	}
	void change_vel(const vec2 vel_) {
		vel = vel_;
	}
	void change_rspped(const float rspeed_) {
		rspeed = rspeed_;
	}
	void upd_angle() {
		angle += rspeed;
	}
};
class lifespan {
public:
	int timeleft;
	int totaltime;
	lifespan(int time) :totaltime(time),timeleft(time) {}
	void update() {
		timeleft--;
	}
	bool alive() {
		if (timeleft <= 0) return false;
		return true;
	}
	int getalpha() {
		return (int)((255.0f / totaltime) * timeleft);
	}
};
class user_input {
public:
	bool A=false;
	bool S=false;
	bool D=false;
	bool W=false;
	bool shoot=false;
	vec2 shootpos;
	user_input() {}
};
#endif // !classes_game
