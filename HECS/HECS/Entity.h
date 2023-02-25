#pragma once

#include "ECS.h"
#include "Component.h"
#include <memory>
#include <vector>

class Entity { // Entity Class
public: // write something about public 
	Entity() = default;	// entity default constructor 
	virtual ~Entity() = default;	// entity default destructor

	/* Takes takes typename T with multiple arguments of Typename... Targs. Function addComponent that returns reference from Derived Class for further use and pointer is stored on
	   stack that points to adress that stores object on heap. After, "uptr " it stores pointer that points to derived Class object, reason why we do this is because vector of unique_ptr
	   (components)is container of unique pointers meaning that we cannot store raw pointer to vector of unique pointers. Then checks if given class to function is actually derived class
	   from class Component if so stores Derived Class to compList and True to compBitset, also assigns pointer of entity clas to another instantiated object of Entity in Component that
	   will point to this Entity class and returns dereferenced Derived Class since it was pointer on dynamically alocated on heap  and static_cast to T* = nullptr so it returns reference
	   since nullptr doesn't have a reference */
	template<typename T, typename... TArgs>		// template with that takes multiple arguments
	inline T& addComponent(TArgs&&... args) {	// inline function that returns Type Reference in parantesis accepts accepts multiple arguments Rvalue or lvalue
		T* comp = new T(std::forward<TArgs>(args)...);		// pointer of type T dynamically allocates memory on heap while perfect forwarding of an rvalue arguments 
		std::unique_ptr<Component> uptr{ comp };		// creates unique pointer of type Component that points to another Class Type 
		components.emplace_back(std::move(uptr));		/* type Component pointer " uptr " gets destroyed and transfers an rvalue pointer to an vector of unique pointers that now points
														   another Derived/notDerived Class Type */

		if (comp->init()) {		// Checks if Base class Component is initialised and if Comp is derived clase of Component	else return static cast to nullpointer
			compList[getComponentTypeID<T>()] = comp;		// it assigns comp of type T(derived class) to a compList Array index ( getComponentTypeID<T>() ) 
			compBitset[getComponentTypeID<T>()] = true;		// same thing just with bitset on same index meaning that it is true that comp is actually Derived Class from Component
			comp->entity = this;	/* comp of type T(Derived class) takes pointer of " this " Entity class so it can be accesed within Component Class since its associated with it*/
			return *comp;		// *(dereferences)comp pointe and shows value stored in so that returns a reference and does't copy object so it can be used later on 
		}

		return *static_cast<T*>(nullptr);		// static_cast if Condition (comp->init()) = fail, meaning that passed object of Type T is not Derived Class 
	}

	/* this function getsComponent for future use, we are not modifying contents of this function we are jsut getting reference of Component that we want to use so this function
	   meaning it is Read-only function and  const after parantesis promises that content inside won't modify contents */
	template<typename T>		// tamplate with typename T
	inline T& getComponent() const {		// getComponent that returns reference to Derived Class of Base class Component
		Component* ptr(compList[getComponentTypeID<T>()]);		// Component * ptr points to compList type Component, takes index that is signed to TypeID then compList returns type T
		return *static_cast<T*>(ptr);		// static_cast dereferences ptr mostly used because has safety checks also optimizes code 
	}

	// Getter function that returns value in bits 0 if false and 1 if true 
	template<typename T>
	inline bool hasComponent() const {
		return compBitset[getComponentTypeID<T>()];		// every type has its own unique typeID 
	}

	// this checks if entity is active
	inline bool isActive() const {
		return active;
	}
	// sets active to fals when we dont need it
	inline void destroy() {
		active = false;
	}

	//iterates trough vector of unique pointers that are pointing to Component's derived class(components) and calls function draw and update
	inline void draw() {
		for (auto& comp : components) {
			comp->draw();
		}
	}
	inline void update() {
		for (auto& comp : components) {
			comp->update();
		}
	}

private:
	bool active;	// initialise Bool, flag for component state 
	CompnentList compList;		// initialise bitset object 
	ComponentBitset compBitset;		// alias for array 
	std::vector<std::unique_ptr<Component>> components;		// vector of unique pointers of type Component that point to Derived Classes of base Component 
};
