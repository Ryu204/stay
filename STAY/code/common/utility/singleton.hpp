#pragma once

#include <memory>
#include <cassert>

namespace stay
{
    namespace utils
    {
        template <typename Trait>
        class Singleton
        {
            public:
                using Type = typename Trait::Type;
                Singleton() = delete;
                virtual ~Singleton() = default;
                static void init()
                {
                    Trait::create().swap(mInstance());
                }
                static void shutdown()
                {
                    assert(mInstance().get() != nullptr && "Call init first");
                    Trait::shutdown(mInstance());
                    mInstance().reset();
                }
                static Type& get()
                {
                    assert(mInstance().get() != nullptr && "call init first");
                    return *mInstance();
                }
                static bool avail()
                {
                    return mInstance().get() != nullptr;
                }
            private:
                static std::unique_ptr<Type>& mInstance()
                {
                    static std::unique_ptr<Type> res;
                    return res;
                };
        };
    } // namespace utils
} // namespace stay
