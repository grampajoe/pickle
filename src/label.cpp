#include <sstream>

#include "label.h"

Label::Label(TTF_Font* font, const char* str, GLdouble x, GLdouble y, int anchor_x, int anchor_y)
{
	this->font = font;
	this->color.r = 255;
	this->color.b = 255;
	this->color.g = 255;
	this->x = x;
	this->y = y;
	this->anchor_x = anchor_x;
	this->anchor_y = anchor_y;
	this->text = "";

	sprite = new Sprite;

	set_text(str);
}

void Label::set_text(const char* str)
{
	if (strcmp(text, str) != 0)
	{
		text = str;

		SDL_Surface* temp_surface = TTF_RenderText_Blended(font, str, color);
		if (!temp_surface)
			return;

		texture* tex = Sprite::texture_from_surface(temp_surface);
		if (!tex)
			return;

		sprite->set_texture(tex);

		switch (anchor_x)
		{
			case ANCHOR_LEFT:
				sprite->x = x + sprite->width/2;
				break;

			case ANCHOR_RIGHT:
				sprite->x = x - sprite->width/2;
				break;

			case ANCHOR_CENTER:
			default:
				sprite->x = x;
				break;
		}

		switch (anchor_y)
		{
			case ANCHOR_TOP:
				sprite->y = y - sprite->height/2;
				break;

			case ANCHOR_BOTTOM:
				sprite->y = y + sprite->height/2;
				break;

			case ANCHOR_CENTER:
			default:
				sprite->y = y;
				break;
		}

		sprite->update();
	}
}

void Label::set_text(int n)
{
	std::stringstream ss;

	ss << n;

	set_text(ss.str().c_str());
}

void Label::draw()
{
	sprite->draw();
}
