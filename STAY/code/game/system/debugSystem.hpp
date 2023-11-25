#pragma once 

#include "../../common/ecs/manager.hpp"
#include "../../common/utility/typedef.hpp"

namespace stay
{
    class Camera;
    struct DebugSystem : public ecs::UpdateSystem, public ecs::System
    {
            DebugSystem(ecs::Manager* manager);
            void initialize(Camera* camera, RWin* window);
            void update(float dt) override;
        private:
            Vector2 mousePosition() const;
            Camera* mCamera;
            RWin* mWindow;
    };
} // namespace stay