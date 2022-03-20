// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Class.h"
#include "Engine/NetSerialization.h"
#include "UObject/Interface.h"
class UDevMenuItemBase;
#include "DevMenuItemInterface.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UDevMenuItemInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()

public:
};

/**
 * 
 */
class UNREALDEVMENU_API IDevMenuItemInterface
{
	GENERATED_IINTERFACE_BODY()
public:
#if WITH_EDITOR

	// 新規メニュー項目を追加する
	virtual bool AddNewMenuItem(UClass* NewClass) = 0;
	// 新規メニュー項目を追加する
	virtual void InsertNewMenuItem(UDevMenuItemBase* NewItem, int32 Index) = 0;
	// 子項目を挿入出来るか？
	virtual bool CanInsertChildItem() const = 0;
	// 指定した項目を削除する
	virtual bool RemoveMenuItem(UDevMenuItemBase* RemoveItem) = 0;
	// 親要素を取得する
	virtual IDevMenuItemInterface* GetParentMenu() const = 0;
	// 親の配列に自分が配置されているIndex
	virtual int32 GetPlacedIndex() const = 0;
	// 指定した要素が配置されているIndex
	virtual int32 GetChildIndex(const UDevMenuItemBase& ChildItem) const = 0;
#endif
};
