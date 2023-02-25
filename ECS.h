#pragma once

#include <iostream>
#include <array>
#include <bitset>
#include <iostream>



class Entity;
class Component;	// class definition

using ComponentTypeID = std::size_t;	// keyword " using " ComponentTypeID as alias to size_t, size_t(unsigned int long long since ist's 64bit) maximal value your system can handle 

// purpose of this function is to increment lastID every time we attach new component to an entity also using static variable so that it wont be deleted at the end of the scope
inline ComponentTypeID getUniqueComponentID() { //suggesting compiler with keyword " inline " to duplicate code from getUniqueComponentID with return type of ComponentTypeID to a call site.
	static ComponentTypeID lastID = 0u;		// static variable lastID of type ComponentType ID assigned initial value of 0u(unsigned int), since size_t currently represents size of container.
	return lastID++;	// returns incremented value of lastID
}

/* this is template function that ensures a template typename T is indeed derived from Component Class and assigns lastID to typeID, also static is set to const reason for this
   every time different type is introduced new function is created as ComponentTypeID typeID is created and set to lastID value that was previously incremented */
template<typename T>	//template that takes single parameter, typename T, which is expected to be a type derived from the base class Component
inline ComponentTypeID getComponentTypeID() noexcept {		// getComponentTypeID inline function that returns ComponentTypeID and has function specifier noexcept to generate more efficent code
	static_assert(std::is_base_of<Component, T>::value, "Type not based on component"); // compile-time assertion static_assert if class of Type T is derived from Component class(is_base_of<BaseClass,isDerivedClass>)
	static const ComponentTypeID typeID = getUniqueComponentID();	/*variable typeID of static const ComponentTypeID storing lastID from getUniqueComponentID,
	every time we attach new component to Component class we increment lastID and store value to TypeID */
	return typeID;	// returns stored incremented value of lastID to typeID
}


constexpr std::size_t MAX_ENTITY = 500; /*constexpr guaranties compile - time evaluationand const cannot be used as template argument since it doesn't guarantee compile-time evaluation
while MAX_ENTITY depends on what limit you want to set to your game/program depending on general population system specs or any other hardware limits */
constexpr std::size_t MAX_COMPONENTS = 32; // same explanation 

// goal of this aliases to initialise values that are going to keep track of components associated with Entity
using ComponentBitset = std::bitset<MAX_COMPONENTS>; /* using ComponentBitset as alias to bitset<Max_Components> that is going to  keep track which components are associated
with which entities each bit in bitset will represent a single component, with value of (0 , 1) which will indicate whether component is associated with entity or not*/
using CompnentList = std::array<Component*, MAX_COMPONENTS>; /* using ComponentList as alias to array of pointers to Component objects with maximum number of element specified by constexpr
using an array istead of vetors since we have constant expression of Max_components, meaning that array size is known and is more efficient than vector since stores data sequentially
instead of random places in memory. This array is used, to store the pointers of all the components associated with a particualr entity*/

