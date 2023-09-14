#pragma once

#include <json/json.h>

namespace stay
{
    class Serializable 
    {
        public:
            virtual Json::Value toJSONObject() const = 0;
            virtual bool fetch(const Json::Value& /*data*/) = 0;
    };
} // namespace stay
