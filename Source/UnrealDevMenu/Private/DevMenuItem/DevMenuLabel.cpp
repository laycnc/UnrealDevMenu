// Fill out your copyright notice in the Description page of Project Settings.

#include "DevMenuItem/DevMenuLabel.h"
#include "DevMenuSubsystem.h"

UDevMenuLabel::UDevMenuLabel(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

// メニューの更新処理
void UDevMenuLabel::UpdateMenu(UDevMenuSubsystem& InSubsystem) const
{
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
}
