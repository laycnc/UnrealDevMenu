// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DevMenuItemInterface.h"
class UDevMenuItemBase;
class UDevMenuInstanceBase;
class UDevMenuSubsystem;
struct FDevMenuSubWindowInfo;
#include "DevMenu.generated.h"

/**
 *
 */
UCLASS()
class UNREALDEVMENU_API UDevMenu
    : public UObject
    , public IDevMenuItemInterface
{
	GENERATED_UCLASS_BODY()


public:
	// 実行用のインスタンスを作成する
	void GeneratedInstance(UDevMenuSubsystem& Subsystem) const;

	// メニューの更新処理
	void UpdateMenu(UDevMenuSubsystem& InSubsystem) const;
	// メニューの更新処理
	void UpdateGroupMenu(UDevMenuSubsystem& InSubsystem) const;

	// メニューウインドウの表示
	void UpdateMenuWindow(UDevMenuSubsystem&     InSubsystem,
	                      FDevMenuSubWindowInfo& WindowInfo) const;

	// 子階層を取得する
	void GetChildren(TArray<UDevMenuItemBase*>& OutChildren) const;

public:
#if WITH_EDITOR

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
	// 親の配列に自分が配置されているIndex
	virtual int32 GetPlacedIndex() const override;
	// 指定した要素が配置されているIndex
	virtual int32 GetChildIndex(const UDevMenuItemBase& ChildItem) const override;

#endif

public:
	// デバッグメニューの識別子(必須)
	UPROPERTY(EditDefaultsOnly)
	FName Id;

	// ウインドウを出すか？
	UPROPERTY(EditDefaultsOnly)
	bool bWindow;

    // メニューに配置されている要素
	UPROPERTY(VisibleDefaultsOnly, Instanced, AdvancedDisplay)
	TArray<TObjectPtr<UDevMenuItemBase>> Items;

private:
	// ウインドウ表示プロパティの説明ラベル
	UPROPERTY()
	FText ShowWindowLabel;

	// ウインドウ表示プロパティの説明ラベル
	UPROPERTY()
	FText ShowWindowTooltip;
};
