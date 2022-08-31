from envpool.registration import register

register(
    task_id="Minesweeper-Beginner",
    import_path="envpool.minesweeper",
    spec_cls="MineSweeperEnvSpec",
    dm_cls="MineSweeperDMEnvPool",
    gym_cls="MineSweeperGymEnvPool",
    rows=9, columns=9, mines=10,
)

register(
    task_id="Minesweeper-Intermediate",
    import_path="envpool.minesweeper",
    spec_cls="MineSweeperEnvSpec",
    dm_cls="MineSweeperDMEnvPool",
    gym_cls="MineSweeperGymEnvPool",
    rows=16, columns=16, mines=40,
)

register(
    task_id="Minesweeper-Expert",
    import_path="envpool.minesweeper",
    spec_cls="MineSweeperEnvSpec",
    dm_cls="MineSweeperDMEnvPool",
    gym_cls="MineSweeperGymEnvPool",
    rows=16, columns=30, mines=99,
)
