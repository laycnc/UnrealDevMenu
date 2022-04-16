// Fill out your copyright notice in the Description page of Project Settings.

#include "ParamType/DevParamType_Name.h"

UDevParamType_Name::UDevParamType_Name(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , DefaultValue(NAME_None)
{
}

void UDevParamType_Name::InitializeValue(void* Dest) const
{
	if ( FName* Value = static_cast<FName*>(Dest) )
	{
		(*Value) = DefaultValue;
	}
}

void UDevParamType_Name::DestroyValue(void* Dest) const
{
	(void)Dest;
}

#if WITH_EDITOR

// エディター用
// DevParamエディター内の表に表示されるパラメータ値
FText UDevParamType_Name::GetDefaultValueExtension() const
{
	return FText::FromName(DefaultValue);
}

#endif
