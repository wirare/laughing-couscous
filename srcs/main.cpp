#include <af/defines.h>
#include <raylib.h>
#include <Board.hpp>
#include <ReLU.hpp>
#include <Dense.hpp>
#include <Sequential.hpp>
#include <Snake.hpp>
#include <App.hpp>
#include <arrayfire.h>
#include <NN.hpp>

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
/*
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
			if (app.inputs[KEY_R])
				app.reset_environment();
			if (app.inputs[KEY_ESCAPE])
				break;
			moveTimer = 0.0f;
		}

        app.board.drawBoard();

        if (app.snake.isDead())
            app.reset_environment();
    }
}
*/

void init_model(Sequential& model)
{
	model.add(std::make_unique<Dense>(Dense(23, 64)));
	model.add(std::make_unique<ReLU>());
	model.add(std::make_unique<Dense>(Dense(64, 32)));
	model.add(std::make_unique<ReLU>());
	model.add(std::make_unique<Dense>(Dense(32, 4)));
}

int main()
{
	af::setBackend(AF_BACKEND_CUDA);
	af::info();
    SetTraceLogLevel(LOG_WARNING);
	bool draw = false;
	if (draw)
	{
		InitWindow(WIDTH, HEIGHT, "Learn2Slither");
		SetTargetFPS(60);
	}

    Sequential model;

	init_model(model);
	training(model, 0.0001f, "First_Session.lts", draw);

    if (draw) CloseWindow();
    return 0;
}
