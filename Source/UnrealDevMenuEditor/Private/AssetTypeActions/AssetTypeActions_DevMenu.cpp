// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetTypeActions_DevMenu.h"
#include "DevMenu.h"

#define LOCTEXT_NAMESPACE "FAssetTypeActions_DevMenu"

// IAssetTypeActions Implementation
FText FAssetTypeActions_DevMenu::GetName() const
{
	return NSLOCTEXT(
	    "AssetTypeActions", "AssetTypeActions_DevMenu", "DevMenu");
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

#undef LOCTEXT_NAMESPACE
