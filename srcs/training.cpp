#include <NN.hpp>
#include <Sequential.hpp>
#include <Utility.hpp>
#include <ReplayBuffer.hpp>
#include <App.hpp>
#include <Snake.hpp>
#include <climits>
#include <cstdlib>

void training(Sequential& model, float lr, const std::string& save_path, bool draw)
{
    ReplayBuffer replay(10000);
    float epsilon = 1.0f;
    float epsilon_min = 0.05f;
    float epsilon_decay = 0.9995f;
    float gamma = 0.99f;
    size_t batch_size = 32;
    size_t episode_num = 1000;
    int previous_reward = INT_MIN;

    for (size_t episode = 0; episode < episode_num; ++episode)
    {
        app.reset_environment();
        State state = app.snake.getSnakeState();
        int total_reward = 0;

        // Loss accumulators
        double loss_sum = 0.0;
        size_t loss_count = 0;

        while (!state.isTerminal())
        {
            Dir action = epsilon_greedy(model, state, epsilon);
            auto [next_state, reward] = take_action(action);
            if (draw) app.board.drawBoard();

            replay.add({state, action, reward, next_state, next_state.isTerminal()});
            state = next_state;
            total_reward += reward;

            if (replay.is_ready(batch_size))
            {
                auto batch = replay.sample(batch_size);

                for (const Experience& exp : batch)
                {
                    // 1) Forward current Qs
                    array current_qs = model.forward(state_to_tensor(exp.state));

                    // 2) Compute our TD target
                    float target = exp.reward;
                    if (!exp.done)
                    {
                        array next_qs = model.forward(state_to_tensor(exp.next_state));
                        float max_next_q = max<float>(next_qs);
                        target += gamma * max_next_q;
                    }

                    // 3) Compute error for that action
                    int action_idx = dir_to_idx(exp.action);
                    float pred = current_qs(0, action_idx).scalar<float>();
                    float error = pred - target;

                    // 4) Accumulate squared error
                    loss_sum   += error * error;
                    loss_count += 1;

                    // 5) Build gradient and backpropagate
                    array grad_output = constant(0, current_qs.dims(), f32);
					float grad = std::abs(error) <= 1.0f ? error : ((error > 0) ? 1.0f : -1.0f);
                    grad_output(0, action_idx) = grad;
                    model.backward(grad_output, lr);
                }
            }
        }

        // Decay epsilon
        epsilon = std::max(epsilon * epsilon_decay, epsilon_min);

        // Compute mean loss for this episode
        double mean_loss = (loss_count > 0) ? (loss_sum / loss_count) : 0.0;

        // Log to stdout and stderr
        std::cout << "Episode " << episode << ": Reward = " << total_reward << "\n";
        std::cerr << "Episode " << episode << ": Loss   = " << mean_loss  << "\n";

        // Save best model
        if (total_reward > previous_reward)
        {
            previous_reward = total_reward;
            model.save_model(save_path);
        }
    }
}

