#pragma once

namespace stay
{
    namespace utils
    {
        // Variant visitor
        template <typename... funcs>
        struct VariantVisitor : funcs...
        {
            using funcs::operator()...;
        };
        // deduction guide
        template <typename... funcs>
        VariantVisitor(funcs...) -> VariantVisitor<funcs...>;
    }
} // namespace stay