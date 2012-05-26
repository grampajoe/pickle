#ifndef PICKLE_GAME_H_
#define PICKLE_GAME_H_

#include <list>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "sprite.h"
#include "background.h"
#include "cursor.h"
#include "thing.h"
#include "label.h"

#define GAME_TITLE   0
#define GAME_PLAYING 1
#define GAME_LOSE    2

class Game
{
	private:
		bool running;

		bool paused;

		SDL_Surface* surf_display;
		SDL_Rect window;

		Uint32 last_update;

		Cursor* cursor;

		Background* background;

		Sprite* title_sprite;
		Sprite* full_jar_sprite;
		Sprite* splat_sprite;
		std::vector<Sprite*> heart_sprites;

		int state;

		SDL_TimerID update_timer_id;
		SDL_TimerID drop_timer_id;
		SDL_TimerID speedup_timer_id;
		SDL_TimerID render_timer_id;

		Uint32 drop_interval;
		Uint32 max_drop;

		TTF_Font* label_font;
		TTF_Font* label_font_big;
		Label* click_label;
		Label* score_label;
		Label* lose_label;

	public:
		Game();

		std::list<Thing*> things;

		bool init();
		void change_state(int new_state);
		void reset();
		int loop();
		void handle(SDL_Event* event);
		void update();
		void drop();
		void speed_up();
		void toggle_pause();
		void render();
		void cleanup();

		bool screenshot(const char* filename);

		int score;
		int lives;
		float pickle_prob;
};

void push_timer_event(int code);
Uint32 update_timer(Uint32 interval, void* param);
Uint32 drop_timer(Uint32 interval, void* param);
Uint32 speedup_timer(Uint32 interval, void* param);
Uint32 render_timer(Uint32 interval, void* param);

#endif
