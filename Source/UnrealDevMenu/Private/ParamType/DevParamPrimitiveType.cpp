// Fill out your copyright notice in the Description page of Project Settings.

#include "ParamType/DevParamPrimitiveType.h"

UDevParamPrimitiveType::UDevParamPrimitiveType(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UDevParamPrimitiveType::InitializeValue(void* Dest) const {}
void UDevParamPrimitiveType::DestroyValue(void* Dest) const {}