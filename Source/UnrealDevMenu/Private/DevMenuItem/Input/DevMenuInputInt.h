// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DevMenuItemBase.h"
#include "DevMenuInputInt.generated.h"

/**
 * 整数値テキスト入力ボックス
 */
UCLASS(Category = "Input")
class UDevMenuInputInt : public UDevMenuItemBase
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

	// スライダーの参照する値
	UPROPERTY(EditAnywhere, Instanced, Category = "DevMenu")
	TObjectPtr<UDevMenuAdapterInt> TargetValue;
};
