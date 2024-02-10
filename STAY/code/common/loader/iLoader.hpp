#pragma once

#include "utility/typedef.hpp"
#include "world/node.hpp"

namespace stay
{
    struct ILoader : std::enable_shared_from_this<ILoader>
    {
        virtual void load(const std::filesystem::path& /* path */, Node* node);
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
                    mLoader = std::make_shared<T>(std::forward<Args>(args)...);
                }
                void setLoaderPtr(std::shared_ptr<ILoader>&& ptr)
                {
                    mLoader = std::move(ptr);
                }
                ILoader& getLoader()
                {
                    return *mLoader;
                }
            private:
                SPtr<ILoader> mLoader{std::make_shared<ILoader>()};
        };
        LoaderHolder& loader();
    } // namespace detail

    template <typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<ILoader, T>, bool>>
    void constructLoader(Args&&... args)
    {
        detail::loader().setLoader<T>(std::forward<Args>(args)...);
    }

    void setLoader(std::shared_ptr<ILoader>&& ptr);
} // namespace stay
