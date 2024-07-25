// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Asio2_Rpc_Net.h"
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
 * 基于Asio2库实现的Rpc框架客户端
 */
UCLASS()
class TC_RPC_API UAsio2_Rpc_Client : public UAsio2_Rpc_Net
{
public:
	GENERATED_BODY()
	virtual bool Connect_Start(FString host,uint32 port);

	//测试方法
	//UFUNCTION()
	virtual void Test_Client_Request(TWeakObjectPtr<URPC_Message>  ptr_rpc_message);

	virtual void Test_Client_Request2();
	virtual void Test_Client_Request3();
	//TCoroutine<TC_RPC_Message> Request(  )

	//序列化,反序列化操作放在GameThread，因为想用UE的反射系统，不
	//TCoroutine<URPC_Message> Request(uint16  messageType,TWeakObjectPtr<URPC_Message> ptr_rpc_message);
	//调用和返回时候在主线程
	TCoroutine<TObjectPtr<URPC_Message>> Request(const TObjectPtr<URPC_Message>&  ptr_rpc_message);

	
	
	void Stop();

protected:
	TUniquePtr<asio2::rpc_kcp_client> Ptr_Rpc_Client;//rpc_kcp_client
};
