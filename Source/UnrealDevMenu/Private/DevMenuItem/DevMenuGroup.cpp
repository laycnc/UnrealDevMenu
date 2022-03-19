// Fill out your copyright notice in the Description page of Project Settings.

#include "DevMenuItem/DevMenuGroup.h"
#include "DevMenuSubsystem.h"

UDevMenuGroup::UDevMenuGroup(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

// 初期化
void UDevMenuGroup::Initialize(UDevMenuSubsystem&    InSubsystem,
                               UDevMenuInstanceBase* InInstance) const
{
	// 子要素のインスタンスを作成する
	for ( auto Item : Items )
	{
		if ( Item )
		{
			Item->GeneratedInstance(InSubsystem);
		}
	}
}

// メニューの更新処理
void UDevMenuGroup::UpdateMenu(UDevMenuSubsystem&    InSubsystem,
                               UDevMenuInstanceBase* InInstance) const
{
#if WITH_IMGUI
	if ( ImGui::BeginMenu(TCHAR_TO_UTF8(*Id.ToString())) )
	{
		// 子要素のインスタンスを作成する
		for ( auto Item : Items )
		{
			if ( Item )
			{
				UDevMenuInstanceBase* Instance = InSubsystem.FindInstance(Item->Id);
				Item->UpdateMenu(InSubsystem, Instance);
			}
		}

		ImGui::EndMenu();
	}

	if ( ImGui::IsItemHovered() )
	{
		// ホバー中なのでツールチップを表示する
		if ( !Tooltip.IsEmpty() )
		{
			ImGui::SetTooltip(TCHAR_TO_UTF8(*Tooltip.ToString()));
		}
	}

#endif
}

// 子階層を取得する
void UDevMenuGroup::GetChildren(TArray<UDevMenuItemBase*>& OutChildren) const
{
	OutChildren.Append(Items);
}

#if WITH_EDITOR

// 新規メニュー項目を追加する
bool UDevMenuGroup::AddNewMenuItem(UClass* NewClass)
{
	if ( NewClass )
	{
		Modify();

		// 指定されたクラスで新規アイテムを生成して追加する
		UDevMenuItemBase* NewItem =
		    NewObject<UDevMenuItemBase>(this, NewClass, NAME_None, RF_Transactional);
		Items.Add(NewItem);

		PostEditChange();
		MarkPackageDirty();

		return true;
	}
	return false;
}

// 子項目を挿入出来るか？
bool UDevMenuGroup::CanInsertChildItem() const
{
	return true;
}

// 指定した項目を削除する
bool UDevMenuGroup::RemoveMenuItem(UDevMenuItemBase* RemoveItem)
{
	if ( Items.Contains(RemoveItem) )
	{
		Items.Remove(RemoveItem);

		PostEditChange();
		MarkPackageDirty();
		return true;
	}
	return false;
}

#endif
