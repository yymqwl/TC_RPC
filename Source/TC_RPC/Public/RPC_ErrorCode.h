#pragma once

//#include "RPC_ErrorCode.generated.h"


namespace TC_RPC
{
	//错误码
	//UENUM()
	enum class ERPC_ErrorCode :uint8
	{
		None = 0,
		PtrInvalid,//指针无效
		ClientNotStart,//客户端没打开
		Asio2NetError,//网络问题
		Serialize,
		Max
	};
	ENUM_CLASS_FLAGS(ERPC_ErrorCode)
}
/*
//错误码
UENUM()
enum class ERPC_ErrorCode :uint8
{
	None = 0,
	Serialize,
	Max
};
ENUM_CLASS_FLAGS(ERPC_ErrorCode)
*/