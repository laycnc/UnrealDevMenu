// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParamType/DevParamStructType.h"
#include "DevParamType_DevMenuWindowInfo.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "DPT DevMenuWindowInfo"), Category = "Primitive")
class UDevParamType_DevMenuWindowInfo : public UDevParamStructType
{
	GENERATED_UCLASS_BODY()

public:
	// デフォルト値
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	FDevMenuSubWindowInfo DefaultValue;
};
