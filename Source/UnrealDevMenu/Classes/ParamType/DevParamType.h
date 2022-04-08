// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "DevParamType.generated.h"

/**
 * 
 */
UCLASS(Abstract,
       NotBlueprintable,
       NotBlueprintType,
       editinlinenew,
       collapseCategories)
class UNREALDEVMENU_API UDevParamType : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	// パラメータ
	UPROPERTY(EditAnywhere,
	          meta     = (Categories = "DevMenuParam"),
	          Category = "DevMenu")
	FGameplayTag ParamId;
};
