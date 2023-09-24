#include "tinyECS/tiny_ecs.hpp"
#include <string>
#include <stdint.h>
#include <assert.h>
#include <iostream>
#include <cstring>

// Components
// Note: since `memcpy` is used to insert components, this implementation can only handle components that are
// Plain Old Data (POD) structs. Attempting to insert non-POD types, such as a component with an std::string field,
// is unsafe and will likely result in memory access violations.

struct Swims {
    float swim_speed;
    Swims(float speed = 0) : swim_speed(speed) {};
};

struct Walks {
};

struct Flies {
};

struct Position {
    float x;
    float y;
    Position(float xPosition = 0, float yPosition = 0) : x(xPosition), y(yPosition) {};
};

// Component registry
enum ComponentIndex
{
    swims = 0,
    walks = 1,
    flies = 2,
    position = 3,
};

constexpr unsigned int MAX_ENTITY = 64;

class Registry
{
public:
    // The sparse arrays of components for each entity
    Swims swims[MAX_ENTITY];
    Walks walks[MAX_ENTITY];
    Flies flies[MAX_ENTITY];
    Position positions[MAX_ENTITY];

    // Array of bitsets for each entity, where bit i indicates whether entity has component i
    uint64_t hasComponent[MAX_ENTITY] = { 0 };

    Registry() {}

    Entity create_entity() {
        return Entity();
    }

    // Checks if entity e has a component
    bool has(Entity e, ComponentIndex index) {
        // TODO: return false if entity maps to an index outside the hasComponent array:
        // It did not check if Entity e is greater than MAX_ENTITY in has() function.
        return hasComponent[e - 1] & (UINT64_C(1) << index);
    }

    // Removes a component from entity e
    void remove(Entity e, ComponentIndex index) {
        if (has(e, index)) {
            hasComponent[e - 1] &= ~(UINT64_C(1) << index);
        }
    }

    // Inserts a component for entity e by copying its data into componentArray
    void* insert(Entity e, void* component, void* componentArray, size_t componentSize, ComponentIndex index) {
        assert(!has(e, index) && "Entity already contained in registry");

        unsigned int cID = e - 1;
        char* dest = (char*)componentArray + componentSize * cID;
        memcpy(dest, component, componentSize);
        hasComponent[cID] |= (UINT64_C(1) << index);
        return dest;
    }

    // TODO: Gets component at correct index in component array
};

// Tests
int main(int argc, char* argv[])
{
    Registry registry;

    // Create a salmon entity that can swim with speed 3
    // Note: we require a temporary variable so that we can pass an address to `insert`
    Entity salmon = registry.create_entity();
    Swims tempSwimsSpeed3(3);
    registry.insert(salmon, &tempSwimsSpeed3, &registry.swims, sizeof(Swims), ComponentIndex::swims);

    printf("Salmon can swim?: %d\n", registry.has(salmon, ComponentIndex::swims));
    printf("Salmon can fly?: %d\n", registry.has(salmon, ComponentIndex::flies));
    printf("Salmon can walk?: %d\n", registry.has(salmon, ComponentIndex::walks));

    printf("Salmon swim speed: %f\n", registry.swims[salmon - 1].swim_speed);

    // Create a chicken entity that can swim, fly, and walk
    Entity chicken = registry.create_entity();
    Swims tempSwims; Flies tempFlies; Walks tempWalks;
    registry.insert(chicken, &tempSwims, &registry.swims, sizeof(Swims), ComponentIndex::swims);
    registry.insert(chicken, &tempFlies, &registry.flies, sizeof(Flies), ComponentIndex::flies);
    registry.insert(chicken, &tempWalks, &registry.walks, sizeof(Walks), ComponentIndex::walks);

    printf("\n");
    printf("Chicken can swim?: %d\n", registry.has(chicken, ComponentIndex::swims));
    printf("Chicken can fly?: %d\n", registry.has(chicken, ComponentIndex::flies));
    printf("Chicken can walk?: %d\n", registry.has(chicken, ComponentIndex::walks));

    // Add the ability to fly to the salmon
    registry.insert(salmon, &tempFlies, &registry.flies, sizeof(Flies), ComponentIndex::flies);
    printf("\n");
    printf("Salmon can fly?: %d\n", registry.has(salmon, ComponentIndex::flies));

    // Remove the ability to fly from the chicken
    registry.remove(chicken, ComponentIndex::flies);
    printf("\n");
    printf("Chicken can fly?: %d\n", registry.has(chicken, ComponentIndex::flies));

    // Change the swim speed of the existing salmon to 5 more than previous speed
    printf("\n");
    // We need to use reference variable like Swims&
    Swims salmonSwim = registry.swims[salmon - 1];
    printf("Salmon old swim speed: %f\n", salmonSwim.swim_speed);

    salmonSwim.swim_speed += 5;
    printf("Salmon new swim speed: %f\n", registry.swims[salmon - 1].swim_speed);

    // Add x and y position to salmon and chicken
    Position tempPosSalmon(1, 1), tempPosChicken(2, 2);
    registry.insert(salmon, &tempPosSalmon, &registry.positions, sizeof(Position), ComponentIndex::position);
    registry.insert(chicken, &tempPosChicken, &registry.positions, sizeof(Position), ComponentIndex::position);

    printf("\n");
    printf("Salmon position: (x = %f, y = %f)\n", registry.positions[salmon - 1].x, registry.positions[salmon - 1].y);
    printf("Chicken position: (x = %f, y = %f)\n", registry.positions[chicken - 1].x, registry.positions[chicken - 1].y);

    /*
    Task 10 explanation:
    
    This implementation stores the x and y position information in an Array of Structs layout. The Position array in the Registry acts as the “array” in “Array of
    Structs”. It stores a list of Position “structs”, each containing an x and y position. Thus, the x and y positions of each entity are stored adjacently in memory.
    This layout provides better cache locality when we need to loop through entities and access both the x and y position in each iteration.
    */

    return EXIT_SUCCESS;
}
