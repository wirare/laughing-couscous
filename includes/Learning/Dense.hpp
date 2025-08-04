#pragma once

#include <Layer.hpp>

class Dense : public Layer
{
	protected:
		array W;
		array b;
	
	private:
		array X_cache;
	
	public:
		Dense(size_t in_dim, size_t out_dim)
		{
			float stddev = std::sqrt(2.0f/ static_cast<float>(in_dim));
			
			W = randn(in_dim, out_dim, f32) * stddev;
			b = constant(0, 1, out_dim);
		}

		array forward(const array& X) override
		{
			X_cache = X;
			return matmul(X, W) + b;
		}

		array backward(const array& grad_output, float lr) override
		{
			array dW = matmulTN(X_cache, grad_output);
			array db = sum(grad_output, 0);
			array dA_prev = matmul(grad_output, W.T());

			float lambda = 1e-4f;

			W -= lr * (dW + lambda * W);
			b -= lr * db;

			return dA_prev;
		}

		array& getWeights() override { return W; }
		array& getBiases() override { return b; }
		void setWeights(const array& Weights) override { W = Weights; }
		void setBiases(const array& Biases) override { b = Biases; }
		bool canLoad() const override { return true; }
		LayerType getType() const override { return DENSE; }
};
