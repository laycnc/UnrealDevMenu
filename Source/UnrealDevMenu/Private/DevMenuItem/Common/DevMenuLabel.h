// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DevMenuItemBase.h"
#include "DevMenuLabel.generated.h"

/**
 * デバッグメニュー・ラベル
 */
UCLASS(Category = "Common", meta = (DisplayName = "Label"))
class UDevMenuLabel : public UDevMenuItemBase
{
	GENERATED_UCLASS_BODY()
public:
	// メニューの更新処理
	virtual void UpdateMenu(UDevMenuSubsystem& InSubsystem) const override;


public:
	// メニューの項目名
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	FText Id;
	// メニューの項目名
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	FText Label;
	// メニューの説明
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	FText Tooltip;
};
