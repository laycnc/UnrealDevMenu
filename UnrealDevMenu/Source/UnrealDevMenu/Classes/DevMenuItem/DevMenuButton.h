// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DevMenuBinding.h"
#include "DevMenuItemBase.h"
#include "DevMenuButton.generated.h"

/**
 * 
 */
UCLASS()
class UDevMenuButton : public UDevMenuItemBase
{
	GENERATED_UCLASS_BODY()

public:
	// メニューの更新処理
	virtual void UpdateMenu(UDevMenuSubsystem& InSubsystem,
	                        UDevMenuInstanceBase*   InInstance) const override;

public:
	// ボタンが押された時のコールバック
	UPROPERTY(EditAnywhere)
	FDevMenuBinding Callback;
};
