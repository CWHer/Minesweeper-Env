from envpool.python.api import py_env

from .minesweeper_envpool import _MineSweeperEnvPool, _MineSweeperEnvSpec

MineSweeperEnvSpec, MineSweeperDMEnvPool, \
    MineSweeperGymEnvPool = py_env(_MineSweeperEnvSpec, _MineSweeperEnvPool)

__all__ = [
    "MineSweeperEnvSpec",
    "MineSweeperDMEnvPool",
    "MineSweeperGymEnvPool",
]
