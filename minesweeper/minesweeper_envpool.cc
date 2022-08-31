#include "envpool/minesweeper/minesweeper_envpool.hpp"
#include "envpool/core/py_envpool.h"

using MineSweeperEnvSpec = PyEnvSpec<MineSweeper::MineSweeperEnvSpec>;
using MineSweeperEnvPool = PyEnvPool<MineSweeper::MineSweeperEnvPool>;

// generate *.so
PYBIND11_MODULE(minesweeper_envpool, m)
{
    REGISTER(m, MineSweeperEnvSpec, MineSweeperEnvPool)
}