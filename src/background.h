#ifndef PICKLE_BACKGROUND_H_
#define PICKLE_BACKGROUND_H_

#define UNHURT_TIMER 4
#define UNSCORE_TIMER 5

class Background
{
	private:
		std::vector<GLdouble> vertices;

		GLdouble offset;
		GLdouble speed;
		GLdouble interval;
		GLdouble length;

		GLuint color[3];
		GLuint hurt_color[3];
		GLclampf bg_color[3];
		GLclampf hurt_bg_color[3];

		GLuint* current_color;

		SDL_TimerID unhurt_timer_id;
		SDL_TimerID unscore_timer_id;
	public:
		Background();

		void update(GLdouble dt);
		void hurt();
		void score();
		void unhurt();
		void unscore();
		void draw();
};

Uint32 unhurt_timer(Uint32 interval, void* param);
Uint32 unscore_timer(Uint32 interval, void* param);

#endif
