// Copyright Epic Games, Inc. All Rights Reserved.

#include "SDevMenuHierarchyItem.h"
#include "DevMenuHierarchyModel.h"

void SDevMenuHierarchyViewItem::Construct(
    const FArguments&                  InArgs,
    const TSharedRef<STableViewBase>&  InOwnerTableView,
    TSharedPtr<FDevMenuHierarchyModel> InModel)
{
	Model = InModel;

	// clang-format off
	SuperType::Construct(SuperType::FArguments()
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

FText SDevMenuHierarchyViewItem::GetMenuName() const
{
	return Model.IsValid() ? Model->GetMenuName() : FText::GetEmpty();
}