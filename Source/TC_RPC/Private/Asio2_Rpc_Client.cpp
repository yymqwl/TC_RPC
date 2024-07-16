// Fill out your copyright notice in the Description page of Project Settings.


#include "Asio2_Rpc_Client.h"
#include "Asio2_Rpc_Server.h"
#include "RPC_Log.h"


bool UAsio2_Rpc_Client::Connect_Start(FString host, uint32 port)
{
	bool bret=false;
	if (!Ptr_Rpc_Client.IsValid())
	{
		Ptr_Rpc_Client = MakeUnique<asio2::rpc_kcp_client>();
	}
	
	if (Ptr_Rpc_Client->is_started())
	{
		return bret;
	}
	
	
	Ptr_Rpc_Client->bind_connect([&]()
	{
		if (asio2::get_last_error())
		{
			RPC_ERROR(TEXT("bind_connect Error %d %s"),asio2::last_error_val(),UTF8_TO_TCHAR(asio2::last_error_msg().c_str()) );
			return;
		}
		RPC_LOG(TEXT("连接成功"));
	});

	Ptr_Rpc_Client->async_start(std::string_view(TCHAR_TO_UTF8(*host)),port);//("127.0.0.1",8010);
	bret=true;
	return bret;
}



void UAsio2_Rpc_Client::Test_Client_Request( TSharedPtr<URPC_Message>  ptr_rpc_message)
{
	if (!Ptr_Rpc_Client.IsValid())
	{
		return;
	}
	if (Ptr_Rpc_Client->is_started())
	{
		RPC_LOG(TEXT("Client_Request"));
		TC_RPC::TC_RPC_Message tc_rpc_request;
		tc_rpc_request.MessageType = 10;
		tc_rpc_request.Data =  ptr_rpc_message;
		Ptr_Rpc_Client->async_call(UAsio2_Rpc_Server::Get_STR_RPC_Server_Reply(),tc_rpc_request).response([](TC_RPC::TC_RPC_Message tc_rpc_reply)
		{
			if (asio2::get_last_error())
			{
				RPC_LOG(TEXT("last_error_msg: %s"),UTF8_TO_TCHAR(asio2::last_error_msg().c_str()) );
			}
		});
	}
}
TCoroutine<TC_RPC_Message> UAsio2_Rpc_Client::Request(uint16 messageType,TSharedPtr<URPC_Message> ptr_rpc_message)
{
	TC_RPC::TC_RPC_Message tc_rpc_request;
	if(!ptr_rpc_message.IsValid())
	{
		tc_rpc_request.ErrorCode = StaticCast<uint8>(ERPC_ErrorCode::PtrInvalid);
		co_return tc_rpc_request;
	}
	if (!Ptr_Rpc_Client->is_started())
	{
		tc_rpc_request.ErrorCode = StaticCast<uint8>(ERPC_ErrorCode::ClientNotStart);
		co_return tc_rpc_request;
	}
	tc_rpc_request.MessageType = messageType;
	tc_rpc_request.Data = ptr_rpc_message;
	TPromise<TC_RPC_Message> result;
	Ptr_Rpc_Client->async_call(UAsio2_Rpc_Server::Get_STR_RPC_Server_Reply(),tc_rpc_request).response([&](TC_RPC_Message tc_rpc_reply)
	{
		if (asio2::get_last_error())
		{
			RPC_ERROR(TEXT("last_error_msg: %s"),UTF8_TO_TCHAR(asio2::last_error_msg().c_str()) );
			tc_rpc_reply.ErrorCode = StaticCast<uint8>(ERPC_ErrorCode::Asio2NetError);
			result.SetValue(tc_rpc_reply);
		}
		else
		{
			result.SetValue(tc_rpc_reply);
		}
	});

	co_return co_await result.GetFuture();
	
}


void UAsio2_Rpc_Client::Stop()
{
	
	if (Ptr_Rpc_Client.IsValid())
	{
		Ptr_Rpc_Client->stop();
	}
}
