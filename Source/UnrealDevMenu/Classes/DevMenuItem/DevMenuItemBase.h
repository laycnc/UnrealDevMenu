// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
class UDevMenuInstanceBase;
class UDevMenuSubsystem;
#include "DevMenuItemBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, editinlinenew, collapseCategories)
class UNREALDEVMENU_API UDevMenuItemBase : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	// 実行用のインスタンスを作成する
	void GeneratedInstance(UDevMenuSubsystem& InSubsystem) const;
	// 初期化
	virtual void Initialize(UDevMenuSubsystem&    InSubsystem,
	                        UDevMenuInstanceBase* InInstance) const;
	// メニューの更新処理
	virtual void UpdateMenu(UDevMenuSubsystem&    InSubsystem,
	                        UDevMenuInstanceBase* InInstance) const;

	// 子階層を取得する
	virtual void GetChildren(TArray<UDevMenuItemBase*>& OutChildren);

protected:
	// メニューの説明が存在するか？
	bool HasTooltip() const;

public:
	// インスタンスクラス
	UPROPERTY()
	TSubclassOf<UDevMenuInstanceBase> InstanceClass;
	// メニュー要素名
	UPROPERTY(EditAnywhere)
	FName Id;

	// メニューの項目名
	UPROPERTY(EditAnywhere)
	FText Label;

	// メニューの説明
	UPROPERTY(EditAnywhere)
	FText Tooltip;
};
