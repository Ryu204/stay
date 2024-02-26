#pragma once

#include <fstream>

#include <stay/type/serializable.hpp>
#include <stay/type/vector.hpp>
#include <nlohmann/json.hpp>

namespace stay::editor
{
    struct ApplicationInfo : Serializable
    {
        float fontScale;
        bool darkMode;
        float framePerSec;
        SERIALIZE(fontScale, darkMode, framePerSec);

        void fetch(const std::filesystem::path& configFile)
        {
            std::ifstream reader{configFile};
            auto object = nlohmann::json::parse(reader);
            if (!this->deserialization(object))
                throw std::runtime_error{"Cannot parse application info"};
        }
    };
}