// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RpcHeader.h"
#include "RPC_Message.h"
#include "Asio2_Rpc_Client.generated.h"

/**
 * 
 */
UCLASS()
class TC_RPC_API UAsio2_Rpc_Client : public UObject
{
public:
	GENERATED_BODY()
	virtual bool Connect_Start(FString host,uint32 port);
	
	virtual void Client_Request(TObjectPtr<URPC_Message>  rpc_message);

	void Stop();
	
protected:
	TUniquePtr<asio2::rpc_kcp_client> Ptr_Rpc_Client;
};
