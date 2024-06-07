#ifndef entity_manager
#define entity_manager
#include"entity.h"
#include<vector>
#include<map>
#include<string>
#include<memory>
typedef std::vector<std::shared_ptr<Entity>> entity_list;
class EntityManager {
	entity_list entities;
	std::map<std::string, entity_list> entity_map;
	long int no_entities=0;
	entity_list changes;
public:
	EntityManager() {}
	void update() {
		for (int i = 0; i < changes.size(); i++) {
			entity_map[changes[i]->tag].push_back(changes[i]);
			entities.push_back(changes[i]);
		}
		/*for (int i = 0; i < no_chages; i++) {
			entity_map[entities[entities.size() - 1 - i]->tag].pop_back();
			entities.pop_back();
		}*/
		changes.clear();
		//no_chages = 0;
	}
	std::shared_ptr<Entity> add_entity(const std::string &tag) {
		std::shared_ptr<Entity> temp= std::make_shared<Entity>(no_entities++,tag);
		changes.push_back(temp);
		return temp;
	}
	entity_list& get_entities(const std::string& tag) {
		return(entity_map[tag]);
	}
	entity_list& get_entities() {
		return(entities);
	}
	void del_entity(std::shared_ptr<Entity> edel) {
		for (int i = 0; i < entities.size(); i++) {
			if (entities[i] == edel) {
				entities[i] = entities[entities.size() - 1];
				entities[entities.size()-1].reset();
				(entities.pop_back());
				break;
			}
		}
		entity_list& temp = entity_map[edel->tag];
		for (int i = 0; i < temp.size(); i++) {
			if (temp[i] == edel) {
				temp[i] = temp[temp.size() - 1];
				temp[temp.size() - 1].reset();
				temp.pop_back();
				break;
			}
		}
		edel.reset();
	}

};
#endif // !entity_manager
