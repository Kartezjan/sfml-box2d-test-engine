#pragma once

#include <cfloat>

#include "config.h"
#include "world.h"
#include "virtue.h"

class closest_ray_callback : public b2RayCastCallback {
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
	void send_message(abstract_entity* source);
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