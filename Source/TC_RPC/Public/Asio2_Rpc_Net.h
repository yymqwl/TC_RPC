// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RpcSubsystem.h"
#include "TC_RPC_Message.h"
#include "UObject/Object.h"
#include "Asio2_Rpc_Net.generated.h"

/**
 * 
 */
UCLASS()
class TC_RPC_API UAsio2_Rpc_Net : public UObject
{
	GENERATED_BODY()
public:

	//根据模块去做运算
	virtual void Create_RPC_MessageHandler_Map(uint64 iAbility);

	TObjectPtr<URpcSubsystem>  Get_RpcSubsystem();

	
	TSubclassOf<URPC_MessageHandler> Find_URPC_MessageHandler(uint16 id);


	//std::shared_ptr<TC_RPC::TC_RPC_Message> Make
	std::shared_ptr<TC_RPC::TC_RPC_Message> Make_TC_RPC_Message(const TObjectPtr<URPC_Message>& rpc_message);

	
	TObjectPtr<URPC_Message> Take_RPC_Message(const std::shared_ptr<TC_RPC::TC_RPC_Message>& ptr_tc_rpc_message);
	
	//因为在UE里面实现这个把消息处理拉到主线程
	virtual std::shared_ptr<TC_RPC::TC_RPC_Message> Reply(std::shared_ptr<TC_RPC::TC_RPC_Message> tc_rpc_message);
	
	static std::string Get_STR_RPC_Reply()
	{
		return "Reply";
	}
protected:
	UPROPERTY()
	TMap<uint16,TSubclassOf<URPC_MessageHandler>> RPC_MessageHandler_Map;

	UPROPERTY()
	TObjectPtr<URpcSubsystem> Ptr_RpcSubsystem;
};
