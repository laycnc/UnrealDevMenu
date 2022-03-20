// Fill out your copyright notice in the Description page of Project Settings.

#include "DevMenuItem/DevMenuItemBase.h"
#include "DevMenuSubsystem.h"
#include "DevMenu.h"

UDevMenuItemBase::UDevMenuItemBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

// 初期化
void UDevMenuItemBase::Initialize(UDevMenuSubsystem& InSubsystem) const {}

// メニューの更新処理
void UDevMenuItemBase::UpdateMenu(UDevMenuSubsystem& InSubsystem) const {}

// 子階層を取得する
void UDevMenuItemBase::GetChildren(TArray<UDevMenuItemBase*>& OutChildren) const
{
	(void)OutChildren;
}

// ルートメニューを取得
UDevMenu* UDevMenuItemBase::GetRootMenu() const
{
	UObject* Outer = GetOuter();
	while ( Outer )
	{
		if ( UDevMenu* DevMenu = Cast<UDevMenu>(Outer) )
		{
			return DevMenu;
		}
		Outer = Outer->GetOuter();
	}

	return nullptr;
}

#if WITH_EDITOR

// 新規メニュー項目を追加する
bool UDevMenuItemBase::AddNewMenuItem(UClass* NewClass)
{
	static_cast<void>(NewClass);
	return false;
}

// 新規メニュー項目を追加する
void UDevMenuItemBase::InsertNewMenuItem(UDevMenuItemBase* NewItem, int32 Index)
{
	static_cast<void>(NewItem);
	static_cast<void>(Index);
}

// 子項目を挿入出来るか？
bool UDevMenuItemBase::CanInsertChildItem() const
{
	return false;
}

// 指定した項目を削除する
bool UDevMenuItemBase::RemoveMenuItem(UDevMenuItemBase* RemoveItem)
{
	static_cast<void>(RemoveItem);
	return false;
}

// 親要素を取得する
IDevMenuItemInterface* UDevMenuItemBase::GetParentMenu() const
{
	return Cast<IDevMenuItemInterface>(GetOuter());
}

// 所属している親Index
int32 UDevMenuItemBase::GetPlacedIndex() const
{
	if ( IDevMenuItemInterface* MenuItemInterface = GetParentMenu() )
	{
		return MenuItemInterface->GetChildIndex(*this);
	}

	return INDEX_NONE;
}

// 所属しているIndex
int32 UDevMenuItemBase::GetChildIndex(const UDevMenuItemBase& ChildItem) const
{
	return INDEX_NONE;
}

#endif