#ifndef PICKLE_VECTOR_H_
#define PICKLE_VECTOR_H_

#include <vector>
#include <math.h>
#include <SDL/SDL_opengl.h>

GLdouble dot_product(GLdouble ux, GLdouble uy, GLdouble vx, GLdouble vy);

GLdouble* normalize(GLdouble x, GLdouble y);

GLdouble* projection(GLdouble edge[2], std::vector<GLdouble>* points);

bool intersects(std::vector<GLdouble> box1, std::vector<GLdouble> box2);

#endif
