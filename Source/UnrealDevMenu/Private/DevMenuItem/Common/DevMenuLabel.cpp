// Fill out your copyright notice in the Description page of Project Settings.

#include "DevMenuItem/Common/DevMenuLabel.h"
#include "DevMenuSubsystem.h"
#include "Adapter/DevMenuAdapterString.h"
#include "DevMenuUtility.h"

UDevMenuLabel::UDevMenuLabel(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

// メニューの更新処理
void UDevMenuLabel::UpdateMenu(UDevMenuSubsystem& InSubsystem) const
{
#if WITH_IMGUI

	if ( !IsValid(TargetText) )
	{
		// TargetValueが未設定なのでエラーを出して終了
		FDevMenuUtility::MessageLog(this,
		                            FText::FromString("TargetText : invalid"));
		ImGui::Text("[Error]");

		return;
	}

	// 現在の値を取得する
	FString CurrentValue = TargetText->GetValue(&InSubsystem);

	// ラベルを表示する
	ImGui::LabelText(TCHAR_TO_UTF8(*Label.ToString()), TCHAR_TO_UTF8(*CurrentValue));

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
