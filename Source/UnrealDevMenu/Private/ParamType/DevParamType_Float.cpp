// Fill out your copyright notice in the Description page of Project Settings.

#include "ParamType/DevParamType_Float.h"

UDevParamType_Float::UDevParamType_Float(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , DefaultValue(0)
{
}

void UDevParamType_Float::InitializeValue(void* Dest) const
{
	if ( float* Value = static_cast<float*>(Dest) )
	{
		(*Value) = DefaultValue;
	}
}

void UDevParamType_Float::DestroyValue(void* Dest) const
{
	(void)Dest;
}
