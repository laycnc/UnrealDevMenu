// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DevMenuItemBase.h"
#include "DevMenuText.generated.h"

/**
 * デバッグメニュー・テキスト
 */
UCLASS(Category = "Common", meta = (DisplayName = "Text"))
class UDevMenuText : public UDevMenuItemBase
{
	GENERATED_UCLASS_BODY()
public:
	// メニューの更新処理
	virtual void UpdateMenu(UDevMenuSubsystem& InSubsystem) const override;

public:
	// 表示テキスト
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	FText Text;
	// テキストの説明
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	FText Tooltip;
};
