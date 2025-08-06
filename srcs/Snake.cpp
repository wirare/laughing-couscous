#include <Snake.hpp>
#include <App.hpp>
#include <algorithm>

#define CASE_PUSH(c, x) case c: state.state.push_back(x); break

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

    auto normalize = [](int x) {
        return float(x) / 3.0f;
    };

    for (char c : vision.first)
    {
        int v = 0;
        switch (c)
        {
            case EMPTY_CHR:       v =  0; break;
            case SNAKE_HEAD_CHR:  v =  1; break;
            case WALL_CHR:        v =  3; break;
            case SNAKE_BODY_CHR:  v =  2; break;
            case GREEN_APPLE_CHR: v = -1; break;
            case RED_APPLE_CHR:   v = -2; break;
        }
        state.state.push_back(normalize(v));
    }

    for (char c : vision.second)
    {
        int v = 0;
        switch (c)
        {
            case EMPTY_CHR:       v =  0; break;
            case WALL_CHR:        v =  3; break;
            case SNAKE_BODY_CHR:  v =  2; break;
            case GREEN_APPLE_CHR: v = -1; break;
            case RED_APPLE_CHR:   v = -2; break;
            case SNAKE_HEAD_CHR:              // skip, already counted
                continue;
        }
        state.state.push_back(normalize(v));
    }
    return state;
}
