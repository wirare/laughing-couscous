#include <NN.hpp>
#include <random>
#include <af/algorithm.h>
#include <App.hpp>
#include <Snake.hpp>

Dir epsilon_greedy(Sequential& model, const State &s, float epsilon)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution rand(0, 100);
	static Dir actions[4] = {UP, DOWN, LEFT, RIGHT};

	//(void) model;
	//(void) s;
	if (rand(gen) < (epsilon * 100))
		return actions[rand(gen) % 4];
	else
	{
		array q_value = model.forward(state_to_tensor(s));
		array max_vals, max_indices;
		max(max_vals, max_indices, q_value);
		return actions[max_indices.as(s32).scalar<int>()];
		return actions[0];
	}
};
 
std::pair<State, Reward> take_action(Dir action)
{
	if (action == UP)
		app.inputs[KEY_W] = true;
	else if (action == DOWN)
		app.inputs[KEY_S] = true;
	else if (action == LEFT)
		app.inputs[KEY_A] = true;
	else
		app.inputs[KEY_D] = true;

	Reward reward = app.snake.move();
	State ns = app.snake.getSnakeState();

	app.inputs[KEY_W] = false;
	app.inputs[KEY_S] = false;
	app.inputs[KEY_A] = false;
	app.inputs[KEY_D] = false;
	return {ns, reward};
}
