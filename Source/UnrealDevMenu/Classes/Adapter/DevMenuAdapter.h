// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DevMenuAdapter.generated.h"

/**
 * デバッグメニュー・項目アダプター
 * 
 * CheckBoxなどの操作時に参照する値にアクセスする為の機構
 * Instancedで生成される想定
 */
UCLASS(MinimalAPI, editinlinenew, collapseCategories)
class UDevMenuAdapter : public UObject
{
	GENERATED_UCLASS_BODY()
};
