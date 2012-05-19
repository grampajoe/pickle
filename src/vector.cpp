#include <vector>
#include <stdlib.h>
#include <SDL/SDL_opengl.h>
#include "vector.h"

GLdouble dot_product(GLdouble ux, GLdouble uy, GLdouble vx, GLdouble vy)
{
	return (ux * vx) + (uy * vy);
}

GLdouble* normalize(GLdouble x, GLdouble y)
{
	GLdouble mag = (GLdouble)sqrt(x*x + y*y);
	GLdouble* normal = (GLdouble*)malloc(sizeof(GLdouble)*2);

	normal[0] = x/mag;
	normal[1] = y/mag;

	return normal;
}

GLdouble* projection(GLdouble edge[2], std::vector<GLdouble> points)
{
	GLdouble temp;
	GLdouble* bounds = (GLdouble*)malloc(sizeof(GLdouble)*2);
	GLdouble* unit = normalize(-edge[1], edge[0]);

	std::vector<GLdouble>::iterator it;
	for (it = points.begin(); it != points.end(); it += 2)
	{
		temp = dot_product(unit[0], unit[1], *it, *(it + 1));
		if (it == points.begin() || temp < bounds[0])
			bounds[0] = temp;
		if (it == points.begin() || temp > bounds[1])
			bounds[1] = temp;
	}

	return bounds;
}

/* Loop through box1's edges and determine whether the projections of each
   bounding box onto edges' normals intersect. */
bool intersects(std::vector<GLdouble> box1, std::vector<GLdouble> box2)
{
	int i;
	GLdouble edge[2], *box1_proj, *box2_proj;

	for (i = 2; i < 8; i += 2)
	{
		if (i + 3 < 8)
		{
			edge[0] = box1[i] - box1[i + 2];
			edge[1] = box1[i + 1] - box1[i + 3];
		}
		else
		{
			edge[0] = box1[i] - box1[2];
			edge[1] = box1[i + 1] - box1[3];
		}

		box1_proj = projection(edge, box1);
		box2_proj = projection(edge, box2);

		if (box1_proj[0] < box2_proj[0])
		{
			if (box2_proj[0] > box1_proj[1])
				return false;
		}
		else
		{
			if (box1_proj[0] > box2_proj[1])
				return false;
		}
	}

	return true;
}
