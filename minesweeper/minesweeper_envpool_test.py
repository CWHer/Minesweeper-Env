"""Unit test for minesweeper envpool and speed benchmark."""

import os
import time

import numpy as np
from absl import logging
from absl.testing import absltest
from tqdm import tqdm

# @fmt off
from envpool.minesweeper.minesweeper_envpool \
    import _MineSweeperEnvPool, _MineSweeperEnvSpec
# @fmt on


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

    def test_envpool(self) -> None:
        conf = dict(
            zip(_MineSweeperEnvSpec._config_keys,
                _MineSweeperEnvSpec._default_config_values)
        )
        conf["num_envs"] = num_envs = 10
        conf["batch_size"] = batch = 5
        conf["num_threads"] = min(5, os.cpu_count())

        env_spec = _MineSweeperEnvSpec(tuple(conf.values()))
        action_spec = env_spec._action_spec
        action_keys = env_spec._action_keys
        action_conf = dict(zip(action_keys, action_spec))

        env = _MineSweeperEnvPool(env_spec)
        state_keys = env._state_keys
        env._reset(np.arange(num_envs, dtype=np.int32))

        n_step = 10000
        t = time.time()
        for _ in tqdm(range(n_step)):
            state = dict(zip(state_keys, env._recv()))
            action = {
                "env_id": state["info:env_id"],
                "players.env_id": state["info:players.env_id"],
                "players.action": np.random.randint(
                    low=action_conf["action"][-2][0],
                    high=action_conf["action"][-2][1],
                    size=(batch, 1), dtype=np.int32),
            }
            env._send(tuple(action.values()))
        duration = time.time() - t
        fps = n_step * batch / duration
        logging.info(f"FPS = {fps:.6f}")


if __name__ == "__main__":
    absltest.main()
