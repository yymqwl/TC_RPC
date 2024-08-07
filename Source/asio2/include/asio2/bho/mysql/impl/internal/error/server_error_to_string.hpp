//
// Copyright (c) 2019-2023 Ruben Perez Hidalgo (rubenperez038 at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BHO_MYSQL_IMPL_INTERNAL_ERROR_SERVER_ERROR_TO_STRING_HPP
#define BHO_MYSQL_IMPL_INTERNAL_ERROR_SERVER_ERROR_TO_STRING_HPP

#include <asio2/bho/mysql/detail/config.hpp>

namespace bho {
namespace mysql {
namespace detail {

// Returns NULL if this is not a common error (not a member of common_server_errc)
BHO_MYSQL_DECL
const char* common_error_to_string(int v) noexcept;

// These return a default string if the error is not known
BHO_MYSQL_DECL
const char* mysql_error_to_string(int v) noexcept;

BHO_MYSQL_DECL
const char* mariadb_error_to_string(int v) noexcept;

}  // namespace detail
}  // namespace mysql
}  // namespace bho

#ifdef BHO_MYSQL_HEADER_ONLY
#include <asio2/bho/mysql/impl/internal/error/server_error_to_string.ipp>
#endif

#endif
