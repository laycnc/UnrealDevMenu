// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ImGuiCommon.h"
class UDevMenu;
#include "DevMenuSubsystem.generated.h"

USTRUCT()
struct FDevMenuSubWindowInfo
{
	GENERATED_BODY();
public:
	// window location
	UPROPERTY()
	FVector2D Position;
	// window size
	UPROPERTY()
	FVector2D Size;
	// window visible
	UPROPERTY()
	bool bVisible;
};

/**
 * デバッグメニューサブシステム
 */
UCLASS(MinimalAPI)
class UDevMenuSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	// USubsystem implementation Begin
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// USubsystem implementation End

	// UTickableWorldSubsystem implementation Begin
	virtual void    Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	// UTickableWorldSubsystem implementation End

	/**
	 * @brief メニューの初期化
	*/
	UFUNCTION(BlueprintCallable, Category = "DevMenu")
	UNREALDEVMENU_API void InitializeMenu(UDevMenu* InMenuAsset);

	void RegisterWindow(const UDevMenu& InWindowDevMenu);

	FDevMenuSubWindowInfo* GetWindowVariable(FName Id);
private:
	void ImGuiTick();

private:
	// Root DebugMenu
	UPROPERTY(Transient)
	TObjectPtr<UDevMenu> RootDevMenu;

	// Window DebugMenu
	UPROPERTY(Transient)
	TArray<TObjectPtr<const UDevMenu>> WindowDevMenus;

	// DevMenuの値
	UPROPERTY(Transient)
	TMap<FName, FDevMenuSubWindowInfo> WindowVariables;
};
