// Fill out your copyright notice in the Description page of Project Settings.

#include "ParamType/DevParamType_Double.h"

UDevParamType_Double::UDevParamType_Double(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , DefaultValue(0)
{
}

void UDevParamType_Double::InitializeValue(void* Dest) const
{
	if ( double* Value = static_cast<double*>(Dest) )
	{
		(*Value) = DefaultValue;
	}
}

void UDevParamType_Double::DestroyValue(void* Dest) const
{
	(void)Dest;
}

#if WITH_EDITOR

// エディター用
// DevParamエディター内の表に表示されるパラメータ値
FText UDevParamType_Double::GetDefaultValueExtension() const
{
	return FText::AsNumber(DefaultValue);
}

#endif
