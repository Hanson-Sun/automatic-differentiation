#pragma once
#include <cmath>
#include <iostream>
#include <array> 

// something about using templates to get a compile time thing
namespace AD {

// https://en.cppreference.com/w/cpp/language/crtp
// this uses a forward approach
template <typename Derived, size_t N, typename ValueType>
struct Expr {
    using value_type = ValueType;
    static constexpr size_t num_vars = N;

    // ideally this is inlined to hell since exact types are known..
    ValueType value(const std::array<ValueType, N>& vars) const {
        return static_cast<const Derived*>(this)->value(vars);
    }

    template <size_t VarIndex>
    ValueType deriv(const std::array<ValueType, N>& vars) const {
        static_assert(VarIndex < N, "Variable index out of bounds.");
        return static_cast<const Derived*>(this)->template deriv<VarIndex>(vars);
    }
};


template <size_t Index, typename VT = double, size_t NumVars = 1>
struct Variable : Expr<Variable<Index, VT, NumVars>, NumVars, VT> {
    static_assert(Index < NumVars, "Variable index out of bounds for specified N.");

    VT value(const std::array<VT, NumVars>& vars) const {
        return vars[Index];
    }

    template <size_t VarIndex>
    VT deriv(const std::array<VT, NumVars>& /*vars*/) const {
        return (Index == VarIndex) ? VT(1.0) : VT(0.0);
    }
};


template <typename VT = double>
struct Constant : Expr<Constant<VT>, 1, VT> {
    VT val;

    explicit Constant(VT v) : val(v) {}

    VT value(const std::array<VT, Constant::num_vars>& /*vars*/) const {
        return val;
    }

    template <size_t VarIndex>
    VT deriv(const std::array<VT, Constant::num_vars>& /*vars*/) const {
        return VT(0.0);
    }
};

template <typename LHS, typename RHS>
struct Add : Expr<Add<LHS, RHS>, LHS::num_vars, typename LHS::value_type> {
    static_assert(std::is_same_v<typename LHS::value_type, typename RHS::value_type>,
                  "Type mismatch in addition operands.");
    static_assert(LHS::num_vars == RHS::num_vars,
                  "Variable count mismatch in addition operands.");

    using ValueType = typename LHS::value_type;
    static constexpr size_t N = LHS::num_vars;

    const LHS lhs;
    const RHS rhs;

    Add(const LHS& l, const RHS& r) : lhs(l), rhs(r) {}

    ValueType value(const std::array<ValueType, N>& vars) const {
        return lhs.value(vars) + rhs.value(vars);
    }

    template <size_t VarIndex>
    ValueType deriv(const std::array<ValueType, N>& vars) const {
        return lhs.template deriv<VarIndex>(vars) + rhs.template deriv<VarIndex>(vars);
    }
};

template <typename C>
struct Log : Expr<Log<C>, C::num_vars, typename C::value_type> {
    using ValueType = typename C::value_type;
    static constexpr size_t N = C::num_vars;

    const C child;

    Log(const C& child) : child(child) {}

    ValueType value(const std::array<ValueType, N>& vars) const {
        return logFun(child.value);
    }

    template<size_t VarIndex>
    ValueType deriv(const std::array<ValueType, N>& vars) const {
        return child.template deriv<VarIndex>(vars) / child.value(vars);;
    }
};



};  // namespace AD

template <typename L, typename R, typename ValueType = double>
AD::Add<L, R> operator+(const AD::Expr<L, L::N, ValueType>& lhs, const AD::Expr<R, R::N, ValueType>& rhs) {
    // nothng is evaluated yet, THATS CRAZY!!
    return Add<L, R>(static_cast<const L&>(lhs), static_cast<const R&>(rhs));
}

template <typename Derived, typename VT, size_t N>
AD::Log<AD::Expr<Derived, N, VT>> log(const AD::Expr<Derived, N, VT>& child_expr) {
    return Log<Derived>(static_cast<const Derived&>(child_expr));
}
