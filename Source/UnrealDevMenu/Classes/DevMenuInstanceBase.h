// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
class UDevMenuItemBase;
#include "DevMenuInstanceBase.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI)
class UDevMenuInstanceBase : public UObject
{
	GENERATED_UCLASS_BODY()

	// 生成元のアイテム
	TWeakObjectPtr<const UDevMenuItemBase> SourceItem;
};
