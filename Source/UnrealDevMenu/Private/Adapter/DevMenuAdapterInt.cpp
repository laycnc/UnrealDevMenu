// Fill out your copyright notice in the Description page of Project Settings.

#include "Adapter/DevMenuAdapterInt.h"

UDevMenuAdapterInt::UDevMenuAdapterInt(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

// 値が設定された
void UDevMenuAdapterInt::SetValue_Implementation(UDevMenuSubsystem* InSubsystem,
                                                 int32              NewValue) const
{
}

// 値の取得
int32 UDevMenuAdapterInt::GetValue_Implementation(
    UDevMenuSubsystem* InSubsystem) const
{
	return 0;
}
