// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"
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
		return  0;/*StaticCast<uint16>(ERPC_MessageType::None);*/
	}
	
};
/*
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
*/


//namespace TC_RPC
//{

//顺序 ErrorCode
//MessageType
//
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