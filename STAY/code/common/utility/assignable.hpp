#pragma once

namespace stay
{
    namespace utils
    {
        // @brief A base class that user can assign a `T` value to and retrieve it later
        template <typename T>
        struct Assignable
        {
                void assign(T val)
                {
                    mInternal = val;
                    mAssigned |= true;
                    postAssignment();
                };
                const T& get() const
                {
                    assert(mAssigned && "unassigned value");   
                    return mInternal;
                }
                bool assigned() const
                {
                    return mAssigned;
                }
                virtual ~Assignable() = default;
            protected:
                virtual void postAssignment() {}
            private:
                bool mAssigned{false};
                T mInternal{static_cast<T>(0)};
        };
    } // namespace utils
    
} // namespace stay
