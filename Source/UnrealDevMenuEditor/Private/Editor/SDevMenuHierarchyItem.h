// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class FDevMenuHierarchyModel;

class SDevMenuHierarchyViewItem
    : public STableRow<TSharedPtr<FDevMenuHierarchyModel>>
{
	using SuperType = STableRow<TSharedPtr<FDevMenuHierarchyModel>>;
public:
	SLATE_BEGIN_ARGS(SDevMenuHierarchyViewItem) {}

	/** The current text to highlight */
	SLATE_ATTRIBUTE(FText, HighlightText)

	SLATE_END_ARGS()

public:
	void Construct(const FArguments&                  InArgs,
	               const TSharedRef<STableViewBase>&  InOwnerTableView,
	               TSharedPtr<FDevMenuHierarchyModel> InModel);
	virtual ~SDevMenuHierarchyViewItem();

private:
	FText GetMenuName() const;

private:
	TSharedPtr<FDevMenuHierarchyModel> Model;
};