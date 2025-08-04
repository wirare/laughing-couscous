#pragma once

#include <arrayfire.h>

using namespace af;

typedef enum {
	BASE = 0,
	DENSE = 1,
	RELU = 2
}	LayerType;

class Layer
{
	public:
		virtual array forward(const array& x) = 0;
		virtual array backward(const array& grad_output, float lr) = 0;
		virtual ~Layer() = default;

		virtual array& getWeights() = 0;
		virtual array& getBiases() = 0;
		virtual void setWeights(const array& W) = 0;
		virtual void setBiases(const array& b) = 0;
		virtual bool canLoad() const = 0;
		virtual LayerType getType() const = 0;
};
