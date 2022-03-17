#include "SDevMenuHierarchy.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Layout/SScrollBorder.h"
#include "SDevMenuHierarchyItem.h"
#include "DevMenuHierarchyModel.h"
#include "UnrealDevMenuEditor.h"

#define LOCTEXT_NAMESPACE "SDevMenuHierarchyView"

SDevMenuHierarchyView ::~SDevMenuHierarchyView()
{
	Editor.Pin()->OnChangedMenu.Remove(ChangedMenuHandle);
}

void SDevMenuHierarchyView::Construct(const FArguments&                InArgs,
                                      TSharedPtr<FUnrealDevMenuEditor> InEditor)
{
	OnChangeHierarchyItem = InArgs._OnChangeHierarchyItem;

	Editor            = InEditor.ToSharedRef();
	ChangedMenuHandle = Editor.Pin()->OnChangedMenu.AddRaw(
	    this, &SDevMenuHierarchyView::OnChangedMenu);

	SearchBoxMenuFilter = MakeShareable(
	    new FMenuTextFilter(FMenuTextFilter::FItemToStringArray::CreateSP(
	        this, &SDevMenuHierarchyView::GetFilterStrings)));

	FilterHandler = MakeShareable(new TreeFilterHandler<FTreeViewItem>());
	FilterHandler->SetFilter(SearchBoxMenuFilter.Get());
	FilterHandler->SetRootItems(&RootMenus, &TreeRootMenus);
	FilterHandler->SetGetChildrenDelegate(
	    TreeFilterHandler<FTreeViewItem>::FOnGetChildren::CreateRaw(
	        this, &SDevMenuHierarchyView::OnGetChildren));

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

	bRefreshTreeRequested = true;
}

// Begin SWidget

void SDevMenuHierarchyView::Tick(const FGeometry& AllottedGeometry,
                                 const double     InCurrentTime,
                                 const float      InDeltaTime)
{
	if ( bRebuildTreeRequested || bRefreshTreeRequested )
	{
		if ( bRebuildTreeRequested )
		{
			ReBuildTreeView();
		}
		RefreshTreeView();

        UpdateExpansionRecursive();

		bRebuildTreeRequested = false;
		bRefreshTreeRequested = false;
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

// メニューが変更された
void SDevMenuHierarchyView::OnChangedMenu()
{
	bRefreshTreeRequested = true;
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
	    SNew(STreeView<FTreeViewItem>)
	        .ItemHeight(20.f)
	        .SelectionMode(ESelectionMode::Single)
	        .OnGetChildren(FilterHandler.ToSharedRef(),
	                       &TreeFilterHandler<FTreeViewItem>::OnGetFilteredChildren)
	        .OnGenerateRow(this, &SDevMenuHierarchyView::OnGenerateRow)
	        .OnSelectionChanged(this, &SDevMenuHierarchyView::OnSelectionChanged)
	        .OnSetExpansionRecursive(this,
	                                 &SDevMenuHierarchyView::OnSetExpansionRecursive)
	        .TreeItemsSource(&TreeRootMenus);

	FilterHandler->SetTreeView(MenuTreeView.Get());

	TreeViewArea->SetContent(
	    SNew(SScrollBorder, MenuTreeView.ToSharedRef())[MenuTreeView.ToSharedRef()]);

	// Restore the previous scroll offset
	MenuTreeView->SetScrollOffset(OldScrollOffset);
}

TSharedRef<ITableRow> SDevMenuHierarchyView::OnGenerateRow(
    FTreeViewItem                     InItem,
    const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(SDevMenuHierarchyViewItem, Editor.Pin(), OwnerTable, InItem)
	    //.HighlightText(this, &SHierarchyView::GetSearchText)
	    ;
}

void SDevMenuHierarchyView::OnGetChildren(FTreeViewItem          InParent,
                                          TArray<FTreeViewItem>& OutChildren)
{
	InParent->GatherChildren(OutChildren);
}

void SDevMenuHierarchyView::OnSelectionChanged(FTreeViewItem     SelectedItem,
                                               ESelectInfo::Type SelectInfo)
{
	if ( SelectedItem.IsValid() )
	{
		// 変更通知
		OnChangeHierarchyItem.ExecuteIfBound(SelectedItem->GetObject());
	}
}

// 展開状態が設定された時
void SDevMenuHierarchyView::OnSetExpansionRecursive(FTreeViewItem Item,
                                                    bool          bInExpansionState)
{
	Item->SetExpansion(bRebuildTreeRequested);
}

void SDevMenuHierarchyView::GetFilterStrings(FTreeViewItem    Item,
                                             TArray<FString>& OutString)
{
	Item->GetFilterStrings(OutString);
}

// 展開状態を更新する
void SDevMenuHierarchyView::UpdateExpansionRecursive()
{
	for ( auto& Item : RootMenus )
	{
		UpdateExpansionRecursive(Item);
	}
}

// 展開状態を更新する
void SDevMenuHierarchyView::UpdateExpansionRecursive(const FTreeViewItem& Item)
{
	// メニュー側に現在の展開状態を設定する
	MenuTreeView->SetItemExpansion(Item, Item->IsExpansion());

    // 小階層の展開状態を再起ループで設定する
	TArray < FTreeViewItem > Children;
	Item->GatherChildren(Children);

	for ( auto& ChildModel : Children )
	{
		UpdateExpansionRecursive(ChildModel);
	}
}

void SDevMenuHierarchyView::OnSearchChanged(const FText& InFilterText)
{
	bRefreshTreeRequested            = true;
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
