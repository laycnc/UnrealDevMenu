// Copyright Epic Games, Inc. All Rights Reserved.
#include "SDevMenuLibrary.h"

#define LOCTEXT_NAMESPACE "SDevMenuLibraryView"

//////////////////////////////////////////////////////////////////////////
// SDevMenuLibraryView

void SDevMenuLibraryView::Construct(const FArguments&                InArgs,
                                    TSharedPtr<FUnrealDevMenuEditor> InEditor)
{
	Editor = InEditor;

	SearchBoxMenuFilter = MakeShareable(
	    new FMenuTextFilter(FMenuTextFilter::FItemToStringArray::CreateSP(
	        this, &SDevMenuLibraryView::GetFilterStrings)));

	FilterHandler = MakeShareable(new TreeFilterHandler<FTreeViewItem>());
	FilterHandler->SetFilter(SearchBoxMenuFilter.Get());
	FilterHandler->SetRootItems(&InEditor->GetMenuItemViewModel(), &TreeRootMenus);
	FilterHandler->SetGetChildrenDelegate(
	    TreeFilterHandler<FTreeViewItem>::FOnGetChildren::CreateRaw(
	        this, &SDevMenuLibraryView::OnGetChildren));

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
				.OnTextChanged(this, &SDevMenuLibraryView::OnSearchChanged)
			]

			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SAssignNew(ClassViewArea, SBorder)
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
void SDevMenuLibraryView::Tick(const FGeometry& AllottedGeometry,
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

// ツリービューを再構築する
void SDevMenuLibraryView::ReBuildTreeView()
{
	float OldScrollOffset = 0.0f;

	if ( MenuTreeView.IsValid() )
	{
		OldScrollOffset = MenuTreeView->GetScrollOffset();
	}

	//
	MenuTreeView =
	    SNew(STreeView<FTreeViewItem>)
	        .ItemHeight(20.f)
	        .SelectionMode(ESelectionMode::Single)
	        .OnGetChildren(FilterHandler.ToSharedRef(),
	                       &TreeFilterHandler<FTreeViewItem>::OnGetFilteredChildren)
	        .OnGenerateRow(this, &SDevMenuLibraryView::OnGenerateRow)
	        .OnSelectionChanged(this, &SDevMenuLibraryView::OnSelectionChanged)
	        .TreeItemsSource(&TreeRootMenus);

	FilterHandler->SetTreeView(MenuTreeView.Get());

	ClassViewArea->SetContent(
	    SNew(SScrollBorder, MenuTreeView.ToSharedRef())[MenuTreeView.ToSharedRef()]);

	// Restore the previous scroll offset
	MenuTreeView->SetScrollOffset(OldScrollOffset);
}

// ツリービューをリフレッシュする
void SDevMenuLibraryView::RefreshTreeView()
{
	FilterHandler->RefreshAndFilterTree();
	bRebuildTreeRequested = false;
}

// ツリーの構築を取得する
void SDevMenuLibraryView::OnGetChildren(FTreeViewItem          InParent,
                                        TArray<FTreeViewItem>& OutChildren)
{
	InParent->GetChildren(OutChildren);
}

void SDevMenuLibraryView::OnSelectionChanged(FTreeViewItem     SelectedItem,
                                             ESelectInfo::Type SelectInfo)
{
}

TSharedRef<ITableRow> SDevMenuLibraryView::OnGenerateRow(
    FTreeViewItem                     InItem,
    const TSharedRef<STableViewBase>& OwnerTable)
{
	return InItem->BuildRow(OwnerTable);
}

// 検索ボックスの値を変更した時
void SDevMenuLibraryView::OnSearchChanged(const FText& InFilterText)
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

// フィルタイング用のテキストを取得する
void SDevMenuLibraryView::GetFilterStrings(FTreeViewItem    Item,
                                           TArray<FString>& OutString)
{
	Item->GetFilterStrings(OutString);
}

#undef LOCTEXT_NAMESPACE
