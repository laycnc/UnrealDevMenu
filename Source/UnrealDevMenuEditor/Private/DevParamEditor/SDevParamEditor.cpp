#include "DevParamEditor/SDevParamEditor.h"
#include "Widgets/Input/SSearchBox.h"

SDevParamEditor::SDevParamEditor() {}

SDevParamEditor::~SDevParamEditor() {}

void SDevParamEditor::Construct(const FArguments&           InArgs,
                                TSharedRef<FDevParamEditor> InDevMenuEditor)
{

	// clang-format off
    ChildSlot
    [
        SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .AutoHeight()
        [
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SAssignNew(SearchBoxWidget, SSearchBox)
				//.InitialText(this, &SDevParamEditor::GetFilterText)
				//.OnTextChanged(this, &SDevParamEditor::OnFilterTextChanged)
				//.OnTextCommitted(this, &SDevParamEditor::OnFilterTextCommitted)
			]
        ]
    ];
	// clang-format on
}
