#pragma once

#include "RPC_MessageType.generated.h"





//消息类型
UENUM()
enum class ERPC_MessageType : uint16
{
	None = 0,
	Hello_Message = 100,
	Max
};
ENUM_CLASS_FLAGS(ERPC_MessageType)



//错误码
UENUM()
enum class ERPC_ErrorCode :uint8
{
	None = 0,
	Serialize=10,
	Max
};
ENUM_CLASS_FLAGS(ERPC_ErrorCode)