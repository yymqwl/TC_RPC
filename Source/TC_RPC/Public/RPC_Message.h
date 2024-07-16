// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include <cereal/cereal.hpp>

#include "CoreMinimal.h"
#include "RPC_Log.h"
#include "RPC_MessageType.h"
#include "Serialization/BufferArchive.h"
#include "UObject/Object.h"
#include "RPC_Message.generated.h"

/**
 * 
 */
UCLASS()
class TC_RPC_API URPC_Message : public UObject
{
	GENERATED_BODY()

public:
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
	virtual  uint16 GetMessageType()
	{
		return  0;/*StaticCast<UINT16>(ERPC_MessageType::None);*/
	}
	
};

UCLASS()
class TC_RPC_API URPC_Hello_Message : public URPC_Message
{
	GENERATED_BODY()

public:
	virtual uint16 GetMessageType() override
	{
		return  StaticCast<uint16>(ERPC_MessageType::Hello_Message);
	}
};

struct SomeData
{
	int32_t id;
  
	template <class Archive>
	void save( Archive & ar ) const
	{
		ar( id );
	}
      
	template <class Archive>
	void load( Archive & ar )
	{
		ar( id );
	}
};



//namespace TC_RPC
//{

//顺序 ErrorCode
//MessageType
//
struct  TC_RPC_Message//必须要默认值,否者UE会设置乱值
{
public:
	uint16 MessageType;//
	uint8 ErrorCode;//
	TObjectPtr<URPC_Message> Data;
	TC_RPC_Message(uint16 messageType = StaticCast<uint16>(ERPC_MessageType::None),
		uint8 errorCode = StaticCast<uint8>(ERPC_ErrorCode::None),
		TObjectPtr<URPC_Message> ptr = nullptr
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

		Data = NewObject<URPC_Hello_Message>();
		Data->Serialize(MessageReader);
		RPC_LOG(TEXT("消息读取成功%d"),Data->GetMessageType());
		
	}
};

/*
template<class Archive>
void save(Archive& ar, TC_RPC_Message const& p_rpc_msg)
{
	RPC_LOG(TEXT("save"));
	ar << p_rpc_msg.MessageType;
	if (p_rpc_msg.Data.Get() == nullptr)
	{
		return;
	}
	FBufferArchive MessageWriter;
	p_rpc_msg.Data->Serialize(MessageWriter);
	ar << MessageWriter.TotalSize();
	for (uint32 i=0;i< MessageWriter.TotalSize();++i)
	{
		ar << MessageWriter[i];
	}
}

template<class Archive>
void load(Archive& ar,TC_RPC_Message& p_rpc_msg)
{
	RPC_LOG(TEXT("load"));
	ar >> p_rpc_msg.MessageType;
}*/
//}