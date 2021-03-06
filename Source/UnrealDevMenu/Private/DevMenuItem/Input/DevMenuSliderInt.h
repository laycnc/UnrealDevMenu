// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DevMenuItemBase.h"
class UDevMenuAdapterInt;
#include "DevMenuSliderInt.generated.h"

/**
 * 整数値テキスト入力スライダー
 */
UCLASS(Category = "Input", meta = (DisplayName = "Slider Int"))
class UDevMenuSliderInt : public UDevMenuItemBase
{
	GENERATED_UCLASS_BODY()

public:
	// 初期化
	virtual void Initialize(UDevMenuSubsystem& InSubsystem) const override;
	// メニューの更新処理
	virtual void UpdateMenu(UDevMenuSubsystem& InSubsystem) const override;

public:
	// スライダー名
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	FText Label;
	// 説明
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	FText Tooltip;

	// デフォルト値
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	int32 DefaultValue;
	// スライダーの最小値
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	int32 MinValue;
	// スライダーの最大値
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	int32 MaxValue;

	// スライダーの参照する値
	UPROPERTY(EditAnywhere, Instanced, Category = "DevMenu")
	TObjectPtr<UDevMenuAdapterInt> TargetValue;
};
