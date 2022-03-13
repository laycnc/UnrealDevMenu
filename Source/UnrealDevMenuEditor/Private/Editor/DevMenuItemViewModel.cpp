// Copyright Epic Games, Inc. All Rights Reserved.
#include "DevMenuItemViewModel.h"
#include "UnrealDevMenuEditorModule.h"
#include "AIGraphTypes.h"
#include "DragDrop/MenuTemplateDragDropOp.h"
#include "IDocumentation.h"

#define LOCTEXT_NAMESPACE "DevMenuItemViewModel"

//////////////////////////////////////////////////////////////////////////
// FDevMenuItemViewModel

FDevMenuItemViewModel::FDevMenuItemViewModel() {}

void FDevMenuItemViewModel::GetChildren(
    TArray<TSharedPtr<FDevMenuItemViewModel>>& OutChildren)
{
	(void)OutChildren;
}

//////////////////////////////////////////////////////////////////////////
//

FDevMenuItemCategoryViewModel::FDevMenuItemCategoryViewModel(
    TSharedPtr<FUnrealDevMenuEditor> InEditor,
    FText                            InCategory)
    : FDevMenuItemViewModel()
    , Editor(InEditor)
    , Category(InCategory)
{
}

void FDevMenuItemCategoryViewModel::GetFilterStrings(
    TArray<FString>& OutString) const
{
	OutString.Add(Category.ToString());
}

TSharedRef<ITableRow> FDevMenuItemCategoryViewModel::BuildRow(
    const TSharedRef<STableViewBase>& OwnerTable)
{
	const FText CategoryName =
	    Category.IsEmpty() ? LOCTEXT("DefaultCategory", "default") : Category;

	// clang-format off
	return SNew(STableRow<TSharedPtr<FDevMenuItemViewModel>>, OwnerTable)
		.Padding(5.0f)
		.ShowSelection(false)
	    .Style(FEditorStyle::Get(), "UMGEditor.PaletteHeader")
        [
			SNew(STextBlock)
			.TransformPolicy(ETextTransformPolicy::ToUpper)
			.Text(CategoryName)
			.Font(FAppStyle::Get().GetFontStyle("SmallFontBold"))
        ];
	// clang-format on
}

void FDevMenuItemCategoryViewModel::GetChildren(
    TArray<TSharedPtr<FDevMenuItemViewModel>>& OutChildren)
{
	if ( ChildViewModels.Num() == 0 )
	{
		IUnrealDevMenuEditorModule& Module =
		    FModuleManager::GetModuleChecked<IUnrealDevMenuEditorModule>(
		        TEXT("UnrealDevMenuEditor"));
		TSharedRef<FGraphNodeClassHelper> ClassCache =
		    Module.GetMenuItemClassCache();
		TArray<FGraphNodeClassData> NodeClasses;
		ClassCache->GatherClasses(UDevMenuItemBase::StaticClass(), NodeClasses);

		TArray<FText> Categorys;

		// カテゴリ情報を収集する
		for ( FGraphNodeClassData& Data : NodeClasses )
		{
			if ( Category.ToString() != Data.GetCategory().ToString() )
			{
				continue;
			}

			OutChildren.Add(MakeShareable(new FDevMenuItemClassViewModel(Data)));
		}
	}

	OutChildren.Append(ChildViewModels);
}

//////////////////////////////////////////////////////////////////////////
// FDevMenuItemClassViewModel

FDevMenuItemClassViewModel::FDevMenuItemClassViewModel(
    const FGraphNodeClassData& InClassData)
    : FDevMenuItemViewModel()
    , ClassData(InClassData)
{
}

void FDevMenuItemClassViewModel::GetFilterStrings(TArray<FString>& OutString) const
{
	OutString.Add(ClassData.GetPackageName());
}

TSharedRef<ITableRow> FDevMenuItemClassViewModel::BuildRow(
    const TSharedRef<STableViewBase>& OwnerTable)
{
	const UClass* Class = ClassData.GetClass();
	const FText   ClassName =
        IsValid(Class) ? FText::FromString(Class->GetName()) : FText::GetEmpty();
	const FText Description = Class->GetToolTipText();

	// clang-format off
	return SNew(STableRow<TSharedPtr<FDevMenuItemViewModel>>, OwnerTable)
	    .Padding(5.0f)
	    .OnDragDetected(this, &FDevMenuItemClassViewModel::OnDraggingWidgetTemplateItem)
		.ToolTip(IDocumentation::Get()->CreateToolTip(Description, nullptr, TEXT(""), TEXT("")))
	    .ShowSelection(false)
        [
            SNew(STextBlock)
            .Text(ClassName)
            .Font(FAppStyle::Get().GetFontStyle("SmallFontBold"))
        ];
	// clang-format on
}

void FDevMenuItemClassViewModel::GetChildren(
    TArray<TSharedPtr<FDevMenuItemViewModel>>& OutChildren)
{
	static_cast<void>(OutChildren);
}

FReply FDevMenuItemClassViewModel::OnDraggingWidgetTemplateItem(
    const FGeometry&     MyGeometry,
    const FPointerEvent& MouseEvent)
{
	return FReply::Handled().BeginDragDrop(FMenuTemplateDragDropOp::New(ClassData));
}

#undef LOCTEXT_NAMESPACE
