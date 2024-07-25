// Fill out your copyright notice in the Description page of Project Settings.


#include "Asio2_Rpc_Net.h"

#include "Serialization/BufferArchive.h"


void UAsio2_Rpc_Net::Create_RPC_MessageHandler_Map(uint64 iAbility)
{
	RPC_MessageHandler_Map.Empty();


	auto SubsystemClasses = Get_RpcSubsystem()->Get_All_RPC_MessageHandler();
	//TArray<UClass*> SubsystemClasses = Get_RpcSubsystem()->Get_All_RPC_MessageHandler();

	for (TSubclassOf<URPC_MessageHandler> SubsystemClass : SubsystemClasses)
	{
		RPC_LOG(TEXT("URPC_MessageHandler Add:%s"),*SubsystemClass->GetName())
		if (SubsystemClass->HasAnyClassFlags(CLASS_Abstract))//
		{
			continue;
		}
		
		auto p_rpc_messagehandler =SubsystemClass->GetDefaultObject<URPC_MessageHandler>();
		if ( (p_rpc_messagehandler->Get_Rpc_Ability()&iAbility) > 0)
		{
			auto rpc_message =p_rpc_messagehandler->Get_Rpc_Request_Class()->GetDefaultObject<URPC_Message>();

			auto key= rpc_message->GetMessageType();
			if (RPC_MessageHandler_Map.Contains(key))
			{
				auto exist_message =RPC_MessageHandler_Map.FindRef(key);
				RPC_ERROR(TEXT("RPC URPC_MessageHandler Same key %s_%s"),*exist_message->GetName(),*SubsystemClass->GetName());
				continue;
			}
			RPC_MessageHandler_Map.Add(key,SubsystemClass);
		}
	}
	
}

TObjectPtr<URPC_Message> UAsio2_Rpc_Net::Take_RPC_Message(const std::shared_ptr<TC_RPC::TC_RPC_Message>& ptr_tc_rpc_message)
{
	TObjectPtr<URPC_Message> ptr_rpc_message= nullptr;
	if ( ptr_tc_rpc_message == nullptr)
	{
		return ptr_rpc_message;
	}
	if (ptr_tc_rpc_message->Data.IsEmpty())
	{
		return ptr_rpc_message;
	}
	auto rpc_message_class = Get_RpcSubsystem()->Find_RPC_Message(ptr_tc_rpc_message->MessageType);
	if (!IsValid(rpc_message_class))
	{
		return ptr_rpc_message;
	}
	ptr_rpc_message = NewObject<URPC_Message>(GetTransientPackage(),rpc_message_class);
	return ptr_rpc_message;
}

std::shared_ptr<TC_RPC::TC_RPC_Message> UAsio2_Rpc_Net::Make_TC_RPC_Message(const TObjectPtr<URPC_Message>& rpc_message)
{
	if (!IsValid(rpc_message))
	{
		return nullptr;
	}
	std::shared_ptr<TC_RPC::TC_RPC_Message> ptr_tc_rpc_message=std::make_shared<TC_RPC::TC_RPC_Message>();
	ptr_tc_rpc_message->MessageType = rpc_message->GetMessageType();

	FBufferArchive MessageWriter;
	rpc_message->Serialize(MessageWriter);
	ptr_tc_rpc_message->Data = MoveTemp(MessageWriter);//不用重复复制
	return ptr_tc_rpc_message;
}


TObjectPtr<URpcSubsystem> UAsio2_Rpc_Net::Get_RpcSubsystem()
{
	if (!IsValid(Ptr_RpcSubsystem))
	{
		Ptr_RpcSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<URpcSubsystem>();
	}

	return Ptr_RpcSubsystem;
}

TSubclassOf<URPC_MessageHandler> UAsio2_Rpc_Net::Find_URPC_MessageHandler(uint16 id)
{
	auto pv =   RPC_MessageHandler_Map.Find(id);
	if (pv == nullptr)
	{
		return nullptr;
	}
	return *pv;
}


std::shared_ptr<TC_RPC::TC_RPC_Message> UAsio2_Rpc_Net::Reply(std::shared_ptr<TC_RPC::TC_RPC_Message> tc_rpc_message)
{
	std::shared_ptr<TC_RPC::TC_RPC_Message> ptr_tc_rpc_message=std::make_shared<TC_RPC::TC_RPC_Message>();
	
	if (tc_rpc_message->ErrorCode != StaticCast<uint8>(TC_RPC::ERPC_ErrorCode::None)  )
	{
		RPC_ERROR(TEXT("UAsio2_Rpc_Net Reply Error Code:%d"),tc_rpc_message->ErrorCode);
		ptr_tc_rpc_message->ErrorCode = tc_rpc_message->ErrorCode;//返回一样错误码
		return ptr_tc_rpc_message;
	}
	auto rpc_MessageHandler = Find_URPC_MessageHandler(tc_rpc_message->MessageType);
	if (rpc_MessageHandler == nullptr)
	{
		RPC_ERROR(TEXT("UAsio2_Rpc_Net Reply rpc_MessageHandler MessageType Not Found:%d"),tc_rpc_message->MessageType);
		ptr_tc_rpc_message->ErrorCode= StaticCast<uint8>(TC_RPC::ERPC_ErrorCode::MessageHandleNotFound);  
		return ptr_tc_rpc_message;
	}


	////////////////////////////////////////////查找前的条件选择
	auto rpc_mh= rpc_MessageHandler->GetDefaultObject<URPC_MessageHandler>();
	check(rpc_mh);
		
	auto rpc_message_class = Get_RpcSubsystem()->Find_RPC_Message(tc_rpc_message->MessageType);
	if (!IsValid(rpc_message_class))
	{
		RPC_ERROR(TEXT("rpc_message_class Error: %d"), tc_rpc_message->MessageType );
		ptr_tc_rpc_message->ErrorCode= StaticCast<uint8>(TC_RPC::ERPC_ErrorCode::MessageTypeNotFound);  
		return ptr_tc_rpc_message;
	}
	///////////////////////////////////////////

	/*没有切线程
	TObjectPtr<URPC_Message> rpc_requst_msg = Take_RPC_Message(tc_rpc_message);
	auto tc_rpc_message_reply = Make_TC_RPC_Message(rpc_mh->Process(rpc_requst_msg));
	if (IsValid(rpc_requst_msg))
	{
		rpc_requst_msg->MarkAsGarbage();
	}
	return tc_rpc_message_reply;
	*/
	/*
	TObjectPtr<URPC_Message> rpc_requst_msg = Take_RPC_Message(tc_rpc_message);
	auto tc_rpc_message_reply = Make_TC_RPC_Message(rpc_mh->Process(rpc_requst_msg));
	return  tc_rpc_message_reply;
	*/
	//切换到主线程处理函数,并发变差，但是效果
	const TSharedRef<TPromise<std::shared_ptr<TC_RPC::TC_RPC_Message>>, ESPMode::ThreadSafe> Promise = MakeShareable(new TPromise<std::shared_ptr<TC_RPC::TC_RPC_Message>>());
	auto main_reply_fun = [this,tc_rpc_message,Promise,rpc_mh]()
	{
		TObjectPtr<URPC_Message> rpc_requst_msg = Take_RPC_Message(tc_rpc_message);
		auto tc_rpc_message_reply = Make_TC_RPC_Message(rpc_mh->Process(rpc_requst_msg));
		if (IsValid(rpc_requst_msg))
		{
			rpc_requst_msg->MarkAsGarbage();
		}
		Promise->SetValue(tc_rpc_message_reply);
	};
	
	if (IsInGameThread())
	{
		main_reply_fun();
	}
	else
	{
		AsyncTask(
		ENamedThreads::GameThread,
		MoveTemp(main_reply_fun)
		);
	}
	return 	Promise->GetFuture().Get();
}
