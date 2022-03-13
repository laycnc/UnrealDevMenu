// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealDevMenuEditorModule.h"
#include "Modules/ModuleManager.h"
#include "AssetToolsModule.h"
#include "PropertyEditorModule.h"
#include "AssetTypeActions/AssetTypeActions_DevMenu.h"
#include "Details/DevMenuBindingDetails.h"
#include "AIGraphTypes.h"

#define LOCTEXT_NAMESPACE "FDevMenuModule"

class FUnrealDevMenuEditorModule : public IUnrealDevMenuEditorModule
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	// クラスキャッシュを取得する
	virtual TSharedRef<FGraphNodeClassHelper> GetMenuItemClassCache() override;
private:
	TSharedPtr<IAssetTypeActions> Action;
	// クラスキャッシュノード
	TSharedPtr<FGraphNodeClassHelper> ClassCache;
};

void FUnrealDevMenuEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Register asset types
	IAssetTools& AssetTools =
	    FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	Action = MakeShareable(new FAssetTypeActions_DevMenu());
	AssetTools.RegisterAssetTypeActions(Action.ToSharedRef());

	FPropertyEditorModule& PropertyModule =
	    FModuleManager::GetModuleChecked<FPropertyEditorModule>(
	        TEXT("PropertyEditor"));

	PropertyModule.RegisterCustomPropertyTypeLayout(
	    "DevMenuBinding",
	    FOnGetPropertyTypeCustomizationInstance::CreateStatic(
	        &FDevMenuBindingCustomization::MakeInstance));
}

void FUnrealDevMenuEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Register asset types
	IAssetTools& AssetTools =
	    FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	AssetTools.UnregisterAssetTypeActions(Action.ToSharedRef());
	Action = nullptr;

	FPropertyEditorModule& PropertyModule =
	    FModuleManager::GetModuleChecked<FPropertyEditorModule>(
	        TEXT("PropertyEditor"));

	PropertyModule.UnregisterCustomPropertyTypeLayout("DevMenuBinding");
}

// クラスキャッシュを取得する
TSharedRef<FGraphNodeClassHelper> FUnrealDevMenuEditorModule::GetMenuItemClassCache()
{
	if ( !ClassCache.IsValid() )
	{
		// 初回呼び出しの時にクラスキャッシュを作成する
		ClassCache = MakeShareable(
		    new FGraphNodeClassHelper(UDevMenuItemBase::StaticClass()));
		ClassCache->UpdateAvailableBlueprintClasses();
	}

	return ClassCache.ToSharedRef();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUnrealDevMenuEditorModule, DevMenuEditor)