// Fill out your copyright notice in the Description page of Project Settings.


#include "Asio2_Rpc_Client.h"
#include "Asio2_Rpc_Server.h"
#include "RPC_Log.h"
#include "Serialization/ArrayWriter.h"


bool UAsio2_Rpc_Client::Connect_Start(FString host, uint32 port)
{
	bool bret=false;
	if (!Ptr_Rpc_Client.IsValid())
	{
		Ptr_Rpc_Client = MakeUnique<asio2::rpc_kcp_client>();//rpc_client
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

	Ptr_Rpc_Client->async_start(std::string(TCHAR_TO_UTF8(*host)),port);//("127.0.0.1",8010);
	bret=true;
	return bret;
}




void UAsio2_Rpc_Client::Test_Client_Request2()
{
	if (!Ptr_Rpc_Client.IsValid())
	{
		return;
	}
	if (Ptr_Rpc_Client->is_started())
	{
		auto ptr_person_request = std::make_shared<Person>();
		ptr_person_request->age = 10;
		ptr_person_request->name = "xxx";
		Ptr_Rpc_Client->async_call("test_Person",  ptr_person_request).response([](std::shared_ptr<Person> ptr_person_reply)
		{
			if (asio2::get_last_error())
			{
				RPC_LOG(TEXT("last_error_msg: %d_%s"),asio2::get_last_error_val(),UTF8_TO_TCHAR(asio2::last_error_msg().c_str()) );
			}
			else
			{
				RPC_LOG(TEXT("bytes len: %hs"),ptr_person_reply->name.c_str());
			}
		});
		
	}
	
}
void UAsio2_Rpc_Client::Test_Client_Request3()
{
	if (!Ptr_Rpc_Client.IsValid())
	{
		return;
	}
	if (Ptr_Rpc_Client->is_started())
	{
	}
}

void UAsio2_Rpc_Client::Test_Client_Request(TWeakObjectPtr<URPC_Message>  ptr_rpc_message)
{
	if (!Ptr_Rpc_Client.IsValid())
	{
		return;
	}
	if (Ptr_Rpc_Client->is_started())
	{
		RPC_LOG(TEXT("Client_Request"));
		auto tc_rpc_request = Make_TC_RPC_Message(ptr_rpc_message.Get());
	
		//tc_rpc_request.Data =  ptr_rpc_message;
		/*RPC_LOG(TEXT("Rpc_Request: %d"),tc_rpc_request->Data.Num() );
		Ptr_Rpc_Client->async_call(UAsio2_Rpc_Server::Get_STR_RPC_Server_Reply(),tc_rpc_request).response([](std::shared_ptr<TC_RPC::TC_RPC_Message> tc_rpc_reply)
		{
			if (asio2::get_last_error())
			{
				RPC_LOG(TEXT("last_error_msg: %s"),UTF8_TO_TCHAR(asio2::last_error_msg().c_str()) );
			}
			if(tc_rpc_reply != nullptr)
			{
				RPC_LOG(TEXT("Rpc_Reply: %d"),tc_rpc_reply->Data.Num() );
			}
		});*/
	}
}


TCoroutine<TObjectPtr<URPC_Message>>  UAsio2_Rpc_Client::Request(const TObjectPtr<URPC_Message>& ptr_rpc_message)
{
	TObjectPtr<URPC_Message> ptr_reply_message;
	TC_RPC_Message tc_rpc_request;
	if(!IsValid(ptr_rpc_message) )
	{
		RPC_ERROR(TEXT("Request Null ptr_rpc_message"));
		co_return ptr_reply_message;
	}
	if (!Ptr_Rpc_Client->is_started())
	{
		RPC_ERROR(TEXT("Ptr_Rpc_Client not started"));
		co_return ptr_reply_message;
	}

	//ptr_rpc_message->GetMessageType();
	auto ptr_tc_rpc_msg = Make_TC_RPC_Message(ptr_rpc_message);
	if (ptr_tc_rpc_msg == nullptr)
	{
		RPC_ERROR(TEXT("Make_TC_RPC_Message Null"));
		co_return ptr_reply_message;
	}
	TPromise<std::shared_ptr<TC_RPC_Message>> p_result;
	Ptr_Rpc_Client->async_call(UAsio2_Rpc_Server::Get_STR_RPC_Reply(),ptr_tc_rpc_msg).response([&p_result](std::shared_ptr<TC_RPC_Message> ptr_tc_rpc_reply)
	{
		if (asio2::get_last_error())
		{
			RPC_ERROR(TEXT("last_error_msg: %s"),UTF8_TO_TCHAR(asio2::last_error_msg().c_str()) );
			if (ptr_tc_rpc_reply != nullptr)
			{
				ptr_tc_rpc_reply->ErrorCode = StaticCast<uint8>(ERPC_ErrorCode::Asio2NetError);
			}
		}
		p_result.SetValue(ptr_tc_rpc_reply);
	});
	auto ptr_rpc_reply = co_await p_result.GetFuture();
	
	if (ptr_rpc_reply == nullptr ) //有可能为 null
	{
		RPC_ERROR(TEXT("UAsio2_Rpc_Client ptr_rpc_reply Null"));
		co_return nullptr;
	}
	

	if (ptr_rpc_reply->ErrorCode != StaticCast<uint8>(ERPC_ErrorCode::None))
	{
		RPC_ERROR(TEXT("ptr_rpc_reply Error: %d"),ptr_rpc_reply->ErrorCode );
		co_return ptr_reply_message;
	}
	//因为UE的默认GC系统所以必须要切线程干
	if (!IsInGameThread())
	{
		co_await UE5Coro::Async::MoveToThread(ENamedThreads::GameThread);
	}

	auto rpc_message_class = Get_RpcSubsystem()->Find_RPC_Message(ptr_rpc_reply->MessageType);
	if (!IsValid(rpc_message_class))
	{
		RPC_ERROR(TEXT("rpc_message_class Error: %d"),ptr_rpc_reply->MessageType );
		co_return ptr_reply_message;
	}

	//co_return ptr_reply_message;
	
	ptr_reply_message = NewObject<URPC_Message>(GetTransientPackage(),rpc_message_class);
	FMemoryReader MessageReader(ptr_rpc_reply->Data);//引用传值，不用考虑拷贝
	ptr_reply_message->Serialize(MessageReader);
	if(IsValid(ptr_reply_message))
	{
		ptr_reply_message->MarkAsGarbage();	
	}
	co_return ptr_reply_message;
}


/*
TCoroutine<TC_RPC_Message> UAsio2_Rpc_Client::Request(uint16 messageType,TWeakObjectPtr<URPC_Message> ptr_rpc_message)
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
	//tc_rpc_request.Data = ptr_rpc_message;

	
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

	auto tc_rpc_reply = co_await result.GetFuture();
	co_return tc_rpc_reply;
}*/




void UAsio2_Rpc_Client::Stop()
{
	if (Ptr_Rpc_Client.IsValid())
	{
		Ptr_Rpc_Client->stop();
	}
}
