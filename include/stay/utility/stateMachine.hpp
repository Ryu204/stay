#pragma once

#include <memory>
#include <ranges>
#include <vector>
#include <unordered_map>
#include <functional>
#include <cassert>
#include <variant>
#include <queue>

#include "variantHelper.hpp"

namespace stay
{
    namespace fsm
    {
        template <typename Context, typename Id>
        class Stack;

        template <typename Context, typename Id>
        class State
        {
            public: 
                virtual ~State() = default;
                void requestPush(const Id& id)
                {
                    assert(mStack != nullptr);
                    mStack->queryPush(id);
                }
                void requestPop()
                {
                    assert(mStack != nullptr);
                    mStack->queryPop();
                }
                void requestClear()
                {
                    assert(mStack != nullptr);
                    mStack->queryClear();
                }
                virtual void enter() {}
                virtual void exit() {}
                Context& context()
                {
                    assert(mStack != nullptr);
                    return mStack->context();
                }
            private:
                friend class Stack<Context, Id>;
                void setStack(Stack<Context, Id>* s)
                {
                    mStack = s;
                }
                Stack<Context, Id>* mStack{nullptr};
        };

        template <typename Context, typename Id>
        class Stack
        {   
            public:
                template <typename... Args>
                void setContext(Args&&... args)
                {
                    mContext = std::make_unique<Context>(std::forward<Args>(args)...);
                }

                ~Stack()
                {
                    clear();
                }

                template <typename T, typename... Args, std::enable_if_t<std::is_base_of_v<State<Context, Id>, T>, bool> = true>
                void add(const Id& id, Args&&... args)
                {
                    auto& state = mStates.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
                    state->setStack(this);
                    mIds.emplace(id, mStates.size() - 1);
                }
                
                void push(const Id& id)
                {
                    assert(mIds.find(id) != mIds.end() && "id not added");
                    const auto index = mIds.at(id);
                    mStack.push_back(index);
                    mStates.at(index)->enter();
                }
                void queryPush(const Id& id)
                {
                    mPendingChanges.push(Push{id});
                }
                void pop()
                {
                    assert(!mStack.empty() && "empty stack"); 
                    mStates.at(mStack.back())->exit();
                    mStack.pop_back();
                }
                void queryPop()
                {
                    mPendingChanges.push(Pop{});
                }
                void clear()
                {
                    while (!mStack.empty())
                        pop();
                }
                void queryClear()
                {
                    mPendingChanges.push(Clear{});
                }
                template <typename T, std::enable_if_t<std::is_base_of_v<State<Context, Id>, T>, bool> = true>
                void untilFalse(std::function<bool(T&)>&& func)
                {
                    for (const auto index : std::ranges::reverse_view(mStack))
                    {
                        auto& state = mStates.at(index);
                        auto* casted = dynamic_cast<T*>(state.get());
                        assert(casted != nullptr && "state type must derive from T");
                        if (!func(*casted))
                            break;
                    }
                    applyPendingChanges();
                }
                template <typename T, std::enable_if_t<std::is_base_of_v<State<Context, Id>, T>, bool> = true>
                void untilFalseRev(std::function<bool(State<Context, Id>&)>&& func)
                {
                    for (const auto index : mStack)
                    {
                        auto& state = mStates.at(index);
                        auto* casted = dynamic_cast<T*>(state.get());
                        assert(casted != nullptr && "state type must derive from T");
                        if (!func(*casted))
                            break;
                    }
                    applyPendingChanges();
                }
                Context& context()
                {
                    assert(mContext != nullptr && "did not call `setContext` first");
                    return *mContext;
                }
            private:
                struct Pop {};
                struct Clear{};
                struct Push{Id id;};
                using Change = std::variant<Pop, Clear, Push>;
                void applyPendingChanges()
                {
                    while (!mPendingChanges.empty())
                    {
                        const auto& c = mPendingChanges.front();
                        std::visit(utils::VariantVisitor{
                            [this](const Clear&) { clear(); },
                            [this](const Pop&) { pop(); },
                            [this](const Push& p) { push(p.id); },
                        }, c);
                        mPendingChanges.pop();
                    }
                }

                std::unique_ptr<Context> mContext;
                std::unordered_map<Id, std::size_t> mIds;
                std::vector<std::unique_ptr<State<Context, Id>>> mStates{};
                std::vector<std::size_t> mStack{};
                std::queue<Change> mPendingChanges;
        };
    }
} // namespace stay