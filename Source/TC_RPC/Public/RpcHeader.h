#pragma once

#pragma warning(push)
#pragma push_macro("check")
#undef check

//#pragma warning(disable: 2334)
//#pragma warning(disable: 2988)
#pragma warning(disable: 4668)


#include <asio2/rpc/rpc_server.hpp>
#include <asio2/rpc/rpc_client.hpp>
#pragma warning(pop)
#pragma pop_macro("check")


#define RPC_RECEIVE_BUFFER_SIZE 1024
#define RPC_RECEIVE_MAX_BUFFER_SIZE 1024*8


//#define U_MESSAGE_TYPE uint16

//#define RPC_SERVER_REPLY "Server_Reply"
//#define 



