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
class FUnrealDevMenuEditor;

class FDevMenuHierarchyModel : public TSharedFromThis<FDevMenuHierarchyModel>
{
public:
	FDevMenuHierarchyModel(TSharedPtr<FUnrealDevMenuEditor> InEditor);
	virtual ~FDevMenuHierarchyModel() {}

	void GatherChildren(TArray<TSharedPtr<FDevMenuHierarchyModel>>& Children);

	void             GetFilterStrings(TArray<FString>& OutString) const;
	virtual FText    GetMenuName() const;
	virtual UObject* GetObject() const;
protected:
	virtual void GetChildren(TArray<TSharedPtr<FDevMenuHierarchyModel>>& Children);

protected:
	TSharedPtr<FUnrealDevMenuEditor> Editor;
};

//////////////////////////////////////////////////////////////////////////
//

class FDevMenuHierarchyItem : public FDevMenuHierarchyModel
{
public:
	FDevMenuHierarchyItem(TSharedPtr<FUnrealDevMenuEditor> InEditor,
	                      UDevMenuItemBase*                InItem);

	virtual FText    GetMenuName() const override;
	virtual UObject* GetObject() const override;
	virtual void     GetChildren(
	        TArray<TSharedPtr<FDevMenuHierarchyModel>>& Children) override;

private:
	TWeakObjectPtr<UDevMenuItemBase> HostItem;
};

//////////////////////////////////////////////////////////////////////////
//

class FDevMenuHierarchyRoot : public FDevMenuHierarchyModel
{
public:
	FDevMenuHierarchyRoot(TSharedPtr<FUnrealDevMenuEditor> InEditor,
	                      UDevMenu*                        InItem);

	virtual FText    GetMenuName() const override;
	virtual UObject* GetObject() const override;
	virtual void     GetChildren(
	        TArray<TSharedPtr<FDevMenuHierarchyModel>>& Children) override;

private:
	TWeakObjectPtr<UDevMenu> HostItem;
};
