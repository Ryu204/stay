# Stay

Core frameworks/utilities for developing games in C++.

Features:
* ECS and scene graph system
* Scene/Node serialization and configure game with JSON
* Box2D as physics backend

# How to use

You will need `CMake` with version `>= 3.24` and `git` to build the project directly.

Build:
```sh
git submodule update --init --recursive --remote
cmake --preset=default
cmake --build build/default
```
Use as a dependency:
```bash
# Your projejct directory
git submodule add https://github.com/Ryu204/stay.git extern/stay
```
```cmake
# CMakeLists.txt
add_subdirectory(extern/stay)
target_link_libraries(<your_target> PUBLIC stay)
```
```cpp
// main.cpp
#include <stay/program/application.hpp>
using namespace stay;
int main() {
    program::Application app;
    app.run();
}
```

# License and dependencies

This repository is licensed under MIT license.

The dependencies are provided with compatible licenses:

* **SFML** with [zlib](https://github.com/SFML/SFML/blob/2.6.x/license.md) license
* **json** with [MIT](https://github.com/nlohmann/json/blob/develop/LICENSE.MIT) license
* **glm** with [modified MIT](https://github.com/g-truc/glm/blob/master/manual.md#-licenses) license
* **entt** with [MIT](https://github.com/skypjack/entt/blob/v3.12.x/LICENSE) license
* **box2d** with [MIT](https://github.com/erincatto/box2d/blob/main/LICENSE) license
* **efsw** with [MIT](https://github.com/SpartanJ/efsw/blob/master/LICENSE) license

# Trivial info

* The scene is structured as a graph, with each node corresponding to an entity in ECS
* Root node handles save and load
* Entity behaviours are defined via systems
* There is currently 2 builtin systems: render and physics
* Each entity along with components is able to be serialized to and deserialized from json

# Ignore

> for future reference

This section lists what user must do to make a game from core codebase (a.k.a `STAY/code/common`):

* Create more components by:
    * inheriting from `ecs::Component`
    * insert a `COMPONENT` macro to register and (de)serialize inside the class's implementation
* Create more systems by:
    * insert a `REGISTER_SYSTEM` macro to register inside the class's implementation
    * inherits from a set of possible base systems to perform corresponding jobs
    * inherits from `ecs::System` to have access to entities and components
* Create an optional loader class to generate the scene by calling `constructLoader` (see example in `game/system/levelSystem.hpp`). Once the scene is run, it will save a loader-independent file for later use without loader.

Headers of system classes are not supposed to be included anywhere, as systems work independently. However their code needs to be compiled somehow. That means if a system's definition is so short that it can be contained in a single file, the file must be a source file (`*.cpp`).
