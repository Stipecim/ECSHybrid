#pragma once

class Entity; // whe pre-define entity class there so we can store adress of Main entity class inside " Entity.h"

class Component {
public:
	Component() {}
	~Component() {}	// default constructors

	Entity* entity; // entity pointer 

	// functions that will be overriten by subclasses( components ) 
	virtual bool init() { return true; }
	virtual void draw() {}
	virtual void update() {}
};
