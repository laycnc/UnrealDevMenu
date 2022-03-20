// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SlateFwd.h"
#include "Fonts/SlateFontInfo.h"
#include "Input/DragAndDrop.h"
#include "Input/Reply.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Views/STableViewBase.h"
#include "Widgets/Views/STableRow.h"

class UDevMenu;
class UDevMenuItemBase;
class IDevMenuItemInterface;
class FUnrealDevMenuEditor;

class FDevMenuHierarchyModel : public TSharedFromThis<FDevMenuHierarchyModel>
{
public:
	FDevMenuHierarchyModel();
	virtual ~FDevMenuHierarchyModel() {}

	void GatherChildren(TArray<TSharedPtr<FDevMenuHierarchyModel>>& Children);

	void                   GetFilterStrings(TArray<FString>& OutString) const;
	virtual FText          GetMenuName() const;
	virtual UObject*       GetObject() const;
	IDevMenuItemInterface* GetItemInterface() const;
	// ルートか？
	virtual bool IsRootModel() const;
	// 展開状態を設定する
	virtual void SetExpansion(bool bNewExpanded);
	// 展開状態か判定する
	virtual bool IsExpansion() const;
	// ドラッグを検出したときの処理
	virtual FReply HandleDragDetected(const FGeometry&     MyGeometry,
	                                  const FPointerEvent& MouseEvent);


protected:
	virtual void GetChildren(TArray<TSharedPtr<FDevMenuHierarchyModel>>& Children);
private:
	// 小階層
	TArray<TSharedPtr<FDevMenuHierarchyModel>> ChildrenModels;
	// 小階層を作成したか？
	bool bInitializeChildren = false;
	// 展開状態
	bool bIsExpanded = true;
};

//////////////////////////////////////////////////////////////////////////
//

class FDevMenuHierarchyItem : public FDevMenuHierarchyModel
{
public:
	FDevMenuHierarchyItem(UDevMenuItemBase* InItem);

	virtual FText    GetMenuName() const override;
	virtual UObject* GetObject() const override;
	// ルートか？
	virtual bool IsRootModel() const override;
	virtual void GetChildren(
	    TArray<TSharedPtr<FDevMenuHierarchyModel>>& Children) override;
	// ドラッグを検出したときの処理
	virtual FReply HandleDragDetected(const FGeometry&     MyGeometry,
	                                  const FPointerEvent& MouseEvent) override;

private:
	TWeakObjectPtr<UDevMenuItemBase> HostItem;
};

//////////////////////////////////////////////////////////////////////////
//

class FDevMenuHierarchyRoot : public FDevMenuHierarchyModel
{
public:
	FDevMenuHierarchyRoot(UDevMenu* InItem);

	virtual FText    GetMenuName() const override;
	virtual UObject* GetObject() const override;
	// ルートか？
	virtual bool IsRootModel() const override;
	virtual void GetChildren(
	    TArray<TSharedPtr<FDevMenuHierarchyModel>>& Children) override;
private:
	TWeakObjectPtr<UDevMenu> HostItem;
};
