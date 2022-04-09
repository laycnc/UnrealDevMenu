// Fill out your copyright notice in the Description page of Project Settings.

#include "Adapter/DevMenuAdapterString.h"

UDevMenuAdapterString::UDevMenuAdapterString(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

// 値が設定された
void UDevMenuAdapterString::SetValue_Implementation(UDevMenuSubsystem* InSubsystem,
                                                    const FString& NewValue) const
{
}

// 値の取得
FString UDevMenuAdapterString::GetValue_Implementation(
    UDevMenuSubsystem* InSubsystem) const
{
	return TEXT("");
}
