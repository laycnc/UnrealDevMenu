// Copyright Epic Games, Inc. All Rights Reserved.

#include "SDevMenuHierarchyItem.h"
#include "UnrealDevMenuEditor.h"
#include "DevMenuHierarchyModel.h"
#include "DragDrop/MenuTemplateDragDropOp.h"
#include "DragDrop/MenuHierarchyDragDropOp.h"
#include "DevMenu.h"

#define LOCTEXT_NAMESPACE "SDevMenuHierarchyViewItem"

void SDevMenuHierarchyViewItem::Construct(
    const FArguments&                       InArgs,
    const TSharedPtr<FUnrealDevMenuEditor>& InEditor,
    const TSharedRef<STableViewBase>&       InOwnerTableView,
    TSharedPtr<FDevMenuHierarchyModel>      InModel)
{
	Editor = InEditor;
	Model  = InModel;

	// clang-format off
	SuperType::Construct(SuperType::FArguments()
		.OnCanAcceptDrop(this, &SDevMenuHierarchyViewItem::HandleCanAcceptDrop)
		.OnAcceptDrop(this, &SDevMenuHierarchyViewItem::HandleAcceptDrop)
		.OnDragDetected(this, &SDevMenuHierarchyViewItem::HandleDragDetected)
		//.OnDragEnter(this, &SDevMenuHierarchyViewItem::HandleDragEnter)
		//.OnDragLeave(this, &SDevMenuHierarchyViewItem::HandleDragLeave)
        .Content()
        [
            SNew( SHorizontalBox )
            + SHorizontalBox::Slot()
            .AutoWidth()
            .VAlign( VAlign_Center )
            [
                SNew( STextBlock )
                .Text( this, &SDevMenuHierarchyViewItem::GetMenuName )
            ]
        ], 
        InOwnerTableView);
	// clang-format on
}

SDevMenuHierarchyViewItem::~SDevMenuHierarchyViewItem() {}

// ドラッグを検出したときの処理
FReply SDevMenuHierarchyViewItem::HandleDragDetected(const FGeometry&     MyGeometry,
                                                     const FPointerEvent& MouseEvent)
{
	if ( Model.IsValid() )
	{
		return Model->HandleDragDetected(MyGeometry, MouseEvent);
	}

	return FReply::Unhandled();
}

// ドラッグ開始時
void SDevMenuHierarchyViewItem::HandleDragEnter(const FDragDropEvent& DragDropEvent)
{
}

// ドラック終了時
void SDevMenuHierarchyViewItem::HandleDragLeave(const FDragDropEvent& DragDropEvent)
{
}

// ドラックを受け入れる事が出来るか？
TOptional<EItemDropZone> SDevMenuHierarchyViewItem::HandleCanAcceptDrop(
    const FDragDropEvent& DragDropEvent,
    EItemDropZone         DropZone,
    FTreeViewItem         Item)
{
	if ( TSharedPtr<FMenuTemplateDragDropOp> HierarchyDragDropOp =
	         DragDropEvent.GetOperationAs<FMenuTemplateDragDropOp>() )
	{
		HierarchyDragDropOp->ResetToDefaultToolTip();

		// メニュー作成テンプレートの場合
		if ( const auto* MenuItem = Item->GetItemInterface() )
		{
			if ( MenuItem->CanInsertChildItem() )
			{
				HierarchyDragDropOp->CurrentIconBrush =
				    FEditorStyle::GetBrush(TEXT("Graph.ConnectorFeedback.OK"));
				return EItemDropZone::OntoItem;
			}
		}

		// テンプレートを配置できない

		//エラーテキストを入れる
		HierarchyDragDropOp->CurrentHoverText = LOCTEXT(
		    "NoAdditionalChildren", "Menu can't accept additional children.");
		// エラーアイコンを入れる
		HierarchyDragDropOp->CurrentIconBrush =
		    FEditorStyle::GetBrush(TEXT("Graph.ConnectorFeedback.Error"));
	}

	if ( TSharedPtr<FMenuHierarchyDragDropOp> HierarchyDragDropOp =
	         DragDropEvent.GetOperationAs<FMenuHierarchyDragDropOp>() )
	{
#if 0
		if ( DropZone == EItemDropZone::OntoItem )
		{
			HierarchyDragDropOp->CurrentIconBrush =
			    FEditorStyle::GetBrush(TEXT("Graph.ConnectorFeedback.OK"));
		}
		else
		{
			//エラーテキストを入れる
			HierarchyDragDropOp->CurrentHoverText = LOCTEXT(
			    "NoAdditionalChildren", "Menu can't accept additional children.");
			// エラーアイコンを入れる
			HierarchyDragDropOp->CurrentIconBrush =
			    FEditorStyle::GetBrush(TEXT("Graph.ConnectorFeedback.Error"));
		}
#else
		auto* MenuItem = Item->GetItemInterface();

		switch ( DropZone )
		{
			case EItemDropZone::BelowItem:
			case EItemDropZone::AboveItem:
				HierarchyDragDropOp->CurrentIconBrush =
				    FEditorStyle::GetBrush(TEXT("Graph.ConnectorFeedback.OK"));
				return DropZone;
				break;
			case EItemDropZone::OntoItem:
				if ( MenuItem->CanInsertChildItem() )
				{
					// 挿入可能なのでOK扱い
					HierarchyDragDropOp->CurrentIconBrush =
					    FEditorStyle::GetBrush(TEXT("Graph.ConnectorFeedback.OK"));
					return EItemDropZone::OntoItem;
				}
				break;
			default:
				break;
		}

#endif

		return TOptional<EItemDropZone>();
	}

	return TOptional<EItemDropZone>();
}

// ドロップを受け入れ
FReply SDevMenuHierarchyViewItem::HandleAcceptDrop(
    const FDragDropEvent& DragDropEvent,
    EItemDropZone         DropZone,
    FTreeViewItem         Item)
{
	auto* MenuItem = Item->GetItemInterface();
	if ( MenuItem == nullptr )
	{
		return FReply::Unhandled();
	}

	if ( TSharedPtr<FMenuTemplateDragDropOp> HierarchyDragDropOp =
	         DragDropEvent.GetOperationAs<FMenuTemplateDragDropOp>() )
	{
		UClass* GeneratedClass = HierarchyDragDropOp->ClassData.GetClass();
		FScopedTransaction Transaction(LOCTEXT("AddItem", "Add Item"));
		if ( !MenuItem->AddNewMenuItem(GeneratedClass) )
		{
			Transaction.Cancel();
		}

		Editor.Pin()->OnChangedMenu.Broadcast();
		return FReply::Handled();
	}

	if ( TSharedPtr<FMenuHierarchyDragDropOp> HierarchyDragDropOp =
	         DragDropEvent.GetOperationAs<FMenuHierarchyDragDropOp>() )
	{
		UDevMenu* DevMenu = Editor.Pin()->GetDevMenuEdited();
		TArray<TWeakObjectPtr<UDevMenuItemBase>> Items = HierarchyDragDropOp->Items;

		// 挿入するアイテムの所属する座標を取得する
		int32 InsertIndex = MenuItem->GetPlacedIndex();

		switch ( DropZone )
		{
			case EItemDropZone::AboveItem:
				InsertIndex = InsertIndex;
				break;
			case EItemDropZone::BelowItem:
				InsertIndex = InsertIndex + 1;
				break;
		}

		for ( auto& WeakMoveItem : Items )
		{
			if ( WeakMoveItem.IsValid() )
			{
				UDevMenuItemBase* MoveItem = WeakMoveItem.Get();
				DevMenu->RemoveMenuItem(MoveItem);
				if ( EItemDropZone::OntoItem == DropZone )
				{
                    // 自身に挿入する場合にはそのまま挿入する
					MenuItem->InsertNewMenuItem(MoveItem, INDEX_NONE);
				}
				else
				{
                    // 上下に挿入する場合には親のIndexに対して行う
					if ( auto* ParentMenu = MenuItem->GetParentMenu() )
					{
						ParentMenu->InsertNewMenuItem(MoveItem, InsertIndex);
					}
				}
			}
		}
		Editor.Pin()->OnChangedMenu.Broadcast();
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FText SDevMenuHierarchyViewItem::GetMenuName() const
{
	return Model.IsValid() ? Model->GetMenuName() : FText::GetEmpty();
}

#undef LOCTEXT_NAMESPACE
