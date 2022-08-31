"""Unit test for dummy envpool and speed benchmark."""

import os
import time

import numpy as np
from absl import logging
from absl.testing import absltest

# from minesweeper_envpool import _MineSweeperEnvPool, _MineSweeperEnvSpec
from minesweeper_envpool import _MineSweeperEnvPool, _MineSweeperEnvSpec


class _MineSweeperEnvPoolTest(absltest.TestCase):

    def test_config(self) -> None:
        ref_config_keys = [
            "num_envs",
            "batch_size",
            "num_threads",
            "max_num_players",
            "thread_affinity_offset",
            "base_path",
            "seed",
            "gym_reset_return_info",
            "rows",
            "columns",
            "mines",
        ]
        default_conf = _MineSweeperEnvSpec._default_config_values
        self.assertTrue(isinstance(default_conf, tuple))
        config_keys = _MineSweeperEnvSpec._config_keys
        self.assertTrue(isinstance(config_keys, list))
        self.assertEqual(len(default_conf), len(config_keys))
        self.assertEqual(sorted(config_keys), sorted(ref_config_keys))

    # def test_envpool(self) -> None:
    #     conf = dict(
    #         zip(_MineSweeperEnvSpec._config_keys,
    #             _MineSweeperEnvSpec._default_config_values)
    #     )
    #     conf["num_envs"] = num_envs = 10
    #     conf["batch_size"] = batch = 5
    #     conf["num_threads"] = os.cpu_count()
    #     env_spec = _MineSweeperEnvSpec(tuple(conf.values()))
    #     env = _MineSweeperEnvPool(env_spec)
    #     state_keys = env._state_keys
    #     total = 100000
    #     env._reset(np.arange(num_envs, dtype=np.int32))
    #     t = time.time()
    #     for _ in range(total):
    #         state = dict(zip(state_keys, env._recv()))
    #         action = {
    #             "env_id": state["info:env_id"],
    #             "players.env_id": state["info:players.env_id"],
    #             "players.id": state["info:players.id"],
    #             "players.action": state["info:players.id"],
    #         }
    #         env._send(tuple(action.values()))
    #     duration = time.time() - t
    #     fps = total * batch / duration
    #     logging.info(f"FPS = {fps:.6f}")


if __name__ == "__main__":
    absltest.main()
