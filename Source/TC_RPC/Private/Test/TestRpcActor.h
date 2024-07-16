// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Asio2_Rpc_Server.h"
#include "Asio2_Rpc_Client.h"
#include "TestRpcActor.generated.h"



UCLASS(BlueprintType,Blueprintable)
class TC_RPC_API ATestRpcActor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATestRpcActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Call_Test();

private:
	UPROPERTY()
	UAsio2_Rpc_Server* Ptr_Server;
	UPROPERTY()
	UAsio2_Rpc_Client* Ptr_Client;
};
