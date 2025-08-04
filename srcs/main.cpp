#include <raylib.h>
#include <Board.hpp>
#include <ReLU.hpp>
#include <Dense.hpp>
#include <Sequential.hpp>
#include <Snake.hpp>
#include <App.hpp>
#include <arrayfire.h>

Board board(10);
Snake snake(board);
App app(snake, board);

std::ostream& operator<<(std::ostream& os, Board& _board)
{
	auto chr = _board.getCharRep(app.snake.getPos());
	int i = 1;
	for (char c : chr)
	{
		os << c;
		if (i % (app.board.getSize() + 2) == 0)
			os << "\n";
		i++;
	}
	os << std::endl;
	return os;
}

void gameLoop()
{
    const float moveInterval = 0.4f;
    float moveTimer = 0.0f;

    while (!WindowShouldClose()) 
    {
        float delta = GetFrameTime();
        moveTimer += delta;

        app.pollInput();

		if (moveTimer >= moveInterval) {
			app.snake.move();
			app.snake.showVision();
			moveTimer = 0.0f;
		}

        app.board.drawBoard();

        if (app.snake.isDead())
            break;
    }
}

int main()
{
	af::setBackend(AF_BACKEND_CUDA);
	af::info();
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(WIDTH, HEIGHT, "Learn2Slither");
    SetTargetFPS(60);

    gameLoop();

    CloseWindow();
    return 0;
}
