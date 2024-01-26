#pragma once

#include "utility/typedef.hpp"
#include "world/node.hpp"

namespace stay
{
    struct ILoader 
    {
        virtual Uptr<Node> load(std::filesystem::path&& path);
        virtual ~ILoader() = default;
    };

    namespace detail
    {
        class LoaderHolder
        {
            public:
                template <typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<ILoader, T>, bool>>
                void setLoader(Args&&... args)
                {
                    mLoader = std::make_unique<T>(std::forward<Args>(args)...);
                }
                ILoader& getLoader();
            private:
                Uptr<ILoader> mLoader{};
        };
        LoaderHolder& loader();
    } // namespace detail

    template <typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<ILoader, T>, bool>>
    void constructLoader(Args&&... args)
    {
        detail::loader().setLoader<T>(std::forward<Args>(args)...);
    }
} // namespace stay
