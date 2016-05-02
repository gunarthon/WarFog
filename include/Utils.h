#ifndef UTILS_H
#define UTILS_H

#include <SFML/Graphics.hpp>
#include "Box2D/Box2D.h"
#include <math.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdlib.h>

#define PI 3.1415926535897
typedef float Num;
typedef std::string String;
typedef std::vector<b2Vec2> VectorList;
typedef std::vector< std::vector<String> > FileContents;
typedef std::vector<String>  StringList;

typedef union
{
    Num num;
    String text;
}Value;

enum ActionId
{
    DEFAULT_ACTION,
    NEW_RECT,
    NEW_CIRCLE,
    NEW_TRIANGLE,
    NEW_FIXED_JOINT,
    NEW_REVOLUTE_JOINT,
    NEW_PISTON,

    NEW_CHALLENGE_BOX,
    NEW_END_BOX,
    NEW_BUFFER_BOX,
    NEW_NOT_BOX,
    NEW_AND_BOX,
    NEW_OR_BOX,
    NEW_XOR_BOX,

    ROTATE,
    PASTE,
    BOX_SELECTING,
    FINALIZING_JOINT,
    NEW_TEXT,
    RESIZING_TEXT,
    RESIZING_SHAPES,
    NEW_THRUSTER,
    DRAWING_BOX,
    DRAWING_HORIZONTAL_LINE,
    DRAWING_VERTICAL_LINE,
    SELECTING_SHAPE,
    DRAWING_BUILD_BOX,
    NEW_CANNON,
};

class Utils
{
public:
    Num StrToNum(String text)
    {
        return std::atof(text.c_str());
    }
    sf::Keyboard::Key StrToInput(String text)
    {
        int key;
        std::stringstream s(text);
        s >> key;
        if(key >= sf::Keyboard::KeyCount)
            return sf::Keyboard::KeyCount;
        return sf::Keyboard::Key(key);
    }

    String ToString(bool value)
    {
        return value ? "1" : "0";
    }

    String InputToString(sf::Keyboard::Key *key)
    {
        std::stringstream s;
        s << *key;
        return s.str();
    }

    String ToString(Num value)
    {
        std::stringstream s;
        s << value;
        return s.str();
    }
    String ToString(int value)
    {
        std::stringstream s;
        s << value;
        return s.str();
    }
    sf::Color Darken(sf::Color color)
    {
        color.r -= 20;
        color.g -= 20;
        color.b -= 20;
        return color;
    }
    float GetLength(sf::Vector2f vec)
    {
        return std::sqrt(GetLength2(vec));
    }

    float GetLength2(sf::Vector2f vec)
    {
        return (vec.x * vec.x + vec.y * vec.y);
    }

    sf::Vector2f Normalize(sf::Vector2f vec)
    {
        float length = GetLength(vec);
        if(length != 0)
            return (vec / GetLength(vec));
        return vec;
    }

    Num NormalizeAngle(Num angle, bool rad = false)
    {
        if(rad)
        {
            while (angle >= 2*PI) angle -= 2*PI;
            while (angle < 0) angle += 2*PI;
            return angle;
        }
        while (angle >= 360) angle -= 360;
        while (angle < 0) angle += 360;
        return angle;
	}

	Num RadToDeg(Num radian)
	{
        Num degree = 0;
        degree = radian * (180/PI);
        return degree;
	}

	Num DegToRad(Num degree)
	{
        Num radian = 0;
        radian = degree * (PI/180);
        return radian;
	}

    float DotProduct(sf::Vector2f vec1, sf::Vector2f vec2)
    {
        return (vec1.x * vec2.x + vec1.y * vec2.y);
    }

    float GetAngle(sf::Vector2f vec1, sf::Vector2f vec2)
    {
        float angle = DotProduct(vec1, vec2);
        float d = std::sqrt(GetLength2(vec1) * GetLength2(vec2));
        if(d)
            angle /= d;
        return (angle);
    }
    bool SegmentsIntersect(Num x1, Num y1, Num x2, Num y2, Num x3, Num y3, Num x4, Num y4)
    {
		if ((x1 == x3 && y1 == y3) || (x1 == x4 && y1 == y4) || (x2 == x3 && y2 == y3) || (x2 == x4 && y2 == y4)) {
			return false;
		}

			// avoids divide by zero errors
		if (std::abs(x1 - x2) < 0.00001)
			x2 += 0.0001;
		Num m1 = (y2 - y1) / (x2 - x1);

		if(std::abs(x3 - x4) < 0.00001)
			x4 += 0.0001;
		Num m2 = (y4 - y3) / (x4 - x3);

		if (m1 == m2)
            return false;

		// compute slope and y-intercept of both lines, then set y1 = y2, or m1x + b1 = m2x + b2, or x = (b2 - b1) / (m1 - m2)
		Num b1 = y1 - x1 * m1;
		Num b2 = y3 - x3 * m2;
		Num intersectX = (b2 - b1) / (m1 - m2);

		if(((intersectX >= x1 && intersectX <= x2) || (intersectX <= x1 && intersectX >= x2)) &&
			((intersectX >= x3 && intersectX <= x4) || (intersectX <= x3 && intersectX >= x4)))
			return true;

		return false;
	}
	Num Abs(Num x)
	{
	    return x < 0 ? -x : x;
	}
};


#endif
