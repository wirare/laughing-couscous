#include <Snake.hpp>
#include <App.hpp>
#include <algorithm>

#define CASE_PUSH(c, x) case c: state.push_back(x); break

const Vision Snake::getSnakeVisionChar()
{
    Vision vision;
    std::vector<char> charBoard = app.board.getCharRep(pos);
    size_t boardSize = app.board.getSize();
    size_t stride = boardSize + 2;
    size_t row = pos.first + 1;
    size_t col = pos.second + 1;

    std::copy_n(charBoard.begin() + row * stride, stride, std::back_inserter(vision.first));

    for (size_t i = 0; i < stride; ++i)
        vision.second.push_back(charBoard[i * stride + col]);

    return vision;
}

const State Snake::getSnakeState()
{
	State state;
	Vision vision = getSnakeVisionChar();

	for (char c : vision.first)
	{
		switch (c)
		{
			CASE_PUSH(EMPTY_CHR, 0);
			CASE_PUSH(SNAKE_HEAD_CHR, 1);
			CASE_PUSH(WALL_CHR, 3);
			CASE_PUSH(SNAKE_BODY_CHR, 2);
			CASE_PUSH(GREEN_APPLE_CHR, -1);
			CASE_PUSH(RED_APPLE_CHR, -2);
		}
	}	
	for (char c : vision.second)
	{
		switch (c)
		{
			CASE_PUSH(EMPTY_CHR, 0);
			CASE_PUSH(WALL_CHR, 3);
			CASE_PUSH(SNAKE_BODY_CHR, 2);
			CASE_PUSH(GREEN_APPLE_CHR, -1);
			CASE_PUSH(RED_APPLE_CHR, -2);
			case SNAKE_HEAD_CHR: break;
		}
	}

	return state;
}
