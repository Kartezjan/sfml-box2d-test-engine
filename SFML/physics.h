#pragma once

#include <Box2D\Box2D.h>

#include "config.h"

b2Body* create_ground(b2World& World, float X, float Y);

b2Body* create_box(b2World& World, int MouseX, int MouseY);

