#pragma once

#include <App.hpp>
#include <vector>

struct State
{
	std::vector<float> state;
	bool isTerminal() const;

	const float* data() const { return state.data(); }

	size_t size() const { return state.size(); }
};
