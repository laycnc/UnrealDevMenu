// Fill out your copyright notice in the Description page of Project Settings.

#include "DevMenuItem/Common/DevMenuSeparator.h"
#include "DevMenuSubsystem.h"

UDevMenuSeparator::UDevMenuSeparator(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

// メニューの更新処理
void UDevMenuSeparator::UpdateMenu(UDevMenuSubsystem& InSubsystem) const
{
#if WITH_IMGUI
	// スペーサーを表示する
	ImGui::Separator();
#endif
}
