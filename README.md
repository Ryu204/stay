# Stay

A project for my girlfriend's birthday.

It's a game but the "engine" part is taking over.

# How to build

You will need `git` and `CMake` (with version `>= 3.24`) to build the project directly.

Inside project directory:
```sh
git submodule update --init --recursive --remote
cmake --preset=default
cmake --build build/default
```
Run the executable:
```sh
cd build/default
./STAY
```

# Dependencies

A bunch of them, actually :octocat:
* **SFML** with [zlib](https://github.com/SFML/SFML/blob/2.6.x/license.md) license
* **json** with [MIT](https://github.com/nlohmann/json/blob/develop/LICENSE.MIT) license
* **glm** with [modified MIT](https://github.com/g-truc/glm/blob/master/manual.md#-licenses) license
* **entt** with [MIT](https://github.com/skypjack/entt/blob/v3.12.x/LICENSE) license
* **box2d** with [MIT](https://github.com/erincatto/box2d/blob/main/LICENSE) license
* **efsw** with [MIT](https://github.com/SpartanJ/efsw/blob/master/LICENSE) license
* **pybind11** with [BSD-style](https://github.com/pybind/pybind11/blob/master/LICENSE) license

# Trivial info

* The scene is structured as a graph, with each node corresponding to an entity in ECS
* Root node handles save and load
* Entity behaviours are defined via systems
* There is currently 2 builtin systems: render and physics
* Each entity along with components is able to be serialized to and deserialized from json
* Currently the loading process uses generated quicktype file from [LDtk's website](https://ldtk.io/docs/game-dev/json-overview/json-schema/)'s schema, so it's very ugly.

# Usage

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
