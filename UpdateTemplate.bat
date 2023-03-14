git fetch template
git checkout main
git merge template/main --allow-unrelated-histories
git submodule update --init --recursive
PAUSE