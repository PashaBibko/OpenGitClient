# Open Git Client

A small and simple open source git client, designed for basic git operations. Works natively on Windows and macOS
([not Linux](#linux-support)). Built with C++, C# and TypeScript using a primarily CMake build system.

### Current Features, (all planned for v1.0)

- [x] Select/open a repository
- [ ] Commit changes
- [ ] Stash (+ un-stash) changes
- [ ] Pull (fetch + merge) from origin
- [ ] Push to origin
- [ ] Switch branch
- [ ] Basic auth

### Build

External required dependencies (all must be available in the system path)
- CMake
- C++ compiler
- .NET SDK v8 (or higher)
- Node.js / npm

```shell
# Downloads the project, unneeded if downloaded manually
git clone https://github.com/PashaBibko/OpenGitClient.git open-git-client
cd open-git-client

# Generates and runs the build file
mkdir build
cd build
cmake .. # Add -G Ninja if available
cmake --build .
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
node scripts/quickload.js
```

### Libraries used

| Library            | Purpose                                      |
|--------------------|----------------------------------------------|
| **Vite**           | Web bundler                                  |
| **ViteSingleFile** | Helps Photino.NET easily display the webpage |
| **TailwindCSS**    | CSS styling for the frontend                 |
| **Photino.NET**    | Handles the window and webpage display       |
| **Glaze**          | JSON read/write for C++                      |
| **libgit2**        | Git interaction for C/C++                    |
| **nfd**            | Opens native file explorer for C++           |
| **zustand**        | Handles react app global context             |

### Linux Support
Linux is not directly supported for this project, for a couple of reasons. The main one is that I have designed it for
my personal use and do not use linux as part of my workflow. HOWEVER, there is nothing within the codebase that rules
out linux support, so if you wish to implement it feel free too.

### License
All code in this project is licensed under the [MIT License](LICENSE), please do modify and distribute it as much as
you can. This project was primarily made because I was fed up with different git clients requiring subscriptions to work
on private repositories.
