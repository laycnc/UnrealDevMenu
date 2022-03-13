// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/STableViewBase.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Views/STreeView.h"
#include "Misc/TextFilter.h"
#include "Framework/Views/TreeFilterHandler.h"

class SBorder;
class SSearchBox;
class FMenuBuilder;
class FUnrealDevMenuEditor;
class FDevMenuHierarchyModel;

class SDevMenuHierarchyView : public SCompoundWidget
{
public:
	DECLARE_DELEGATE_OneParam(FOnChangeHierarchyItem, UObject*);

	using FTreeViewItem   = TSharedPtr<FDevMenuHierarchyModel>;
	using FMenuTextFilter = TTextFilter<FTreeViewItem>;

public:
	SLATE_BEGIN_ARGS(SDevMenuHierarchyView)
	    : _OnChangeHierarchyItem()
	{
	}
	SLATE_EVENT(FOnChangeHierarchyItem, OnChangeHierarchyItem)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments&                InArgs,
	               TSharedPtr<FUnrealDevMenuEditor> InEditor);
	// Begin SWidget
	virtual void Tick(const FGeometry& AllottedGeometry,
	                  const double     InCurrentTime,
	                  const float      InDeltaTime) override;
	// End SWidget

private:
	// ツリービューを再構築する
	void ReBuildTreeView();
	// ツリービューをリフレッシュする
	void                  RefreshTreeView();
	void                  OnSearchChanged(const FText& InFilterText);
	TSharedRef<ITableRow> OnGenerateRow(
	    TSharedPtr<FDevMenuHierarchyModel> InItem,
	    const TSharedRef<STableViewBase>&  OwnerTable);
	void OnGetChildren(FTreeViewItem InParent, TArray<FTreeViewItem>& OutChildren);
	void OnSelectionChanged(FTreeViewItem     SelectedItem,
	                        ESelectInfo::Type SelectInfo);
	void GetFilterStrings(FTreeViewItem Item, TArray<FString>& OutString);
private:
	TArray<FTreeViewItem> RootMenus;
	TArray<FTreeViewItem> TreeRootMenus;

private:
	TWeakPtr<FUnrealDevMenuEditor>               Editor;
	TSharedPtr<SSearchBox>                       SearchBox;
	TSharedPtr<FMenuTextFilter>                  SearchBoxMenuFilter;
	TSharedPtr<SBorder>                          TreeViewArea;
	TSharedPtr<STreeView<FTreeViewItem>>         MenuTreeView;
	TSharedPtr<TreeFilterHandler<FTreeViewItem>> FilterHandler;
	FOnChangeHierarchyItem                       OnChangeHierarchyItem;
private:
	// ツリーの再構成リクエスト
	bool bRebuildTreeRequested = false;
};
