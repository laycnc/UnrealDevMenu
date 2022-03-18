// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DevMenuItemBase.h"
#include "DevMenuLabel.generated.h"

/**
 * デバッグメニュー・ラベル
 */
UCLASS(Category = "Common")
class UDevMenuLabel : public UDevMenuItemBase
{
	GENERATED_UCLASS_BODY()
public:
	// メニューの更新処理
	virtual void UpdateMenu(UDevMenuSubsystem& InSubsystem,
	                        UDevMenuInstanceBase*   InInstance) const override;
};
