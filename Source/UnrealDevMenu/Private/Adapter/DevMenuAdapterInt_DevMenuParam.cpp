// Fill out your copyright notice in the Description page of Project Settings.

#include "Adapter/DevMenuAdapterInt_DevMenuParam.h"
#include "DevMenuSubsystem.h"
#include "DevParamSubsystem.h"

UDevMenuAdapterInt_DevMenuParam::UDevMenuAdapterInt_DevMenuParam(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

// 値が設定された
void UDevMenuAdapterInt_DevMenuParam::SetValue_Implementation(
    UDevMenuSubsystem* InSubsystem,
    int32              NewValue) const
{
	UDevParamSubsystem* DevParamSystem = UDevParamSubsystem::Get(InSubsystem);
	if ( DevParamSystem )
	{
		DevParamSystem->SetValueByInt32(ParamId, NewValue);
	}
}

// 値の取得
int32 UDevMenuAdapterInt_DevMenuParam::GetValue_Implementation(
    UDevMenuSubsystem* InSubsystem) const
{
	int32               Result         = 0;
	UDevParamSubsystem* DevParamSystem = UDevParamSubsystem::Get(InSubsystem);
	if ( DevParamSystem )
	{
		DevParamSystem->GetValueByInt32(ParamId, Result);
	}
	return Result;
}