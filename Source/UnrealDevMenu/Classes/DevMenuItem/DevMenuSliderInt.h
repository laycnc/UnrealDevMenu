// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DevMenuItem/DevMenuItemBase.h"
#include "DevMenuSliderInt.generated.h"

/**
 * 
 */
UCLASS()
class UDevMenuSliderInt : public UDevMenuItemBase
{
	GENERATED_UCLASS_BODY()

public:
	// 初期化
	virtual void Initialize(UDevMenuSubsystem& InSubsystem,
	                        UDevMenuInstanceBase*   InInstance) const override;
	// メニューの更新処理
	virtual void UpdateMenu(UDevMenuSubsystem& InSubsystem,
	                        UDevMenuInstanceBase*   InInstance) const override;

public:
	// デフォルト値
	UPROPERTY(EditAnywhere)
	int32 DefaultValue;
	// スライダーの最小値
	UPROPERTY(EditAnywhere)
	int32 MinValue;
	// スライダーの最大値
	UPROPERTY(EditAnywhere)
	int32 MaxValue;
};
