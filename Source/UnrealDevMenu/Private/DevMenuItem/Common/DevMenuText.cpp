// Fill out your copyright notice in the Description page of Project Settings.

#include "DevMenuItem/Common/DevMenuText.h"
#include "DevMenuSubsystem.h"

UDevMenuText::UDevMenuText(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

// メニューの更新処理
void UDevMenuText::UpdateMenu(UDevMenuSubsystem& InSubsystem) const
{
#if WITH_IMGUI
	// テキストを表示する
	ImGui::Text(TCHAR_TO_UTF8(*Text.ToString()));

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
