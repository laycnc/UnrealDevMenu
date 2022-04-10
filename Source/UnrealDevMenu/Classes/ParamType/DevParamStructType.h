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

protected:
	UDevParamStructType(const FObjectInitializer& ObjectInitializer,
	                       UScriptStruct*            InStructType);


public:
	//~ Begin UObject Interface.
	virtual void FinishDestroy() override;
	virtual void Serialize(FStructuredArchiveRecord Record) override;
	virtual void PostLoad() override;
	//~ End UObject Interface

	const TSharedPtr<FStructOnScope>& GetDefaultValue() const;

protected:
	// 構造体をプリロードする
	void PreloadStructType();

	void CreateStruct();
	void DestoryStruct();

protected:
	// 対応する構造体
	UPROPERTY(EditDefaultsOnly, Category = "DevMenu")
	TObjectPtr<UScriptStruct> StructType;

	// 構造体データ
	TSharedPtr<FStructOnScope> DefaultValue;
};
