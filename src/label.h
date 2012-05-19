#ifndef PICKLE_LABEL_H_
#define PICKLE_LABEL_H_

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_ttf.h>

#include "sprite.h"

#define ANCHOR_CENTER 0
#define ANCHOR_LEFT 1
#define ANCHOR_RIGHT 2
#define ANCHOR_TOP 3
#define ANCHOR_BOTTOM 4

class Label
{
	private:
		const char * text;
		TTF_Font* font;
		SDL_Color color;

		Sprite* sprite;

	public:
		Label(TTF_Font* font, const char* str, GLdouble x, GLdouble y, int anchor_x, int anchor_y);

		GLdouble x;
		GLdouble y;
		int anchor_x;
		int anchor_y;

		void set_text(const char* str);
		void set_text(int n);

		void draw();
};

#endif
