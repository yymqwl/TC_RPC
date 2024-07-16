#ifndef BHO_CORE_LAUNDER_HPP_INCLUDED
#define BHO_CORE_LAUNDER_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//  Copyright 2023 Peter Dimov
//  Distributed under the Boost Software License, Version 1.0.
//  https://www.boost.org/LICENSE_1_0.txt

#if defined(__has_builtin)
# if __has_builtin(__builtin_launder)
#  define BHO_CORE_HAS_BUILTIN_LAUNDER
# endif
#endif

#if (__cplusplus >= 201703L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L)) && !defined(BHO_CORE_HAS_BUILTIN_LAUNDER)
# include <new>
#endif

namespace bho
{
namespace core
{

#if defined(BHO_CORE_HAS_BUILTIN_LAUNDER)

template<class T> T* launder( T* p )
{
    return __builtin_launder( p );
}

#elif (__cplusplus >= 201703L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L)) && defined(__cpp_lib_launder)

template<class T> T* launder( T* p )
{
    return std::launder( p );
}

#else

template<class T> T* launder( T* p )
{
    return p;
}

#endif

} // namespace core
} // namespace bho

#endif  // #ifndef BHO_CORE_LAUNDER_HPP_INCLUDED
