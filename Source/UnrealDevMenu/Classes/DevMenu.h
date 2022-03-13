// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
class UDevMenuItemBase;
class UDevMenuInstanceBase;
class UDevMenuSubsystem;
struct FDevMenuSubWindowInfo;
#include "DevMenu.generated.h"

/**
 *
 */
UCLASS()
class UNREALDEVMENU_API UDevMenu : public UObject
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
	// デバッグメニューの識別子(必須)
	UPROPERTY(EditDefaultsOnly)
	FName Id;

	// ウインドウを出すか？
	UPROPERTY(EditDefaultsOnly)
	bool bWindow;

	UPROPERTY(EditDefaultsOnly, Instanced, AdvancedDisplay)
	TArray<TObjectPtr<UDevMenuItemBase>> Items;

private:
	// ウインドウ表示プロパティの説明ラベル
	UPROPERTY()
	FText ShowWindowLabel;

	// ウインドウ表示プロパティの説明ラベル
	UPROPERTY()
	FText ShowWindowTooltip;
};
