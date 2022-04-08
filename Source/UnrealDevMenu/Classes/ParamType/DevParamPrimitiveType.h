// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParamType/DevParamType.h"
#include "DevParamPrimitiveType.generated.h"

/**
 * デバッグメニューパラメータ・プリミティブ型
 */
UCLASS(Abstract, MinimalAPI)
class UDevParamPrimitiveType : public UDevParamType
{
	GENERATED_UCLASS_BODY()

public:
	virtual void InitializeValue(void* Dest) const;
	virtual void DestroyValue(void* Dest) const;
};
