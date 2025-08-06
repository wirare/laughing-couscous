#pragma once

#include <Layer.hpp>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>
#include <Error.hpp>
#include <Dense.hpp>
#include <ReLU.hpp>

#define WRITE(dest, x, size) dest.write(reinterpret_cast<const char*>(x), size)
#define READ(src, x, size) src.read(reinterpret_cast<char*>(x), size)

class Sequential
{
	private:
		std::vector<std::unique_ptr<Layer>> layers;
	
	public:
		void add(std::unique_ptr<Layer> layer) {
			layers.push_back(std::move(layer));
		}

		array forward(const array& input)
		{
			array out = input;
			for (auto& layer : layers)
				out = layer->forward(out);
			return out;
		}

		array backward(const array& grad_output, float lr)
		{
			array grad = grad_output;
			for (auto it = layers.rbegin(); it != layers.rend(); it++)
				grad = (*it)->backward(grad, lr);
			return grad;
		}

		void save_model(const std::string& filename)
		{
			std::ofstream ofs(filename, std::ios::binary);
			if (!ofs) throw FAIL_O_S_MODEL;

			size_t L = layers.size();
			WRITE(ofs, &L, sizeof(size_t));
			for (const auto& layer : layers)
			{
				LayerType t = layer->getType();
				WRITE(ofs, &t, sizeof(LayerType));
			}
			for (const auto& layer : layers)
			{
				if (!layer->canLoad()) continue;

				const array W = layer->getWeights();

				dim4 d = W.dims();
				uint32_t ndims = 2;
				uint32_t shape[2] = { uint32_t(d[0]), uint32_t(d[1]) };

				size_t N = W.elements();
				std::vector<float> Weights(N);
				W.host(Weights.data());

				LayerType t = layer->getType();
				WRITE(ofs, &t, sizeof(LayerType));
				
				WRITE(ofs, &ndims, sizeof(ndims));
				WRITE(ofs, shape, sizeof(shape));

				WRITE(ofs, Weights.data(), N * sizeof(float));

				const array b = layer->getBiases();
				size_t bias_size = b.elements();
				std::vector<float> Biases(bias_size);
				b.host(Biases.data());

				WRITE(ofs, &bias_size, sizeof(size_t));
				WRITE(ofs, Biases.data(), bias_size * sizeof(float));
			}
		}

		void load_model(const std::string& filename)
		{
			std::ifstream ifs(filename, std::ios::binary);
			if (!ifs) throw FAIL_O_L_MODEL;

			size_t total_layers = 0;
			READ(ifs, &total_layers, sizeof(total_layers));

			std::vector<LayerType> types(total_layers);
			for (size_t i = 0; i < total_layers; i++)
				READ(ifs, &types[i], sizeof(LayerType));

			for (LayerType t : types)
			{
				switch (t) 
				{
					case LayerType::DENSE:
					{
						LayerType ltype;
						READ(ifs, &ltype, sizeof(LayerType));
						if (ltype != t)
							throw LAYER_TYPE_ERROR;

						uint32_t ndims = 0;
						READ(ifs, &ndims, sizeof(ndims));
						if (ndims != 2) throw DIM_ERR_DENSE;

						uint32_t shape[2];
						READ(ifs, shape, sizeof(shape));
						size_t in_dim = shape[0];
						size_t out_dim = shape[1];

						size_t N = in_dim * out_dim;
						std::vector<float> Weights(N);
						READ(ifs, Weights.data(), N * sizeof(float));

						auto dense = std::make_unique<Dense>(in_dim, out_dim);
						array W(dim4(in_dim, out_dim), Weights.data());
						dense->setWeights(W);

						size_t bias_size;
						READ(ifs, &bias_size, sizeof(size_t));
						if (bias_size != out_dim) throw DIM_ERR_DENSE;
						std::vector<float> Biases(bias_size);
						READ(ifs, Biases.data(), bias_size * sizeof(float));
						array b(dim4(1, out_dim), Biases.data());
						dense->setBiases(b);

						add(std::move(dense));
					} break;

					case LayerType::RELU:
					{
						add(std::make_unique<ReLU>());
					} break;

					default:
						throw UNKNOWN_LAYER;
				}
			}
			if (ifs.fail() && !ifs.eof())
				throw READING_ERR;
		}
};
