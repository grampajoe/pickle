#include <list>
#include <sstream>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "settings.h"
#include "sprite.h"
#include "cursor.h"
#include "thing.h"
#include "label.h"

Game::Game()
{
	running = true;
	window.w = WINDOW_WIDTH;
	window.h = WINDOW_HEIGHT;

	update_timer_id = NULL;
	drop_timer_id = NULL;
	speedup_timer_id = NULL;
}

bool Game::init()
{
	GLdouble aspect;
	int i;
	texture* heart_texture;

	srand(time(NULL));

	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTTHREAD) < 0)
		return false;

	TTF_Init();

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

	SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 8);

	if ((surf_display = SDL_SetVideoMode(window.w, window.h, 32, SDL_HWSURFACE |
			SDL_GL_DOUBLEBUFFER | SDL_OPENGL)) == NULL)
		return false;

	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);

	glViewport(0, 0, window.w, window.h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	// Set up coordinate system. Initial window is from -1 to 1 on each
	// axis, so we set the window coordinates to the window dimensions
	// in pixels with matrix transformations.
	aspect = (GLdouble)window.w/(GLdouble)window.h;

	glOrtho(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(2.0/window.h, 2.0/window.h, 0.0);
	glTranslatef(-(window.w/2), -(window.h/2), 0.0);


	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	
	// Texture loading must be done after the call to SDL_SetVideoMode.
	load_thing_textures();

	title_sprite = new Sprite("title.png");
	title_sprite->x = window.w/2;
	title_sprite->y = window.h/2 + title_sprite->height/2 + WINDOW_MARGIN;
	title_sprite->update();

	full_jar_sprite = new Sprite("jar_full.png");
	full_jar_sprite->x = WINDOW_MARGIN + (full_jar_sprite->width/2);
	full_jar_sprite->y = window.h - (WINDOW_MARGIN + (full_jar_sprite->height/2));
	full_jar_sprite->update();

	heart_texture = Sprite::load_texture("heart.png");
	heart_sprites.resize(MAX_LIVES);
	for (i = 0; i <MAX_LIVES ; i++)
	{
		heart_sprites[i] = Sprite::from_texture(heart_texture);
		heart_sprites[i]->x = (i + 1)*(heart_sprites[i]->width/2 + WINDOW_MARGIN);
		heart_sprites[i]->y = WINDOW_MARGIN + heart_sprites[i]->height/2;
		heart_sprites[i]->update();
	}

	splat_sprite = new Sprite("splat.png");

	cursor = new Cursor(&score, &lives);

	if ((label_font = TTF_OpenFont(PREFIX MEDIA_PREFIX "/fonts/FredokaOne-Regular.ttf", 16)) == NULL ||
			(label_font_big = TTF_OpenFont(PREFIX MEDIA_PREFIX "/fonts/FredokaOne-Regular.ttf", 32)) == NULL)
		return false;

	score_label = new Label(label_font, "0", WINDOW_MARGIN + 10 + full_jar_sprite->width,
			window.h - WINDOW_MARGIN - (full_jar_sprite->height/2),
			ANCHOR_LEFT, ANCHOR_CENTER);

	lose_label = new Label(label_font_big, "You filled N jars!", window.w/2, window.h/2, ANCHOR_CENTER, ANCHOR_CENTER);
	click_label = new Label(label_font_big, "Click to Start", window.w/2, window.h/2 - WINDOW_MARGIN, ANCHOR_CENTER, ANCHOR_TOP);

	background = new Background;
	background->update(0);
	
	return true;
}

void Game::reset()
{
	score = 0;
	lives = MAX_LIVES;
	pickle_prob = 1.0;
	max_drop = 1;

	cursor->x = window.h/2;
	cursor->update(0);
	things.resize(0);

	drop_interval = DROP_INTERVAL;

	paused = false;
}

void Game::change_state(int new_state)
{
	std::stringstream ss;

	state = new_state;

	paused = false;

	if (update_timer_id != NULL)
		SDL_RemoveTimer(update_timer_id);

	if (drop_timer_id != NULL)
		SDL_RemoveTimer(drop_timer_id);

	if (speedup_timer_id != NULL)
		SDL_RemoveTimer(speedup_timer_id);

	background->unhurt();
	background->unscore();

	switch (state)
	{
		case GAME_TITLE:
			SDL_WM_GrabInput(SDL_GRAB_OFF);
			SDL_ShowCursor(1);
			break;

		case GAME_PLAYING:
			SDL_WM_GrabInput(SDL_GRAB_ON);
			SDL_ShowCursor(0);

			reset();
			score_label->set_text(0);

			last_update = SDL_GetTicks();
			update_timer_id = SDL_AddTimer(UPDATE_INTERVAL, update_timer, NULL);
			drop_timer_id = SDL_AddTimer(drop_interval, drop_timer, &drop_interval);
			speedup_timer_id = SDL_AddTimer(SPEEDUP_INTERVAL, speedup_timer, NULL);
			break;

		case GAME_LOSE:
			SDL_WM_GrabInput(SDL_GRAB_OFF);
			SDL_ShowCursor(1);

			ss << "You filled " << (score / JAR_CAPACITY) << " jars!";
			lose_label->set_text(ss.str().c_str());

			things.clear();

			// Drop jars the player filled. The first subtraction
			// gets rid of a partial jar.
			for (score -= JAR_CAPACITY; score >= 0; score -= JAR_CAPACITY)
				things.push_back(Thing::jar(&things));

			splat_sprite->x = window.w/2;
			splat_sprite->y = window.h/2;
			splat_sprite->opacity = 1;
			splat_sprite->update();

			last_update = SDL_GetTicks();
			update_timer_id = SDL_AddTimer(UPDATE_INTERVAL, update_timer, NULL);
			break;
	}
}

int Game::loop()
{
	if (!init())
		return 1;

	SDL_Event event;

	change_state(GAME_TITLE);

	while (running)
	{
		while (SDL_PollEvent(&event))
			handle(&event);

		render();
	}

	cleanup();

	return 0;
}

void Game::handle(SDL_Event* event)
{
	switch (event->type)
	{
		case SDL_USEREVENT:
			switch (event->user.code)
			{
				case UPDATE_TIMER:
					update();
					break;
				case DROP_TIMER:
					drop();
					break;
				case SPEEDUP_TIMER:
					speed_up();
					break;
				case UNHURT_TIMER:
					background->unhurt();
					break;
				case UNSCORE_TIMER:
					background->unscore();
					break;
			}
			break;

		case SDL_KEYDOWN:
			switch (event->key.keysym.sym)
			{
				case SDLK_ESCAPE:
					running = false;
					break;

				case SDLK_p:
					toggle_pause();
					break;

				default:
					break;
			}
			break;

		case SDL_MOUSEMOTION:
			switch (state)
			{
				case GAME_PLAYING:
					if (!paused)
						cursor->move(event->motion.xrel);
					break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			switch (state)
			{
				case GAME_TITLE:
					change_state(GAME_PLAYING);
					break;

				case GAME_LOSE:
					change_state(GAME_TITLE);
					break;
			}
			break;

		case SDL_QUIT:
			running = false;
			break;
	}
}

void Game::update()
{
	Uint32 dt = SDL_GetTicks() - last_update;
	GLdouble secs = (GLdouble)dt/1000.0;

	last_update += dt;

	if (paused)
		return;

	std::list<Thing*>::iterator it;
	std::list<Thing*> remove;

	switch (state)
	{
		case GAME_PLAYING:
			// If collision detection is done before updating positions, it looks
			// more like the collision should've happened.
			for (it = things.begin(); it != things.end(); it++)
			{
				if ((*it)->collide(cursor))
				{
					(*it)->on_hit();

					if ((*it)->pickle)
					{
						score++;
						score_label->set_text(score/JAR_CAPACITY);

						// Lives reset when a jar is filled
						if (score % JAR_CAPACITY == 0)
						{
							background->score();
							lives = MAX_LIVES;
						}
					}
					else
					{
						lives--;
						background->hurt();
						if (lives <= 0)
						{
							change_state(GAME_LOSE);
							return;
						}
					}
				}
				else if ((*it)->y < -100)
				{
					// Remove things that fall off the screen.
					remove.push_back(*it);
				}
				(*it)->update(secs);
			}

			cursor->update(secs);

			background->update(secs);

			for (it = remove.begin(); it != remove.end(); it++)
				things.remove(*it);
			break;

		case GAME_LOSE:
			for (it = things.begin(); it != things.end(); it++)
			{
				if ((*it)->y < -100)
				{
					// Remove things that fall off the screen.
					remove.push_back(*it);
				}
				(*it)->update(secs);
			}

			for (it = remove.begin(); it != remove.end(); it++)
				things.remove(*it);

			if (splat_sprite->opacity > 0)
			{
				splat_sprite->opacity -= 1 * secs;
				splat_sprite->y -= SPLAT_SPEED * secs;
				splat_sprite->update();
			}

			break;
	}
}

/* Drop a random amount of things. */
void Game::drop()
{
	if (state != GAME_PLAYING)
		return;

	int i = ((rand()/(float)RAND_MAX) * max_drop);
	if (i < 1)
		i = 1;

	for (; i > 0; i--)
		things.push_back(Thing::pick(pickle_prob, &things));
}

void Game::speed_up()
{
	if (state != GAME_PLAYING)
		return;

	// Probability of dropping a pickle decreases
	pickle_prob = pickle_prob * PICKLE_PROB_REDUCTION;
	if (pickle_prob < PICKLE_PROB_MIN)
		pickle_prob = PICKLE_PROB_MIN;

	// Delay between drops decreases
	drop_interval *= DROP_INTERVAL_REDUCTION;
	if (drop_interval < DROP_INTERVAL_MIN)
		drop_interval = DROP_INTERVAL_MIN;

	// Chance that the max simultaneous drop will increase
	if (rand()/(float)RAND_MAX > ADDITIONAL_DROP_PROB)
		max_drop++;
}

void Game::toggle_pause()
{
	if (state == GAME_PLAYING)
	{
		paused = !paused;
	}
}

void Game::render()
{
	std::list<Thing*>::iterator it;
	int i;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	background->draw();

	switch (state)
	{
		case GAME_TITLE:
			title_sprite->draw();
			click_label->draw();
			break;

		case GAME_PLAYING:
			for (it = things.begin(); it != things.end(); it++)
				(*it)->draw();

			cursor->draw();

			for (i = 0; i < lives; i++)
				heart_sprites[i]->draw();

			full_jar_sprite->draw();
			score_label->draw();
			break;

		case GAME_LOSE:
			for (it = things.begin(); it != things.end(); it++)
				(*it)->draw();
			lose_label->draw();
			if (splat_sprite->opacity > 0)
				splat_sprite->draw();
			break;
	}

	SDL_GL_SwapBuffers();
}

void Game::cleanup()
{
	SDL_FreeSurface(surf_display);

	SDL_Quit();
	TTF_Quit();
}

Uint32 update_timer(Uint32 interval, void* param)
{
	SDL_Event event;
	SDL_UserEvent userevent;

	userevent.type = SDL_USEREVENT;
	userevent.code = UPDATE_TIMER;
	userevent.data1 = NULL;
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);
	return interval;
}

Uint32 drop_timer(Uint32 interval, void* param)
{
	SDL_Event event;
	SDL_UserEvent userevent;

	userevent.type = SDL_USEREVENT;
	userevent.code = DROP_TIMER;
	userevent.data1 = NULL;
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);
	return *((Uint32*)param);
}

Uint32 speedup_timer(Uint32 interval, void* param)
{
	SDL_Event event;
	SDL_UserEvent userevent;

	userevent.type = SDL_USEREVENT;
	userevent.code = SPEEDUP_TIMER;
	userevent.data1 = NULL;
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);
	return interval;
}
