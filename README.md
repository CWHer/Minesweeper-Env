# Minesweeper-Env

`Minesweeper-Env` is a high throughput environment for Reinforcement Learning (RL). It is based on [envpool](https://github.com/sail-sg/envpool/releases/tag/v0.6.4) and is designed to be used with [Tianshou](https://github.com/thu-ml/tianshou).


## Description

It is based on the classic game Minesweeper. The goal of the agent is to clear the board without detonating any mines. Every step the agent can choose to clear a cell. The agent receives a reward that equals to the number of cleared cells at that step (i.e., if agent clears 3 cells at one step, it receives a reward of 3). The game ends when the agent clears all cells that are not mines or when the agent detonates a mine. The agent receives a reward of 10 if it clears all cells that are not mines and a reward of -10 otherwise.

- `State`: `Box(-1, 8, (n_rows, n_cols), int32)`
    
    The state is a 2D array of integers. The value of each cell is either -1 (unrevealed) or the number of mines in the 8 neighboring cells (0-8). 

- `Action`: `Discrete(n_rows * n_cols)`

    The action indices range from 0 to `n_rows * n_cols - 1`. The action is the index of the cell to be cleared. 
    
    **Note**: repeated actions are not allowed and will end the game with a reward of -10.

- `Reward`: `{n_cleared_cells, 10, -10}`

    The reward is the number of cleared cells at that step. The reward is 10 if the agent clears all cells that are not mines and -10 otherwise.

- `Done`: `True` or `False`

    The game ends when the agent clears all cells that are not mines or when the agent detonates a mine.

- `Info:Cheat`: `Box(-1, 8, (n_rows, n_cols), int32)` 

    A cheat table that shows the entire map. The value of each cell is either -1 (mine) or the number of mines in the 8 neighboring cells (0-8). 


## Getting Started

### Dependencies

- `bazel`

- `conda`

### Installing

Let `build.sh` handle the installation (on Linux). It will create a conda environment named `envpool-dev`. Build and install `envpool` with `Minesweeper-Env`. And finally run the tests.

```bash
git clone git@github.com:CWHer/Minesweeper-Env.git
cd Minesweeper-Env && ./build.sh
```

### Executing program

`env_test.py` gives a usage example of `Minesweeper-Env`. It creates a `MinesweeperEnv` and runs a random agent on it.


## Version History

- [x] `v1.0` Initial Release