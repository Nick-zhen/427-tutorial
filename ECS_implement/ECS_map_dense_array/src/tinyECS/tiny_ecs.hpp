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
        id = id_count++;
    }
    operator unsigned int() { return id; } // this enables automatic casting to int
};

// A container that stores components of type 'Component'
template <typename Component> // A component can be any class
class ComponentContainer
{
private:
    // Dense array of all components of type 'Component'
    std::vector<Component> components;

    // The hash map from Entity -> array index
    std::unordered_map<unsigned int, unsigned int> map_entity_componentID; // the entity is cast to uint to be hashable
public:
    ComponentContainer() {};

    // Inserts a component c associated to entity e
    Component& insert(Entity e, Component c)
    {
        assert(!has(e) && "Entity already contained in registry");

        map_entity_componentID[e] = (unsigned int)components.size();
        components.push_back(std::move(c)); // the move enforces move instead of copy constructor
        return components.back();
    };

    // Takes the the provided arguments Args, creates a new object of type Component, and inserts it into the ECS system
    template<typename... Args>
    Component& emplace(Entity e, Args &&... args) {
        return insert(e, Component(std::forward<Args>(args)...)); // the forward ensures that arguments are moved not copied
    }

    // Checks if entity e has a component of type 'Component'
    bool has(Entity e)
    {
        return map_entity_componentID.count(e) > 0;
    };

    // Removes the component of type 'Component' from entity e
    void remove(Entity e)
    {
        if (has(e))
        {
            map_entity_componentID.erase(e);
            // TODO: remove the component that associated with the entity
        }
    };

    // Returns the component of type 'Component' associated with entity e
    Component& get(Entity e) {
        assert(has(e) && "Entity not contained in ECS registry");
        return components[map_entity_componentID[e]];
    }
};
