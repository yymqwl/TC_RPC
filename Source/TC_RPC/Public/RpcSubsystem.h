// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPC_Message.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RpcSubsystem.generated.h"

/**
 *
 * RPC配置
 */
UCLASS()
class TC_RPC_API URpcSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual  void  Deinitialize() override;

	TSubclassOf<URPC_Message> Find_RPC_Message(uint16 id);

	TArray<TSubclassOf<URPC_MessageHandler>>& Get_All_RPC_MessageHandler();
protected:
	UPROPERTY()
	TMap<uint16,TSubclassOf<URPC_Message>> RPC_Message_Map;//类查找
	
	UPROPERTY()//根据请求的MsgType确定Handle,一个消息不能有二个处理方法。此处为了简化操作这么干，如果前后端处理方式不一样消息会定为ping_client,ping_server
	TArray<TSubclassOf<URPC_MessageHandler>> RPC_MessageHandler_Array;
};
