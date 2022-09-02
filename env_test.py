import time

import numpy as np
import tqdm

import envpool

if __name__ == "__main__":

    print(envpool.list_all_envs()[-5:])

    task_id = "Minesweeper-Intermediate"
    seed = 42
    num_envs = 50
    batch_size = 12
    num_threads = 16
    total_step = 40000

    kwargs = dict(
        num_envs=num_envs,
        batch_size=batch_size,
        num_threads=num_threads,
    )
    env = envpool.make_gym(task_id, **kwargs)

    env.async_reset()
    env.action_space.seed(seed)

    t = time.time()
    for _ in tqdm.trange(total_step):
        info = env.recv()[-1]
        # HACK: generate actions may cause overhead
        action = np.array(
            [env.action_space.sample()
             for _ in range(batch_size)])
        env.send(action, info["env_id"])
    duration = time.time() - t

    fps = total_step * batch_size / duration
    print(f"Duration = {duration:.2f}s")
    print(f"EnvPool FPS = {fps:.2f}")
