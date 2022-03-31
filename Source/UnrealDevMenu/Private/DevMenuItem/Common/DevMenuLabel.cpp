// Fill out your copyright notice in the Description page of Project Settings.

#include "DevMenuItem/Common/DevMenuLabel.h"
#include "DevMenuSubsystem.h"

UDevMenuLabel::UDevMenuLabel(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

// メニューの更新処理
void UDevMenuLabel::UpdateMenu(UDevMenuSubsystem& InSubsystem) const
{
#if WITH_IMGUI

	// ラベルを表示する
	ImGui::LabelText(TCHAR_TO_UTF8(*Id.ToString()),
	                 TCHAR_TO_UTF8(*Label.ToString()));

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
