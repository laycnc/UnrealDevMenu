#include "MenuHierarchyDragDropOp.h"
#include "DevMenuItem/DevMenuItemBase.h"

#define LOCTEXT_NAMESPACE "FMenuHierarchyDragDropOp"

FMenuHierarchyDragDropOp::~FMenuHierarchyDragDropOp()
{
	if ( Transaction )
	{
		delete Transaction;
	}
	Transaction = nullptr;
}

TSharedRef<FMenuHierarchyDragDropOp> FMenuHierarchyDragDropOp::New(
    const TArray<TWeakObjectPtr<UDevMenuItemBase>>& InItems)
{
	TSharedRef<FMenuHierarchyDragDropOp> Operation =
	    MakeShareable(new FMenuHierarchyDragDropOp());
	Operation->Items = InItems;

	if ( InItems.Num() == 1 )
	{
		Operation->CurrentHoverText = Operation->DefaultHoverText =
		    FText::FromString(InItems[0]->GetName());
		Operation->Transaction =
		    new FScopedTransaction(LOCTEXT("Designer_MoveItem", "Move Item"));
	}
	else
	{
		Operation->CurrentHoverText = Operation->DefaultHoverText =
		    LOCTEXT("Designer_DragMultipleWidgets", "Multiple Items");
		Operation->Transaction =
		    new FScopedTransaction(LOCTEXT("Designer_MoveItems", "Move Items"));
	}

	Operation->DefaultHoverText = Operation->CurrentHoverText;
	Operation->Construct();

	return Operation;
}

#undef LOCTEXT_NAMESPACE