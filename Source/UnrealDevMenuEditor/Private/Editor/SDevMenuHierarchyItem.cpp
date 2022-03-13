// Copyright Epic Games, Inc. All Rights Reserved.

#include "SDevMenuHierarchyItem.h"
#include "DevMenuHierarchyModel.h"
#include "DragDrop/MenuTemplateDragDropOp.h"

#define LOCTEXT_NAMESPACE "SDevMenuHierarchyViewItem"

void SDevMenuHierarchyViewItem::Construct(
    const FArguments&                  InArgs,
    const TSharedRef<STableViewBase>&  InOwnerTableView,
    TSharedPtr<FDevMenuHierarchyModel> InModel)
{
	Model = InModel;

	// clang-format off
	SuperType::Construct(SuperType::FArguments()
		.OnCanAcceptDrop(this, &SDevMenuHierarchyViewItem::HandleCanAcceptDrop)
		.OnAcceptDrop(this, &SDevMenuHierarchyViewItem::HandleAcceptDrop)
		//.OnDragDetected(this, &SDevMenuHierarchyViewItem::HandleDragDetected)
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
			return FReply::Handled();
		}
	}

	return FReply::Unhandled();
}

FText SDevMenuHierarchyViewItem::GetMenuName() const
{
	return Model.IsValid() ? Model->GetMenuName() : FText::GetEmpty();
}

#undef LOCTEXT_NAMESPACE
