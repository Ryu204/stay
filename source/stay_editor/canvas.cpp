#include "stay_editor/canvas.hpp"

#include <imgui.h>
#include <imgui-SFML.h>

namespace stay::editor
{
    void Canvas::update()
    {
        if (ImGui::Begin("Canvas"))
        {
            auto currentWindowSize = ImGui::GetWindowSize();
            currentWindowSize.y -= 2 * ImGui::GetFrameHeightWithSpacing();
            const bool resized = currentWindowSize.x != mWidth || currentWindowSize.y != mHeight;
            mWidth = currentWindowSize.x;
            mHeight = currentWindowSize.y;
            if (resized)
            {
                if (!mTexture.create(mWidth, mHeight))
                    throw std::runtime_error{"Cannot create render texture"};
                mCamera.adaptTo(&mTexture);
                mTexture.setView(mCamera.getView());
            }
            mTexture.display();
            ImGui::Image(mTexture.getTexture());
        }
        ImGui::End();
    }

    void Canvas::clear()
    {
        mTexture.clear(sf::Color::Magenta);
    }

    Camera& Canvas::getCamera()
    {
        return mCamera;
    }
}