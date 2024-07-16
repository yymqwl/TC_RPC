// Copyright Epic Games, Inc. All Rights Reserved.

#include "TC_RPC.h"
#include "Modules/ModuleManager.h"
#define LOCTEXT_NAMESPACE "FTC_RPCModule"



class FTC_RPCModule : public FDefaultGameModuleImpl
{
public:
	
	virtual void StartupModule() override
	{
		
	}
	virtual void ShutdownModule() override
	{
		
	}
};
#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTC_RPCModule, TC_RPC)