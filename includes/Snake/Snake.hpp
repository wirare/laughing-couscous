#pragma once

#include "Error.hpp"
#include "Square.hpp"
#include <stdlib.h>
#include <iostream>
#include <random>
#include <Utility.hpp>
#include <App.hpp>

#define DIE {                           \
    do {                                \
        std::cout << "Snake died\n";    \
        dead = true;                    \
        return;                         \
    } while (0);                        \
}

#define PRINT_KEY(key) std::cout << "KEY_"#key" : " << app.inputs[KEY_##key] << std::endl;

class Snake
{
    public:
        Snake(Board& board): len(3)
        {
            Dir dirs[4] = {UP, RIGHT, DOWN, LEFT};
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution dir_dist(0, 3);

            while (true) {
                pos = board.getRandomPos();
                dir = dirs[dir_dist(gen)];

                Dir rand_dir_1 = dirs[dir_dist(gen)];
                while (rand_dir_1 == dir)
                    rand_dir_1 = dirs[dir_dist(gen)];

                Pos tail1 = pos + rand_dir_1;
                if (tail1.first < 0 || tail1.second < 0 || tail1.first >= (int)board.getSize() || tail1.second >= (int)board.getSize())
                    continue;

                Dir rand_dir_2 = dirs[dir_dist(gen)];
                Pos tail2 = tail1 + rand_dir_2;
                if (tail2.first < 0 || tail2.second < 0 || tail2.first >= (int)board.getSize() || tail2.second >= (int)board.getSize() || tail2 == pos)
                    continue;

                std::cout << "Head pos : " << pos << "\n";
                board.getSquare(pos).update(3, NEW_SNAKE);
                board.getSquare(tail1).update(2, NEW_SNAKE);
                board.getSquare(tail2).update(1, NEW_SNAKE);
				board.spawnObject(Object(RED_APPLE_OBJ));
				board.spawnObject(Object(GREEN_APPLE_OBJ));
				board.spawnObject(Object(GREEN_APPLE_OBJ));
                break;
            }
        }
        
        void move()
        {
            Pos new_pos = pos;
            Dir new_dir = dir;
            if (app.inputs[KEY_W] && dir != DOWN)
            {
                new_pos = pos + UP;
                new_dir = UP;
            }
            else if (app.inputs[KEY_S] && dir != UP)
            {
                new_pos = pos + DOWN;
                new_dir = DOWN;
            }
            else if (app.inputs[KEY_D] && dir != LEFT)
            {
                new_pos = pos + RIGHT;
                new_dir = RIGHT;
            }
            else if (app.inputs[KEY_A] && dir != RIGHT)
            {
                new_pos = pos + LEFT;
                new_dir = LEFT;
            }

            if (new_pos == pos)
				new_pos = pos + dir;

			Square *new_square;

			try {
				new_square = &app.board.getSquare(new_pos);
				if (new_square->hasSnake())
					throw SNAKE_ON_SQUARE;
			}
			catch(const std::exception& e)
				DIE;

			Object object = new_square->getObject();
			UpdateType updateType = NORMAL;
			if (object == GREEN_APPLE_OBJ)
			{
				len++;
				updateType = GROW;

			}
			else if (object == RED_APPLE_OBJ)
			{
				len--;
				updateType = SHRINK;
				if (len == 0)
					DIE;
			}
			if (updateType != NORMAL)
			{
				app.board.spawnObject(object);
				new_square->setObject(EMPTY_OBJ);
			}
			new_square->update(len, NEW_SNAKE);
			app.board.update(len, updateType, new_pos);
			pos = new_pos;
			dir = new_dir;
		}

        void printHeadInfo() const
        {
            app.board.getSquare(pos).printInfo();
        }

        bool isDead() const { return dead; }

		Pos getPos() const { return pos; }
        
		const Vision getSnakeVisionChar();
		const State getSnakeState();

		void showVision()
		{
			Vision vision = getSnakeVisionChar();
			size_t size = vision.first.size();
			size_t centerY = pos.first + 1;
			size_t centerX = pos.second + 1;

			for (size_t y = 0; y < size; y++)
			{
				for (size_t x = 0; x < size; x++)
				{
					if (x == centerX && y == centerY)
						std::cout << char(SNAKE_HEAD_CHR);
					else if (y == centerY)
						std::cout << vision.first[x];
					else if (x == centerX)
						std::cout << vision.second[y];
					else
						std::cout << ' ';
				}
				std::cout << '\n';
			}
		}

    private:
        Pos pos;
        size_t len;
        Dir dir;
        bool dead;
};
