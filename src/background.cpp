#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <math.h>
#include "background.h"
#include "settings.h"

Background::Background()
{
	unhurt_timer_id = NULL;
	unscore_timer_id = NULL;

	offset = BACKGROUND_INITIAL_OFFSET;
	speed = BACKGROUND_SPEED;
	interval = BACKGROUND_INTERVAL; // Radians
	length = BACKGROUND_RAY_LENGTH;

	color[0] = 67;
	color[1] = 13;
	color[2] = 50;

	bg_color[0] = 47.0/255;
	bg_color[1] = 9.0/255;
	bg_color[2] = 35.0/255;

	hurt_color[0] = 64;
	hurt_color[1] = 0;
	hurt_color[2] = 0;

	hurt_bg_color[0] = 128.0/255;
	hurt_bg_color[1] = 0;
	hurt_bg_color[2] = 0;

	unhurt();
};

// Show the hurt animation. The background colors flash red.
void Background::hurt()
{
	current_color = hurt_color;
	glClearColor(hurt_bg_color[0], hurt_bg_color[1], hurt_bg_color[2], 1);

	if (unhurt_timer_id != NULL)
		SDL_RemoveTimer(unhurt_timer_id);
	unhurt_timer_id = SDL_AddTimer(100, unhurt_timer, NULL);
}

// End the hurt animation.
void Background::unhurt()
{
	current_color = color;
	glClearColor(bg_color[0], bg_color[1], bg_color[2], 1);
}

// Show the score animation. The background rotation speeds way up for a
// little bit.
void Background::score()
{
	speed = BACKGROUND_SCORE_SPEED;

	if (unscore_timer_id != NULL)
		SDL_RemoveTimer(unscore_timer_id);
	unscore_timer_id = SDL_AddTimer(200, unscore_timer, NULL);
}

// End the score animation.
void Background::unscore()
{
	speed = BACKGROUND_SPEED;
}

void Background::update(GLdouble dt)
{
	offset += speed * dt;
	GLdouble theta = 0;

	vertices.clear();

	// Generate triangles with equal-sized spaces radiating from the
	// background's origin.
	for (theta = 0; theta <= 2*PI; theta += 2 * interval)
	{
		vertices.push_back(BACKGROUND_ORIGIN_X);
		vertices.push_back(BACKGROUND_ORIGIN_Y);

		vertices.push_back(BACKGROUND_ORIGIN_X + length * cos(theta + offset));
		vertices.push_back(BACKGROUND_ORIGIN_Y + length * sin(theta + offset));

		vertices.push_back(BACKGROUND_ORIGIN_X + length * cos(theta + offset + interval));
		vertices.push_back(BACKGROUND_ORIGIN_Y + length * sin(theta + offset + interval));
	}

}

void Background::draw()
{
	std::vector<GLdouble>::iterator i;

	// Disable textures here to draw colored triangles.
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_TRIANGLES);
	for (i = vertices.begin(); i != vertices.end(); i += 2)
	{
		glColor4ub(current_color[0], current_color[1], current_color[2], 255);
		glVertex2d(*i, *(i + 1));
	}
	glEnd();

	// Re-enable textures for everything else.
	glEnable(GL_TEXTURE_2D);
}

Uint32 unhurt_timer(Uint32 interval, void* param)
{
	SDL_Event event;
	SDL_UserEvent userevent;

	userevent.type = SDL_USEREVENT;
	userevent.code = UNHURT_TIMER;
	userevent.data1 = NULL;
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);
	return 0;
}

Uint32 unscore_timer(Uint32 interval, void* param)
{
	SDL_Event event;
	SDL_UserEvent userevent;

	userevent.type = SDL_USEREVENT;
	userevent.code = UNSCORE_TIMER;
	userevent.data1 = NULL;
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);
	return 0;
}
