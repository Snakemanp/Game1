#ifndef entity_class
#define entity_class
#include<string>
#include"classes.h"
#include"vec2.h"
#include<memory>
#include<SFML/Graphics.hpp>
class Entity {
public:
	const long int id;
	const std::string tag;
	Entity(const int id_, const std::string& tag_) :id(id_), tag(tag_) {}
	bool alive=true;
	int score=0;
	std::shared_ptr<sf::CircleShape> shape;
	friend class EntityManager;
	std::shared_ptr<objtrans> obj_trans;
	std::shared_ptr<lifespan> life;
};
#endif // !entity_class
