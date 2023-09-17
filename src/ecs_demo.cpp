#include "tinyECS/tiny_ecs.hpp"
#include <iostream>

// Components
// TODO: create your components

// Component registry
class Registry
{
public:
	// TODO: add your components here, e.g.:
    // 			ComponentContainer<Swims> swims;
	// 			SwimsContainer swims;

    Registry() {}

	// TODO: implement create_entity
    Entity create_entity() {
        
    }
};

// Tests
int main(int argc, char* argv[])
{
    Registry registry;

    // TODO: Create a salmon entity that can swim with speed 3 
    // by using registry.create_entity() and registry.swims.insert(...);
    registry.create_entity();

	// TODO: Always create debugging output to check correctness, e.g.
    printf("Salmon can swim?: %d\n", 0 /*TODO: check registry.swims.has(salmon)*/);
	
	// TODO: create new entities and assign components as described in the task description
	
    // Task 10 explanation:
	/*
    TODO: Add your explanation for SoA or AoS here.
    */

	return EXIT_SUCCESS;
}
