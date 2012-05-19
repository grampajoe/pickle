#include <list>
#include "thing.h"
#include "sprite.h"
#include "vector.h"
#include "settings.h"

texture *pickle_texture, *knife_texture, *jar_texture;

// This has to be called by Game::init() after SDL_SetVideoMode or it'll
// segfault.
void load_thing_textures()
{
	pickle_texture = Sprite::load_texture("pickle.png");
	knife_texture = Sprite::load_texture("knife.png");
	jar_texture = Sprite::load_texture("jar_full.png");
}

Thing::Thing()
{
	x = (rand()/(float)(RAND_MAX)) * WINDOW_WIDTH;
	y = WINDOW_HEIGHT + 50 + ((rand()/(float)RAND_MAX) * 200);
	rotation = ((rand()/(float)(RAND_MAX)) * 360);

	vx = (rand()/(float)(RAND_MAX)) * ((WINDOW_WIDTH/2 - x)/abs(WINDOW_WIDTH/2 - x)) * 50;
	vy = 0.0;
	vr = vx;

	ax = 0.0;
	ay = THING_GRAVITY;
	ar = 0.0;

	scale = (rand()/(float)(RAND_MAX)) * (1.0 - THING_MIN_SCALE) + THING_MIN_SCALE;

	fading = false;
	hittable = true;
}

Thing* Thing::pick(float pickle_prob, std::list<Thing*>* things)
{
	Thing* thing;

	if ((rand()/(float)RAND_MAX) < pickle_prob)
	{
		thing = Thing::from_texture(pickle_texture, things);
		thing->pickle = true;
	}
	else
	{
		thing = Thing::from_texture(knife_texture, things);
		thing->pickle = false;
	}	

	return thing;
}

Thing* Thing::jar(std::list<Thing*>* things)
{
	Thing* thing = Thing::from_texture(jar_texture, things);
	thing->pickle = false;
	thing->hittable = false;
	thing->scale = 1;
	thing->y += ((rand()/(float)RAND_MAX) * LOSE_JAR_SPREAD);

	return thing;
}

Thing* Thing::from_texture(texture* tex, std::list<Thing*>* things)
{
	Thing* thing = new Thing;

	thing->sprite = Sprite::from_texture(tex);
	thing->things = things;
	thing->update(0);

	return thing;
}

void Thing::update(GLdouble secs)
{
	// Fading happens with no other movement.
	if (fading)
	{
		sprite->opacity -= THING_FADE_RATE * secs;

		// Hack to force removal.
		if (sprite->opacity <= 0)
			y = -10000;
	}
	else
	{
		y += vy * secs;

		x += vx * secs;
		rotation += vr * secs;

		vx += ax * secs;
		vy += ay * secs;
		vr += ar * secs;

		sprite->x = x;
		sprite->y = y;
		sprite->rotation = rotation;
		sprite->scale = scale;
		sprite->update();
	}
}

std::vector<GLdouble> Thing::bounds()
{
	return sprite->bounds();
}

bool Thing::collide(Cursor* cursor)
{
	if (!hittable || y > (cursor->y + sprite->height))
		return false;

	std::vector<GLdouble> cursor_bounds = cursor->bounds;
	std::vector<GLdouble> thing_bounds = bounds();

	return (intersects(cursor_bounds, thing_bounds) && intersects(thing_bounds, cursor_bounds));
}

void Thing::on_hit()
{
	hittable = false;

	if (pickle)
	{
		fading = true;
	}
	else
	{
		// Hack to force removal of the object.
		y = -1000;
	}
}

void Thing::draw()
{
	sprite->draw();
}
