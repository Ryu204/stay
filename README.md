# Stay

A project for my girlfriend's birthday.

It's a game but the "engine" part is taking over.

# How to build
You will need `CMake` with version `>= 3.24` to build the project directly.

In the project folder:
```bash
cmake --preset=base
cmake --build build
```
Run the executable:
```bash
cd build
./STAY
# if you're on a Windows machine
# ./STAY.exe
```

# Dependencies

A bunch of them, actually :octocat:
* [SFML](https://github.com/SFML/SFML)
* [glm](https://github.com/g-truc/glm)
* [EnTT](https://github.com/skypjack/entt)
* [Box2D](https://github.com/erincatto/box2d)
* [LDtkLoader](https://github.com/Madour/LDtkLoader)
* [Jsoncpp](https://github.com/open-source-parsers/jsoncpp)

# Trivial info

* The game makes use of ECS thanks to EnTT extensive features
* Each system orders a few moments to perform their tasks (start, update, render, late update,.etc)
* Entity and game object are interchangable
* Each entity along with components is able to be serialized to and deserialized from json
