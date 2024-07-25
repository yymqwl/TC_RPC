#pragma once


#include "RpcHeader.h"
#include "RPC_Log.h"
#include "RPC_ErrorCode.h"
#include "CoreMinimal.h"

namespace TC_RPC
{
	struct TC_RPC_Message
	{
	public:
		uint16 MessageType;//
		uint8 ErrorCode;//
		TArray<uint8> Data;

		TC_RPC_Message(int16 messageType = 0,
			uint8 errorCode = StaticCast<uint8>(ERPC_ErrorCode::None))
				:MessageType(0),ErrorCode(errorCode)
		{
			//RPC_LOG(TEXT("TC_RPC_Message()"));
		}
		/*
		~TC_RPC_Message()
		{
			RPC_LOG(TEXT("~TC_RPC_Message"));
		}*/
		
		template<class Archive>
		void save(Archive& ar) const
		{
			ar << ErrorCode;
			ar << MessageType;
			TArray<uint8>::SizeType len = Data.Num();
			ar << len;
			if (len >0)
			{
				ar << cereal::binary_data(const_cast<uint8*>(Data.GetData()),len) ;
			}
		}

		template<class Archive>
		void load(Archive& ar)
		{
			ar >> ErrorCode;
			ar >> MessageType;
			TArray<uint8>::SizeType len = 0;
			ar >> len;
			if (len < 0)
			{
				RPC_ERROR(TEXT("TC_RPC_Message load len %d"),len);
			}

			Data.Empty();
			//TArray<uint8> BinaryData;
			Data.SetNum(len);
			//BinaryData.SetNum(len);
			if (len > 0)
			{
				FMemoryReader MessageReader(Data);
				ar >> cereal::binary_data(const_cast<uint8*>(Data.GetData()),len);
			}

		}
		/*
		template <class Archive>
		void serialize(Archive& ar)
		{
			//RPC_LOG(TEXT("TC_RPC_Message serialize"));//这个一般不调用
			ar(ErrorCode,MessageType);
			TArray<uint8>::SizeType len = Data.Num();
			ar(len);
			if (len >0)
			{
				ar(cereal::binary_data(const_cast<uint8*>(Data.GetData()),len));
			}
		}*/
	};

	
	template<class Archive>
	void save( Archive& ar,const std::shared_ptr<TC_RPC_Message>& ptr)
	{
		//RPC_LOG(TEXT("TC_RPC_Message ptr save"));
		if (ptr == nullptr)
		{
			RPC_ERROR(TEXT("TC_RPC_Message save Null"));
			return ;
		}
		ptr->save(ar);
	}
	template<class Archive>
	void load(Archive& ar,std::shared_ptr<TC_RPC_Message>& ptr)
	{
		//RPC_LOG(TEXT("TC_RPC_Message ptr load"));
		ptr = std::make_shared<TC_RPC_Message>();
		ptr->load(ar);
	}
	
	#include <string>
	#include <memory>
	struct Person
	{
	public:
		
		Person()
		{
			RPC_LOG(TEXT("Person()"));
		}
		~Person() 
		{
			RPC_LOG(TEXT("~Person()"));
		}
		Person(const Person& other)
		{
			RPC_LOG(TEXT("Person 拷贝"));
		}
		Person& operator=(const Person& other) {
			RPC_LOG(TEXT("Person ="));
			return *this;
		}
		
		int age;
		std::string name;
		template<class Archive>
		void save(Archive& ar) const
		{
			ar << age;
			ar << name;
		}
	
		template<class Archive>
		void load(Archive& ar)
		{
			ar >> age;
			ar >> name;
		}
	};
	template<class Archive>
	void save(Archive& ar, std::shared_ptr<Person>const& j)
	{
		ar << j->age;
		ar << j->name;
	}
	
	template<class Archive>
	void load(Archive& ar,std::shared_ptr<Person>& j)
	{
		j = std::make_shared<Person>();
		ar >> j->age;
		ar >> j->name;
	}
}