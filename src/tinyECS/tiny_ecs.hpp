#pragma once

#include <vector>
#include <unordered_map>
#include <assert.h>

// Unique identifier for all entities
class Entity
{
    unsigned int id;
    static unsigned int id_count; // starts from 1, entity 0 is the default initialization
public:
    Entity()
    {
        // TODO: somehow ensure that each entity gets a unique ID
        id = id_count++;
        // Note, indices of already deleted entities arent re-used in this simple implementation.
    }
    operator unsigned int() { return id; } // this enables automatic casting to int
};



// Common interface to refer to all containers in the ECS registry
struct ContainerInterface
{
	virtual void clear() = 0;
	virtual size_t size() = 0;
	virtual void remove(Entity e) = 0;
	virtual bool has(Entity entity) = 0;
};

// A container that stores components of type 'Component'
// TODO: You will have to change this class to be applicable to different component types
template <typename Component> // A component can be any class
class ComponentContainer : public ContainerInterface
{
private:
    // TODO: add variables to store components and to associate components to entities
    // The hash map from Entity -> array index.
    std::unordered_map<unsigned int, unsigned int> map_entity_componentID; // the entity is cast to uint to be hashable.
	bool registered = false;
public:
	// Container of all components of type 'Component'
	std::vector<Component> components;

	// The corresponding entities
	std::vector<Entity> entities;

    // Constructor that registers the type
    ComponentContainer() {};

    // Inserts a component c associated to entity e
	// TODO: add insert functionality and define the right return type
	/*
    TODO insert(Entity e, Component c)
    {
        assert(!has(e) && "Entity already contained in registry");
    }; */
    inline Component& insert(Entity e, Component c, bool check_for_duplicates = true)
	{
		// Usually, every entity should only have one instance of each component type
		assert(!(check_for_duplicates && has(e)) && "Entity already contained in ECS registry");

		map_entity_componentID[e] = (unsigned int)components.size();
		components.push_back(std::move(c)); // the move enforces move instead of copy constructor
		entities.push_back(e);
		return components.back();
	};

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