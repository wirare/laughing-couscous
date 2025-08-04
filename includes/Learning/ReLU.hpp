#pragma once

#include <Layer.hpp>

class ReLU : public Layer
{
	private:
		array mask_cache;

	public:
		ReLU() = default;

		array forward(const array& X) override 
		{
			mask_cache = (X > 0);
			return X * mask_cache;
		}

		array backward(const array& grad_output, float /*lr*/) override
		{
			return grad_output * mask_cache;
		}

		array& getWeights() override { throw; }
		array& getBiases() override { throw; }
		void setWeights(const array&) override { throw; }
		void setBiases(const array&) override { throw; }
		bool canLoad() const override { return false; }
		LayerType getType() const override { return RELU; }
};
