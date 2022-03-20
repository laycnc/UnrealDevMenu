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
		if ( Item->CanInsertChildItem() )
		{
			HierarchyDragDropOp->CurrentIconBrush =
			    FEditorStyle::GetBrush(TEXT("Graph.ConnectorFeedback.OK"));
			return EItemDropZone::OntoItem;
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
		UObject* Obj = Item->GetObject();

		switch ( DropZone )
		{
			case EItemDropZone::AboveItem:
				HierarchyDragDropOp->CurrentIconBrush =
				    FEditorStyle::GetBrush(TEXT("Graph.ConnectorFeedback.ShowNode"));
				break;
			case EItemDropZone::OntoItem:
				if ( Item->CanInsertChildItem() )
				{
					// 挿入可能なのでOK扱い
					HierarchyDragDropOp->CurrentIconBrush =
					    FEditorStyle::GetBrush(TEXT("Graph.ConnectorFeedback.OK"));
					return EItemDropZone::OntoItem;
				}
				break;
			case EItemDropZone::BelowItem:
				HierarchyDragDropOp->CurrentIconBrush =
				    FEditorStyle::GetBrush(TEXT("Graph.ConnectorFeedback.NewNode"));
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
	if ( TSharedPtr<FMenuTemplateDragDropOp> HierarchyDragDropOp =
	         DragDropEvent.GetOperationAs<FMenuTemplateDragDropOp>() )
	{
		UClass* GeneratedClass = HierarchyDragDropOp->ClassData.GetClass();
		if ( Item->AddNewMenuItem(GeneratedClass) )
		{
			Editor.Pin()->OnChangedMenu.Broadcast();

			return FReply::Handled();
		}
	}

	if ( TSharedPtr<FMenuHierarchyDragDropOp> HierarchyDragDropOp =
	         DragDropEvent.GetOperationAs<FMenuHierarchyDragDropOp>() )
	{
		UDevMenu* DevMenu = Editor.Pin()->GetDevMenuEdited();
		TArray<TWeakObjectPtr<UDevMenuItemBase>> Items = HierarchyDragDropOp->Items;
		for ( auto& WeakMoveItem : Items )
		{
			if ( WeakMoveItem.IsValid() )
			{
				UDevMenuItemBase* MoveItem = WeakMoveItem.Get();
				DevMenu->RemoveMenuItem(MoveItem);
				Item->InsertNewMenuItem(MoveItem, -1);
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
