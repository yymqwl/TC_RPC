// Fill out your copyright notice in the Description page of Project Settings.


#include "TestRpcActor.h"

// Sets default values
ATestRpcActor::ATestRpcActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestRpcActor::BeginPlay()
{
	Super::BeginPlay();

	FString host = TEXT("127.0.0.1");
	uint32 port = 8010;
	
	Ptr_Server = NewObject<UAsio2_Rpc_Server>();
	Ptr_Server->Init();
	Ptr_Server->Bind_Start(host,port);
	
	
	Ptr_Client = NewObject<UAsio2_Rpc_Client>();
	Ptr_Client->Connect_Start(host,port);
	
	//Server->Init();
}
void ATestRpcActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (Ptr_Server!=nullptr)
	{
		Ptr_Server->Stop();
		Ptr_Server = nullptr;
	}
	if (Ptr_Client!=nullptr)
	{
		Ptr_Client->Stop();
		Ptr_Client = nullptr;
	}
}
void ATestRpcActor::Call_Test()
{
	
	//auto rpc_msg =NewObject<URPC_Message>();
	//
	/*
	TC_RPC::TC_RPC_Message msg {.MessageType =100 };
	
	msg.Data = NewObject<URPC_Message>();*/
	if (Ptr_Client!=nullptr)
	{
		TArray<uint8> ay;
		ay.Push(1);
		ay.Push(2);
		ay.Push(3);
		Ptr_Client->Test_Client_Request2( MoveTemp(ay));
		//Ptr_Client->Test_Client_Request( TWeakObjectPtr<URPC_Message>(NewObject<URPC_Message>() ));	
	}
	
}


// Called every frame
void ATestRpcActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

