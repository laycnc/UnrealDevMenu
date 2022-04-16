// Fill out your copyright notice in the Description page of Project Settings.

#include "ParamType/DevParamType_Bool.h"

UDevParamType_Bool::UDevParamType_Bool(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , bDefaultValue(false)
{
}

void UDevParamType_Bool::InitializeValue(void* Dest) const
{
	if ( bool* BoolValue = static_cast<bool*>(Dest) )
	{
		(*BoolValue) = bDefaultValue;
	}
}

void UDevParamType_Bool::DestroyValue(void* Dest) const
{
	(void)Dest;
}

#if WITH_EDITOR

// エディター用
// DevParamエディター内の表に表示されるパラメータ値
FText UDevParamType_Bool::GetDefaultValueExtension() const
{
	return FText::FromString(LexToString(bDefaultValue));
}

#endif
