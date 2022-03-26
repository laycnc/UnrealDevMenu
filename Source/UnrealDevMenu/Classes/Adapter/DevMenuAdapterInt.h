// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Adapter/DevMenuAdapter.h"
class UDevMenuSubsystem;
#include "DevMenuAdapterInt.generated.h"

/**
 * デバッグメニューの値
 */
UCLASS(Abstract, Const, Blueprintable)
class UNREALDEVMENU_API UDevMenuAdapterInt : public UDevMenuAdapter
{
	GENERATED_UCLASS_BODY()

public:
	/**
     * 値が設定された
     * 
     * @param NewValue 新規値
     */
	UFUNCTION(BlueprintNativeEvent, Category = "DevMenu")
	void SetValue(UDevMenuSubsystem* InSubsystem, int32 NewValue) const;

	/**
     * 値の取得
     * 
     */
	UFUNCTION(BlueprintNativeEvent, Category = "DevMenu")
	int32 GetValue(UDevMenuSubsystem* InSubsystem) const;
};
