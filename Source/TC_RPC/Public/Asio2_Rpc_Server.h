// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Asio2_Rpc_Net.h"
#include "RpcHeader.h"
#include "Asio2_Rpc_Server.generated.h"

/**
 * 
 */
UCLASS()
class TC_RPC_API UAsio2_Rpc_Server : public UAsio2_Rpc_Net
{
	GENERATED_BODY()
public:

	UAsio2_Rpc_Server();
	//static TObjectPtr<UAsio2_Rpc_Server> Create();
	virtual bool Init(uint32 recv_buf_size =RPC_RECEIVE_BUFFER_SIZE,uint32 recv_buf_max_size = RPC_RECEIVE_MAX_BUFFER_SIZE);
	
	virtual bool Bind_Start(FString host,uint32 port);

	//virtual std::shared_ptr<TC_RPC::TC_RPC_Message> Server_Reply(std::shared_ptr<TC_RPC::TC_RPC_Message> tc_rpc_message);
	
	virtual void Stop();

	//virtual std::shared_ptr<TC_RPC::TC_RPC_Message2> Server_Reply2(std::shared_ptr<TC_RPC::TC_RPC_Message2>  tc_rpc_message);


	//virtual TArray<uint8> Server_Reply3(TArray<uint8> data);

	/*
	static std::string Get_STR_RPC_Server_Reply()
	{
		return "Server_Reply";
	}*/
	/*
	static std::string Get_STR_RPC_Server_Reply2()
	{
		return "Server_Reply2";
	}
	static std::string Get_STR_RPC_Server_Reply3()
	{
		return "Server_Reply3";
	}*/
protected:
	
	TUniquePtr<asio2::rpc_kcp_server> Ptr_Rpc_Server;//rpc_kcp_server
};
