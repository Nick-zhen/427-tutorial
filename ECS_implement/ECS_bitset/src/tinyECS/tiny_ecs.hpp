#pragma once

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
