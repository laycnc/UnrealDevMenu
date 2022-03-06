// Fill out your copyright notice in the Description page of Project Settings.

#include "DevMenuItem/DevMenuSliderInt.h"
#include "DevMenuSubsystem.h"

UDevMenuSliderInt::UDevMenuSliderInt(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

// 初期化
void UDevMenuSliderInt::Initialize(UDevMenuSubsystem& InSubsystem,
                                   UDevMenuInstanceBase*   InInstance) const
{
	// 初期値を生成する
	InSubsystem.SetVariable(Id, DefaultValue);
}

// メニューの更新処理
void UDevMenuSliderInt::UpdateMenu(UDevMenuSubsystem& InSubsystem,
                                   UDevMenuInstanceBase*   InInstance) const
{
	int32 TargetValue = 0;
	if ( !InSubsystem.GetVariable(Id, TargetValue) )
	{
		return;
	}

	if ( ImGui::SliderInt(
	         TCHAR_TO_UTF8(*Label.ToString()), &TargetValue, MinValue, MaxValue) )
	{
		InSubsystem.SetVariable(Id, TargetValue);
	}

	if ( ImGui::IsItemHovered() )
	{
		// ホバー中なのでツールチップを表示する
		if ( HasTooltip() )
		{
			ImGui::SetTooltip(TCHAR_TO_UTF8(*Tooltip.ToString()));
		}
	}
}
