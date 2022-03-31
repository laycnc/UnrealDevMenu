// Fill out your copyright notice in the Description page of Project Settings.

#include "DevMenuItem/Input/DevMenuSliderInt.h"
#include "DevMenuSubsystem.h"
#include "Adapter/DevMenuAdapterInt.h"
#include "DevMenuUtility.h"

UDevMenuSliderInt::UDevMenuSliderInt(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

// 初期化
void UDevMenuSliderInt::Initialize(UDevMenuSubsystem& InSubsystem) const
{
	// 初期値を生成する
	//InSubsystem.SetVariable(Id, DefaultValue);
}

// メニューの更新処理
void UDevMenuSliderInt::UpdateMenu(UDevMenuSubsystem& InSubsystem) const
{
#if WITH_IMGUI

	if ( !IsValid(TargetValue) )
	{
		// TargetValueが未設定なのでエラーを出して終了
		FDevMenuUtility::MessageLog(this,
		                            FText::FromString("TargetValue : invalid"));
		ImGui::Text("[Error]");

		return;
	}

	// 現在の値を取得する
	int32 CurrentValue = TargetValue->GetValue(&InSubsystem);

	if ( ImGui::SliderInt(
	         TCHAR_TO_UTF8(*Label.ToString()), &CurrentValue, MinValue, MaxValue) )
	{
		// 値の更新があったので反映
		TargetValue->SetValue(&InSubsystem, CurrentValue);
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
