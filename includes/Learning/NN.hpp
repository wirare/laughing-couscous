#pragma once

#include <Utility.hpp>
#include <arrayfire.h>
#include <Sequential.hpp>
#include <State.hpp>

using namespace af;

struct Experience 
{
	State state;
	Dir action;
	Reward reward;
	State next_state;
	bool done;
};

inline array state_to_tensor(const State& s)
{
	return array(1, s.size(), s.data());
}

 Dir epsilon_greedy(Sequential& model, const State& s, float epsilon);
 std::pair<State, Reward> take_action(Dir action);

void training(Sequential& model, float lr, const std::string& save_path, const bool draw = false);
