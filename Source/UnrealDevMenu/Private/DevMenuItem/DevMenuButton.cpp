// Fill out your copyright notice in the Description page of Project Settings.

#include "DevMenuItem/DevMenuButton.h"
#include "DevMenuSubsystem.h"

UDevMenuButton::UDevMenuButton(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
	Callback.FunctionArgsType.Add(UObject::StaticClass());
}

// メニューの更新処理
void UDevMenuButton::UpdateMenu(UDevMenuSubsystem& InSubsystem) const
{
	// ラベルを表示する
	if ( ImGui::Button(TCHAR_TO_UTF8(*Label.ToString())) )
	{
		struct FArgs
		{
			const UObject* WorldContext;
		};

		FArgs Args        = {};
		Args.WorldContext = &InSubsystem;

		// イベントの実行
		Callback.Invoke(&Args);
	}

	if ( ImGui::IsItemHovered() )
	{
		// ホバー中なのでツールチップを表示する
		if ( !Tooltip.IsEmpty() )
		{
			ImGui::SetTooltip(TCHAR_TO_UTF8(*Tooltip.ToString()));
		}
	}
}
