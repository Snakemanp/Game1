#ifndef health_obj
#define health_obj
class health {
public:
	int curhealth;
	int fullhealth;
	int healhealth;
	health(int fullhealth_=100) : fullhealth(fullhealth_), curhealth(fullhealth_), healhealth(0) {}
	void heal() {
		curhealth += healhealth;
	}
	void add_health(int add_health) {
		curhealth += add_health;
		if (curhealth > fullhealth) curhealth = fullhealth;
	}
	void upg_health(int uphealth) {
		fullhealth += uphealth;
	}
	void full_health() {
		curhealth = fullhealth;
	}
	double show() {
		return (((double)curhealth / fullhealth)*100);
	}
};
#endif // !health_obj{}
