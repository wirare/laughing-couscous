#pragma once

#include <raylib.h>
#include <Board.hpp>
#include <Utility.hpp>

class Snake;

struct App
{
    App(Snake& snake, Board& board): snake(snake), board(board){};

	void reset_environment();

    Snake& snake;
    Board& board;
    bool inputs[512] = { false };

    void pollInput()
	{
        for (int key = 0; key < 512; key++)
            inputs[key] = IsKeyDown((KeyboardKey)key);
    }
};

extern App app;
