// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetTypeActions_DevMenu.h"
#include "DevMenu.h"
#include "Editor/UnrealDevMenuEditor.h"

#define LOCTEXT_NAMESPACE "FAssetTypeActions_DevMenu"

// IAssetTypeActions Implementation
FText FAssetTypeActions_DevMenu::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_DevMenu", "DevMenu");
}
FColor FAssetTypeActions_DevMenu::GetTypeColor() const
{
	return FColor(255, 255, 255);
}
UClass* FAssetTypeActions_DevMenu::GetSupportedClass() const
{
	return UDevMenu::StaticClass();
}

bool FAssetTypeActions_DevMenu::HasActions(const TArray<UObject*>& InObjects) const
{
	return false;
}

void FAssetTypeActions_DevMenu::GetActions(const TArray<UObject*>&  InObjects,
                                           struct FToolMenuSection& Section)
{
}

uint32 FAssetTypeActions_DevMenu::GetCategories()
{
	return EAssetTypeCategories::Misc;
}

void FAssetTypeActions_DevMenu::OpenAssetEditor(
    const TArray<UObject*>&        InObjects,
    TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid()
	                                    ? EToolkitMode::WorldCentric
	                                    : EToolkitMode::Standalone;

	for ( UObject* ObjIt : InObjects )
	{
		UDevMenu* DevMenu = Cast<UDevMenu>(ObjIt);
		if ( DevMenu == nullptr )
		{
			continue;
		}

		const bool            bFocusIfOpen = false;
		IAssetEditorInstance* EditorInstance =
		    GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->FindEditorForAsset(
		        DevMenu, bFocusIfOpen);
		FUnrealDevMenuEditor* ExistingInstance =
		    static_cast<FUnrealDevMenuEditor*>(EditorInstance);

		if ( ExistingInstance && ExistingInstance->GetDevMenuEdited() == nullptr )
		{
			// 既存のエディターがあるのでそれを流用する
			ExistingInstance->InitDevMenuEditor(
			    Mode, EditWithinLevelEditor, DevMenu);
		}
		else
		{
			TSharedRef<FUnrealDevMenuEditor> NewDevMenuEditor(
			    new FUnrealDevMenuEditor());
			NewDevMenuEditor->InitDevMenuEditor(
			    Mode, EditWithinLevelEditor, DevMenu);
		}
	}
}

#undef LOCTEXT_NAMESPACE
