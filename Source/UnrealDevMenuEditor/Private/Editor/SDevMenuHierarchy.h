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
	~SDevMenuHierarchyView();

	void Construct(const FArguments&                InArgs,
	               TSharedPtr<FUnrealDevMenuEditor> InEditor);
	// Begin SWidget
	virtual void Tick(const FGeometry& AllottedGeometry,
	                  const double     InCurrentTime,
	                  const float      InDeltaTime) override;
	// End SWidget

private:
	// メニューが変更された
	void OnChangedMenu();

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

    // メニューコンテキスト対応
	TSharedPtr<SWidget> OnContextMenuOpening();

	// 展開状態が設定された時
	void OnSetExpansionRecursive(FTreeViewItem Item, bool bInExpansionState);
	void GetFilterStrings(FTreeViewItem Item, TArray<FString>& OutString);

	// 展開状態を更新する
	void UpdateExpansionRecursive();
	// 展開状態を更新する
	void UpdateExpansionRecursive(const FTreeViewItem& Item);
private:
	TArray<FTreeViewItem> RootMenus;
	TArray<FTreeViewItem> TreeRootMenus;

private:
	// エディター
	TWeakPtr<FUnrealDevMenuEditor> Editor;
	// コマンド
	TSharedPtr<FUICommandList>                   CommandList;
	TSharedPtr<SSearchBox>                       SearchBox;
	TSharedPtr<FMenuTextFilter>                  SearchBoxMenuFilter;
	TSharedPtr<SBorder>                          TreeViewArea;
	TSharedPtr<STreeView<FTreeViewItem>>         MenuTreeView;
	TSharedPtr<TreeFilterHandler<FTreeViewItem>> FilterHandler;
	FOnChangeHierarchyItem                       OnChangeHierarchyItem;
private:
	// ツリーの再構成リクエスト
	bool bRebuildTreeRequested = false;
	// ツリーのリフレッシュリクエスト
	bool bRefreshTreeRequested = false;
	// メニュー変更デリゲートハンドル
	FDelegateHandle ChangedMenuHandle;
};
