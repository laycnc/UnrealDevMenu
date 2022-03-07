// Fill out your copyright notice in the Description page of Project Settings.

#include "DevMenuItem/DevMenuItemBase.h"
#include "DevMenuSubsystem.h"

UDevMenuItemBase::UDevMenuItemBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , InstanceClass()
{
}

// 実行用のインスタンスを作成する
void UDevMenuItemBase::GeneratedInstance(UDevMenuSubsystem& InSubsystem) const
{
	UDevMenuInstanceBase* NewInstance = nullptr;
	if ( InstanceClass )
	{
		// インスタンスクラスからオブジェクト作成
		NewInstance = NewObject<UDevMenuInstanceBase>(&InSubsystem, InstanceClass);
		NewInstance->SourceItem = this;
		InSubsystem.RegisterInstance(Id, NewInstance);
	}
	// 初期化処理を呼ぶ
	Initialize(InSubsystem, NewInstance);
}

// 初期化
void UDevMenuItemBase::Initialize(UDevMenuSubsystem&    InSubsystem,
                                  UDevMenuInstanceBase* InInstance) const
{
}

// メニューの更新処理
void UDevMenuItemBase::UpdateMenu(UDevMenuSubsystem&    InSubsystem,
                                  UDevMenuInstanceBase* InInstance) const
{
}

// 子階層を取得する
void UDevMenuItemBase::GetChildren(TArray<UDevMenuItemBase*>& OutChildren)
{
	(void)OutChildren;
}

// メニューの説明を取得
bool UDevMenuItemBase::HasTooltip() const
{
	return !Tooltip.IsEmpty();
}