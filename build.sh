#!/bin/bash

# create conda environment
eval "$(conda shell.bash hook)"
conda activate envpool-dev
if [ $? -ne 0 ]; then
    conda create -n envpool-dev python=3.9
    conda activate envpool-dev
fi

# clone envpool
rm envpool/ -rf
git clone -b v0.6.4 git@github.com:sail-sg/envpool.git
ln -s ${PWD}/minesweeper envpool/envpool/minesweeper

cd envpool
# test minesweeper build
bazel build @//envpool/minesweeper:minesweeper
# run minesweeper unit tests
bazel test @//envpool/minesweeper:minesweeper_envpool_test

# apply patch 
patch setup.cfg < ../patches/setup.cfg.patch
patch envpool/BUILD < ../patches/envpool.BUILD.patch
patch envpool/entry.py < ../patches/envpool.entry.py.patch
patch envpool/make_test.py < ../patches/envpool.make_test.py.patch

# build all
make bazel-clean
make bazel-release # generate .whl file
# run make_test
make release-test1
# install .whl
pip install dist/envpool-0.6.4-cp39-cp39-linux_x86_64.whl --force-reinstall

cd .. && python env_test.py

conda deactivate
