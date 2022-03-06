// Fill out your copyright notice in the Description page of Project Settings.

#include "DevMenuItem/DevMenuGroup.h"
#include "DevMenuSubsystem.h"

UDevMenuGroup::UDevMenuGroup(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

// 初期化
void UDevMenuGroup::Initialize(UDevMenuSubsystem& InSubsystem,
                               UDevMenuInstanceBase*   InInstance) const
{
	if ( DevMenu == GetOuter() )
	{
		return;
	}

	if ( DevMenu )
	{
		DevMenu->GeneratedInstance(InSubsystem);
	}
}

// メニューの更新処理
void UDevMenuGroup::UpdateMenu(UDevMenuSubsystem& InSubsystem,
                               UDevMenuInstanceBase*   InInstance) const
{
	if ( DevMenu == GetOuter() )
	{
		return;
	}

	if ( DevMenu )
	{
		DevMenu->UpdateGroupMenu(InSubsystem);
	}
}
