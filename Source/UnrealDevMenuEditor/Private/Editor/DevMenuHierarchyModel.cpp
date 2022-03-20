#include "DevMenuHierarchyModel.h"
#include "DevMenu.h"
#include "DevMenuItem/DevMenuItemBase.h"
#include "DragDrop/MenuHierarchyDragDropOp.h"

#define LOCTEXT_NAMESPACE "FDevMenuHierarchyModel"

//////////////////////////////////////////////////////////////////////////
// FDevMenuHierarchyModel

FDevMenuHierarchyModel::FDevMenuHierarchyModel() {}

void FDevMenuHierarchyModel::GatherChildren(
    TArray<TSharedPtr<FDevMenuHierarchyModel>>& Children)
{
	if ( !bInitializeChildren )
	{
		// 初回だけ行う
		GetChildren(ChildrenModels);
		bInitializeChildren = true;
	}
	Children.Append(ChildrenModels);
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

bool FDevMenuHierarchyModel::CanInsertChildItem() const
{
	return false;
}

// 新規メニュー項目を追加する
bool FDevMenuHierarchyModel::AddNewMenuItem(UClass* NewClass) const
{
	static_cast<void>(NewClass);
	return false;
}

// 新規メニュー項目を追加する
void FDevMenuHierarchyModel::InsertNewMenuItem(UDevMenuItemBase* NewItem,
                                               int32             Index) const
{
	static_cast<void>(NewItem);
	static_cast<void>(Index);
}

// 展開状態を設定する
void FDevMenuHierarchyModel::SetExpansion(bool bNewExpanded)
{
	bIsExpanded = bNewExpanded;
}

// 展開状態か判定する
bool FDevMenuHierarchyModel::IsExpansion() const
{
	return bIsExpanded;
}

// ドラッグを検出したときの処理
FReply FDevMenuHierarchyModel::HandleDragDetected(const FGeometry&     MyGeometry,
                                                  const FPointerEvent& MouseEvent)
{
	return FReply::Unhandled();
}

void FDevMenuHierarchyModel::GetChildren(
    TArray<TSharedPtr<FDevMenuHierarchyModel>>& Children)
{
}

//////////////////////////////////////////////////////////////////////////
// FDevMenuHierarchyItem

FDevMenuHierarchyItem::FDevMenuHierarchyItem(UDevMenuItemBase* InItem)
    : FDevMenuHierarchyModel()
    , HostItem(InItem)
{
}

FText FDevMenuHierarchyItem::GetMenuName() const
{
	if ( HostItem.IsValid() )
	{
		return FText::FromString(HostItem->GetName());
	}
	return FText::GetEmpty();
}

UObject* FDevMenuHierarchyItem::GetObject() const
{
	return HostItem.Get();
}

// 子項目を挿入出来るか？
bool FDevMenuHierarchyItem::CanInsertChildItem() const
{
	if ( HostItem.IsValid() )
	{
		return HostItem->CanInsertChildItem();
	}
	return false;
}

bool FDevMenuHierarchyItem::AddNewMenuItem(UClass* NewClass) const
{
	if ( HostItem.IsValid() )
	{
		FScopedTransaction Transaction(LOCTEXT("AddItem", "Add Item"));
		if ( HostItem->AddNewMenuItem(NewClass) )
		{
			return true;
		}
		Transaction.Cancel();
	}
	return false;
}

// 新規メニュー項目を追加する
void FDevMenuHierarchyItem::InsertNewMenuItem(UDevMenuItemBase* NewItem,
                                              int32             Index) const
{
	if ( HostItem.IsValid() )
	{
		HostItem->InsertNewMenuItem(NewItem, Index);
	}
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
			Children.Add(MakeShareable(new FDevMenuHierarchyItem(Item)));
		}
	}
}

// ドラッグを検出したときの処理
FReply FDevMenuHierarchyItem::HandleDragDetected(const FGeometry&     MyGeometry,
                                                 const FPointerEvent& MouseEvent)
{
	if ( HostItem.IsValid() )
	{
		TArray<TWeakObjectPtr<UDevMenuItemBase>> SelectItems;

		SelectItems.Add(HostItem);

		return FReply::Handled().BeginDragDrop(
		    FMenuHierarchyDragDropOp::New(SelectItems));
	}

	return FReply::Unhandled();
}

//////////////////////////////////////////////////////////////////////////
// FDevMenuHierarchyRoot

FDevMenuHierarchyRoot::FDevMenuHierarchyRoot(UDevMenu* InItem)
    : FDevMenuHierarchyModel()
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

// 子項目を挿入出来るか？
bool FDevMenuHierarchyRoot::CanInsertChildItem() const
{
	// Rootの下にはぶら下げられる
	return true;
}

// 新規メニュー項目を追加する
bool FDevMenuHierarchyRoot::AddNewMenuItem(UClass* NewClass) const
{
	if ( HostItem.IsValid() )
	{
		FScopedTransaction Transaction(LOCTEXT("AddItem", "Add Item"));
		if ( HostItem->AddNewMenuItem(NewClass) )
		{
			return true;
		}
		Transaction.Cancel();
	}
	return false;
}

// 新規メニュー項目を追加する
void FDevMenuHierarchyRoot::InsertNewMenuItem(UDevMenuItemBase* NewItem,
                                              int32             Index) const
{
	if ( HostItem.IsValid() )
	{
		HostItem->InsertNewMenuItem(NewItem, Index);
	}
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
			Children.Add(MakeShareable(new FDevMenuHierarchyItem(Item)));
		}
	}
}

#undef LOCTEXT_NAMESPACE
