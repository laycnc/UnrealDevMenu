// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
class UDevParamType;
#include "DevParamDataAsset.generated.h"


/**
 * デバッグパラメータ定義アセット
 */
UCLASS()
class UDevParamDataAsset : public UDataAsset
{
	GENERATED_UCLASS_BODY()

public:
	// パラメータ
	UPROPERTY(EditAnywhere, Instanced, Category = "DevMenu")
	TArray<TObjectPtr<UDevParamType>> Params;
};
