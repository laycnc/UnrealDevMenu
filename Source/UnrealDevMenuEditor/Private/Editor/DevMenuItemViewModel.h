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
#include "AIGraphTypes.h"

class UDevMenu;
class UDevMenuItemBase;
class FUnrealDevMenuEditor;

class FDevMenuItemViewModel : public TSharedFromThis<FDevMenuItemViewModel>
{
public:
	FDevMenuItemViewModel();
	virtual ~FDevMenuItemViewModel() {}

	virtual void GetFilterStrings(TArray<FString>& OutString) const = 0;
	virtual TSharedRef<ITableRow> BuildRow(
	    const TSharedRef<STableViewBase>& OwnerTable) = 0;
	virtual void GetChildren(TArray<TSharedPtr<FDevMenuItemViewModel>>& OutChildren);
};

//////////////////////////////////////////////////////////////////////////
// FDevMenuItemCategoryViewModel

class FDevMenuItemCategoryViewModel : public FDevMenuItemViewModel
{
public:
	FDevMenuItemCategoryViewModel(TSharedPtr<FUnrealDevMenuEditor> InEditor,
	                              FText                            InCategory);

	virtual void GetFilterStrings(TArray<FString>& OutString) const override;
	TSharedRef<ITableRow> BuildRow(
	    const TSharedRef<STableViewBase>& OwnerTable) override;
	virtual void GetChildren(
	    TArray<TSharedPtr<FDevMenuItemViewModel>>& OutChildren) override;

protected:
	TWeakPtr<FUnrealDevMenuEditor>            Editor;
	FText                                     Category;
	TArray<TSharedPtr<FDevMenuItemViewModel>> ChildViewModels;
};

//////////////////////////////////////////////////////////////////////////
// FDevMenuItemCategoryViewModel

class FDevMenuItemClassViewModel : public FDevMenuItemViewModel
{
public:
	FDevMenuItemClassViewModel(const FGraphNodeClassData& InClassData);

	virtual void GetFilterStrings(TArray<FString>& OutString) const override;
	TSharedRef<ITableRow> BuildRow(
	    const TSharedRef<STableViewBase>& OwnerTable) override;
	virtual void GetChildren(
	    TArray<TSharedPtr<FDevMenuItemViewModel>>& OutChildren) override;

protected:
	FGraphNodeClassData ClassData;
};