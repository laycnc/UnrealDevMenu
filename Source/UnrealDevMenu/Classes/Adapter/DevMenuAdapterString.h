// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Adapter/DevMenuAdapter.h"
#include "DevMenuAdapterString.generated.h"

/**
 * デバッグメニューの文字列値アクセスアダプター
 */
UCLASS(Abstract, Const, Blueprintable)
class UNREALDEVMENU_API UDevMenuAdapterString : public UDevMenuAdapter
{
	GENERATED_UCLASS_BODY()

public:
	/**
     * 値が設定された
     * 
     * @param NewValue 新規値
     */
	UFUNCTION(BlueprintNativeEvent, Category = "DevMenu")
	void SetValue(UDevMenuSubsystem* InSubsystem, const FString& NewValue) const;

	/**
     * 値の取得
     * 
     */
	UFUNCTION(BlueprintNativeEvent, Category = "DevMenu")
	FString GetValue(UDevMenuSubsystem* InSubsystem) const;
};
