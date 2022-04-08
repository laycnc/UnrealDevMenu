// Fill out your copyright notice in the Description page of Project Settings.

#include "ParamType/DevParamType_Object.h"

UDevParamType_Object::UDevParamType_Object(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , DefaultValue(nullptr)
{
}

void UDevParamType_Object::InitializeValue(void* Dest) const
{
	if ( TWeakObjectPtr<UObject>* Value =
	         static_cast<TWeakObjectPtr<UObject>*>(Dest) )
	{
		(*Value) = DefaultValue;
	}
}

void UDevParamType_Object::DestroyValue(void* Dest) const
{
	(void)Dest;
}
