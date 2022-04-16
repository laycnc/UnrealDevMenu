// Fill out your copyright notice in the Description page of Project Settings.

#include "ParamType/DevParamStructType.h"
#include "JsonObjectConverter.h"
#include "DevMenuUtility.h"
#include "ParamType/DevParamDataAsset.h"

#define LOCTEXT_NAMESPACE "UDevParamStructType"

UDevParamStructType::UDevParamStructType(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

bool UDevParamStructType::InitializeValue(void* Dest) const
{
	const uint8* StructMemory = nullptr;
	if ( GetDefaultValuePtr(StructMemory) )
	{
		// 構造体の初期化を行う
		StructType->InitializeStruct(Dest);

		// デフォルト値で初期化する
		StructType->CopyScriptStruct(Dest, StructMemory);

		return true;
	}
	return false;
}

void UDevParamStructType::DestroyValue(void* Dest) const
{
	if ( StructType && Dest )
	{
		// 構造体を破棄する
		StructType->DestroyStruct(Dest);
	}
}

UScriptStruct* UDevParamStructType::GetTargetStructType() const
{
	return StructType;
}

#if WITH_EDITOR

// エディター用
// DevParamエディター内の表に表示されるパラメータ値
FText UDevParamStructType::GetDefaultValueExtension() const
{
	const uint8* StructMemory = nullptr;
	if ( GetDefaultValuePtr(StructMemory) )
	{
		FString JsonString;
		if ( FJsonObjectConverter::UStructToJsonObjectString(
		         StructType, StructMemory, JsonString) )
		{
			JsonString = JsonString.Replace(TEXT("\r\n"), TEXT(""));
			return FText::FromString(JsonString);
		}
	}

	return FText::GetEmpty();
}

bool UDevParamStructType::CanEditChange(const FProperty* InProperty) const
{
	if ( GetOuter()->IsA<UDevParamDataAsset>() )
	{
		// データアセットに配置されているものなので特殊化を行う

		FName PropertyName = InProperty->GetFName();
		if ( (PropertyName ==
		      GET_MEMBER_NAME_CHECKED(UDevParamStructType, StructType)) ||
		     (PropertyName == GET_MEMBER_NAME_CHECKED(UDevParamStructType,
		                                              DefaultValuePropertyName)) )
		{
            // 該当のパラメータは編集させない
			return false;
		}
	}

	return Super::CanEditChange(InProperty);
}

#endif

// デフォルト値の値を取得する
bool UDevParamStructType::GetDefaultValuePtr(const uint8*& OutStructMemory) const
{
	UClass*          Class                = GetClass();
	FStructProperty* DefaultValueProperty = CastField<FStructProperty>(
	    Class->FindPropertyByName(DefaultValuePropertyName));

	if ( DefaultValueProperty == nullptr )
	{
		// DefaultValuePropertyが見つからなかった
		FDevMenuUtility::MessageLog(
		    this,
		    FText::Format(LOCTEXT("UDevParamStructTypeErrorNotFound",
		                          "NotFoundProperty: Tag={0} PropertyName={1} "),
		                  FText::FromName(ParamId.GetTagName()),
		                  FText::FromName(DefaultValuePropertyName)));

		return false;
	}

	if ( DefaultValueProperty->Struct != StructType )
	{
		// 構造体の型が不一致
		FDevMenuUtility::MessageLog(
		    this,
		    FText::Format(
		        LOCTEXT(
		            "UDevParamStructTypeErrorMissType",
		            "Structure Type mismatch: PropertyStruct={0} StructType={1} "),
		        FText::FromString(GetNameSafe(DefaultValueProperty->Struct)),
		        FText::FromString(GetNameSafe(StructType))));

		return false;
	}

	//
	const uint8* StructMemory =
	    DefaultValueProperty->ContainerPtrToValuePtr<uint8>(this);

	if ( StructMemory )
	{
		// 取得に成功
		OutStructMemory = StructMemory;

		return true;
	}
	return false;
}

#undef LOCTEXT_NAMESPACE