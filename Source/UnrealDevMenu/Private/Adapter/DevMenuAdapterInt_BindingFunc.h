// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DevMenuBinding.h"
#include "Adapter/DevMenuAdapterInt.h"
#include "DevMenuAdapterInt_BindingFunc.generated.h"

/**
 * 
 */
UCLASS()
class UDevMenuAdapterInt_BindingFunc : public UDevMenuAdapterInt
{
	GENERATED_UCLASS_BODY()

public:
	// 値が設定された
	virtual void SetValue_Implementation(UDevMenuSubsystem* Subsystem,
	                                     int32              NewValue) const override;
	// 値の取得
	virtual int32 GetValue_Implementation(
	    UDevMenuSubsystem* Subsystem) const override;

private:
	// 整数値を取得する為の関数
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	FDevMenuBinding Getter;

	// 整数値を設定する為の関数
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	FDevMenuBinding Setter;
};
