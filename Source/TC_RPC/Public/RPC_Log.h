// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
//using Error = ELogVerbosity::Type;

TC_RPC_API DECLARE_LOG_CATEGORY_EXTERN(RPC_Log,Log,All);

//日志
#define RPC_LOG(Format, ...) \
UE_LOG(RPC_Log,Log,Format,##__VA_ARGS__)

//错误日志
#define RPC_ERROR(Format, ...) \
UE_LOG(RPC_Log,Error,Format,##__VA_ARGS__)


