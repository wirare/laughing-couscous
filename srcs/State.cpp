#include <State.hpp>
#include <Snake.hpp>
#include <App.hpp>

bool State::isTerminal() const { return app.snake.isDead() || app.snake.getLen() >= 15; }
