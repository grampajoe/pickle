#include <string>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#include <math.h>
#include "settings.h"
#include "sprite.h"

Sprite::Sprite()
{
	rotation = 0.0;
	scale = 1.0;
	opacity = 1.0;
	x = 0;
	y = 0;
	width = 0;
	height = 0;

	vertices.resize(8, 0.0);
	texture_region.resize(8, 0.0);
}

Sprite::Sprite(const char* filename)
{
	tex = Sprite::load_texture(filename);

	rotation = 0.0;
	scale = 1.0;
	opacity = 1.0;
	x = 0;
	y = 0;
	width = tex->width;
	height = tex->height;

	vertices.resize(8, 0.0);
	texture_region.resize(8, 0.0);

	set_texture_region(0, 0, width, height);
}

SDL_Surface* Sprite::load_surface(const char* filename)
{
	SDL_Surface* surf_temp;
	SDL_Surface* surf_display;

	std::string location = PREFIX MEDIA_PREFIX IMAGE_DIR;
	location += filename;

	if ((surf_temp = IMG_Load(location.c_str())) == NULL)
		return NULL;

	surf_display = SDL_DisplayFormatAlpha(surf_temp);
	SDL_FreeSurface(surf_temp);

	return surf_display;
}

texture* Sprite::texture_from_surface(SDL_Surface* surface)
{
	texture* new_tex = (texture*)malloc(sizeof(texture));
	GLenum format;

	SDL_LockSurface(surface);

	new_tex->width = surface->w;
	new_tex->height = surface->h;

	// Convert SDL surface to OpenGL texture.
	glGenTextures(1, &new_tex->name);
	glBindTexture(GL_TEXTURE_2D, new_tex->name);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

	if (surface->format->BytesPerPixel == 4)
	{
		if (surface->format->Rmask == 0x000000ff)
			format = GL_RGBA;
		else
			format = GL_BGRA;
	}
	else
	{
		if (surface->format->Rmask == 0x000000ff)
			format = GL_RGB;
		else
			format = GL_BGR;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, surface->format->BytesPerPixel, surface->w,
			surface->h, 0, format, GL_UNSIGNED_BYTE,
			surface->pixels);

	SDL_UnlockSurface(surface);
	SDL_FreeSurface(surface);

	return new_tex;
}

texture* Sprite::load_texture(const char* filename)
{
	SDL_Surface* surf_display = Sprite::load_surface(filename);

	return Sprite::texture_from_surface(surf_display);
}

Sprite* Sprite::from_texture(texture* src)
{
	Sprite* sprite = new Sprite;
	sprite->set_texture(src);

	return sprite;
}

std::vector<GLdouble> Sprite::bounds()
{
	return vertices;
}

void Sprite::set_texture(texture* src)
{
	tex = src;
	width = tex->width;
	height = tex->height;

	set_texture_region(0, 0, width, height);
}

void Sprite::set_texture_region(GLdouble x, GLdouble y, GLdouble width, GLdouble height)
{
	GLdouble ux, uy, vx, vy;
	ux = x/(tex->width);
	uy = y/(tex->height);
	vx = (x + width)/(tex->width);
	vy = (y + height)/(tex->height);

	texture_region[0] = ux; texture_region[1] = uy;
	texture_region[2] = ux; texture_region[3] = vy;
	texture_region[4] = vx; texture_region[5] = vy;
	texture_region[6] = vx; texture_region[7] = uy;
}

void Sprite::update()
{
	// Rotate the sprite's vertices around its center.
	GLdouble w = scale*width/2, h = scale*height/2, radians, sinr, cosr;

	radians = PI * (rotation/180);
	sinr = sin(radians);
	cosr = cos(radians);

	vertices[0] = x + ((-w) * cosr) - (h * sinr);
	vertices[1] = y + ((-w) * sinr) + (h * cosr);

	vertices[2] = x + ((-w) * cosr) - ((-h) * sinr);
	vertices[3] = y + ((-w) * sinr) + ((-h) * cosr);

	vertices[4] = x + (w * cosr) - ((-h) * sinr);
	vertices[5] = y + (w * sinr) + ((-h) * cosr);

	vertices[6] = x + (w * cosr) - (h * sinr);
	vertices[7] = y + (w * sinr) + (h * cosr);
}

/* Draw the sprite as a texture mapped to a quad. */
void Sprite::draw()
{
	int i;

	glBindTexture(GL_TEXTURE_2D, tex->name);

	glBegin(GL_QUADS);

	for (i = 0; i < 8; i += 2)
	{
		glColor4f(1.0, 1.0, 1.0, opacity);
		glTexCoord2d(texture_region[i], texture_region[i+1]);
		glVertex2d(vertices[i], vertices[i+1]);
	}

	glEnd();
}

Sprite::~Sprite()
{
}
