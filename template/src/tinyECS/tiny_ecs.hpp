#pragma once

#include <assert.h>

// Unique identifier for all entities
class Entity
{
    unsigned int id;
public:
    Entity()
    {
        // TODO: somehow ensure that each entity gets a unique ID
    }
    operator unsigned int() { return id; } // this enables automatic casting to int
};

// A container that stores components of type 'Component'
// TODO: You will have to change this class to be applicable to different component types
class ComponentContainer
{
private:
    // TODO: add variables to store components and to associate components to entities
public:
    ComponentContainer() {};

    // Inserts a component c associated to entity e
	// TODO: add insert functionality and define the right return type
	/*
    TODO insert(Entity e, Component c)
    {
        assert(!has(e) && "Entity already contained in registry");
    }; */

    // Checks if entity e has a component of type 'Component'
	// TODO: add has functionality
    bool has(Entity e)
    {
 		
    };

    // Removes the component of type 'Component' from entity e
    // TODO: add remove functionality
    void remove(Entity e)
    {

    };

    // Returns the component of type 'Component' associated with entity e
	// TODO: add get functionality, including the right return type
	/*
    TODO get(Entity e) {
        
    };
	*/
};