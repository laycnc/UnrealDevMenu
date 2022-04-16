// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParamType/DevParamType.h"
#include "DevParamStructType.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class UNREALDEVMENU_API UDevParamStructType : public UDevParamType
{
	GENERATED_UCLASS_BODY()

public:
	virtual bool   InitializeValue(void* Dest) const;
	virtual void   DestroyValue(void* Dest) const;
	UScriptStruct* GetTargetStructType() const;

protected:
#if WITH_EDITOR
	// エディター用
	// DevParamエディター内の表に表示されるパラメータ値
	virtual FText GetDefaultValueExtension() const override final;
	virtual bool  CanEditChange(const FProperty* InProperty) const override;
#endif

	// デフォルト値の値を取得する
	bool GetDefaultValuePtr(const uint8*& OutStructMemory) const;

protected:
	// 対応する構造体
	UPROPERTY(EditDefaultsOnly, Category = "DevMenu")
	TObjectPtr<UScriptStruct> StructType;

	// デフォルト値を設定する為のプロパティ名
	UPROPERTY(EditDefaultsOnly, Category = "DevMenu")
	FName DefaultValuePropertyName;
};
