// internal
#include "physics_system.hpp"
#include "world_init.hpp"
#include "world_system.hpp"

// Returns the local bounding coordinates scaled by the current size of the entity
vec2 get_bounding_box(const Motion& motion)
{
	// abs is to avoid negative scale due to the facing direction.
	return { abs(motion.scale.x), abs(motion.scale.y) };
}

// This is a SUPER APPROXIMATE check that puts a circle around the bounding boxes and sees
// if the center point of either object is inside the other's bounding-box-circle. You can
// surely implement a more accurate detection
bool collides(const Motion& motion1, const Motion& motion2)
{
	vec2 dp = motion1.position - motion2.position;
	float dist_squared = dot(dp,dp);
	const vec2 other_bonding_box = get_bounding_box(motion1) / 2.f;
	const float other_r_squared = dot(other_bonding_box, other_bonding_box);
	const vec2 my_bonding_box = get_bounding_box(motion2) / 2.f;
	const float my_r_squared = dot(my_bonding_box, my_bonding_box);
	const float r_squared = max(other_r_squared, my_r_squared);
	if (dist_squared < r_squared)
		return true;
	return false;
}

bool pebble_collides(const Motion& motion1, const Motion& motion2)
{
	float dx = motion2.position.x - motion1.position.x;
	float dy = motion2.position.y - motion1.position.y;
	float distance = sqrt(dx*dx + dy*dy);

	return distance < (motion1.scale.x + motion2.scale.x)/2.f;
}

bool pebble_turtle_collides(const Motion& motion1, const Motion& motion2)
{
	vec2 dp = motion1.position - motion2.position;
	float dist_squared = dot(dp,dp);
	const vec2 turtle_bonding_box = get_bounding_box(motion2) / 3.f;
	const float turtle_r_squared = dot(turtle_bonding_box, turtle_bonding_box);
	if (dist_squared < turtle_r_squared)
		return true;
	return false;
}

void PhysicsSystem::step(float elapsed_ms)
{
	// Move fish based on how much time has passed, this is to (partially) avoid
	// having entities move at different speed based on the machine.
	auto& motion_container = registry.motions;
	for(uint i = 0; i < motion_container.size(); i++)
	{
		// !!! TODO A1: update motion.position based on step_seconds and motion.velocity
		Motion& motion = motion_container.components[i];
		// Entity entity = motion_container.entities[i];
		float step_seconds = elapsed_ms / 1000.f;
		// (void)elapsed_ms; // placeholder to silence unused warning until implemented

		// decompose the x velocity
		float cos_x_mov = cos(motion.angle) * motion.velocity.x;
		float sin_x_mov = sin(motion.angle) * motion.velocity.x;
		motion.position.x += cos_x_mov * step_seconds;
		motion.position.y += sin_x_mov * step_seconds;
		// decompose the y velocity
		float cos_y_mov = cos(motion.angle + PI/2) * motion.velocity.y;
		float sin_y_mov = sin(motion.angle + PI/2) * motion.velocity.y;
		motion.position.x += cos_y_mov * step_seconds;  
		motion.position.y += sin_y_mov * step_seconds;
	}

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// TODO A2: HANDLE PEBBLE UPDATES HERE
	// DON'T WORRY ABOUT THIS UNTIL ASSIGNMENT 2
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	auto& pebble_container = registry.pebbles;
	for (uint i = 0; i < pebble_container.entities.size(); ++i) {
		Motion& motion = registry.motions.get(pebble_container.entities[i]);

		float acceleration_y = 19.8;
		motion.velocity.y += acceleration_y * elapsed_ms / 1000.f;
	}


	// Check for collisions between all moving entities
	for(uint i = 0; i < motion_container.components.size(); i++)
	{
		Motion& motion_i = motion_container.components[i];
		Entity entity_i = motion_container.entities[i];
		
		// note starting j at i+1 to compare all (i,j) pairs only once (and to not compare with itself)
		for(uint j = i+1; j < motion_container.components.size(); j++)
		{
			Motion& motion_j = motion_container.components[j];
			if (collides(motion_i, motion_j))
			{
				Entity entity_j = motion_container.entities[j];
				// Create a collisions event
				// We are abusing the ECS system a bit in that we potentially insert muliple collisions for the same entity
				registry.collisions.emplace_with_duplicates(entity_i, entity_j);
				registry.collisions.emplace_with_duplicates(entity_j, entity_i);
			}
		}
	}

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// TODO A2: HANDLE PEBBLE collisions HERE
	// DON'T WORRY ABOUT THIS UNTIL ASSIGNMENT 2
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	// handle pebble collides pebbles
	for (int i = 0; i < (signed int) pebble_container.entities.size() - 1; ++i) {
		Motion& motion_i = registry.motions.get(pebble_container.entities[i]);
		for (int j = i + 1; j < (signed int) pebble_container.entities.size(); ++j) {
			Motion& motion_j = registry.motions.get(pebble_container.entities[j]);

			// make sure the collied pebbles are moving away
			vec2 relative_v = motion_j.velocity - motion_i.velocity;
			vec2 relative_dist = motion_j.position - motion_i.position;
			float is_towards = dot(relative_v, relative_dist);

			if (pebble_collides(motion_i, motion_j) && is_towards < 0.f) {
				vec2 v1 = motion_i.velocity;
				float m1 = registry.physics.get(pebble_container.entities[i]).mass;
				vec2 v2 = motion_j.velocity;
				float m2 = registry.physics.get(pebble_container.entities[j]).mass;
				vec2 p1_p2 = motion_i.position - motion_j.position;
				float dist_squared = dot(p1_p2,p1_p2);
				vec2 p2_p1 = motion_j.position - motion_i.position;

				motion_i.velocity = v1-(2*m2/(m1+m2))*dot((v1-v2),p1_p2)/dist_squared*p1_p2;
				motion_j.velocity = v2-(2*m1/(m1+m2))*dot((v2-v1),p2_p1)/dist_squared*p2_p1;
			}
		}
	}
		
	// handle pebble collides turtles
	auto& turtle_container = registry.hardShells;
	for (int i = 0; i < (signed int) pebble_container.entities.size(); ++i) {
		Motion& motion_i = registry.motions.get(pebble_container.entities[i]);
		for (int j = 0; j < (signed int) turtle_container.entities.size(); ++j) {
			Motion& motion_j = registry.motions.get(turtle_container.entities[j]);

			// make sure the collied turtle are moving away
			vec2 relative_v = motion_j.velocity - motion_i.velocity;
			vec2 relative_dist = motion_j.position - motion_i.position;
			float is_towards = dot(relative_v, relative_dist);

			if (pebble_turtle_collides(motion_i, motion_j) && is_towards < 0.f) {
				vec2 v1 = motion_i.velocity;
				float m1 = registry.physics.get(pebble_container.entities[i]).mass;
				vec2 v2 = motion_j.velocity;
				float m2 = registry.physics.get(turtle_container.entities[j]).mass;
				vec2 p1_p2 = motion_i.position - motion_j.position;
				float dist_squared = dot(p1_p2,p1_p2);
				vec2 p2_p1 = motion_j.position - motion_i.position;

				motion_i.velocity = v1-(2*m2/(m1+m2))*dot((v1-v2),p1_p2)/dist_squared*p1_p2;
				motion_j.velocity = v2-(2*m1/(m1+m2))*dot((v2-v1),p2_p1)/dist_squared*p2_p1;
			}
		}
	}
}