#pragma once

#include <cfloat>

#include "config.h"
#include "utillity.h"

#include "virtue_management.h"

class closest_ray_callback : b2RayCastCallback {
public:
	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
	{
		m_body = fixture->GetBody();
		contact_point = point;
		return fraction;
	}
	b2Body* m_body = nullptr;
	b2Vec2 contact_point = b2Vec2_zero;
};

class handles_contacts : public virtue {
public:
	using virtue::virtue;
	void send_message(abstract_entity* source);
};

class destroys_upon_collision : public virtue {
public:
	using virtue::virtue;
	//destroys_upon_collision(universe& uni_ref, std::shared_ptr<physical_entity> obj_ref) : virtue(uni_ref), object(obj_ref) {}
	void send_message(abstract_entity* source);
private:
//	std::shared_ptr<physical_entity> object;
};

class explodes_upon_collision : public virtue {
public:
	explodes_upon_collision(universe& uni_ref, float blast_rad, float blast_pow) : virtue(uni_ref) 
	{
		blast_radius = blast_rad / SCALE;
		blast_power = blast_pow;
	}
	void send_message(abstract_entity* source);
private:
	float num_rays = 32.f;
	float blast_radius = 5.f;
	float blast_power = 1e+10F;
};

class desctructable_upon_collison : public virtue { //The difference between destroyable - it splits into polygons instead of being removed.
public:
	using virtue::virtue;
	void send_message(abstract_entity* source);
private:
	const size_t max_scars_count = 8;
	std::vector<visual_effect&> scars;
	float scar_pos_epsilon = 50 / SCALE ; //in meters
};