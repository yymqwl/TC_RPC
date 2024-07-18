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

protected:
	UPROPERTY()
	TMap<uint16,TSubclassOf<URPC_Message>> RPC_Message_Map;//类查找
};
