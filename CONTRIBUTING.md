# Contributing to Open Git Client

### Pre commit hooks

Commits that have not used the pre-commit hooks, or have bypassed them will be ignored.

```shell
# Make sure pre-commit is installed on your machine, using any of the commands below.
pip install pre-commit
pipx install pre-commit
brew install pre-commit

# Install the hooks
pre-commit install
```

### Frontend only build

If you are working on the frontend web UI project (anything under web/), there is a script to avoid recompiling the
entire project. In order for the script to run properly it does require that the main project has already been built.
It will check for the build in the below directories in the given order, if you have an outdated backend build in any
of the folders either delete or update it in order to not have conflicting versions.
- build
- cmake-build-debug
- cmake-build-release
- cmake-build

To run the frontend only build run either of the commands below in the web/ directory:
```shell
npm run dev
# or
node scripts/quickload.cjs
```
