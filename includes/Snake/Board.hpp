#pragma once

#include <vector>
#include <stdlib.h>
#include <Square.hpp>
#include <Error.hpp>
#include <random>
#include <raylib.h>
#include <iostream>

class Board
{
    public:
        Board(size_t size): size(size)
        {
            for (size_t i = 0; i < size; i++)
                for (size_t j = 0; j < size; j++)
                    Squares.push_back(Square(i, j));
        }

        ~Board() = default;

        void update(uint timer, UpdateType type, Pos head)
        {
            for (Square& square : Squares)
			{
				if (!square.hasSnake())
					continue;
				if (square.getPos() == head)
					continue;
				square.update(timer, type);
			}
        }

        size_t getSize() const { return size; }

        Square& getSquare(size_t x, size_t y)
        {
            if (x >= size || y >= size)
                throw OUT_OF_BOUNDS;
            
            return Squares[y + x * size];
        }

        Square& getSquare(const Pos& pos)
        {
            return getSquare(pos.first, pos.second);
        }

        Pos getRandomPos()
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_int_distribution pos_dist(0, (int)size - 1);

            return std::make_pair(pos_dist(gen), pos_dist(gen));
        }

        void drawBoard() const
        {
            BeginDrawing();
            ClearBackground(BLACK);

            int square_size = std::min(HEIGHT, WIDTH);
            int tile_size = square_size / size;
            float hsize = (float)size / 2;
            int offset = hsize * tile_size;
            int start_x = HWIDTH - offset;
            int start_y = HHEIGHT - offset;

            size_t i = 0;
            size_t j = 0;
            for (const Square square : Squares)
            {
                DrawRectangle(start_x + i * tile_size, start_y + j * tile_size, tile_size, tile_size, square.getColor());
                i++;
                if (i != 0 && i % size == 0)
                {
                    i = 0;
                    j++;
                }
            }
			for (i = 0; i < size; i++)
			{
				DrawLine(start_x + i * tile_size, start_y, start_x + i * tile_size, HHEIGHT + offset, BLACK);
				DrawLine(start_x, start_y + i * tile_size, HWIDTH + offset, start_y + i * tile_size, BLACK);
			}
            EndDrawing();
        }

		void spawnObject(const Object& obj)
		{
			Pos current_pos = getRandomPos();

			while (true)
			{
				Square& square = getSquare(current_pos);
				if (square.getObject().type == EMPTY_OBJ && !square.hasSnake())
				{
					square.setObject(obj);
					break;
				}

				current_pos = getRandomPos();
			}
		}

		std::vector<char> getCharRep(Pos head)
		{
			std::vector<char> charRep;

			for (size_t i = 0; i < size + 2; i++)
				charRep.push_back(WALL_CHR);

			for (size_t row = 0; row < size; row++)
			{
				charRep.push_back(WALL_CHR);
				for (size_t col = 0; col < size; col++)
				{
					Pos currentPos{ (int)row, (int)col };
					Square& square = getSquare(currentPos);
					if (currentPos == head)
						charRep.push_back(SNAKE_HEAD_CHR);
					else
						charRep.push_back(square.getChar());
				}
				charRep.push_back(WALL_CHR);
			}

			for (size_t i = 0; i < size + 2; i++)
				charRep.push_back(WALL_CHR);

			return charRep;
		}

    private:
        const size_t size;
        std::vector<Square> Squares;
};
