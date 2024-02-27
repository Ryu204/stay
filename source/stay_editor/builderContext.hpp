#pragma once

namespace stay::editor
{
    class World;
    struct BuilderContext
    {
        BuilderContext(BuilderContext&&) = default;
        BuilderContext(World& world)
            : world{world}
        {}
        World& world;
    };
}