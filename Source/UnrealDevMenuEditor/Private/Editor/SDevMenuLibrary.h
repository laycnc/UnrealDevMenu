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
class FDevMenuItemViewModel;

/**
 * デバッグメニューの構成アイテムライブラリ画面
 * 
 * 配置出来る要素を表示する画面
 */
class SDevMenuLibraryView : public SCompoundWidget
{
	using FTreeViewItem   = TSharedPtr<FDevMenuItemViewModel>;
	using FMenuTextFilter = TTextFilter<FTreeViewItem>;

public:
	SLATE_BEGIN_ARGS(SDevMenuLibraryView) {}
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
	void RefreshTreeView();

	// ツリーの構築を取得する
	void OnGetChildren(FTreeViewItem InParent, TArray<FTreeViewItem>& OutChildren);
	void OnSelectionChanged(FTreeViewItem     SelectedItem,
	                        ESelectInfo::Type SelectInfo);

	TSharedRef<ITableRow> OnGenerateRow(
	    FTreeViewItem                     InItem,
	    const TSharedRef<STableViewBase>& OwnerTable);

	// 検索ボックスの値を変更した時
	void OnSearchChanged(const FText& InFilterText);
	// フィルタイング用のテキストを取得する
	void GetFilterStrings(FTreeViewItem Item, TArray<FString>& OutString);
private:
	TArray<FTreeViewItem> TreeRootMenus;

private:
	TWeakPtr<FUnrealDevMenuEditor>               Editor;
	TSharedPtr<SSearchBox>                       SearchBox;
	TSharedPtr<FMenuTextFilter>                  SearchBoxMenuFilter;
	TSharedPtr<SBorder>                          ClassViewArea;
	TSharedPtr<STreeView<FTreeViewItem>>         MenuTreeView;
	TSharedPtr<TreeFilterHandler<FTreeViewItem>> FilterHandler;

private:
	// ツリーの再構成リクエスト
	bool bRebuildTreeRequested = false;
};