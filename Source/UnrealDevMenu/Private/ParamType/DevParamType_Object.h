// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParamType/DevParamPrimitiveType.h"
#include "DevParamType_Object.generated.h"

/**
 * 
 */
UCLASS()
class UDevParamType_Object : public UDevParamPrimitiveType
{
	GENERATED_UCLASS_BODY()
public:
	virtual void InitializeValue(void* Dest) const override;
	virtual void DestroyValue(void* Dest) const override;

public:
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	TObjectPtr<UObject> DefaultValue;
};
