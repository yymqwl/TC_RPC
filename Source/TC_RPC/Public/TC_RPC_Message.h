#pragma once

#include "RPC_Log.h"
#include "RPC_ErrorCode.h"
#include "RPC_Message.h"
#include "Serialization/BufferArchive.h"

namespace TC_RPC
{
	struct  TC_RPC_Message//必须要默认值,否者UE会设置乱值
	{
	public:
		uint16 MessageType;//
		uint8 ErrorCode;//
		TSharedPtr<URPC_Message> Data;
		TC_RPC_Message(uint16 messageType = 0 /*StaticCast<uint16>(ERPC_MessageType::None*/,
			uint8 errorCode = StaticCast<uint8>(ERPC_ErrorCode::None),
			TSharedPtr<URPC_Message> ptr = nullptr
			)
		:MessageType(messageType),ErrorCode(errorCode),Data(ptr)
		{
		
		}
		template<class Archive>
		void save( Archive & ar ) const
		{
			//RPC_LOG(TEXT("save"));
			//ar << MessageType;
			if (Data.Get() == nullptr)//没有数值
			{
				RPC_ERROR(TEXT("TC_RPC_Message Data Null"));
				//ErrorCode = StaticCast<uint8>(ERPC_ErrorCode::Serialize);
				ar << StaticCast<uint8>(ERPC_ErrorCode::Serialize);
				return;
			}
		
		
			FBufferArchive MessageWriter;
			Data->Serialize(MessageWriter);
			int64 len = MessageWriter.TotalSize();
			if (len <= 0)
			{
				RPC_ERROR(TEXT("TC_RPC_Message Data Null"));
				ar << StaticCast<uint8>(ERPC_ErrorCode::Serialize);
				return;
			}
			ar << ErrorCode;
			ar << MessageType;
			ar << len;
			ar << cereal::binary_data(const_cast<uint8*>(MessageWriter.GetData()),MessageWriter.TotalSize());
		}
		template<class Archive>
		void load( Archive & ar )
		{
			//RPC_LOG(TEXT("load"));
			ar >> ErrorCode;
			//RPC_LOG(TEXT("load: %d ,%d"), StaticCast<uint8>(ERPC_ErrorCode::None),ErrorCode);
			if (ErrorCode != StaticCast<uint8>(ERPC_ErrorCode::None) )
			{
				RPC_ERROR(TEXT("TC_RPC_Message load ErrorCode %d"),ErrorCode);
				return ;
			}
			ar >> MessageType;
			int64 len ;
			ar >> len;
			if (len  <= 0 )
			{
				RPC_ERROR(TEXT("TC_RPC_Message Load len <0 "));
				return;
			}
			TArray<uint8> BinaryData;
			BinaryData.SetNum(len);

			//BinaryData.GetData()
			//BinaryData.SetNum()

		
			FMemoryReader MessageReader(BinaryData);
			ar >> cereal::binary_data(const_cast<uint8*>(BinaryData.GetData()),len);
			//FBufferArchive MessageWriter();
			//MessageWriter.begin()
			//MessageWriter.Serialize()
			//TArray<uint8> Bufs;
			//Bufs.Reserve(len);

			/*指定消息序列化
			Data = NewObject<URPC_Hello_Message>();
			Data->Serialize(MessageReader);
			RPC_LOG(TEXT("消息读取成功%d"),Data->GetMessageType());
			*/
		
		}
	};
}
