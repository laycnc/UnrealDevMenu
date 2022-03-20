// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Input/DragAndDrop.h"
#include "DragAndDrop/DecoratedDragDropOp.h"
#include "AIGraphTypes.h"

class FMenuHierarchyDragDropOp : public FDecoratedDragDropOp
{
public:
	DRAG_DROP_OPERATOR_TYPE(FMenuHierarchyDragDropOp, FDecoratedDragDropOp)

	TArray<TWeakObjectPtr<UDevMenuItemBase>> Items;

	/** The widget being dragged and dropped */
	FScopedTransaction* Transaction = nullptr;

	virtual ~FMenuHierarchyDragDropOp();
	static TSharedRef<FMenuHierarchyDragDropOp> New(
	    const TArray<TWeakObjectPtr<UDevMenuItemBase>>& InItems);
};
