// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DevMenuItemBase.h"
class UDevMenu;
#include "DevMenuGroup.generated.h"

/**
 * デバッグメニュー・グループ
 */
UCLASS(Category = "Common", meta = (DisplayName = "Group"))
class UNREALDEVMENU_API UDevMenuGroup : public UDevMenuItemBase
{
	GENERATED_UCLASS_BODY()

public:
	// 初期化
	virtual void Initialize(UDevMenuSubsystem& InSubsystem) const override;
	// メニューの更新処理
	virtual void UpdateMenu(UDevMenuSubsystem& InSubsystem) const override;
	// 子階層を取得する
	virtual void GetChildren(TArray<UDevMenuItemBase*>& OutChildren) const override;

#if WITH_EDITOR
public:
	// 新規メニュー項目を追加する
	virtual bool AddNewMenuItem(UClass* NewClass) override;
	// 新規メニュー項目を追加する
	virtual void InsertNewMenuItem(UDevMenuItemBase* NewItem, int32 Index) override;
	// 子項目を挿入出来るか？
	virtual bool CanInsertChildItem() const override;
	// 指定した項目を削除する
	virtual bool RemoveMenuItem(UDevMenuItemBase* RemoveItem) override;
	// 所属しているIndex
	virtual int32 GetChildIndex(const UDevMenuItemBase& ChildItem) const override;
#endif


public:
	// メニューの項目名
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	FText Id;
	// メニューの説明
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	FText Tooltip;
	// 子要素
	UPROPERTY(EditDefaultsOnly, Instanced, AdvancedDisplay, Category = "DevMenu")
	TArray<TObjectPtr<UDevMenuItemBase>> Items;
};
