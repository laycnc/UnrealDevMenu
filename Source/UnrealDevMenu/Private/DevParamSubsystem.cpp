// Fill out your copyright notice in the Description page of Project Settings.

#include "DevParamSubsystem.h"
#include "ParamType/DevParamType_Bool.h"
#include "ParamType/DevParamType_Int32.h"
#include "ParamType/DevParamType_Float.h"
#include "ParamType/DevParamType_Double.h"
#include "ParamType/DevParamType_Name.h"
#include "ParamType/DevParamType_String.h"
#include "ParamType/DevParamType_Object.h"
#include "ParamType/DevParamDataAsset.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"

DEFINE_LOG_CATEGORY(LogDevMenuParam);

// デバッグパラメータサブシステムの取得
UDevParamSubsystem* UDevParamSubsystem::Get(const UObject* InWorldContext)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(
	    InWorldContext, EGetWorldErrorMode::ReturnNull);
	if ( IsValid(World) )
	{
		if ( const UGameInstance* GameInstance = World->GetGameInstance() )
		{
			auto* Subsystem =
			    GameInstance->GetSubsystemBase(UDevParamSubsystem::StaticClass());
			return Cast<UDevParamSubsystem>(Subsystem);
		}
	}
	return nullptr;
}

// USubsystem implementation Begin
void UDevParamSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UDevParamSubsystem::Deinitialize()
{
	Super::Deinitialize();
}
// USubsystem implementation End

void UDevParamSubsystem::InitializeAsset(UDevParamDataAsset* InAsset)
{
	if ( InAsset == nullptr )
	{
		return;
	}
	DataAsset = InAsset;

	for ( auto& Param : DataAsset->Params )
	{
		AddValues(Param);
	}
}

//////////////////////////////////////////////////////////////////////////
// SetValue

// bool型のパラメータを設定する
void UDevParamSubsystem::SetValueByBool(FGameplayTag ParamId, bool NewValue)
{
	SetPrimitiveValue(ParamId, BoolValues, NewValue);
}

// 整数型のパラメータを設定する
void UDevParamSubsystem::SetValueByInt32(FGameplayTag ParamId, int32 NewValue)
{
	SetPrimitiveValue(ParamId, Int32Values, NewValue);
}

// 不動点少数型のパラメータを設定する
void UDevParamSubsystem::SetValueByFloat(FGameplayTag ParamId, float NewValue)
{
	SetPrimitiveValue(ParamId, FloatValues, NewValue);
}

// 倍精度不動点少数型のパラメータを設定する
void UDevParamSubsystem::SetValueByDouble(FGameplayTag ParamId, double NewValue)
{
	SetPrimitiveValue(ParamId, DoubleValues, NewValue);
}

// Name型のパラメータを設定する
void UDevParamSubsystem::SetValueByName(FGameplayTag ParamId, FName NewValue)
{
	SetPrimitiveValue(ParamId, NameValues, NewValue);
}

// String型のパラメータを設定する
void UDevParamSubsystem::SetValueByString(FGameplayTag   ParamId,
                                          const FString& NewValue)
{
	SetPrimitiveValue<FString>(ParamId, StringValues, NewValue);
}

// Object型のパラメータを設定する
void UDevParamSubsystem::SetValueByObject(FGameplayTag ParamId, UObject* NewValue)
{
	TWeakObjectPtr<UObject> NewObj = NewValue;
	SetPrimitiveValue<TWeakObjectPtr<UObject>>(ParamId, ObjectValues, NewObj);
}

template<class T>
void UDevParamSubsystem::SetPrimitiveValue(const FGameplayTag&    ParamId,
                                           TMap<FGameplayTag, T>& TargetValues,
                                           T                      NewValue)
{
	if ( T* Result = TargetValues.Find(ParamId) )
	{
		(*Result) = NewValue;
		OnChangedParam.Broadcast(ParamId);
	}
}

//////////////////////////////////////////////////////////////////////////
// SetValue

// bool型のパラメータを取得する
void UDevParamSubsystem::GetValueByBool(FGameplayTag ParamId,
                                        bool&        ResultValue) const
{
	GetPrimitiveValue(ParamId, BoolValues, ResultValue);
}

// 整数型のパラメータを取得する
void UDevParamSubsystem::GetValueByInt32(FGameplayTag ParamId,
                                         int32&       ResultValue) const
{
	GetPrimitiveValue(ParamId, Int32Values, ResultValue);
}

// 不動点少数型のパラメータを取得する
void UDevParamSubsystem::GetValueByFloat(FGameplayTag ParamId,
                                         float&       ResultValue) const
{
	GetPrimitiveValue(ParamId, FloatValues, ResultValue);
}

// 倍精度不動点少数型のパラメータを取得する
void UDevParamSubsystem::GetValueByDouble(FGameplayTag ParamId,
                                          double&      ResultValue) const
{
	GetPrimitiveValue(ParamId, DoubleValues, ResultValue);
}

// Name型のパラメータを取得する
void UDevParamSubsystem::GetValueByName(FGameplayTag ParamId,
                                        FName&       ResultValue) const
{
	GetPrimitiveValue(ParamId, NameValues, ResultValue);
}

// String型のパラメータを取得する
void UDevParamSubsystem::GetValueByString(FGameplayTag ParamId,
                                          FString&     ResultValue) const
{
	GetPrimitiveValue(ParamId, StringValues, ResultValue);
}

// Object型のパラメータを取得する
void UDevParamSubsystem::GetValueByObject(FGameplayTag ParamId,
                                          UObject*&    ResultValue) const
{
	TWeakObjectPtr<UObject> ResultObj;
	GetPrimitiveValue(ParamId, ObjectValues, ResultObj);
	ResultValue = ResultObj.Get();
}

template<class T>
void UDevParamSubsystem::GetPrimitiveValue(const FGameplayTag&          ParamId,
                                           const TMap<FGameplayTag, T>& TargetValues,
                                           T& ResultValue) const
{
	if ( const T* Result = TargetValues.Find(ParamId) )
	{
		ResultValue = *Result;
	}
}

//////////////////////////////////////////////////////////////////////////
//

namespace
{
	// UE側で関数のオーバーロードが無かったのでしょうがなく記述

	FString Conv_DevMenuToString(bool Value)
	{
		return UKismetStringLibrary::Conv_BoolToString(Value);
	}
	FString Conv_DevMenuToString(int32 Value)
	{
		return UKismetStringLibrary::Conv_IntToString(Value);
	}
	FString Conv_DevMenuToString(float Value)
	{
		return UKismetStringLibrary::Conv_FloatToString(Value);
	}

	FString Conv_DevMenuToString(double Value)
	{
		return UKismetStringLibrary::Conv_DoubleToString(Value);
	}

	FString Conv_DevMenuToString(FName Value)
	{
		return Value.ToString();
	}

	const FString& Conv_DevMenuToString(const FString& Value)
	{
		return Value;
	}

	FString Conv_DevMenuToString(const TWeakObjectPtr<UObject>& Value)
	{
		if ( Value.IsValid() )
		{
			return Value->GetName();
		}
		return TEXT("[None]");
	}

} // namespace

void UDevParamSubsystem::PrintValue() const
{
	PrintPrimitiveValue(BoolValues);
	PrintPrimitiveValue(Int32Values);
	PrintPrimitiveValue(FloatValues);
	PrintPrimitiveValue(DoubleValues);
	PrintPrimitiveValue(NameValues);
	PrintPrimitiveValue(StringValues);
	PrintPrimitiveValue(ObjectValues);

	for ( auto& StructPair : StructValues )
	{
		const TObjectPtr<UStruct>&                Type        = StructPair.Key;
		const TMap<FGameplayTag, FStructOnScope>& StructLists = StructPair.Value;

		for ( const auto& Pair : StructLists )
		{
			const FGameplayTag&   Tag   = Pair.Key;
			const FStructOnScope& Value = Pair.Value;

			// todo
		}
	}
}

template<class T>
void UDevParamSubsystem::PrintPrimitiveValue(
    const TMap<FGameplayTag, T>& TargetValues) const
{
	for ( auto& Pair : TargetValues )
	{
		const FGameplayTag& Tag   = Pair.Key;
		const T&            Value = Pair.Value;

		UKismetSystemLibrary::PrintString(
		    this,
		    FString::Printf(TEXT("Tag = %s, Value = %s"),
		                    *Tag.ToString(),
		                    *Conv_DevMenuToString(Value)),
		    false);
	}
}

//////////////////////////////////////////////////////////////////////////
//

// 値の追加
void UDevParamSubsystem::AddValues(UDevParamType* ParamType)
{
	// 構造体の生成の場合
	if ( auto* ParamType_Bool = Cast<UDevParamType_Bool>(ParamType) )
	{
		bool& NewValue = BoolValues.Emplace(ParamType_Bool->ParamId);
		ParamType_Bool->InitializeValue(&NewValue);
	}
	else if ( auto* ParamType_Int32 = Cast<UDevParamType_Int32>(ParamType) )
	{
		int32& NewValue = Int32Values.Emplace(ParamType_Int32->ParamId);
		ParamType_Int32->InitializeValue(&NewValue);
	}
	else if ( auto* ParamType_Float = Cast<UDevParamType_Float>(ParamType) )
	{
		float& NewValue = FloatValues.Emplace(ParamType_Float->ParamId);
		ParamType_Float->InitializeValue(&NewValue);
	}
	else if ( auto* ParamType_Double = Cast<UDevParamType_Double>(ParamType) )
	{
		double& NewValue = DoubleValues.Emplace(ParamType_Double->ParamId);
		ParamType_Double->InitializeValue(&NewValue);
	}
	else if ( auto* ParamType_Name = Cast<UDevParamType_Name>(ParamType) )
	{
		FName& NewValue = NameValues.Emplace(ParamType_Name->ParamId);
		ParamType_Name->InitializeValue(&NewValue);
	}
	else if ( auto* ParamType_String = Cast<UDevParamType_String>(ParamType) )
	{
		FString& NewValue = StringValues.Emplace(ParamType_String->ParamId);
		ParamType_String->InitializeValue(&NewValue);
	}
	else if ( auto* ParamType_Object = Cast<UDevParamType_Object>(ParamType) )
	{
		TWeakObjectPtr<UObject>& NewValue =
		    ObjectValues.Emplace(ParamType_Object->ParamId);
		ParamType_Object->InitializeValue(&NewValue);
	}
}
