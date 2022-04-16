// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "DevParamType.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI,
       Abstract,
       NotBlueprintable,
       NotBlueprintType,
       editinlinenew,
       collapseCategories)
class UDevParamType : public UObject
{
	GENERATED_UCLASS_BODY()

public:
#if WITH_EDITOR

	// エディター用
	// DevParamエディター内の表に表示されるパラメータ値
	virtual FText GetDefaultValueExtension() const;

#endif

public:
	// パラメータ
	UPROPERTY(EditAnywhere,
	          meta     = (Categories = "DevMenuParam"),
	          Category = "DevMenu")
	FGameplayTag ParamId;

	// パラメータを保存するか？
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	bool bSaveParam;
};
