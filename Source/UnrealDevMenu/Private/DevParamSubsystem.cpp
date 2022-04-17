// Fill out your copyright notice in the Description page of Project Settings.

#include "DevParamSubsystem.h"
#include "ParamType/DevParamType_Bool.h"
#include "ParamType/DevParamType_Int32.h"
#include "ParamType/DevParamType_Float.h"
#include "ParamType/DevParamType_Double.h"
#include "ParamType/DevParamType_Name.h"
#include "ParamType/DevParamType_String.h"
#include "ParamType/DevParamType_Object.h"
#include "ParamType/DevParamStructType.h"
#include "ParamType/DevParamDataAsset.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"
#include "JsonObjectConverter.h"
#include "DevMenuUtility.h"

DEFINE_LOG_CATEGORY(LogDevMenuParam);

#define LOCTEXT_NAMESPACE "UDevParamSubsystem"

// デバッグパラメータサブシステムの取得
UDevParamSubsystem* UDevParamSubsystem::Get(const UObject* InWorldContext)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(
	    InWorldContext, EGetWorldErrorMode::ReturnNull);
	if ( IsValid(World) )
	{
		auto* Subsystem = World->GetSubsystemBase(UDevParamSubsystem::StaticClass());
		return Cast<UDevParamSubsystem>(Subsystem);
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

	// パラメータの終了処理を呼ぶ
	FinalizeDevParam();
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
void UDevParamSubsystem::SetPrimitiveValue(const FGameplayTag& ParamId,
                                           TMap<FName, T>&     TargetValues,
                                           T                   NewValue)
{
	if ( T* Result = TargetValues.Find(ParamId.GetTagName()) )
	{
		(*Result) = NewValue;
		OnChangedParam.Broadcast(ParamId);
		return;
	}

	// TargetValueが未設定なのでエラーを出して終了
	FDevMenuUtility::MessageLog(
	    DataAsset,
	    FText::Format(LOCTEXT("SetPrimitiveValue_NotFoundStruct",
	                          "NotFound DevParam: Tag={0}"),
	                  FText::FromName(ParamId.GetTagName())));
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

void UDevParamSubsystem::GetValueByStructInternal(
    FGameplayTag              ParamId,
    FDevParamStructDummyType& ResultValue) const
{
	check(0);
}

void UDevParamSubsystem::GetValueByStruct_Impl(FName          ParamId,
                                               UScriptStruct* ParamType,
                                               void*          ResultValue) const
{
	const TMap<FName, TSharedPtr<FStructOnScope>>* StructValues =
	    StructValueMap.Find(ParamType);

	if ( StructValues == nullptr )
	{
		// 構造体が見つからなかった
		FDevMenuUtility::MessageLog(
		    DataAsset,
		    FText::Format(LOCTEXT("GetValueByStruct_NotFoundStruct",
		                          "NotFound ParamStruct: Struct={0} Tag={1}"),
		                  FText::FromString(GetNameSafe(ParamType)),
		                  FText::FromName(ParamId)));

		return;
	}

	auto* Result = StructValues->Find(ParamId);

	if ( Result == nullptr )
	{
		// GameplayTagが見つからなかった
		FDevMenuUtility::MessageLog(
		    DataAsset,
		    FText::Format(LOCTEXT("GetValueByStruct_NotFoundParamId",
		                          "NotFound ParamId: Struct={0} Tag={1}"),
		                  FText::FromString(GetNameSafe(ParamType)),
		                  FText::FromName(ParamId)));

		return;
	}

	// 構造体をコピーをする
	const uint8* StructMemory = (*Result)->GetStructMemory();
	ParamType->CopyScriptStruct(ResultValue, StructMemory);
}

DEFINE_FUNCTION(UDevParamSubsystem::execGetValueByStructInternal)
{
	P_GET_STRUCT(FGameplayTag, Z_Param_ParamId);

	Stack.StepCompiledIn<FStructProperty>(NULL);
	void*            OutRowPtr = Stack.MostRecentPropertyAddress;
	FStructProperty* StructProp =
	    CastField<FStructProperty>(Stack.MostRecentProperty);

	P_FINISH;
	P_NATIVE_BEGIN;

	UScriptStruct* ParamType = nullptr;
	if ( StructProp )
	{
		ParamType = StructProp->Struct;
	}

	P_THIS->GetValueByStruct_Impl(
	    Z_Param_ParamId.GetTagName(), ParamType, OutRowPtr);
	P_NATIVE_END;
}

void UDevParamSubsystem::SetValueByStructInternal(
    FGameplayTag                    ParamId,
    const FDevParamStructDummyType& NewValue)
{
	check(0);
}

void UDevParamSubsystem::SetValueByStruct_Impl(FName          ParamId,
                                               UScriptStruct* ParamType,
                                               const void*    NewValue) const
{
	const TMap<FName, TSharedPtr<FStructOnScope>>* StructValues =
	    StructValueMap.Find(ParamType);

	if ( StructValues == nullptr )
	{
		// 構造体が見つからなかった
		FDevMenuUtility::MessageLog(
		    DataAsset,
		    FText::Format(LOCTEXT("SetValueByStruct_NotFoundStruct",
		                          "NotFound ParamStruct: Struct={0} Tag={1}"),
		                  FText::FromString(GetNameSafe(ParamType)),
		                  FText::FromName(ParamId)));

		return;
	}

	auto* Result = StructValues->Find(ParamId);

	if ( Result == nullptr )
	{
		// GameplayTagが見つからなかった
		FDevMenuUtility::MessageLog(
		    DataAsset,
		    FText::Format(LOCTEXT("SetValueByStruct_NotFoundParamId",
		                          "NotFound ParamId: Struct={0} Tag={1}"),
		                  FText::FromString(GetNameSafe(ParamType)),
		                  FText::FromName(ParamId)));

		return;
	}

	// 構造体をコピーをする
	uint8* StructMemory = (*Result)->GetStructMemory();
	ParamType->CopyScriptStruct(StructMemory, NewValue);
}

DEFINE_FUNCTION(UDevParamSubsystem::execSetValueByStructInternal)
{
	P_GET_STRUCT(FGameplayTag, Z_Param_ParamId);

	Stack.StepCompiledIn<FStructProperty>(NULL);
	void*            OutRowPtr = Stack.MostRecentPropertyAddress;
	FStructProperty* StructProp =
	    CastField<FStructProperty>(Stack.MostRecentProperty);

	P_FINISH;
	P_NATIVE_BEGIN;

	UScriptStruct* ParamType = nullptr;
	if ( StructProp )
	{
		ParamType = StructProp->Struct;
	}

	P_THIS->SetValueByStruct_Impl(
	    Z_Param_ParamId.GetTagName(), ParamType, OutRowPtr);
	P_NATIVE_END;
}

template<class T>
void UDevParamSubsystem::GetPrimitiveValue(const FGameplayTag&   ParamId,
                                           const TMap<FName, T>& TargetValues,
                                           T&                    ResultValue) const
{
	if ( const T* Result = TargetValues.Find(ParamId.GetTagName()) )
	{
		ResultValue = *Result;
		return;
	}

	// TargetValueが未設定なのでエラーを出して終了
	FDevMenuUtility::MessageLog(
	    DataAsset,
	    FText::Format(LOCTEXT("GetPrimitiveValue_NotFoundStruct",
	                          "NotFound DevParam: Tag={0}"),
	                  FText::FromName(ParamId.GetTagName())));
}

void UDevParamSubsystem::RegisterStructParam(FName          ParamId,
                                             UScriptStruct* ParamType,
                                             const void*    NewValue)
{
	TMap<FName, TSharedPtr<FStructOnScope>>& StructValue =
	    StructValueMap.FindOrAdd(ParamType);

	TSharedPtr<FStructOnScope> NewStruct =
	    MakeShareable(new FStructOnScope(ParamType));

	// コピーする
	ParamType->CopyScriptStruct(NewStruct->GetStructMemory(), NewValue);

	// 成功したのでリストに追加する
	StructValue.Emplace(ParamId, NewStruct);
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

	for ( auto& StructPair : StructValueMap )
	{
		const TObjectPtr<UStruct>& StructType  = StructPair.Key;
		const auto&                StructValue = StructPair.Value;

		for ( const auto& Pair : StructValue )
		{
			const FName&                      Tag   = Pair.Key;
			const TSharedPtr<FStructOnScope>& Value = Pair.Value;
			const uint8* StructMemory               = Value->GetStructMemory();

			FString JsonString;
			if ( FJsonObjectConverter::UStructToJsonObjectString(
			         StructType, StructMemory, JsonString) )
			{
				JsonString = JsonString.Replace(TEXT("\r\n"), TEXT(""));
				UKismetSystemLibrary::PrintString(
				    this,
				    FString::Printf(
				        TEXT("Tag = %s, Value = %s"), *Tag.ToString(), *JsonString));
			}
			// todo
		}
	}
}

template<class T>
void UDevParamSubsystem::PrintPrimitiveValue(
    const TMap<FName, T>& TargetValues) const
{
	for ( auto& Pair : TargetValues )
	{
		const FName& Tag   = Pair.Key;
		const T&     Value = Pair.Value;

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
	FName ParamId = ParamType->ParamId.GetTagName();

	// 構造体の生成の場合
	if ( auto* ParamType_Bool = Cast<UDevParamType_Bool>(ParamType) )
	{
		bool& NewValue = BoolValues.Emplace(ParamId);
		ParamType_Bool->InitializeValue(&NewValue);
	}
	else if ( auto* ParamType_Int32 = Cast<UDevParamType_Int32>(ParamType) )
	{
		int32& NewValue = Int32Values.Emplace(ParamId);
		ParamType_Int32->InitializeValue(&NewValue);
	}
	else if ( auto* ParamType_Float = Cast<UDevParamType_Float>(ParamType) )
	{
		float& NewValue = FloatValues.Emplace(ParamId);
		ParamType_Float->InitializeValue(&NewValue);
	}
	else if ( auto* ParamType_Double = Cast<UDevParamType_Double>(ParamType) )
	{
		double& NewValue = DoubleValues.Emplace(ParamId);
		ParamType_Double->InitializeValue(&NewValue);
	}
	else if ( auto* ParamType_Name = Cast<UDevParamType_Name>(ParamType) )
	{
		FName& NewValue = NameValues.Emplace(ParamId);
		ParamType_Name->InitializeValue(&NewValue);
	}
	else if ( auto* ParamType_String = Cast<UDevParamType_String>(ParamType) )
	{
		FString& NewValue = StringValues.Emplace(ParamId);
		ParamType_String->InitializeValue(&NewValue);
	}
	else if ( auto* ParamType_Object = Cast<UDevParamType_Object>(ParamType) )
	{
		TWeakObjectPtr<UObject>& NewValue = ObjectValues.Emplace(ParamId);
		ParamType_Object->InitializeValue(&NewValue);
	}
	else if ( auto* ParamType_Struct = Cast<UDevParamStructType>(ParamType) )
	{
		// 構造体の場合
		UStruct* StructType = ParamType_Struct->GetTargetStructType();

		TMap<FName, TSharedPtr<FStructOnScope>>& StructValue =
		    StructValueMap.FindOrAdd(StructType);

		FStructOnScope NewStructValue(StructType);

		if ( ParamType_Struct->InitializeValue(NewStructValue.GetStructMemory()) )
		{
			// 成功したのでリストに追加する
			StructValue.Emplace(
			    ParamId,
			    MakeShareable(new FStructOnScope(std::move(NewStructValue))));
		}
	}
}

// パラメータの終了処理
void UDevParamSubsystem::FinalizeDevParam()
{

	BoolValues.Empty();
	Int32Values.Empty();
	FloatValues.Empty();
	DoubleValues.Empty();
	NameValues.Empty();
	StringValues.Empty();
	ObjectValues.Empty();
	StructValueMap.Empty();
}

#undef LOCTEXT_NAMESPACE