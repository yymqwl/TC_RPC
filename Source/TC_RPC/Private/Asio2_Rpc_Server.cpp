// Fill out your copyright notice in the Description page of Project Settings.


#include "Asio2_Rpc_Server.h"
#include "RPC_Log.h"


/*
TObjectPtr<UAsio2_Rpc_Server> UAsio2_Rpc_Server::Create()
{
	
}*/

UAsio2_Rpc_Server::UAsio2_Rpc_Server()
{
	
}

bool UAsio2_Rpc_Server::Init(uint32 recv_buf_size ,uint32 recv_buf_max_size)
{
	bool bret=true;
	if (Ptr_Rpc_Server.IsValid())
	{
		Ptr_Rpc_Server->destroy();
		Ptr_Rpc_Server = nullptr;
	}
	
	auto threadNum = std::max(std::thread::hardware_concurrency(),(uint32)1);
	
	
	Ptr_Rpc_Server = MakeUnique<asio2::rpc_kcp_server>(recv_buf_size,recv_buf_max_size,threadNum);
	
	return bret;
}

void UAsio2_Rpc_Server::Stop()
{
	if (Ptr_Rpc_Server.IsValid())
	{
		Ptr_Rpc_Server->stop();
	}
}


bool UAsio2_Rpc_Server::Bind_Start(FString host, uint32 port)
{
	bool bret=false;
	if (!Ptr_Rpc_Server.IsValid() || Ptr_Rpc_Server->is_started())
	{
		return bret;
	}
	
	
	Ptr_Rpc_Server->bind_connect([&](auto & session_ptr)
	{
		RPC_LOG(TEXT("client enter : %s %u %s %u"),UTF8_TO_TCHAR(session_ptr->remote_address().c_str()), session_ptr->remote_port(),
			UTF8_TO_TCHAR(session_ptr->local_address().c_str()), session_ptr->local_port());
	}).bind_start([&]()
	{
		RPC_LOG(TEXT("start rpc server : %hs-%u-%d-%s"),Ptr_Rpc_Server->listen_address().c_str(),Ptr_Rpc_Server->listen_port(),
			asio2::get_last_error_val(), UTF8_TO_TCHAR(asio2::get_last_error_msg().c_str()) );
	});
	
	//StringCast<UTF8CHAR>(*host)
	//auto str = StringCast<UTF8CHAR>(*host));
	
	Ptr_Rpc_Server->start( std::string_view( TCHAR_TO_UTF8(*host)),port);
	
	Ptr_Rpc_Server->bind(UAsio2_Rpc_Server::Get_STR_RPC_Server_Reply(),&UAsio2_Rpc_Server::Server_Reply,this);
	
	bret = true;
	return bret;
}

TC_RPC_Message  UAsio2_Rpc_Server::Server_Reply(TC_RPC_Message tc_rpc_message)
{
	//RPC_LOG(TEXT("Server_C2S: %d"),tc_rpc_message.MessageType);
	TC_RPC_Message tc_rpc_message2;
	tc_rpc_message2.Data = NewObject<URPC_Hello_Message>();
	
	return tc_rpc_message2;
}
