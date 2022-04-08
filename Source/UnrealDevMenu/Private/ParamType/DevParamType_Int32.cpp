// Fill out your copyright notice in the Description page of Project Settings.

#include "ParamType/DevParamType_Int32.h"

UDevParamType_Int32::UDevParamType_Int32(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , DefaultValue(0)
{
}

void UDevParamType_Int32::InitializeValue(void* Dest) const
{
	if ( int32* Value = static_cast<int32*>(Dest) )
	{
		(*Value) = DefaultValue;
	}
}

void UDevParamType_Int32::DestroyValue(void* Dest) const
{
	(void)Dest;
}
