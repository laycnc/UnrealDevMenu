#include "MenuTemplateDragDropOp.h"

TSharedRef<FMenuTemplateDragDropOp> FMenuTemplateDragDropOp::New(
    const FGraphNodeClassData& InClassData)
{
	TSharedRef<FMenuTemplateDragDropOp> Operation =
	    MakeShareable(new FMenuTemplateDragDropOp());
	Operation->ClassData = InClassData;

	const UClass* Class = Operation->ClassData.GetClass();
	const FText   ClassName =
        IsValid(Class) ? FText::FromString(Class->GetName()) : FText::GetEmpty();

	Operation->DefaultHoverText = ClassName;
	Operation->CurrentHoverText = ClassName;
	Operation->Construct();

	return Operation;
}
