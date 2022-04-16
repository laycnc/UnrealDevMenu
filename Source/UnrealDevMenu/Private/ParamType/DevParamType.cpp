// Fill out your copyright notice in the Description page of Project Settings.

#include "ParamType/DevParamType.h"

UDevParamType::UDevParamType(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

#if WITH_EDITOR

// エディター用
// DevParamエディター内の表に表示されるパラメータ値
FText UDevParamType::GetDefaultValueExtension() const
{
	return FText::GetEmpty();
}

#endif
