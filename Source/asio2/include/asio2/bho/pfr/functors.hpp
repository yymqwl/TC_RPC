// Copyright (c) 2016-2023 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BHO_PFR_FUNCTORS_HPP
#define BHO_PFR_FUNCTORS_HPP
#pragma once

#include <asio2/bho/pfr/detail/config.hpp>

#include <asio2/bho/pfr/ops.hpp>

#include <asio2/bho/pfr/detail/functional.hpp>

/// \file bho/pfr/functors.hpp
/// Contains functors that are close to the Standard Library ones.
/// Each functor calls corresponding BHO.PFR function from bho/pfr/ops.hpp
///
/// \b Example:
/// \code
///     #include <asio2/bho/pfr/functors.hpp>
///     struct my_struct {      // No operators defined for that structure
///         int i; short s; char data[7]; bool bl; int a,b,c,d,e,f;
///     };
///     // ...
///
///     std::unordered_set<
///         my_struct,
///         bho::pfr::hash<>,
///         bho::pfr::equal_to<>
///     > my_set;
/// \endcode
///
/// \b Synopsis:
namespace bho { namespace pfr {

///////////////////// Comparisons

/// \brief std::equal_to like comparator that returns \forcedlink{eq}(x, y)
template <class T = void> struct equal_to {
    /// \return \b true if each field of \b x equals the field with same index of \b y.
    bool operator()(const T& x, const T& y) const {
        return bho::pfr::eq(x, y);
    }

#ifdef BHO_PFR_DOXYGEN_INVOKED
    /// This typedef exists only if T \b is void
    typedef std::true_type is_transparent;

    /// This operator allows comparison of \b x and \b y that have different type.
    /// \pre Exists only if T \b is void.
    template <class V, class U> bool operator()(const V& x, const U& y) const;
#endif
};

/// @cond
template <> struct equal_to<void> {
    template <class T, class U>
    bool operator()(const T& x, const U& y) const {
        return bho::pfr::eq(x, y);
    }

    typedef std::true_type is_transparent;
};
/// @endcond

/// \brief std::not_equal like comparator that returns \forcedlink{ne}(x, y)
template <class T = void> struct not_equal {
    /// \return \b true if at least one field \b x not equals the field with same index of \b y.
    bool operator()(const T& x, const T& y) const {
        return bho::pfr::ne(x, y);
    }

#ifdef BHO_PFR_DOXYGEN_INVOKED
    /// This typedef exists only if T \b is void
    typedef std::true_type is_transparent;

    /// This operator allows comparison of \b x and \b y that have different type.
    /// \pre Exists only if T \b is void.
    template <class V, class U> bool operator()(const V& x, const U& y) const;
#endif
};

/// @cond
template <> struct not_equal<void> {
    template <class T, class U>
    bool operator()(const T& x, const U& y) const {
        return bho::pfr::ne(x, y);
    }

    typedef std::true_type is_transparent;
};
/// @endcond

/// \brief std::greater like comparator that returns \forcedlink{gt}(x, y)
template <class T = void> struct greater {
    /// \return \b true if field of \b x greater than the field with same index of \b y and all previous fields of \b x equal to the same fields of \b y.
    bool operator()(const T& x, const T& y) const {
        return bho::pfr::gt(x, y);
    }

#ifdef BHO_PFR_DOXYGEN_INVOKED
    /// This typedef exists only if T \b is void
    typedef std::true_type is_transparent;

    /// This operator allows comparison of \b x and \b y that have different type.
    /// \pre Exists only if T \b is void.
    template <class V, class U> bool operator()(const V& x, const U& y) const;
#endif
};

/// @cond
template <> struct greater<void> {
    template <class T, class U>
    bool operator()(const T& x, const U& y) const {
        return bho::pfr::gt(x, y);
    }

    typedef std::true_type is_transparent;
};
/// @endcond

/// \brief std::less like comparator that returns \forcedlink{lt}(x, y)
template <class T = void> struct less {
    /// \return \b true if field of \b x less than the field with same index of \b y and all previous fields of \b x equal to the same fields of \b y.
    bool operator()(const T& x, const T& y) const {
        return bho::pfr::lt(x, y);
    }

#ifdef BHO_PFR_DOXYGEN_INVOKED
    /// This typedef exists only if T \b is void
    typedef std::true_type is_transparent;

    /// This operator allows comparison of \b x and \b y that have different type.
    /// \pre Exists only if T \b is void.
    template <class V, class U> bool operator()(const V& x, const U& y) const;
#endif
};

/// @cond
template <> struct less<void> {
    template <class T, class U>
    bool operator()(const T& x, const U& y) const {
        return bho::pfr::lt(x, y);
    }

    typedef std::true_type is_transparent;
};
/// @endcond

/// \brief std::greater_equal like comparator that returns \forcedlink{ge}(x, y)
template <class T = void> struct greater_equal {
    /// \return \b true if field of \b x greater than the field with same index of \b y and all previous fields of \b x equal to the same fields of \b y;
    /// or if each field of \b x equals the field with same index of \b y.
    bool operator()(const T& x, const T& y) const {
        return bho::pfr::ge(x, y);
    }

#ifdef BHO_PFR_DOXYGEN_INVOKED
    /// This typedef exists only if T \b is void
    typedef std::true_type is_transparent;

    /// This operator allows comparison of \b x and \b y that have different type.
    /// \pre Exists only if T \b is void.
    template <class V, class U> bool operator()(const V& x, const U& y) const;
#endif
};

/// @cond
template <> struct greater_equal<void> {
    template <class T, class U>
    bool operator()(const T& x, const U& y) const {
        return bho::pfr::ge(x, y);
    }

    typedef std::true_type is_transparent;
};
/// @endcond

/// \brief std::less_equal like comparator that returns \forcedlink{le}(x, y)
template <class T = void> struct less_equal {
    /// \return \b true if field of \b x less than the field with same index of \b y and all previous fields of \b x equal to the same fields of \b y;
    /// or if each field of \b x equals the field with same index of \b y.
    bool operator()(const T& x, const T& y) const {
        return bho::pfr::le(x, y);
    }

#ifdef BHO_PFR_DOXYGEN_INVOKED
    /// This typedef exists only if T \b is void
    typedef std::true_type is_transparent;

    /// This operator allows comparison of \b x and \b y that have different type.
    /// \pre Exists only if T \b is void.
    template <class V, class U> bool operator()(const V& x, const U& y) const;
#endif
};

/// @cond
template <> struct less_equal<void> {
    template <class T, class U>
    bool operator()(const T& x, const U& y) const {
        return bho::pfr::le(x, y);
    }

    typedef std::true_type is_transparent;
};
/// @endcond


/// \brief std::hash like functor that returns \forcedlink{hash_value}(x)
template <class T> struct hash {
    /// \return hash value of \b x.
    std::size_t operator()(const T& x) const {
        return bho::pfr::hash_value(x);
    }
};

}} // namespace bho::pfr

#endif // BHO_PFR_FUNCTORS_HPP
