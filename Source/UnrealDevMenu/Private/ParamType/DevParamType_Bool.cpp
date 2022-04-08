// Fill out your copyright notice in the Description page of Project Settings.

#include "ParamType/DevParamType_Bool.h"

UDevParamType_Bool::UDevParamType_Bool(
    const FObjectInitializer& ObjectInitializer)
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
