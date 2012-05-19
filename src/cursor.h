#ifndef PICKLE_CURSOR_H_
#define PICKLE_CURSOR_H_

#include <vector>
#include "sprite.h"

class Cursor
{
	private:
		Sprite* sprite_jar;
		Sprite* sprite_pickles;

		int* score;
		int* lives;
	public:
		Cursor(int* score, int* lives);
		~Cursor();

		GLfloat x;
		GLfloat y;
		GLfloat width;
		GLfloat height;

		int min_x;
		int max_x;

		std::vector<GLdouble> bounds;

		void move(int xrel);

		void update(GLdouble dt);

		void draw();
};

#endif
