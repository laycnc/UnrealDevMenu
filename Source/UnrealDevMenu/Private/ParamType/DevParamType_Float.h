// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParamType/DevParamPrimitiveType.h"
#include "DevParamType_Float.generated.h"

/**
 * デバッグパラメータ 不動点少数型
 */
UCLASS(meta = (DisplayName = "DPT Float"), Category = "Primitive")
class UDevParamType_Float : public UDevParamPrimitiveType
{
	GENERATED_UCLASS_BODY()
public:
	virtual void InitializeValue(void* Dest) const override;
	virtual void DestroyValue(void* Dest) const override;

#if WITH_EDITOR
	// エディター用
	// DevParamエディター内の表に表示されるパラメータ値
	virtual FText GetDefaultValueExtension() const override final;
#endif

public:
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	float DefaultValue;
};
