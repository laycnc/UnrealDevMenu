// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DevMenuBinding.h"
#include "DevMenuItemBase.h"
#include "DevMenuButton.generated.h"

/**
 * デバッグメニュー・ボタン
 */
UCLASS(Category = "Input", meta = (DisplayName = "Button"))
class UDevMenuButton : public UDevMenuItemBase
{
	GENERATED_UCLASS_BODY()

public:
	// メニューの更新処理
	virtual void UpdateMenu(UDevMenuSubsystem& InSubsystem) const override;

public:
    // ボタンラ名
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	FText Label;
	// メニューの説明
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	FText Tooltip;

	// ボタンが押された時のコールバック
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	FDevMenuBinding Callback;
};
