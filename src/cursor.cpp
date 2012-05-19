#include "settings.h"
#include "cursor.h"
#include "sprite.h"

Cursor::Cursor(int* score_p, int* lives_p)
{
	score = score_p;
	lives = lives_p;

	sprite_jar = new Sprite("jar_cracks.png");
	sprite_pickles = new Sprite("jar_pickles.png");

	x = CURSOR_X;
	y = CURSOR_Y;
	width = CURSOR_WIDTH;
	height = CURSOR_HEIGHT;

	min_x = CURSOR_MIN_X;
	max_x = CURSOR_MAX_X;

	// Align top of sprite to top of cursor bounds.
	sprite_jar->y = y - (sprite_jar->height/2 - height/2);
	sprite_pickles->y = y - (sprite_pickles->height/2 - height/2);

	sprite_jar->width = 90;
	sprite_pickles->width = 90;

	bounds.resize(8, 0);
}

void Cursor::move(int xrel)
{
	x += xrel;
	if (x > max_x)
		x = max_x;
	else if (x < min_x)
		x = min_x;
}

void Cursor::update(GLdouble dt)
{
	GLfloat sx;
	GLdouble w = width/2, h = height/2;

	// Set current tiles
	sx = (GLdouble)(MAX_LIVES - *lives) * 100;
	sprite_jar->set_texture_region(sx, 0, 90, 135);
	
	sx = (GLdouble)(*score % JAR_CAPACITY * 100);
	sprite_pickles->set_texture_region(sx, 0, 90, 135);

	// Everything's horizontally centered
	sprite_jar->x = x;
	sprite_pickles->x = x;

	bounds[0] = x - w; bounds[1] = y + h;
	bounds[2] = x - w; bounds[3] = y - h;
	bounds[4] = x + w; bounds[5] = y - h;
	bounds[6] = x + w; bounds[7] = y + h;
}

void Cursor::draw()
{
	sprite_pickles->update();
	sprite_jar->update();
	sprite_pickles->draw();
	sprite_jar->draw();

	int i;

	glBegin(GL_QUADS);
	for (i = 0; i < 8; i += 2)
	{
		glVertex2d(bounds[i], bounds[i + 1]);
	}
	glEnd();
}

Cursor::~Cursor()
{
	delete sprite_jar;
	delete sprite_pickles;
}
