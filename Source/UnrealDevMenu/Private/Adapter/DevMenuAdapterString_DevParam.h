// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Adapter/DevMenuAdapterString.h"
#include "DevMenuAdapterString_DevParam.generated.h"

/**
 * 
 */
UCLASS()
class UDevMenuAdapterString_DevParam : public UDevMenuAdapterString
{
	GENERATED_UCLASS_BODY()

public:
	// 値が設定された
	virtual void SetValue_Implementation(UDevMenuSubsystem* Subsystem,
	                                     const FString&     NewValue) const override;
	// 値の取得
	virtual FString GetValue_Implementation(
	    UDevMenuSubsystem* Subsystem) const override;

private:
	// パラメータ
	UPROPERTY(EditAnywhere,
	          meta     = (Categories = "DevMenuParam"),
	          Category = "DevMenu")
	FGameplayTag ParamId;
};
