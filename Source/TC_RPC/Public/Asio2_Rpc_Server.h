// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RpcHeader.h"
#include "RPC_Message.h"

#include "Asio2_Rpc_Server.generated.h"

/**
 * 
 */
UCLASS()
class TC_RPC_API UAsio2_Rpc_Server : public UObject
{
	GENERATED_BODY()
public:

	UAsio2_Rpc_Server();
	//static TObjectPtr<UAsio2_Rpc_Server> Create();
	virtual bool Init(uint32 recv_buf_size =RPC_RECEIVE_BUFFER_SIZE,uint32 recv_buf_max_size = RPC_RECEIVE_MAX_BUFFER_SIZE);
	virtual bool Bind_Start(FString host,uint32 port);

	virtual TC_RPC_Message Server_Reply(TC_RPC_Message tc_rpc_message);

	virtual void Stop();

	static std::string Get_STR_RPC_Server_Reply()
	{
		return "Server_Reply";
	}
protected:
	TUniquePtr<asio2::rpc_kcp_server> Ptr_Rpc_Server;
};
