#include "SDevMenuHierarchy.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Layout/SScrollBorder.h"
#include "SDevMenuHierarchyItem.h"
#include "DevMenuHierarchyModel.h"

#define LOCTEXT_NAMESPACE "SDevMenuHierarchyView"

void SDevMenuHierarchyView::Construct(const FArguments&                InArgs,
                                      TSharedPtr<FUnrealDevMenuEditor> InEditor)
{
	OnChangeHierarchyItem = InArgs._OnChangeHierarchyItem;

	Editor = InEditor.ToSharedRef();

	SearchBoxMenuFilter = MakeShareable(
	    new FMenuTextFilter(FMenuTextFilter::FItemToStringArray::CreateSP(
	        this, &SDevMenuHierarchyView::GetFilterStrings)));

	FilterHandler = MakeShareable(new TreeFilterHandler<FTreeViewItem>());
	FilterHandler->SetFilter(SearchBoxMenuFilter.Get());
	FilterHandler->SetRootItems(&RootMenus, &TreeRootMenus);
	FilterHandler->SetGetChildrenDelegate(
	    TreeFilterHandler<FTreeViewItem>::FOnGetChildren::CreateRaw(
	        this, &SDevMenuHierarchyView::Hierarchy_OnGetChildren));

	// clang-format off
    ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.Padding(4)
			.AutoHeight()
			[
                // サーチボックス
				SAssignNew(SearchBox, SSearchBox)
				.HintText(LOCTEXT("SearchWidgets", "Search Menu"))
				.OnTextChanged(this, &SDevMenuHierarchyView::OnSearchChanged)
			]

			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SAssignNew(TreeViewArea, SBorder)
				.Padding(0)
				.BorderImage( FEditorStyle::GetBrush( "NoBrush" ) )
			]
		]
	];
	// clang-format on

	ReBuildTreeView();

	RefreshTreeView();
}

// Begin SWidget

void SDevMenuHierarchyView::Tick(const FGeometry& AllottedGeometry,
                                 const double     InCurrentTime,
                                 const float      InDeltaTime)
{
	if ( bRebuildTreeRequested )
	{
		RefreshTreeView();

		bRebuildTreeRequested = false;
	}
}

// End SWidget

// ツリービューをリフレッシュする
void SDevMenuHierarchyView::RefreshTreeView()
{
	RootMenus.Empty();
	RootMenus.Add(
	    MakeShareable(new FDevMenuHierarchyRoot(Editor.Pin()->GetDevMenuEdited())));

	FilterHandler->RefreshAndFilterTree();
}

// ツリービューを再構築する
void SDevMenuHierarchyView::ReBuildTreeView()
{
	float OldScrollOffset = 0.0f;

	if ( MenuTreeView.IsValid() )
	{
		OldScrollOffset = MenuTreeView->GetScrollOffset();
	}

	//
	MenuTreeView =
	    SNew(STreeView<TSharedPtr<FDevMenuHierarchyModel>>)
	        .ItemHeight(20.f)
	        .SelectionMode(ESelectionMode::Single)
	        .OnGetChildren(
	            FilterHandler.ToSharedRef(),
	            &TreeFilterHandler<
	                TSharedPtr<FDevMenuHierarchyModel>>::OnGetFilteredChildren)
	        .OnGenerateRow(this, &SDevMenuHierarchyView::MenuHierarchy_OnGenerateRow)
	        .OnSelectionChanged(this,
	                            &SDevMenuHierarchyView::Hierarchy_OnSelectionChanged)
	        .TreeItemsSource(&TreeRootMenus);

	FilterHandler->SetTreeView(MenuTreeView.Get());

	TreeViewArea->SetContent(
	    SNew(SScrollBorder, MenuTreeView.ToSharedRef())[MenuTreeView.ToSharedRef()]);

	// Restore the previous scroll offset
	MenuTreeView->SetScrollOffset(OldScrollOffset);
}

TSharedRef<ITableRow> SDevMenuHierarchyView::MenuHierarchy_OnGenerateRow(
    TSharedPtr<FDevMenuHierarchyModel> InItem,
    const TSharedRef<STableViewBase>&  OwnerTable)
{
	return SNew(SDevMenuHierarchyViewItem, OwnerTable, InItem)
	    //.HighlightText(this, &SHierarchyView::GetSearchText)
	    ;
}

void SDevMenuHierarchyView::Hierarchy_OnGetChildren(
    FTreeViewItem          InParent,
    TArray<FTreeViewItem>& OutChildren)
{
	InParent->GatherChildren(OutChildren);
}

void SDevMenuHierarchyView::Hierarchy_OnSelectionChanged(
    FTreeViewItem     SelectedItem,
    ESelectInfo::Type SelectInfo)
{
	if ( SelectedItem.IsValid() )
	{
		// 変更通知
		OnChangeHierarchyItem.ExecuteIfBound(SelectedItem->GetObject());
	}
}

void SDevMenuHierarchyView::GetFilterStrings(FTreeViewItem    Item,
                                             TArray<FString>& OutString)
{
	Item->GetFilterStrings(OutString);
}

void SDevMenuHierarchyView::OnSearchChanged(const FText& InFilterText)
{
	bRebuildTreeRequested        = true;
	const bool bFilteringEnabled = !InFilterText.IsEmpty();
	if ( bFilteringEnabled != FilterHandler->GetIsEnabled() )
	{
		// フィルターの状態変更
		FilterHandler->SetIsEnabled(bFilteringEnabled);
	}
	SearchBoxMenuFilter->SetRawFilterText(InFilterText);
	SearchBox->SetError(SearchBoxMenuFilter->GetFilterErrorText());
}

#undef LOCTEXT_NAMESPACE
