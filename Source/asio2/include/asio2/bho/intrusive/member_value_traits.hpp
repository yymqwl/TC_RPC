/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2006-2013
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BHO_INTRUSIVE_MEMBER_VALUE_TRAITS_HPP
#define BHO_INTRUSIVE_MEMBER_VALUE_TRAITS_HPP

#include <asio2/bho/intrusive/detail/config_begin.hpp>
#include <asio2/bho/intrusive/intrusive_fwd.hpp>

#include <asio2/bho/intrusive/link_mode.hpp>
#include <asio2/bho/intrusive/detail/parent_from_member.hpp>
#include <asio2/bho/move/detail/to_raw_pointer.hpp> 
#include <asio2/bho/intrusive/pointer_traits.hpp>

#if defined(BHO_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace bho {
namespace intrusive {

//!This value traits template is used to create value traits
//!from user defined node traits where value_traits::value_type will
//!store a node_traits::node
template< class T, class NodeTraits
        , typename NodeTraits::node T::* PtrToMember
        , link_mode_type LinkMode
   #ifdef BHO_INTRUSIVE_DOXYGEN_INVOKED
   = safe_link
   #endif
>
struct member_value_traits
{
   public:
   typedef NodeTraits                                                   node_traits;
   typedef T                                                            value_type;
   typedef typename node_traits::node                                   node;
   typedef typename node_traits::node_ptr                               node_ptr;
   typedef typename node_traits::const_node_ptr                         const_node_ptr;
   typedef pointer_traits<node_ptr>                                     node_ptr_traits;
   typedef typename pointer_traits<node_ptr>::template
      rebind_pointer<T>::type                                           pointer;
   typedef typename pointer_traits<node_ptr>::template
      rebind_pointer<const T>::type                                     const_pointer;
   //typedef typename pointer_traits<pointer>::reference                  reference;
   //typedef typename pointer_traits<const_pointer>::reference            const_reference;
   typedef value_type &                                                 reference;
   typedef const value_type &                                           const_reference;
   static const link_mode_type link_mode = LinkMode;

   BHO_INTRUSIVE_FORCEINLINE static node_ptr to_node_ptr(reference value) BHO_NOEXCEPT
   {  return pointer_traits<node_ptr>::pointer_to(value.*PtrToMember);   }

   BHO_INTRUSIVE_FORCEINLINE static const_node_ptr to_node_ptr(const_reference value) BHO_NOEXCEPT
   {  return pointer_traits<const_node_ptr>::pointer_to(value.*PtrToMember);   }

   BHO_INTRUSIVE_FORCEINLINE static pointer to_value_ptr(node_ptr n) BHO_NOEXCEPT
   {
      return pointer_traits<pointer>::pointer_to(*detail::parent_from_member<value_type, node>
         (bho::movelib::to_raw_pointer(n), PtrToMember));
   }

   BHO_INTRUSIVE_FORCEINLINE static const_pointer to_value_ptr(const_node_ptr n) BHO_NOEXCEPT
   {
      return pointer_traits<const_pointer>::pointer_to(*detail::parent_from_member<value_type, node>
         (bho::movelib::to_raw_pointer(n), PtrToMember));

   }
};

} //namespace intrusive
} //namespace bho

#include <asio2/bho/intrusive/detail/config_end.hpp>

#endif //BHO_INTRUSIVE_MEMBER_VALUE_TRAITS_HPP
