// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"
#include "RPC_Log.h"
#include "UObject/Object.h"
#include <memory>
#include "RPC_Message.generated.h"


/**
 *Rpc_Message消息基类
 */
UCLASS(Abstract)
class TC_RPC_API URPC_Message : public UObject
{
	GENERATED_BODY()

public:
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
	virtual  uint16 GetMessageType()
	{
		return  0;/*StaticCast<uint16>(ERPC_MessageType::None);*/
	}
};

/********
*******URPC_MessageHandler消息处理基类
*******
*******/

UCLASS(Abstract)
class TC_RPC_API URPC_MessageHandler : public UObject
{
	GENERATED_BODY()
public:
	virtual uint64 Get_Rpc_Ability()
	{
		return 0;
	}
	virtual TSubclassOf<class URPC_Message> Get_Rpc_Request_Class()
	{
		return nullptr;
	}
	virtual TSubclassOf<class URPC_Message> Get_Rpc_Reply_Class()
	{
		return nullptr;
	}
	virtual TObjectPtr<URPC_Message> Process(TObjectPtr<URPC_Message> rpc_request)
	{
		TObjectPtr<URPC_Message> rpc_reply;
		if ( !IsValid(Get_Rpc_Reply_Class()) )
		{
			RPC_ERROR(TEXT("URPC_MessageHandler Get_Rpc_Reply_Class"));
			return rpc_reply;
		}
		if ( !IsValid(Get_Rpc_Request_Class()) )
		{
			RPC_ERROR(TEXT("URPC_MessageHandler Get_Rpc_Request_Class"));
			return rpc_reply;
		}
		if (!IsValid(rpc_request))
		{
			RPC_ERROR(TEXT("URPC_MessageHandler rpc_request Null"));
			return rpc_reply;
		}
		rpc_reply = NewObject<URPC_Message>(this,Get_Rpc_Reply_Class());
		return rpc_reply;
	}
};

