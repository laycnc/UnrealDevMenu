// Fill out your copyright notice in the Description page of Project Settings.

#include "DevMenu.h"
#include "DevMenuItemBase.h"
#include "DevMenuInstanceBase.h"
#include "DevMenuSubsystem.h"
#include "DevParamSubsystem.h"

#define LOCTEXT_NAMESPACE "UDevMenu"

UDevMenu::UDevMenu(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , ShowWindowLabel(LOCTEXT("UDevMenu_ShowWindowLabel", "Show Window"))
    , ShowWindowTooltip(LOCTEXT("UDevMenu_ShowWindowTooltip", "switch Show Window"))
{
}

// ウインドウ情報のIDを取得する
FName UDevMenu::GetWindowInfoParamId() const
{
	return FName(*FString::Printf(TEXT("DevMenuWindow.%s"), *Id.ToString()));
}

// 実行用のインスタンスを作成する
void UDevMenu::GeneratedInstance(UDevMenuSubsystem& Subsystem) const
{
	if ( bWindow )
	{
		Subsystem.RegisterWindow(*this);
	}

	// 子要素のインスタンスを作成する
	for ( auto Item : Items )
	{
		if ( Item )
		{
			Item->Initialize(Subsystem);
		}
	}
}

// メニューの更新処理
void UDevMenu::UpdateMenu(UDevMenuSubsystem& InSubsystem) const
{
	// 子要素のインスタンスを作成する
	for ( auto Item : Items )
	{
		if ( Item )
		{
			Item->UpdateMenu(InSubsystem);
		}
	}
}

// メニューの更新処理
void UDevMenu::UpdateGroupMenu(UDevMenuSubsystem& InSubsystem) const
{
#if WITH_IMGUI
	if ( ImGui::BeginMenu(TCHAR_TO_UTF8(*Id.ToString())) )
	{
		// ウインドウの表示切り替えをグループ側で出来るようにする
		if ( bWindow )
		{
			UDevParamSubsystem* DevParam = UDevParamSubsystem::Get(&InSubsystem);

			FDevMenuSubWindowInfo WindowInfo        = {};
			FName                 WindowInfoParamId = GetWindowInfoParamId();
			DevParam->GetValueByStruct(WindowInfoParamId, WindowInfo);

			bool bShowWindow = WindowInfo.bVisible;
			if ( ImGui::Checkbox(TCHAR_TO_UTF8(*ShowWindowLabel.ToString()),
			                     &bShowWindow) )
			{
				// 表示状態が切り替わった
				WindowInfo.bVisible = bShowWindow;

				DevParam->SetValueByStruct(WindowInfoParamId, WindowInfo);
			}

			if ( ImGui::IsItemHovered() )
			{
				// ホバー中なのでツールチップを表示する
				if ( !ShowWindowTooltip.IsEmpty() )
				{
					ImGui::SetTooltip(TCHAR_TO_UTF8(*ShowWindowTooltip.ToString()));
				}
			}
		}

		UpdateMenu(InSubsystem);
		ImGui::EndMenu();
	}
#endif
}

// メニューウインドウの表示
void UDevMenu::UpdateMenuWindow(UDevMenuSubsystem& InSubsystem) const
{
#if WITH_IMGUI

	UDevParamSubsystem* DevParam = UDevParamSubsystem::Get(&InSubsystem);

	FDevMenuSubWindowInfo WindowInfo        = {};
	FName                 WindowInfoParamId = GetWindowInfoParamId();
	bool                  bChangedValue     = false;

	DevParam->GetValueByStruct(WindowInfoParamId, WindowInfo);

	if ( !WindowInfo.bVisible )
	{
		// ウインドウを表示する必要が無い場合はスルー
		return;
	}

	bool             no_scrollbar      = false;
	bool             no_collapse       = false;
	bool             no_nav            = false;
	bool             no_background     = false;
	bool             no_bring_to_front = false;
	ImGuiWindowFlags window_flags      = 0;
	//window_flags |= ImGuiWindowFlags_MenuBar;
	if ( no_scrollbar )
		window_flags |= ImGuiWindowFlags_NoScrollbar;
	if ( no_collapse )
		window_flags |= ImGuiWindowFlags_NoCollapse;
	if ( no_nav )
		window_flags |= ImGuiWindowFlags_NoNav;
	if ( no_background )
		window_flags |= ImGuiWindowFlags_NoBackground;
	if ( no_bring_to_front )
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

	// We specify a default position/size in case there's no data in the .ini file. Typically this isn't required! We only do it to make the Demo applications a little more welcoming.
	const ImVec2 CurrentPosition =
	    ImVec2(WindowInfo.Position.X, WindowInfo.Position.Y);
	const ImVec2 CurrentSize = ImVec2(WindowInfo.Size.X, WindowInfo.Size.Y);
	ImGui::SetNextWindowPos(ImVec2(WindowInfo.Position.X, WindowInfo.Position.Y),
	                        ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(550, 340), ImGuiCond_FirstUseEver);

	// Main body of the Demo window starts here.
	if ( ImGui::Begin(
	         TCHAR_TO_UTF8(*Id.ToString()), &WindowInfo.bVisible, window_flags) )
	{
		UpdateMenu(InSubsystem);

		// Early out if the window is collapsed, as an optimization.
		ImGui::End();

		const ImVec2 NewWindowPos = ImGui::GetWindowPos();
		if ( NewWindowPos.x != CurrentPosition.x ||
		     NewWindowPos.y != CurrentPosition.y )
		{
			// 座標が変わっているので保存値を変える
			WindowInfo.Position.X = NewWindowPos.x;
			WindowInfo.Position.Y = NewWindowPos.y;
			bChangedValue         = true;
		}

		const ImVec2 NewWindowSize = ImGui::GetWindowSize();
		if ( NewWindowSize.x != CurrentSize.x || NewWindowSize.y != CurrentSize.y )
		{
			// サイズが変わっているので保存値を変える
			WindowInfo.Size.X = NewWindowSize.x;
			WindowInfo.Size.Y = NewWindowSize.y;
			bChangedValue     = true;
		}

		if ( !WindowInfo.bVisible )
		{
			// 表示が消えている場合にはパラメータ変更扱い
			bChangedValue = true;
		}
	}

	if ( bChangedValue )
	{
		// 値が変更されたのでパラメータを設定する
		DevParam->SetValueByStruct(WindowInfoParamId, WindowInfo);
	}

#endif // WITH_IMGUI
}

// 子階層を取得する
void UDevMenu::GetChildren(TArray<UDevMenuItemBase*>& OutChildren) const
{
	for ( auto Item : Items )
	{
		if ( Item )
		{
			OutChildren.Add(Item);
		}
	}
}

#if WITH_EDITOR

bool UDevMenu::AddNewMenuItem(UClass* NewClass)
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

// 新規メニュー項目を追加する
void UDevMenu::InsertNewMenuItem(UDevMenuItemBase* NewItem, int32 Index)
{
	Modify();

	if ( Index == INDEX_NONE )
	{
		Items.Add(NewItem);
	}
	else
	{
		Items.Insert(NewItem, Index);
	}
	NewItem->Rename(nullptr, this);

	PostEditChange();
	MarkPackageDirty();
}

// 子項目を挿入出来るか？
bool UDevMenu::CanInsertChildItem() const
{
	return true;
}

// 指定した項目を削除する
bool UDevMenu::RemoveMenuItem(UDevMenuItemBase* RemoveItem)
{
	if ( Items.Contains(RemoveItem) )
	{
		Modify();

		Items.Remove(RemoveItem);

		PostEditChange();
		MarkPackageDirty();

		return true;
	}
	for ( auto Item : Items )
	{
		if ( Item->RemoveMenuItem(RemoveItem) )
		{
			return true;
		}
	}
	return false;
}

// 親要素を取得する
IDevMenuItemInterface* UDevMenu::GetParentMenu() const
{
	return nullptr;
}

// 親の配列に自分が配置されているIndex
int32 UDevMenu::GetPlacedIndex() const
{
	return INDEX_NONE;
}

// 指定した要素が配置されているIndex
int32 UDevMenu::GetChildIndex(const UDevMenuItemBase& ChildItem) const
{
	return Items.IndexOfByKey(&ChildItem);
}

#endif

#undef LOCTEXT_NAMESPACE
