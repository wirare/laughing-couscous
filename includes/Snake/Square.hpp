#pragma once

#include <Utility.hpp>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <raylib.h>

#define NUM_OBJ 3

typedef enum {
    EMPTY_OBJ = 0b01,
    RED_APPLE_OBJ = 0b10,
    GREEN_APPLE_OBJ = 0b11
}   ObjectType;

typedef enum {
	EMPTY_CHR = '0',
	SNAKE_HEAD_CHR = 'H',
	WALL_CHR = 'W',
	SNAKE_BODY_CHR = 'S',
	GREEN_APPLE_CHR = 'G',
	RED_APPLE_CHR = 'R'
}	SqrChar;

struct Object
{
    static constexpr Color colorTable[NUM_OBJ] = {GRAY, //EMPTY
                                                  RED, //RED_APPLE
                                                  GREEN};//GREEN_APPLE
	static constexpr SqrChar charTable[NUM_OBJ] = {EMPTY_CHR, RED_APPLE_CHR, GREEN_APPLE_CHR};
    Object(ObjectType type): type(type)
    {
        color = colorTable[type-1];
		c = charTable[type-1];
    }
    Color color;
    ObjectType type;
	SqrChar c;
    bool operator==(ObjectType _type) { if (type == _type) return 1; return 0; }
};

inline std::ostream& operator<<(std::ostream& os, const Object& obj)
{
    os << "{ObjectType : ";
    switch (obj.type)
    {
        case (EMPTY_OBJ): os << "EMPTY}"; break;
        case (RED_APPLE_OBJ): os << "RED_APPLE}"; break;
        case (GREEN_APPLE_OBJ): os << "GREEN_APPLE}"; break;
    }
    return os; 
}

typedef enum {
    GROW = 0,
    NORMAL = 1,
    SHRINK = 2,
    NEW_SNAKE = 3,
	RESET = 4
}   UpdateType;

class Square
{
    public:
        Square(): pos(0, 0), snake_timer(-1), object(EMPTY_OBJ) {};
        Square(uint x, uint y): pos(x, y), snake_timer(-1), object(EMPTY_OBJ) {}
        Square(uint x, uint y, Object& object) : pos(x, y), snake_timer(-1), object(object) {}
        ~Square() = default;

        Object getObject() const { return object; }
        void setObject(const Object& obj) 
		{ 
			//printInfo();
			object=obj;
		}

        Color getColor() const
        {
            if (snake_timer >= 1)
                return LIME;
            else
                return object.color;
        }

        bool hasSnake() const { return snake_timer >= 0; }

        uint getTimer() const { return snake_timer; }
        void update(uint new_timer, UpdateType updateType)
        {
            if (snake_timer < 0 && updateType != NEW_SNAKE)
                return;
            if (updateType == NEW_SNAKE)
            {
                if (snake_timer <= 0)
                    snake_timer = new_timer;
                return;
            }
            int minus_coef = updateType;
            snake_timer -= minus_coef;
            if (snake_timer <= 0)
            {
                snake_timer = -1;
                object = Object(EMPTY_OBJ);
            }
        }

        void printInfo() const
        {
            std::cout << "Square at pos : " << pos << "\nObject : " << object << "\nSnake_timer : " << snake_timer << std::endl;
		}

		Pos getPos() const { return pos; }

		char getChar() const
		{
			if (hasSnake())
				return SNAKE_BODY_CHR;
			return object.c;
		}

		void reset()
		{
			snake_timer = -1;
			object = EMPTY_OBJ;
		}

    private:
        Pos pos;
        int snake_timer;
        Object object;
};
