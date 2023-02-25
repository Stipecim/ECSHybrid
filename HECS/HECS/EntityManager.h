
#pragma once

#include <vector>
#include <memory>
#include "Entity.h"

// purpose of this class is to manage multiple entities, and call on update/draw functions from entities that are going to call
// components derived class that overrode virtual functions of base class
class EntityManager {
public:
	EntityManager() = default;
	~EntityManager() = default;

	void draw() { // updates all entities  components 
		for (auto& e : entities) e->draw();
	}

	void update() {
		for (auto& e : entities) e->update();
	}
	// this function will erase all unactive entities, erase will erase all entities using std remove parameter that returns iterator and std::end as second parameter
	void refresh() {
		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity>& mEntity)
			{
				return !mEntity->isActive();
			}
		), std::end(entities));
	}
	Entity& addEntity() { // Creates Pointer to an Entity class and than moves that pointer to entities vector of unique pointers 
		Entity* e = new Entity();
		std::unique_ptr<Entity> uptr{ e };
		entities.emplace_back(std::move(uptr));
		return *e;
	}

private:
	std::vector<std::unique_ptr<Entity>> entities; // vector of unique pointers called entities to store all Entity types
};