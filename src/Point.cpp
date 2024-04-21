/******************************************************************************************************
 * @file  Point.cpp
 * @brief Implementation of the Point class
 ******************************************************************************************************/

#include "Point.hpp"

Point::Point() : SDL_Point({0, 0}) { }

Point::Point(int x, int y) : SDL_Point({x, y}) { }

Point::Point(int scalar) : SDL_Point({scalar, scalar}) { }
