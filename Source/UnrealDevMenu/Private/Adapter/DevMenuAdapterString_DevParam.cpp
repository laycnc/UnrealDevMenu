// Fill out your copyright notice in the Description page of Project Settings.

#include "Adapter/DevMenuAdapterString_DevParam.h"
#include "DevMenuSubsystem.h"
#include "DevParamSubsystem.h"

UDevMenuAdapterString_DevParam::UDevMenuAdapterString_DevParam(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

// 値が設定された
void UDevMenuAdapterString_DevParam::SetValue_Implementation(
    UDevMenuSubsystem* InSubsystem,
    const FString&     NewValue) const
{
	UDevParamSubsystem* DevParamSystem = UDevParamSubsystem::Get(InSubsystem);
	if ( DevParamSystem )
	{
		DevParamSystem->SetValueByString(ParamId, NewValue);
	}
}

// 値の取得
FString UDevMenuAdapterString_DevParam::GetValue_Implementation(
    UDevMenuSubsystem* InSubsystem) const
{
	FString             Result         = TEXT("");
	UDevParamSubsystem* DevParamSystem = UDevParamSubsystem::Get(InSubsystem);
	if ( DevParamSystem )
	{
		DevParamSystem->GetValueByString(ParamId, Result);
	}
	return Result;
}
