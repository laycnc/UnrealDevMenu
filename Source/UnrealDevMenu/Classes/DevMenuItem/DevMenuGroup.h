// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DevMenuItemBase.h"
class UDevMenu;
#include "DevMenuGroup.generated.h"

/**
 * 
 */
UCLASS()
class UNREALDEVMENU_API UDevMenuGroup : public UDevMenuItemBase
{
	GENERATED_UCLASS_BODY()

public:
	// 初期化
	virtual void Initialize(UDevMenuSubsystem& InSubsystem,
	                        UDevMenuInstanceBase*   InInstance) const override;
	// メニューの更新処理
	virtual void UpdateMenu(UDevMenuSubsystem& InSubsystem,
	                        UDevMenuInstanceBase*   InInstance) const override;

private:
	// メニュー要素名
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDevMenu> DevMenu;
};
