#pragma once

#include <string>
#include "../utility/typedef.hpp"
#include "../type/serializable.hpp"

namespace stay
{
    namespace program
    {
        namespace detail
        {
            struct AppInfo : Serializable
            {
                std::size_t width;
                std::size_t height;
                std::string name;
                float updatesPerSec;

                Json::Value toJSONObject() const override
                {
                    Json::Value res;
                    res["width"] = width;
                    res["height"] = height;
                    res["name"] = name;
                    res["updatesPerSec"] = updatesPerSec;
                    return res;
                }

                bool fetch(const Json::Value& data) override
                {
                    if (data.type() == Json::objectValue && data["width"].isInt() && data["height"].isInt() && data["name"].isString() && data["updatesPerSec"].isNumeric())
                    {
                        width = data["width"].asInt();
                        height = data["height"].asInt();
                        name = data["name"].asString();
                        updatesPerSec = data["updatesPerSec"].asFloat();
                        return true;
                    }
                    else
                    {
                        // NOLINTBEGIN(*-magic-numbers)
                        width = 500;
                        height = 500;
                        name = "Unknown";
                        updatesPerSec = 60;
                        return false;
                        // NOLINTEND(*-magic-numbers)
                    }
                }
            };
        } // namespace detail
        
        class Application
        {
            public:
                static constexpr const char* INIT_FILE = "asset/init/init.json";

                Application();
                ~Application();
                void run();
            private:
                void input();
                void update();
                void render();
                void initialize();    
                void saveProperties();

                detail::AppInfo mAppInfo;
                Uptr<RWin> mWindow;
        };
    }
} // namespace stay