// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealDevMenuEditorModule.h"
#include "Modules/ModuleManager.h"
#include "AssetToolsModule.h"
#include "PropertyEditorModule.h"
#include "AssetTypeActions/AssetTypeActions_DevMenu.h"
#include "AssetTypeActions/AssetTypeActions_DevParamDataAsset.h"
#include "Details/DevMenuBindingDetails.h"
#include "Details/DevParamStructTypeDetails.h"
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
	virtual TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager()
	    const override;
private:
	TArray<TSharedPtr<IAssetTypeActions>> CreatedAssetTypeActions;
	// クラスキャッシュノード
	TSharedPtr<FGraphNodeClassHelper> ClassCache;

	TSharedPtr<FExtensibilityManager> ToolBarExtensibilityManager;

	EAssetTypeCategories::Type DevMenuAssetCategoryBit = EAssetTypeCategories::Misc;
};

void FUnrealDevMenuEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	ToolBarExtensibilityManager = MakeShareable(new FExtensibilityManager);

	{
		// アセットタイプアクションを登録する
		IAssetTools& AssetTools =
		    FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

		DevMenuAssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(
		    FName(TEXT("DevMenu")), LOCTEXT("DevMenuAssetCategory", "DevMenu"));

		auto AddActionType = [&](TSharedPtr<IAssetTypeActions> NewAction)
		{
			CreatedAssetTypeActions.Add(NewAction);
			AssetTools.RegisterAssetTypeActions(NewAction.ToSharedRef());
		};

		AddActionType(
		    MakeShareable(new FAssetTypeActions_DevMenu(DevMenuAssetCategoryBit)));
		AddActionType(MakeShareable(
		    new FAssetTypeActions_DevParamDataAsset(DevMenuAssetCategoryBit)));
	}

	{
		// 詳細タブ拡張を登録解除
		FPropertyEditorModule& PropertyModule =
		    FModuleManager::GetModuleChecked<FPropertyEditorModule>(
		        TEXT("PropertyEditor"));

		PropertyModule.RegisterCustomClassLayout(
		    TEXT("DevParamStructType"),
		    FOnGetDetailCustomizationInstance::CreateStatic(
		        &FDevParamStructTypeCustomization::MakeInstance));

		PropertyModule.RegisterCustomPropertyTypeLayout(
		    TEXT("DevMenuBinding"),
		    FOnGetPropertyTypeCustomizationInstance::CreateStatic(
		        &FDevMenuBindingCustomization::MakeInstance));
	}
}

void FUnrealDevMenuEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	{
		// アセットタイプアクションをクリアする
		IAssetTools& AssetTools =
		    FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

		for ( TSharedPtr<IAssetTypeActions>& Action : CreatedAssetTypeActions )
		{
			AssetTools.UnregisterAssetTypeActions(Action.ToSharedRef());
		}
		CreatedAssetTypeActions.Empty();
	}

	{
		// 詳細タブ拡張を登録解除
		FPropertyEditorModule& PropertyModule =
		    FModuleManager::GetModuleChecked<FPropertyEditorModule>(
		        TEXT("PropertyEditor"));

		PropertyModule.UnregisterCustomPropertyTypeLayout(TEXT("DevMenuBinding"));
		PropertyModule.UnregisterCustomClassLayout(TEXT("DevParamStructType"));
	}

	ToolBarExtensibilityManager.Reset();
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

TSharedPtr<FExtensibilityManager> FUnrealDevMenuEditorModule::
    GetToolBarExtensibilityManager() const
{
	return ToolBarExtensibilityManager;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUnrealDevMenuEditorModule, DevMenuEditor)