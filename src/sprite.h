#ifndef PICKLE_SPRITE_H_
#define PICKLE_SPRITE_H_

#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

/* Information about a loaded texture. */
typedef struct
{
	GLuint name;
	GLuint width;
	GLuint height;
} texture;

class Sprite
{
	private:
		texture* tex;

		std::vector<GLdouble> vertices;
		std::vector<GLdouble> texture_region;
	public:
		Sprite();
		Sprite(const char* filename);
		~Sprite();

		static SDL_Surface* load_surface(const char* filename);
		static texture* texture_from_surface(SDL_Surface* src);
		static texture* load_texture(const char* filename);

		static Sprite* from_surface(SDL_Surface* surface);
		static Sprite* from_texture(texture* src);

		GLdouble width;
		GLdouble height;
		GLdouble x;
		GLdouble y;
		GLdouble rotation;
		GLdouble scale;
		GLdouble opacity;

		std::vector<GLdouble> bounds();

		void set_texture(texture* src);
		void set_texture_region(GLdouble x, GLdouble y, GLdouble width,
				GLdouble height);

		void update();

		void draw();
};

#endif
