// Fill out your copyright notice in the Description page of Project Settings.

#include "DevMenuItem/DevMenuSubMenu.h"
#include "DevMenuSubsystem.h"

UDevMenuSubMenu::UDevMenuSubMenu(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

// 初期化
void UDevMenuSubMenu::Initialize(UDevMenuSubsystem&    InSubsystem,
                                 UDevMenuInstanceBase* InInstance) const
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
void UDevMenuSubMenu::UpdateMenu(UDevMenuSubsystem&    InSubsystem,
                                 UDevMenuInstanceBase* InInstance) const
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
