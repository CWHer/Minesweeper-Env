#pragma once

#include "envpool/core/async_envpool.h"
#include "envpool/core/env.h"

#include <iostream>

namespace MineSweeper
{

    // env-specific definition of config and state/action spec
    class MineSweeperEnvFns
    {
    public:
        static decltype(auto) DefaultConfig()
        {
            return MakeDict(
                "rows"_.Bind(16), "columns"_.Bind(16),
                "mines"_.Bind(40));
        }

        template <typename Config>
        static decltype(auto) StateSpec(const Config &conf)
        {
            return MakeDict(
                "obs"_.Bind(Spec<int>(
                    {conf["rows"_], conf["columns"_]},
                    {-1, 8})),
                "info:cheat"_.Bind(Spec<int>(
                    {conf["rows"_], conf["columns"_]},
                    {-1, 8})));
        }

        template <typename Config>
        static decltype(auto) ActionSpec(const Config &conf)
        {
            return MakeDict("action"_.Bind(Spec<int>(
                {-1}, {0, conf["rows"_] * conf["columns"_] - 1})));
        }
    };

    // this line will concat common config and common state/action spec
    using MineSweeperEnvSpec = EnvSpec<MineSweeperEnvFns>;

    class MineSweeperEnv : public Env<MineSweeperEnvSpec>
    {
    protected:
        const int rows_, columns_, mines_;
        int n_steps_; // remaining steps to end the game
        std::vector<int> map_;
        std::vector<int> masks_;
        bool is_first_step_;
        bool done_;

    private:
        void displayMap()
        {
            std::cout << "rows: " << rows_
                      << ", columns: " << columns_
                      << ", mines: " << mines_ << std::endl;
            std::cout << "Actual Map\n";
            for (int i = 0; i < rows_; ++i)
            {
                for (int j = 0; j < columns_; ++j)
                {
                    auto x = map_[toIdx(i, j)];
                    x == -1 ? std::cout << "*" : std::cout << x;
                    std::cout << ' ';
                }
                std::cout << '\n';
            }
            std::cout << "Visible Map\n";
            for (int i = 0; i < rows_; ++i)
            {
                for (int j = 0; j < columns_; ++j)
                {
                    !masks_[toIdx(i, j)]
                        ? std::cout << "#"
                        : std::cout << map_[toIdx(i, j)];
                    std::cout << ' ';
                }
                std::cout << '\n';
            }
            std::cout << std::endl;
        }

        void writeState(float reward)
        {
#ifdef ENVPOOL_TEST
            displayMap();
#endif

            State state = Allocate();
            std::vector<int> visible_map(rows_ * columns_, -1);
            // clang-format off
            for (size_t i = 0; i < visible_map.size(); ++i) 
                if (masks_[i]) visible_map[i] = map_[i];
            // clang-format on
            for (int idx = 0, i = 0; i < rows_; ++i)
                for (int j = 0; j < columns_; ++j, ++idx)
                {
                    state["obs"_](i, j) = visible_map[idx];
                    state["info:cheat"_](i, j) = map_[idx];
                }
            state["reward"_] = reward;
        }

        inline int toIdx(int row, int col)
        {
            return row * columns_ + col;
        }

        inline std::tuple<int, int> toCoord(int idx)
        {
            return std::make_tuple(idx / columns_, idx % columns_);
        }

        void generateMap(int action)
        {
            std::vector<int> mine_locs;
            mine_locs.reserve(rows_ * columns_ - 1);
            // clang-format off
            for (int i = 0; i < rows_ * columns_; i++)
                if (i != action) mine_locs.push_back(i);
            // clang-format on
            std::shuffle(mine_locs.begin(), mine_locs.end(), gen_);
            for (int i = 0; i < mines_; i++)
                map_[mine_locs[i]] = -1;

            std::vector<std::vector<int>> prefix_sum(
                rows_, std::vector<int>(columns_, 0));
            for (int i = 0; i < rows_; i++)
                for (int j = 0; j < columns_; j++)
                    prefix_sum[i][j] = (map_[toIdx(i, j)] == -1) +
                                       (i > 0 ? prefix_sum[i - 1][j] : 0) +
                                       (j > 0 ? prefix_sum[i][j - 1] : 0) -
                                       (i > 0 && j > 0 ? prefix_sum[i - 1][j - 1] : 0);
            for (int i = 0; i < rows_; i++)
                for (int j = 0; j < columns_; j++)
                    if (map_[toIdx(i, j)] != -1)
                        map_[toIdx(i, j)] = prefix_sum[std::min(i + 1, rows_ - 1)][std::min(j + 1, columns_ - 1)] +
                                            (i > 2 && j > 2 ? prefix_sum[i - 2][j - 2] : 0) -
                                            (i > 2 ? prefix_sum[i - 2][std::min(j + 1, columns_ - 1)] : 0) -
                                            (j > 2 ? prefix_sum[std::min(i + 1, rows_ - 1)][j - 2] : 0);
        }

        bool expandMap(int index)
        {
            if (masks_[index])
                return false; // already expanded

            if (map_[index] == -1)
                return true; // boom!!!
            masks_[index] = 1;
            n_steps_--;

            if (map_[index] == 0)
            {
                static const int n_dirs = 8;
                static const int dx[] = {-1, 0, 1, -1, 1, -1, 0, 1};
                static const int dy[] = {-1, -1, -1, 0, 0, 1, 1, 1};

                auto [x, y] = toCoord(index);
                for (int i = 0; i < n_dirs; ++i)
                {
                    auto [nx, ny] = std::make_tuple(x + dx[i], y + dy[i]);
                    if (nx >= 0 && nx < rows_ &&
                        ny >= 0 && ny < columns_)
                        expandMap(toIdx(nx, ny));
                }
            }

            return false;
        }

    public:
        MineSweeperEnv(const Spec &spec, int env_id)
            : Env<MineSweeperEnvSpec>(spec, env_id),
              rows_(spec.config["rows"_]),
              columns_(spec.config["columns"_]),
              mines_(spec.config["mines"_]),
              n_steps_(rows_ * columns_ - mines_),
              map_(rows_ * columns_, 0),
              masks_(rows_ * columns_, 0),
              is_first_step_(false), done_(true) {}

        bool IsDone() override { return done_; }

        void Reset() override
        {
            n_steps_ = rows_ * columns_ - mines_;
            std::fill(map_.begin(), map_.end(), 0);
            std::fill(masks_.begin(), masks_.end(), 0);
            is_first_step_ = true;
            done_ = false;
            writeState(0.0);
        }

        void Step(const Action &action) override
        {
            int act = action["action"_];
            int last_step = n_steps_;

            if (is_first_step_)
            {
                // HACK: generate map
                generateMap(act);
                is_first_step_ = false;
            }

            done_ = masks_[act] || expandMap(act) || n_steps_ == 0;
            writeState(step_reward_ * (last_step - n_steps_) +
                       final_reward_ * done_ * (n_steps_ == 0 ? 1.0 : -1.0));
        }
    };

    using MineSweeperEnvPool = AsyncEnvPool<MineSweeperEnv>;

};