# Stay

A project for my girlfriend's birthday.

It's a game but the "engine" part is taking over.

# How to build
You will need `CMake` with version `>= 3.24` to build the project directly.

In the project folder:
```sh
cmake --preset=default
cmake --build build/default -j2
```
Run the executable:
```sh
cd build/default
./STAY
```

# Dependencies

A bunch of them, actually :octocat:
* [SFML](https://github.com/SFML/SFML)
* [glm](https://github.com/g-truc/glm)
* [EnTT](https://github.com/skypjack/entt)
* [Box2D](https://github.com/erincatto/box2d)
* [Jsoncpp](https://github.com/open-source-parsers/jsoncpp)
* [Json](https://github.com/nlohmann/json)

# Trivial info

* The game makes use of ECS thanks to EnTT extensive features
* Each system orders a few moments to perform their tasks (start, update, render, late update,.etc)
* Entity and game object are interchangable
* Each entity along with components is able to be serialized to and deserialized from json
* Currently the loading process uses generated quicktype file from [LDtk's website](https://ldtk.io/docs/game-dev/json-overview/json-schema/)'s schema, so it's very ugly.
