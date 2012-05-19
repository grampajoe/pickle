#ifndef PICKLE_THING_H_
#define PICKLE_THING_H_

#include <list>
#include "sprite.h"
#include "cursor.h"

class Thing
{
	private:
		Sprite* sprite;

		bool fading;
		bool hittable;

	public:
		Thing();

		static Thing* from_texture(texture* tex, std::list<Thing*>* things);
		static Thing* pick(float pickle_prob, std::list<Thing*>* things);
		static Thing* jar(std::list<Thing*>* things);

		std::list<Thing*>* things;

		GLdouble vx;
		GLdouble vy;
		GLdouble vr;

		GLdouble ax;
		GLdouble ay;
		GLdouble ar;

		GLdouble x;
		GLdouble y;
		GLdouble rotation;

		GLdouble scale;

		bool pickle;

		bool collide(Cursor* cursor);

		std::vector<GLdouble> bounds();

		void update(GLdouble dt);

		void on_hit();

		void draw();
};

void load_thing_textures();

#endif
