// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Input/DragAndDrop.h"
#include "DragAndDrop/DecoratedDragDropOp.h"
#include "AIGraphTypes.h"

class FMenuTemplateDragDropOp : public FDecoratedDragDropOp
{
public:
	DRAG_DROP_OPERATOR_TYPE(FMenuTemplateDragDropOp, FDecoratedDragDropOp)

	FGraphNodeClassData ClassData;

	static TSharedRef<FMenuTemplateDragDropOp> New(const FGraphNodeClassData& InClassData);
};
