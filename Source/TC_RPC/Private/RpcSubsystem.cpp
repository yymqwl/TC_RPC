// Fill out your copyright notice in the Description page of Project Settings.


#include "RpcSubsystem.h"

#include "RPC_Log.h"


void URpcSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	RPC_Message_Map.Empty();//清空所有


	TArray<UClass*> SubsystemClasses;
	GetDerivedClasses(URPC_Message::StaticClass(), SubsystemClasses, true);
	for (UClass* SubsystemClass : SubsystemClasses)
	{
		RPC_LOG(TEXT("RPC Add Message:%s"),*SubsystemClass->GetName())
		if (SubsystemClass->HasAnyClassFlags(CLASS_Abstract))//
		{
			continue;
		}

		auto p_rpc_message =SubsystemClass->GetDefaultObject<URPC_Message>();
		auto key= p_rpc_message->GetMessageType();
		//重复添加处理
		if (RPC_Message_Map.Contains(key))
		{
			auto exist_message =RPC_Message_Map.FindRef(key);
			RPC_ERROR(TEXT("RPC Message Same key %s_%s"),*exist_message->GetName(),*SubsystemClass->GetName());
			continue;
		}
		RPC_Message_Map.Add(key,SubsystemClass);

	}
}


void URpcSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

TSubclassOf<URPC_Message> URpcSubsystem::Find_RPC_Message(uint16 id)
{
	auto pv =   RPC_Message_Map.Find(id);
	if (pv == nullptr)
	{
		return nullptr;
	}
	return *pv;
}
