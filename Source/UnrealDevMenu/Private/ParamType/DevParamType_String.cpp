// Fill out your copyright notice in the Description page of Project Settings.

#include "ParamType/DevParamType_String.h"

UDevParamType_String::UDevParamType_String(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , DefaultValue()
{
}

void UDevParamType_String::InitializeValue(void* Dest) const
{
	if ( FString* Value = static_cast<FString*>(Dest) )
	{
		(*Value) = DefaultValue;
	}
}

void UDevParamType_String::DestroyValue(void* Dest) const
{
	(void)Dest;
}
