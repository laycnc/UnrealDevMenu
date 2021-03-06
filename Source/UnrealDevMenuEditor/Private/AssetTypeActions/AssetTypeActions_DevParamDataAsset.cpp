// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetTypeActions_DevParamDataAsset.h"
#include "ParamType/DevParamDataAsset.h"
#include "DevParamEditor/DevParamEditor.h"

#define LOCTEXT_NAMESPACE "FAssetTypeActions_DevParamDataAsset"

FAssetTypeActions_DevParamDataAsset::FAssetTypeActions_DevParamDataAsset(
    uint32 InCategory)
    : FAssetTypeActions_Base()
    , Category(InCategory)
{
}

// IAssetTypeActions Implementation
FText FAssetTypeActions_DevParamDataAsset::GetName() const
{
	return NSLOCTEXT("AssetTypeActions",
	                 "AssetTypeActions_DevParamDataAsset:",
	                 "DevParamDataAsset");
}
FColor FAssetTypeActions_DevParamDataAsset::GetTypeColor() const
{
	return FColor(255, 255, 255);
}
UClass* FAssetTypeActions_DevParamDataAsset::GetSupportedClass() const
{
	return UDevParamDataAsset::StaticClass();
}

bool FAssetTypeActions_DevParamDataAsset::HasActions(
    const TArray<UObject*>& InObjects) const
{
	return false;
}

void FAssetTypeActions_DevParamDataAsset::GetActions(
    const TArray<UObject*>&  InObjects,
    struct FToolMenuSection& Section)
{
}

uint32 FAssetTypeActions_DevParamDataAsset::GetCategories()
{
	return Category;
}

void FAssetTypeActions_DevParamDataAsset::OpenAssetEditor(
    const TArray<UObject*>&        InObjects,
    TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid()
	                                    ? EToolkitMode::WorldCentric
	                                    : EToolkitMode::Standalone;

	for ( UObject* ObjIt : InObjects )
	{
		UDevParamDataAsset* DevParamDataAsset = Cast<UDevParamDataAsset>(ObjIt);
		if ( DevParamDataAsset == nullptr )
		{
			continue;
		}

		const bool            bFocusIfOpen = false;
		IAssetEditorInstance* EditorInstance =
		    GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->FindEditorForAsset(
		        DevParamDataAsset, bFocusIfOpen);
		FDevParamEditor* ExistingInstance =
		    static_cast<FDevParamEditor*>(EditorInstance);

		if ( ExistingInstance && ExistingInstance->GetDevParamEdited() == nullptr )
		{
			// ????????????????????????????????????????????????????????????
			ExistingInstance->InitDevMenuEditor(
			    Mode, EditWithinLevelEditor, DevParamDataAsset);
		}
		else
		{
			TSharedRef<FDevParamEditor> NewDevMenuEditor(new FDevParamEditor());
			NewDevMenuEditor->InitDevMenuEditor(
			    Mode, EditWithinLevelEditor, DevParamDataAsset);
		}
	}
}

#undef LOCTEXT_NAMESPACE
