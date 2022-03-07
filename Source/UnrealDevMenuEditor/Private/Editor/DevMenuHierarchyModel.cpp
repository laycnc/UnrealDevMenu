#include "DevMenuHierarchyModel.h"
#include "DevMenu.h"
#include "DevMenuItem/DevMenuItemBase.h"

#define LOCTEXT_NAMESPACE "FDevMenuHierarchyModel"

//////////////////////////////////////////////////////////////////////////
// FDevMenuHierarchyModel

FDevMenuHierarchyModel::FDevMenuHierarchyModel(
    TSharedPtr<FUnrealDevMenuEditor> InEditor)
    : Editor(InEditor)
{
}

void FDevMenuHierarchyModel::GatherChildren(
    TArray<TSharedPtr<FDevMenuHierarchyModel>>& Children)
{
	GetChildren(Children);
}

void FDevMenuHierarchyModel::GetFilterStrings(TArray<FString>& OutString) const
{
	//
	OutString.Add(GetMenuName().ToString());
}

FText FDevMenuHierarchyModel::GetMenuName() const
{
	return LOCTEXT("MenuName_Unknown", "[Unknown]");
}

UObject* FDevMenuHierarchyModel::GetObject() const
{
	return nullptr;
}

void FDevMenuHierarchyModel::GetChildren(
    TArray<TSharedPtr<FDevMenuHierarchyModel>>& Children)
{
}

//////////////////////////////////////////////////////////////////////////
// FDevMenuHierarchyItem

FDevMenuHierarchyItem::FDevMenuHierarchyItem(
    TSharedPtr<FUnrealDevMenuEditor> InEditor,
    UDevMenuItemBase*                InItem)
    : FDevMenuHierarchyModel(InEditor)
    , HostItem(InItem)
{
}

FText FDevMenuHierarchyItem::GetMenuName() const
{
	return FText::FromString(HostItem->GetName());
}

UObject* FDevMenuHierarchyItem::GetObject() const
{
	return HostItem.Get();
}

void FDevMenuHierarchyItem::GetChildren(
    TArray<TSharedPtr<FDevMenuHierarchyModel>>& Children)
{
	if ( HostItem.IsValid() )
	{
		TArray<UDevMenuItemBase*> ChildItems;
		HostItem->GetChildren(ChildItems);

		for ( UDevMenuItemBase* Item : ChildItems )
		{
			Children.Add(MakeShareable(new FDevMenuHierarchyItem(Editor, Item)));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// FDevMenuHierarchyRoot

FDevMenuHierarchyRoot::FDevMenuHierarchyRoot(
    TSharedPtr<FUnrealDevMenuEditor> InEditor,
    UDevMenu*                        InItem)
    : FDevMenuHierarchyModel(InEditor)
    , HostItem(InItem)
{
}

FText FDevMenuHierarchyRoot::GetMenuName() const
{
	return FText::FromString(HostItem->GetName());
}

UObject* FDevMenuHierarchyRoot::GetObject() const
{
	return HostItem.Get();
}

void FDevMenuHierarchyRoot::GetChildren(
    TArray<TSharedPtr<FDevMenuHierarchyModel>>& Children)
{
	if ( HostItem.IsValid() )
	{
		TArray<UDevMenuItemBase*> ChildItems;
		HostItem->GetChildren(ChildItems);

		for ( UDevMenuItemBase* Item : ChildItems )
		{
			Children.Add(MakeShareable(new FDevMenuHierarchyItem(Editor, Item)));
		}
	}
}

#undef LOCTEXT_NAMESPACE
