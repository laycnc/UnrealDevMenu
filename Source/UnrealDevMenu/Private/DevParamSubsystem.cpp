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
#include "Misc/FileHelper.h"

DEFINE_LOG_CATEGORY(LogDevMenuParam);

#define LOCTEXT_NAMESPACE "UDevParamSubsystem"

namespace
{
	bool FDevParameterToJsonAttributes(
	    const FDevParameter&                              DevParameter,
	    TSharedRef<FJsonObject>                           OutJsonObject,
	    int64                                             CheckFlags = 0,
	    int64                                             SkipFlags  = 0,
	    const FJsonObjectConverter::CustomExportCallback* ExportCb   = nullptr)
	{
		// 構造体の箇所以外はデフォルトの処理を行う
		if ( !FJsonObjectConverter::UStructToJsonAttributes(
		         FDevParameter::StaticStruct(),
		         &DevParameter,
		         OutJsonObject->Values,
		         CheckFlags,
		         SkipFlags,
		         ExportCb) )
		{
			return false;
		}

		TSharedPtr<FJsonObject> StructValueMapJson = MakeShared<FJsonObject>();

		for ( const auto& StructValueMapPair : DevParameter.StructValueMap )
		{
			//
			UStruct*    StructKey   = StructValueMapPair.Key;
			const auto& StructValue = StructValueMapPair.Value;

			TSharedPtr<FJsonObject> StructMapValueJson = MakeShared<FJsonObject>();

			const FString StructValueMapKeyPath =
			    FObjectPropertyBase::GetExportPath(StructKey, nullptr, nullptr, 0);

			for ( const auto& StructValuePair : StructValue )
			{
				//
				FName       Key   = StructValuePair.Key;
				const auto& Value = StructValuePair.Value;

				TSharedPtr<FJsonObject> StructValueJson = MakeShared<FJsonObject>();

				FJsonObjectConverter::UStructToJsonObject(
				    Value->GetStruct(),
				    Value->GetStructMemory(),
				    StructValueJson.ToSharedRef(),
				    CheckFlags,
				    SkipFlags,
				    ExportCb);

				StructMapValueJson->SetField(
				    Key.ToString(), MakeShared<FJsonValueObject>(StructValueJson));
			}

			StructValueMapJson->SetField(
			    StructValueMapKeyPath,
			    MakeShared<FJsonValueObject>(StructMapValueJson));
		}

		OutJsonObject->Values.Add(FString(TEXT("StructValueMap")),
		                          MakeShared<FJsonValueObject>(StructValueMapJson));

		return true;
	}

	bool JsonToFDevParameterAttributes(TSharedRef<FJsonObject> JsonObject,
	                                   FDevParameter&          DevParameter,
	                                   int64                   CheckFlags = 0,
	                                   int64                   SkipFlags  = 0)
	{
		// 構造体の箇所以外はデフォルトの処理を行う
		if ( !FJsonObjectConverter::JsonObjectToUStruct(
		         JsonObject,
		         FDevParameter::StaticStruct(),
		         &DevParameter,
		         CheckFlags,
		         SkipFlags) )
		{
			return false;
		}

		TSharedPtr<FJsonObject> StructValueMapJson =
		    JsonObject->GetObjectField(TEXT("StructValueMap"));

		for ( auto& StructValueMapJsonPair : StructValueMapJson->Values )
		{
			const FString& StructValueMapKeyPath = StructValueMapJsonPair.Key;

			if ( StructValueMapJsonPair.Value->Type != EJson::Object )
			{
				// Obj型が来るはず…
				// クラッシュさせない為に念の為チェックを入れる
				continue;
			}

			const TSharedPtr<FJsonObject>& StructDevParamValuesJson =
			    StructValueMapJsonPair.Value->AsObject();

			// PropertyBaseObject.cpp
			// ↑でのロードを簡略化して実装
			TStringBuilder<256> Temp;
			const TCHAR*        Buffer = FPropertyHelpers::ReadToken(
                *StructValueMapKeyPath, /* out */ Temp, true);
			if ( Buffer == nullptr )
			{
				return false;
			}

			UObject*       Obj = StaticFindObjectSafe(nullptr, nullptr, Buffer);
			UScriptStruct* StructType = Cast<UScriptStruct>(Obj);

			if ( !IsValid(StructType) )
			{
				continue;
			}

			// 該当の構造体が見つかったので追加する
			TMap<FName, TSharedPtr<FStructOnScope>>& StructValue =
			    DevParameter.StructValueMap.FindOrAdd(StructType);

			for ( auto& StructDevParamValuesJsonPair :
			      StructDevParamValuesJson->Values )
			{
				const FString& ParamId = StructDevParamValuesJsonPair.Key;

				if ( StructDevParamValuesJsonPair.Value->Type != EJson::Object )
				{
					continue;
				}

				const TSharedPtr<FJsonObject>& StructDevParamJson =
				    StructDevParamValuesJsonPair.Value->AsObject();

				TSharedPtr<FStructOnScope> StructData =
				    MakeShareable(new FStructOnScope(StructType));

				// 構造体の箇所以外はデフォルトの処理を行う
				if ( FJsonObjectConverter::JsonObjectToUStruct(
				         StructDevParamJson.ToSharedRef(),
				         StructType,
				         StructData->GetStructMemory(),
				         CheckFlags,
				         SkipFlags) )
				{
					StructValue.Add(FName(*ParamId), StructData);
				}
			}
		}
		return true;
	}

} // namespace

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

void UDevParamSubsystem::AddReferencedObjects(UObject*             InThis,
                                              FReferenceCollector& Collector)
{
	UDevParamSubsystem* This = CastChecked<UDevParamSubsystem>(InThis);

	// GCにStructMemoryを認識させる必要が有る
	for ( auto& StructPair : This->Parameter.StructValueMap )
	{
		const UScriptStruct* StructType  = Cast<UScriptStruct>(StructPair.Key);
		const auto&          StructValue = StructPair.Value;

		for ( const auto& Pair : StructValue )
		{
			const FName&                      Tag   = Pair.Key;
			const TSharedPtr<FStructOnScope>& Value = Pair.Value;
			void* StructMemory                      = Value->GetStructMemory();

			Collector.AddReferencedObjects(StructType, StructMemory);
		}
	}

	Super::AddReferencedObjects(InThis, Collector);
}

void UDevParamSubsystem::InitializeAsset(UDevParamDataAsset* InAsset,
                                         FString             LoadFilePath)
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

	// ファイルの指定が無い
	if ( LoadFilePath.IsEmpty() )
	{
		return;
	}

	if ( FPaths::FileExists(LoadFilePath) )
	{
		// ファイルが存在している場合には読み込み処理を行う

		FDevParameter LoadDevParam;

		LoadParam(LoadFilePath, LoadDevParam);

		auto SetCopyValues = [](auto& DestMap, const auto& CopyMap)
		{
			for ( auto& Pair : CopyMap )
			{
				DestMap.FindOrAdd(Pair.Key) = Pair.Value;
			}
		};
		SetCopyValues(Parameter.BoolValues, LoadDevParam.BoolValues);
		SetCopyValues(Parameter.DoubleValues, LoadDevParam.DoubleValues);
		SetCopyValues(Parameter.FloatValues, LoadDevParam.FloatValues);
		SetCopyValues(Parameter.Int32Values, LoadDevParam.Int32Values);
		SetCopyValues(Parameter.NameValues, LoadDevParam.NameValues);
		SetCopyValues(Parameter.ObjectValues, LoadDevParam.ObjectValues);
		SetCopyValues(Parameter.StringValues, LoadDevParam.StringValues);

		for ( auto& StructPair : LoadDevParam.StructValueMap )
		{
			UScriptStruct*                                 Struct = StructPair.Key;
			const TMap<FName, TSharedPtr<FStructOnScope>>& CopyValues =
			    StructPair.Value;
			TMap<FName, TSharedPtr<FStructOnScope>>& DestValues =
			    Parameter.StructValueMap.FindOrAdd(Struct);

			for ( const auto& CopyValuePair : CopyValues )
			{
				FName                             ParamId   = CopyValuePair.Key;
				const TSharedPtr<FStructOnScope>& CopyValue = CopyValuePair.Value;

				if ( const TSharedPtr<FStructOnScope>* DestStructPtr =
				         DestValues.Find(ParamId) )
				{
					// 既に構築されている場合
					Struct->CopyScriptStruct((*DestStructPtr)->GetStructMemory(),
					                         CopyValue->GetStructMemory());
				}
				else
				{
					// 新規で構築する場合
					TSharedPtr<FStructOnScope> DestStruct =
					    MakeShareable(new FStructOnScope(Struct));
					DestValues.Emplace(ParamId, DestStruct);
					Struct->CopyScriptStruct(DestStruct->GetStructMemory(),
					                         CopyValue->GetStructMemory());
				}
			}
		}
	}
}

// 指定されたパラメータが存在するか？
bool UDevParamSubsystem::ExistsParam(FGameplayTag ParamId) const
{
	return ExistsParam(ParamId.GetTagName());
}

// 指定されたパラメータが存在するか？
bool UDevParamSubsystem::ExistsParam(FName ParamId) const
{
	const auto ExistsParam = [ParamId](auto& TargetValueMap) -> bool
	{
		return TargetValueMap.Find(ParamId) != nullptr;
	};

	if ( ExistsParam(Parameter.BoolValues) )
	{
		return true;
	}
	if ( ExistsParam(Parameter.DoubleValues) )
	{
		return true;
	}
	if ( ExistsParam(Parameter.FloatValues) )
	{
		return true;
	}
	if ( ExistsParam(Parameter.Int32Values) )
	{
		return true;
	}
	if ( ExistsParam(Parameter.NameValues) )
	{
		return true;
	}
	if ( ExistsParam(Parameter.ObjectValues) )
	{
		return true;
	}
	if ( ExistsParam(Parameter.StringValues) )
	{
		return true;
	}

	for ( const auto& Pair : Parameter.StructValueMap )
	{
		if ( ExistsParam(Pair.Value) )
		{
			return true;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
// SetValue

// bool型のパラメータを設定する
void UDevParamSubsystem::SetValueByBool(FGameplayTag ParamId, bool NewValue)
{
	SetPrimitiveValue(ParamId, Parameter.BoolValues, NewValue);
}

// 整数型のパラメータを設定する
void UDevParamSubsystem::SetValueByInt32(FGameplayTag ParamId, int32 NewValue)
{
	SetPrimitiveValue(ParamId, Parameter.Int32Values, NewValue);
}

// 不動点少数型のパラメータを設定する
void UDevParamSubsystem::SetValueByFloat(FGameplayTag ParamId, float NewValue)
{
	SetPrimitiveValue(ParamId, Parameter.FloatValues, NewValue);
}

// 倍精度不動点少数型のパラメータを設定する
void UDevParamSubsystem::SetValueByDouble(FGameplayTag ParamId, double NewValue)
{
	SetPrimitiveValue(ParamId, Parameter.DoubleValues, NewValue);
}

// Name型のパラメータを設定する
void UDevParamSubsystem::SetValueByName(FGameplayTag ParamId, FName NewValue)
{
	SetPrimitiveValue(ParamId, Parameter.NameValues, NewValue);
}

// String型のパラメータを設定する
void UDevParamSubsystem::SetValueByString(FGameplayTag   ParamId,
                                          const FString& NewValue)
{
	SetPrimitiveValue<FString>(ParamId, Parameter.StringValues, NewValue);
}

// Object型のパラメータを設定する
void UDevParamSubsystem::SetValueByObject(FGameplayTag ParamId, UObject* NewValue)
{
	TWeakObjectPtr<UObject> NewObj = NewValue;
	SetPrimitiveValue<TWeakObjectPtr<UObject>>(
	    ParamId, Parameter.ObjectValues, NewObj);
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
	GetPrimitiveValue(ParamId, Parameter.BoolValues, ResultValue);
}

// 整数型のパラメータを取得する
void UDevParamSubsystem::GetValueByInt32(FGameplayTag ParamId,
                                         int32&       ResultValue) const
{
	GetPrimitiveValue(ParamId, Parameter.Int32Values, ResultValue);
}

// 不動点少数型のパラメータを取得する
void UDevParamSubsystem::GetValueByFloat(FGameplayTag ParamId,
                                         float&       ResultValue) const
{
	GetPrimitiveValue(ParamId, Parameter.FloatValues, ResultValue);
}

// 倍精度不動点少数型のパラメータを取得する
void UDevParamSubsystem::GetValueByDouble(FGameplayTag ParamId,
                                          double&      ResultValue) const
{
	GetPrimitiveValue(ParamId, Parameter.DoubleValues, ResultValue);
}

// Name型のパラメータを取得する
void UDevParamSubsystem::GetValueByName(FGameplayTag ParamId,
                                        FName&       ResultValue) const
{
	GetPrimitiveValue(ParamId, Parameter.NameValues, ResultValue);
}

// String型のパラメータを取得する
void UDevParamSubsystem::GetValueByString(FGameplayTag ParamId,
                                          FString&     ResultValue) const
{
	GetPrimitiveValue(ParamId, Parameter.StringValues, ResultValue);
}

// Object型のパラメータを取得する
void UDevParamSubsystem::GetValueByObject(FGameplayTag ParamId,
                                          UObject*&    ResultValue) const
{
	TWeakObjectPtr<UObject> ResultObj;
	GetPrimitiveValue(ParamId, Parameter.ObjectValues, ResultObj);
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
	    Parameter.StructValueMap.Find(ParamType);

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
	    Parameter.StructValueMap.Find(ParamType);

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
	    Parameter.StructValueMap.FindOrAdd(ParamType);

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
		return FString::SanitizeFloat(Value);
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
	PrintPrimitiveValue(Parameter.BoolValues);
	PrintPrimitiveValue(Parameter.Int32Values);
	PrintPrimitiveValue(Parameter.FloatValues);
	PrintPrimitiveValue(Parameter.DoubleValues);
	PrintPrimitiveValue(Parameter.NameValues);
	PrintPrimitiveValue(Parameter.StringValues);
	PrintPrimitiveValue(Parameter.ObjectValues);

	for ( auto& StructPair : Parameter.StructValueMap )
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

// デバッグパラメータを保存する
bool UDevParamSubsystem::SaveParam(FString SaveFilePath) const
{
	TSharedPtr<FJsonObject> JsonRootObject = MakeShareable(new FJsonObject);

	FDevParameterToJsonAttributes(Parameter, JsonRootObject.ToSharedRef());

	// FStringにJsonを書き込むためのWriterを作成
	FString                   OutPutString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutPutString);

	// JsonをFStringに書き込み
	FJsonSerializer::Serialize(JsonRootObject.ToSharedRef(), Writer);

	return FFileHelper::SaveStringToFile(OutPutString, *SaveFilePath);
}

// デバッグパラメータを読み込みする
bool UDevParamSubsystem::LoadParam(const FString& LoadFilePath,
                                   FDevParameter& OutDevParam)
{
	FString OutPutString;
	if ( !FFileHelper::LoadFileToString(OutPutString, *LoadFilePath) )
	{
		return false;
	}

	// FJsonObject(Jsonデータの入れ物)を作成
	TSharedPtr<FJsonObject> JsonRootObject = MakeShareable(new FJsonObject());

	// FStringからJsonを読み込むためのReaderを作成
	TSharedRef<TJsonReader<>> JsonReader =
	    TJsonReaderFactory<>::Create(OutPutString);

	// Json文字列からJsonオブジェクトに読み込み
	if ( !FJsonSerializer::Deserialize(JsonReader, JsonRootObject) )
	{
		return false;
	}

	if ( !JsonToFDevParameterAttributes(JsonRootObject.ToSharedRef(), OutDevParam) )
	{
		return false;
	}

	return true;
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
		bool& NewValue = Parameter.BoolValues.Emplace(ParamId);
		ParamType_Bool->InitializeValue(&NewValue);
	}
	else if ( auto* ParamType_Int32 = Cast<UDevParamType_Int32>(ParamType) )
	{
		int32& NewValue = Parameter.Int32Values.Emplace(ParamId);
		ParamType_Int32->InitializeValue(&NewValue);
	}
	else if ( auto* ParamType_Float = Cast<UDevParamType_Float>(ParamType) )
	{
		float& NewValue = Parameter.FloatValues.Emplace(ParamId);
		ParamType_Float->InitializeValue(&NewValue);
	}
	else if ( auto* ParamType_Double = Cast<UDevParamType_Double>(ParamType) )
	{
		double& NewValue = Parameter.DoubleValues.Emplace(ParamId);
		ParamType_Double->InitializeValue(&NewValue);
	}
	else if ( auto* ParamType_Name = Cast<UDevParamType_Name>(ParamType) )
	{
		FName& NewValue = Parameter.NameValues.Emplace(ParamId);
		ParamType_Name->InitializeValue(&NewValue);
	}
	else if ( auto* ParamType_String = Cast<UDevParamType_String>(ParamType) )
	{
		FString& NewValue = Parameter.StringValues.Emplace(ParamId);
		ParamType_String->InitializeValue(&NewValue);
	}
	else if ( auto* ParamType_Object = Cast<UDevParamType_Object>(ParamType) )
	{
		TWeakObjectPtr<UObject>& NewValue = Parameter.ObjectValues.Emplace(ParamId);
		ParamType_Object->InitializeValue(&NewValue);
	}
	else if ( auto* ParamType_Struct = Cast<UDevParamStructType>(ParamType) )
	{
		// 構造体の場合
		UScriptStruct* StructType = ParamType_Struct->GetTargetStructType();

		TMap<FName, TSharedPtr<FStructOnScope>>& StructValue =
		    Parameter.StructValueMap.FindOrAdd(StructType);

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
	Parameter.BoolValues.Empty();
	Parameter.Int32Values.Empty();
	Parameter.FloatValues.Empty();
	Parameter.DoubleValues.Empty();
	Parameter.NameValues.Empty();
	Parameter.StringValues.Empty();
	Parameter.ObjectValues.Empty();
	Parameter.StructValueMap.Empty();
}

#undef LOCTEXT_NAMESPACE