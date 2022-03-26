// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Adapter/DevMenuAdapterInt.h"
#include "DevMenuAdapterInt_DevMenuParam.generated.h"

/**
 * 
 */
UCLASS()
class UDevMenuAdapterInt_DevMenuParam : public UDevMenuAdapterInt
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
	// デバッグメニューパラメータのID
	UPROPERTY(EditAnywhere, Category = "DevMenu|DevMenuParam")
	FName Id;
};
