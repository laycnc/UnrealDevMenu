// Fill out your copyright notice in the Description page of Project Settings.

#include "ParamType/DevParamStructType.h"

UDevParamStructType::UDevParamStructType(
    const FObjectInitializer& ObjectInitializer)
    : UDevParamStructType(ObjectInitializer, nullptr)
{
}

UDevParamStructType::UDevParamStructType(
    const FObjectInitializer& ObjectInitializer,
    UScriptStruct*            InStructType)
    : Super(ObjectInitializer)
    , StructType(InStructType)
{
	CreateStruct();
}

//~ Begin UObject Interface.

void UDevParamStructType::FinishDestroy()
{
	Super::FinishDestroy();

	DefaultValue = nullptr;
}

void UDevParamStructType::Serialize(FStructuredArchiveRecord Record)
{
	FArchive& BaseArchive = Record.GetUnderlyingArchive();

	Super::Serialize(Record);

	PreloadStructType();

	if ( BaseArchive.IsLoading() )
	{
		//
		DestoryStruct();
		CreateStruct();
	}

	if ( BaseArchive.IsLoading() || BaseArchive.IsSaving() )
	{
		FStructuredArchiveSlot StructDataSlot =
		    Record.EnterField(SA_FIELD_NAME(TEXT("StructData")));

		int32                   NumRows = DefaultValue.IsValid() ? 1 : 0;
		FStructuredArchiveArray Array   = StructDataSlot.EnterArray(NumRows);

		if ( DefaultValue.IsValid() )
		{
			FStructuredArchiveRecord Row     = Array.EnterElement().EnterRecord();
			uint8*                   RowData = DefaultValue->GetStructMemory();

			StructType->SerializeItem(
			    Row.EnterField(SA_FIELD_NAME(TEXT("DefaultValue"))),
			    RowData,
			    nullptr);
		}
	}
}

void UDevParamStructType::PostLoad()
{
	Super::PostLoad();
}

//~ End UObject Interface

void UDevParamStructType::CreateStruct()
{
	if ( !DefaultValue.IsValid() && StructType )
	{
		DefaultValue = MakeShared<FStructOnScope>(StructType);
	}
}

void UDevParamStructType::DestoryStruct() {}

// 構造体をプリロードする
void UDevParamStructType::PreloadStructType()
{
	if ( StructType && StructType->HasAnyFlags(RF_NeedLoad) )
	{
		auto RowStructLinker = StructType->GetLinker();
		if ( RowStructLinker )
		{
			RowStructLinker->Preload(StructType);
		}
	}
}

const TSharedPtr<FStructOnScope>& UDevParamStructType::GetDefaultValue() const
{
	return DefaultValue;
}