# Stay

A project for my girlfriend's birthday.

It's a game but the "engine" part is taking over.

# How to build
You will need `CMake` with version `>= 3.24` to build the project directly.

Inside project directory:
```sh
cmake --preset=default # This will take a while to download sources of dependencies
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

# Trivial info
* The scene is structured as a graph, with each node corresponding to an entity in ECS
* Root node handles save and load
* Entity behaviours are defined via systems
* There is currently 2 builtin systems: render and physics
* Each entity along with components is able to be serialized to and deserialized from json
* Currently the loading process uses generated quicktype file from [LDtk's website](https://ldtk.io/docs/game-dev/json-overview/json-schema/)'s schema, so it's very ugly.
