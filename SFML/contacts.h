#pragma once

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
	void process() override;
};
class destroys_upon_collision : public virtue {
public:
	using virtue::virtue;
	void process() override;
};

class explodes_upon_collision : public virtue {
public:
	explodes_upon_collision(universe& uni_ref, _entity& owner_ref, const float blast_rad, const float blast_pow) : virtue(uni_ref, owner_ref) 
	{
		blast_radius = blast_rad / SCALE;
		blast_power = blast_pow;
	}
	void process() override;
private:
	float num_rays = 32.f;
	float blast_radius = 5.f;
	float blast_power = 1e+10F;
};