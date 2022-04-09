// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DevMenuBinding.h"
#include "Adapter/DevMenuAdapterString.h"
#include "DevMenuAdapterString_BindingFunc.generated.h"

/**
 * 
 */
UCLASS()
class UDevMenuAdapterString_BindingFunc : public UDevMenuAdapterString
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
	/**
     * 文字列値を取得する為の関数
     * 
     * static void(FString, const UObject*)
     */
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	FDevMenuBinding Getter;

	/**
     * 文字列値を設定する為の関数
     * 
     * static FString(const UObject*)
     */
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	FDevMenuBinding Setter;
};
