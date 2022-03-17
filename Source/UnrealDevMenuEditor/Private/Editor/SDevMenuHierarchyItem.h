// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class FUnrealDevMenuEditor;
class FDevMenuHierarchyModel;

class SDevMenuHierarchyViewItem
    : public STableRow<TSharedPtr<FDevMenuHierarchyModel>>
{
	using SuperType     = STableRow<TSharedPtr<FDevMenuHierarchyModel>>;
	using FTreeViewItem = TSharedPtr<FDevMenuHierarchyModel>;
public:
	SLATE_BEGIN_ARGS(SDevMenuHierarchyViewItem) {}

	/** The current text to highlight */
	SLATE_ATTRIBUTE(FText, HighlightText)

	SLATE_END_ARGS()

public:
	void Construct(const FArguments&                       InArgs,
	               const TSharedPtr<FUnrealDevMenuEditor>& InEditor,
	               const TSharedRef<STableViewBase>&       InOwnerTableView,
	               TSharedPtr<FDevMenuHierarchyModel>      InModel);
	virtual ~SDevMenuHierarchyViewItem();

	// ドラッグを検出したときの処理
	FReply HandleDragDetected(const FGeometry&     MyGeometry,
	                          const FPointerEvent& MouseEvent);
	// ドラッグ開始時
	virtual void HandleDragEnter(const FDragDropEvent& DragDropEvent);
	// ドラック終了時
	virtual void HandleDragLeave(const FDragDropEvent& DragDropEvent);
	// ドラックを受け入れる事が出来るか？
	virtual TOptional<EItemDropZone> HandleCanAcceptDrop(
	    const FDragDropEvent& DragDropEvent,
	    EItemDropZone         DropZone,
	    FTreeViewItem         Item);

	// ドロップを受け入れ
	virtual FReply HandleAcceptDrop(const FDragDropEvent& DragDropEvent,
	                                EItemDropZone         DropZone,
	                                FTreeViewItem         Item);
private:
	FText GetMenuName() const;

private:
	TWeakPtr<FUnrealDevMenuEditor>     Editor;
	TSharedPtr<FDevMenuHierarchyModel> Model;
};