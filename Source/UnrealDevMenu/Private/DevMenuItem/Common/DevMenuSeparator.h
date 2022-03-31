// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DevMenuItemBase.h"
#include "DevMenuSeparator.generated.h"

/**
 * 
 */
UCLASS(Category = "Common")
class UDevMenuSeparator : public UDevMenuItemBase
{
	GENERATED_UCLASS_BODY()
public:
	// メニューの更新処理
	virtual void UpdateMenu(UDevMenuSubsystem& InSubsystem) const override;
};
