// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RpcHeader.h"
#include "RPC_Message.h"
#include "TC_RPC_Message.h"
#include "UE5Coro.h"

#include "Asio2_Rpc_Client.generated.h"

using namespace TC_RPC;
using namespace UE5Coro;
/**
 * 
 */
UCLASS()
class TC_RPC_API UAsio2_Rpc_Client : public UObject
{
public:
	GENERATED_BODY()
	virtual bool Connect_Start(FString host,uint32 port);

	//测试方法
	//UFUNCTION()
	virtual void Test_Client_Request(TWeakObjectPtr<URPC_Message>  ptr_rpc_message);

	virtual void Test_Client_Request2(TArray<uint8>&& bytes);
	virtual void Test_Client_Request3(TArray<uint8>&& bytes);
	//TCoroutine<TC_RPC_Message> Request(  )

	TCoroutine<TC_RPC_Message> Request(uint16  messageType,TWeakObjectPtr<URPC_Message> ptr_rpc_message);

	void Stop();
	
protected:
	TUniquePtr<asio2::rpc_kcp_client> Ptr_Rpc_Client;
};
