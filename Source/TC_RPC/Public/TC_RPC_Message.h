#pragma once

#include "RPC_Log.h"
#include "RPC_ErrorCode.h"
#include "RPC_Message.h"
#include "CoreMinimal.h"
#include "RpcSubsystem.h"
#include "Serialization/BufferArchive.h"

namespace TC_RPC
{
	struct  TC_RPC_Message//必须要默认值,否者UE会设置乱值
	{
	public:
		uint16 MessageType;//
		uint8 ErrorCode;//
		TWeakObjectPtr<URPC_Message> Data;
		TC_RPC_Message(uint16 messageType = 0 /*StaticCast<uint16>(ERPC_MessageType::None*/,
			uint8 errorCode = StaticCast<uint8>(ERPC_ErrorCode::None),
			TWeakObjectPtr<URPC_Message> ptr = nullptr
			)
		:MessageType(messageType),ErrorCode(errorCode),Data(ptr)
		{
			//Data->RemoveFromRoot()
		}

		void AddToRoot()
		{
			if (Data.IsValid())
			{
				Data->AddToRoot();
			}
		}
		void RemoveFromRoot()
		{
			if (Data.IsValid())
			{
				Data->RemoveFromRoot();
			}
		}
		
		template<class Archive>
		void save( Archive & ar ) const
		{
			//RPC_LOG(TEXT("save"));
			//ar << MessageType;
			if (!Data.IsValid())//没有数值
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
			int64 len;
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


			
			checkf((GWorld == nullptr),TEXT("Null GWorld"));
			auto  RpcSubsystem = GWorld->GetGameInstance()->GetSubsystem<URpcSubsystem>();
			checkf((RpcSubsystem == nullptr),TEXT("Null RpcSubsystem"));
			auto rpc_message_class = RpcSubsystem->Find_RPC_Message(MessageType);

			if (rpc_message_class == nullptr)
			{
				ErrorCode = StaticCast<uint8>(ERPC_ErrorCode::MessageTypeNotFound);
				return;
			}
			
			
			//auto Data= NewObject<URPC_Message>(GetTransientPackage(),rpc_message_class);
			//Data->Serialize(MessageReader);
			//AddToRoot();//添加缓存
			
			//GetWorld()
			//GetGameInstance()
			//GetWorld();
			//FBufferArchive MessageWriter();
			//MessageWriter.begin()
			//MessageWriter.Serialize()
			//TArray<uint8> Bufs;
			//Bufs.Reserve(len);

			//指定消息序列化
			/*Data = NewObject<URPC_Hello_Message>();
			Data->Serialize(MessageReader);
			RPC_LOG(TEXT("消息读取成功%d"),Data->GetMessageType());
			*/
		
		}
	};


	struct TC_RPC_Message2
	{
	public:
		uint16 MessageType;//
		uint8 ErrorCode;//
		TArray<uint8> Data;
		TC_RPC_Message2()
		{
			RPC_LOG(TEXT("TC_RPC_Message2()"));
		}
		~TC_RPC_Message2()
		{
			RPC_LOG(TEXT("~TC_RPC_Message2"));
		}
		template<class Archive>
		void save( Archive& ar ) const
		{
			RPC_LOG(TEXT("TC_RPC_Message2 save"));
			//ar << MessageType;
			if (Data.Num() == 0)//没有数值
			{
				RPC_ERROR(TEXT("TC_RPC_Message Data Null"));
				//ErrorCode = StaticCast<uint8>(ERPC_ErrorCode::Serialize);
				ar << StaticCast<uint8>(ERPC_ErrorCode::Serialize);
				return;
			}
		

			/*
			FBufferArchive MessageWriter;
			Data->Serialize(MessageWriter);
			int64 len = MessageWriter.TotalSize();
			
			if (len <= 0)
			{
				RPC_ERROR(TEXT("TC_RPC_Message Data Null"));
				ar << StaticCast<uint8>(ERPC_ErrorCode::Serialize);
				return;
			}*/
			ar << ErrorCode;
			ar << MessageType;
			ar << Data.Num();
			ar << cereal::binary_data(const_cast<uint8*>(Data.GetData()),Data.Num());
		}
		template<class Archive>
		void load( Archive& ar )
		{
			RPC_LOG(TEXT("TC_RPC_Message2 load"));
			ar >> ErrorCode;
			//RPC_LOG(TEXT("load: %d ,%d"), StaticCast<uint8>(ERPC_ErrorCode::None),ErrorCode);
			if (ErrorCode != StaticCast<uint8>(ERPC_ErrorCode::None) )
			{
				RPC_ERROR(TEXT("TC_RPC_Message load ErrorCode %d"),ErrorCode);
				return ;
			}
			ar >> MessageType;
			int64 len;
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
		}
	};
	
	
}


template<class Archive>
void save(Archive& ar,const TArray<uint8>& ay)
{
	RPC_LOG(TEXT("TArray save"));
	ar << ay.Num();
	ar << cereal::binary_data(const_cast<uint8*>(ay.GetData()),ay.Num());
	//ar << j.dump();
}

template<class Archive>
void load(Archive& ar,const TArray<uint8>& ay)
{
	RPC_LOG(TEXT("TArray load"));
	int32 len;
	TArray<uint8> BinaryData;
	ar >> len;
	BinaryData.SetNum(len);
	ar >> cereal::binary_data(const_cast<uint8*>(BinaryData.GetData()),len);
}