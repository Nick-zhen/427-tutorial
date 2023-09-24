#include "tinyECS/tiny_ecs.hpp"
#include <iostream>

// Components
struct Swims {
    float swim_speed;
    Swims(float speed = 0) : swim_speed(speed) {};
};

struct Walks {
};

struct Flies {
};

struct XPosition {
    float x;
    XPosition(float xPosition = 0) : x(xPosition) {};
};

struct YPosition {
    float y;
    YPosition(float yPosition = 0) : y(yPosition) {};
};

// Component registry
class Registry
{
public:
    ComponentContainer<Swims> swims;
    ComponentContainer<Flies> flies;
    ComponentContainer<Walks> walks;
    ComponentContainer<XPosition> xPositions;
    ComponentContainer<YPosition> yPositions;

    Registry() {}

    Entity create_entity() {
        return Entity();
    }
};

// Tests
int main(int argc, char* argv[])
{
    Registry registry;

    // Create a salmon entity that can swim with speed 3
    Entity salmon = registry.create_entity();
    registry.swims.insert(salmon, Swims(3));

    printf("Salmon can swim?: %d\n", registry.swims.has(salmon));
    printf("Salmon can fly?: %d\n", registry.flies.has(salmon));
    printf("Salmon can walk?: %d\n", registry.walks.has(salmon));

    printf("Salmon swim speed: %f\n", registry.swims.get(salmon).swim_speed);

    // Create a chicken entity that can swim, fly, and walk (using emplace)
    Entity chicken = registry.create_entity();
    registry.swims.emplace(chicken);
    registry.flies.emplace(chicken);
    registry.walks.emplace(chicken);

    printf("\n");
    printf("Chicken can swim?: %d\n", registry.swims.has(chicken));
    printf("Chicken can fly?: %d\n", registry.flies.has(chicken));
    printf("Chicken can walk?: %d\n", registry.walks.has(chicken));

    // Add the ability to fly to the salmon
    registry.flies.emplace(salmon);
    printf("\n");
    printf("Salmon can fly?: %d\n", registry.flies.has(salmon));

    // Remove the ability to fly from the chicken
    registry.flies.remove(chicken);
    printf("\n");
    printf("Chicken can fly?: %d\n", registry.flies.has(chicken));

    // Change the swim speed of the existing salmon to 5 more than previous speed
    printf("\n");
    Swims& salmonSwim = registry.swims.get(salmon);
    printf("Salmon old swim speed: %f\n", salmonSwim.swim_speed);

    salmonSwim.swim_speed += 5;
    printf("Salmon new swim speed: %f\n", registry.swims.get(salmon).swim_speed);

    // Add x and y position to salmon and chicken
    registry.xPositions.insert(salmon, XPosition(1));
    registry.yPositions.insert(salmon, YPosition(1));

    registry.xPositions.insert(chicken, XPosition(2));
    registry.yPositions.insert(chicken, YPosition(2));

    printf("\n");
    printf("Salmon position: (x = %f, y = %f)\n", registry.xPositions.get(salmon).x, registry.yPositions.get(salmon).y);
    printf("Chicken position: (x = %f, y = %f)\n", registry.xPositions.get(chicken).x, registry.yPositions.get(chicken).y);

    /*
    Task 10 explanation:

    This implementation can be described as an Array of Structs. The component data, such as x and y position, is stored in structs such as XPosition and YPosition.
    The entities act as the “arrays” since each entity can contain zero or more component structs.
    */

	return EXIT_SUCCESS;
}
