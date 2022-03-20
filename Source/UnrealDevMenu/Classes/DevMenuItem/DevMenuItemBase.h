// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DevMenuItemInterface.h"
class UDevMenuInstanceBase;
class UDevMenuSubsystem;
#include "DevMenuItemBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, editinlinenew, collapseCategories)
class UNREALDEVMENU_API UDevMenuItemBase
    : public UObject
    , public IDevMenuItemInterface
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
	virtual void GetChildren(TArray<UDevMenuItemBase*>& OutChildren) const;

protected:
	// メニューの説明が存在するか？
	bool HasTooltip() const;
	// ルートメニューを取得
	UDevMenu* GetRootMenu() const;

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
	// 親要素を取得する
	virtual IDevMenuItemInterface* GetParentMenu() const override;
	// 所属している親Index
	virtual int32 GetPlacedIndex() const override;
	// 所属しているIndex
	virtual int32 GetChildIndex(const UDevMenuItemBase& ChildItem) const;

#endif

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
