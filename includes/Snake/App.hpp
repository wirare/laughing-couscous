#pragma once

#include <raylib.h>
#include <Board.hpp>
#include <Utility.hpp>

class Snake;

struct App
{
    App();
    App(Snake& snake, Board& board): snake(snake), board(board) {}

    Snake& snake;
    Board& board;
    bool inputs[512] = { false };

    void pollInput() {
        for (int key = 0; key < 512; key++)
        {
            inputs[key] = IsKeyDown((KeyboardKey)key);
            //if (inputs[key])
            //    std::cout << "Key : " << key << " is pressed.\n";
        }
    }
};

extern App app;
