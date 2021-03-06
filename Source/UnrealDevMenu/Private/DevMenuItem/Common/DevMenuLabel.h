// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DevMenuItemBase.h"
class UDevMenuAdapterString;
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
	// 表示する文字列
	UPROPERTY(EditAnywhere, Instanced, Category = "DevMenu")
	TObjectPtr<UDevMenuAdapterString> TargetText;

	// メニューの項目名
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	FText Label;
	// メニューの説明
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	FText Tooltip;
};
