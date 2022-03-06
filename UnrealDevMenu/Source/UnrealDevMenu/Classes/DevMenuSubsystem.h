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
 * DevMenuで記録する値
 * Key   = UDevMenuItemBase::Id
 * Value = 記録値
 */
USTRUCT()
struct FDevMenuVariableInfo
{
	GENERATED_BODY();

public:
	UPROPERTY()
	TMap<FName, int32> IntVariables;

	UPROPERTY()
	TMap<FName, float> FloatVariables;

	UPROPERTY()
	TMap<FName, FName> NameVariables;

	UPROPERTY()
	TMap<FName, FString> StringVariables;

	UPROPERTY()
	TMap<FName, FDevMenuSubWindowInfo> WindowVariables;
};

/**
 *
 */
UCLASS()
class UNREALDEVMENU_API UDevMenuSubsystem : public UTickableWorldSubsystem
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
	void InitializeMenu(UDevMenu* InMenuAsset);

	void RegisterInstance(FName Id, UDevMenuInstanceBase* InInstance);
	UDevMenuInstanceBase* FindInstance(FName Id) const;

	void RegisterWindow(const UDevMenu& InWindowDevMenu);

	FDevMenuSubWindowInfo* GetWindowVariable(FName Id);
	bool                   GetVariable(FName Id, int32& OutValue) const;
	bool                   GetVariable(FName Id, float& OutValue) const;
	bool                   GetVariable(FName Id, FName& OutValue) const;
	bool                   GetVariable(FName Id, FString& OutValue) const;
	void                   SetVariable(FName Id, int32 NewValue);
	void                   SetVariable(FName Id, float NewValue);
	void                   SetVariable(FName Id, FName NewValue);
	void                   SetVariable(FName Id, const FString& NewValue);


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
	FDevMenuVariableInfo VariableInfo;

	// DevMenuの値
	UPROPERTY(Transient)
	TMap<FName, TObjectPtr<UDevMenuInstanceBase>> InstanceMap;
};
