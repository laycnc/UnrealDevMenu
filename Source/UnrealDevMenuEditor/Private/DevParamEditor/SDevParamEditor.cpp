#include "DevParamEditor/SDevParamEditor.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "IDocumentation.h"
#include "ParamType/DevParamType.h"
#include "ParamType/DevParamDataAsset.h"
#include "DevParamEditor/DevParamEditor.h"
#include "JsonObjectConverter.h"
#include "Serialization/JsonSerializer.h"
#include "GameplayTagsManager.h"

#define LOCTEXT_NAMESPACE "SDevParamEditor"

const FName DevParamColumnDragDrop  = TEXT("[DevParamColumnDragDrop]");
const FName DevParamColumnClass     = TEXT("[DevParamTypeClass]");
const FName DevParamColumnExpansion = TEXT("[DefaultValueExtension]");

void SDevParamListViewRow::Construct(
    const FArguments&                 InArgs,
    const TSharedRef<STableViewBase>& InOwnerTableView)
{
	DevParamEditor = InArgs._DevParamEditor;
	RowData        = InArgs._RowDataPtr;

	SMultiColumnTableRow<FDevParamEditorRowListViewDataPtr>::Construct(
	    FSuperRowType::FArguments().Style(FEditorStyle::Get(),
	                                      "DataTableEditor.CellListViewRow")
	    //.OnDrop(this, &SDevParamListViewRow::OnRowDrop)
	    //.OnDragEnter(this, &SDevParamListViewRow::OnRowDragEnter)
	    //.OnDragLeave(this, &SDevParamListViewRow::OnRowDragLeave)
	    ,
	    InOwnerTableView);
}

TSharedRef<SWidget> SDevParamListViewRow::GenerateWidgetForColumn(
    const FName& ColumnName)
{
	TSharedPtr<FDevParamEditor> DevParamEditorPtr = DevParamEditor.Pin();
	return (DevParamEditorPtr.IsValid()) ? MakeCellWidget(IndexInList, ColumnName)
	                                     : SNullWidget::NullWidget;
}

TSharedRef<SWidget> SDevParamListViewRow::MakeCellWidget(const int32  InRowIndex,
                                                         const FName& InColumnId)
{
	if ( !RowData->ParamType.IsValid() )
	{
		return SNullWidget::NullWidget;
	}

	if ( InColumnId.IsEqual(DevParamColumnDragDrop) )
	{
		// SDataTableRowHandle
		// clang-format off
		return SNew(SBox)
		    .Content()
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .Padding( 5.0f, 1.0f)
                [
                    SNew(SImage)
                    .Image(FCoreStyle::Get().GetBrush("VerticalBoxDragIndicatorShort"))
                ]
            ]
		    //.ParentRow(SharedThis(this))
            ;
		// clang-format on
	}

	UDevParamType*   ParamType    = RowData->ParamType.Get();
	FDevParamEditor* DevParamEdit = DevParamEditor.Pin().Get();

	// デバッグパラメータタイプ
	if ( InColumnId.IsEqual(DevParamColumnClass) )
	{
		FText ParamTypeName = ParamType->GetClass()->GetDisplayNameText();
		if ( ParamTypeName.IsEmpty() )
		{
			ParamTypeName = FText::FromString(ParamType->GetClass()->GetName());
		}

		FText ParamTypeTooltip = ParamType->GetClass()->GetToolTipText();

		// clang-format off
    	return SNew(SBox)
        .Padding(FMargin(4, 2, 4, 2))
        [
            SNew(STextBlock)
            .TextStyle(FEditorStyle::Get(), "DataTableEditor.CellText")
            .Text(ParamTypeName)
            .ColorAndOpacity(DevParamEdit, &FDevParamEditor::GetDevParamColor, RowData->ParamType)
            //.HighlightText(DevParamEdit, &FDevParamEditor::GetFilterText)
            .ToolTipText(ParamTypeTooltip)
        ];
		// clang-format on
	}
	//
	if ( InColumnId.IsEqual(DevParamColumnExpansion) )
	{
		// clang-format off
    	return SNew(SBox)
        .Padding(FMargin(4, 2, 4, 2))
        [
            SNew(STextBlock)
            .TextStyle(FEditorStyle::Get(), "DataTableEditor.CellText")
            .Text(ParamType->GetDefaultValueExtension())
            .ColorAndOpacity(DevParamEdit, &FDevParamEditor::GetDevParamColor, RowData->ParamType)
            //.Text(DevParamEdit, &FDevParamEditor::GetCellText, RowDataPtr, ColumnIndex)
            //.HighlightText(DevParamEdit, &FDevParamEditor::GetFilterText)
            //.ToolTipText(DevParamEdit, &FDevParamEditor::GetCellToolTipText, RowDataPtr, ColumnIndex)
        ];
		// clang-format on
	}

	if ( FProperty* Property =
	         FindFProperty<FProperty>(ParamType->GetClass(), InColumnId) )
	{
		FText       ValueText   = FText::GetEmpty();
		FText       TooltipText = FText::GetEmpty();
		const void* Value       = Property->ContainerPtrToValuePtr<uint8>(ParamType);

		// 一部の型は特殊化
		if ( const FStructProperty* StructProperty =
		         CastField<FStructProperty>(Property) )
		{
			if ( StructProperty->Struct == FGameplayTag::StaticStruct() )
			{
				// GameplayTagの場合は特殊化
				if ( const FGameplayTag* GamePlayTagValue =
				         reinterpret_cast<const FGameplayTag*>(Value) )
				{
					ValueText = FText::FromName(GamePlayTagValue->GetTagName());
					UGameplayTagsManager& TagManager = UGameplayTagsManager::Get();

					FString OutComment;
					FName   OutTagSource;
					bool    bOutIsTagExplicit;
					bool    bOutIsRestrictedTag;
					bool    bOutAllowNonRestrictedChildren;

					if ( TagManager.GetTagEditorData(
					         GamePlayTagValue->GetTagName(),
					         OutComment,
					         OutTagSource,
					         bOutIsTagExplicit,
					         bOutIsRestrictedTag,
					         bOutAllowNonRestrictedChildren) )
					{
						// エディター情報がある場合
						TooltipText = FText::FromString(OutComment);
					}
				}
			}
		}

		if ( ValueText.IsEmpty() )
		{
			TSharedPtr<FJsonValue> JsonValue =
			    FJsonObjectConverter::UPropertyToJsonValue(Property, Value);

			if ( JsonValue.IsValid() )
			{
				using FStringWriter =
				    TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>;
				using FStringWriterFactory =
				    TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>;

				FString                   CopyStr;
				TSharedRef<FStringWriter> Writer =
				    FStringWriterFactory::Create(&CopyStr);
				FJsonSerializer::Serialize(
				    JsonValue.ToSharedRef(), TEXT(""), Writer);

				CopyStr = CopyStr.Replace(TEXT("\r\n"), TEXT(""));

				ValueText = FText::FromString(CopyStr);
			}
		}

		// 値のテキストが変換していれば…
		if ( !ValueText.IsEmpty() )
		{
			// clang-format off
                return SNew(SBox)
                .Padding(FMargin(4, 2, 4, 2))
                [
                    SNew(STextBlock)
                    .TextStyle(FEditorStyle::Get(), "DataTableEditor.CellText")
                    .Text(ValueText)
                    .ColorAndOpacity(DevParamEdit, &FDevParamEditor::GetDevParamColor, RowData->ParamType)
                    //.Text(DevParamEdit, &FDevParamEditor::GetCellText, RowDataPtr, ColumnIndex)
                    //.HighlightText(DevParamEdit, &FDevParamEditor::GetFilterText)
                    .ToolTipText(TooltipText)
                ];
			// clang-format on
		}
	}

#if 0
	const FName RowDragDropColumnId("RowDragDrop");

	int32 ColumnIndex = 0;

	FDevParamEditor* DevParamEdit = DevParamEditor.Pin().Get();
	TArray<FDevParamEditorRowListViewDataPtr>& AvailableColumns =
	    DevParamEdit->AvailableColumns;

	if ( InColumnId.IsEqual(RowDragDropColumnId) )
	{
		return SNew(SDataTableRowHandle)
		    .Content()[SNew(SHorizontalBox) +
		               SHorizontalBox::Slot().Padding(
		                   5.0f, 1.0f)[SNew(SImage).Image(FCoreStyle::Get().GetBrush(
		                   "VerticalBoxDragIndicatorShort"))]]
		    .ParentRow(SharedThis(this));
	}

	const FName RowNumberColumnId("RowNumber");

	if ( InColumnId.IsEqual(RowNumberColumnId) )
	{
		// clang-format off
	    return SNew(SBox)
        .Padding(FMargin(4, 2, 4, 2))
        [
            SNew(STextBlock)
            .TextStyle(FEditorStyle::Get(), "DataTableEditor.CellText")
            .Text(FText::FromString(FString::FromInt(RowDataPtr->RowNum)))
            .ColorAndOpacity(DevParamEdit, &FDevParamEditor::GetRowTextColor, RowDataPtr->RowId)
            .HighlightText(DevParamEdit, &FDevParamEditor::GetFilterText)
        ];
		// clang-format on
	}

	const FName RowNameColumnId("RowName");

	if ( InColumnId.IsEqual(RowNameColumnId) )
	{
		// clang-format off
    	return SNew(SBox)
            .Padding(FMargin(4, 2, 4, 2))
            [
                SAssignNew(InlineEditableText, SInlineEditableTextBlock)
		        .Text(RowDataPtr->DisplayName)
		        .OnTextCommitted(this, &SDevParamListViewRow::OnRowRenamed)
		        .HighlightText(DevParamEdit, &FDevParamEditor::GetFilterText)
		        .ColorAndOpacity(DevParamEdit,
		                         &FDevParamEditor::GetRowTextColor,
		                         RowDataPtr->RowId)
		        .IsReadOnly(!IsEditable)];
		// clang-format on
	}

	for ( ; ColumnIndex < AvailableColumns.Num(); ++ColumnIndex )
	{
		const FDevParamEditorRowListViewDataPtr& ColumnData =
		    AvailableColumns[ColumnIndex];
		if ( ColumnData->ColumnId == InColumnId )
		{
			break;
		}
	}

	// Valid column ID?
	if ( AvailableColumns.IsValidIndex(ColumnIndex) &&
	     RowDataPtr->CellData.IsValidIndex(ColumnIndex) )
	{
		// clang-format off
    	return SNew(SBox)
        .Padding(FMargin(4, 2, 4, 2))
        [
            SNew(STextBlock)
            .TextStyle(FEditorStyle::Get(), "DataTableEditor.CellText")
            .ColorAndOpacity(DevParamEdit, &FDevParamEditor::GetRowTextColor, RowDataPtr->RowId)
            .Text(DevParamEdit, &FDevParamEditor::GetCellText, RowDataPtr, ColumnIndex)
            .HighlightText(DevParamEdit, &FDevParamEditor::GetFilterText)
            .ToolTipText(DevParamEdit, &FDevParamEditor::GetCellToolTipText, RowDataPtr, ColumnIndex)
        ];
		// clang-format on
	}
#endif

	return SNullWidget::NullWidget;
}

//////////////////////////////////////////////////////////////////////////
// SDevParamEditor

SDevParamEditor::SDevParamEditor() {}

SDevParamEditor::~SDevParamEditor()
{
	if ( DevParamEditor.IsValid() )
	{
		DevParamEditor.Pin()->OnChangedDevParamAsset.Remove(
		    OnChangedDevParamAssetHandle);

		DevParamEditor.Pin()->OnChangedDevParam.Remove(OnChangedDevParamHandle);
	}
}

void SDevParamEditor::Construct(const FArguments&           InArgs,
                                TSharedRef<FDevParamEditor> InDevMenuEditor)
{
	DevParamEditor = InDevMenuEditor;

	OnChangedDevParamAssetHandle =
	    DevParamEditor.Pin()->OnChangedDevParamAsset.AddRaw(
	        this, &SDevParamEditor::OnChangedDevParamAsset);

	OnChangedDevParamHandle = DevParamEditor.Pin()->OnChangedDevParam.AddRaw(
	    this, &SDevParamEditor::OnChangedDevParam);

	// Viewを更新
	UpdateVisibleRows();

	TSharedRef<SScrollBar> HorizontalScrollBar =
	    SNew(SScrollBar)
	        .Orientation(Orient_Horizontal)
	        .Thickness(FVector2D(12.0f, 12.0f));

	TSharedRef<SScrollBar> VerticalScrollBar =
	    SNew(SScrollBar)
	        .Orientation(Orient_Vertical)
	        .Thickness(FVector2D(12.0f, 12.0f));

	ColumnNamesHeaderRow = SNew(SHeaderRow);

	CellsListView =
	    SNew(SListView<FDevParamEditorRowListViewDataPtr>)
	        .ListItemsSource(&VisibleRows)
	        .HeaderRow(ColumnNamesHeaderRow)
	        .OnGenerateRow(this, &SDevParamEditor::MakeRowWidget)
	        .OnSelectionChanged(this, &SDevParamEditor::OnRowSelectionChanged)
	        .ExternalScrollbar(VerticalScrollBar)
	        .ConsumeMouseWheel(EConsumeMouseWheel::Always)
	        .SelectionMode(ESelectionMode::Single)
	        .AllowOverscroll(EAllowOverscroll::No);

	RefreshHeaderRow();

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
		+SVerticalBox::Slot()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			[
				SNew(SScrollBox)
				.Orientation(Orient_Horizontal)
				.ExternalScrollbar(HorizontalScrollBar)
				+SScrollBox::Slot()
				[
					CellsListView.ToSharedRef()
				]
			]
			+SHorizontalBox::Slot()
			.FillWidth(1.f)
			[
				VerticalScrollBar
			]
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			[
				HorizontalScrollBar
			]
		]
    ];
	// clang-format on
}

// 行ウィジェットの作成
TSharedRef<ITableRow> SDevParamEditor::MakeRowWidget(
    FDevParamEditorRowListViewDataPtr InRowDataPtr,
    const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(SDevParamListViewRow, OwnerTable)
	    .DevParamEditor(DevParamEditor.Pin())
	    .RowDataPtr(InRowDataPtr)
	    //.IsEditable(CanEditRows())
	    ;
}

// 選択の切り替わり
void SDevParamEditor::OnRowSelectionChanged(
    FDevParamEditorRowListViewDataPtr InNewSelection,
    ESelectInfo::Type                 InSelectInfo)
{
	if ( InNewSelection.IsValid() )
	{
		DevParamEditor.Pin()->SetSelectItem(InNewSelection->ParamType);
		return;
	}
	DevParamEditor.Pin()->SetSelectItem(nullptr);
}

// ヘッダーのリフレッシュ
void SDevParamEditor::RefreshHeaderRow()
{
	// ヘッダーのクリア
	ColumnNamesHeaderRow->ClearColumns();

	// clang-format off
    ColumnNamesHeaderRow->AddColumn(
		SHeaderRow::Column(DevParamColumnDragDrop)
        .FixedWidth(15.f)
    	[
			SNew(SBox)
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SNew(STextBlock)
				.Text(FText::GetEmpty())
			]
		]
    );
	// clang-format on

	ColumnNamesHeaderRow->AddColumn(
	    SHeaderRow::Column(DevParamColumnClass)
	        .DefaultLabel(
	            LOCTEXT("ColumnNamesHeaderRow_DevParamType", "DevParamType"))
	        .ToolTip(IDocumentation::Get()->CreateToolTip(
	            LOCTEXT("ColumnNamesHeaderRow_DevParamTypeTooltip",
	                    "Dev Param Type"),
	            nullptr,
	            TEXT(""),
	            TEXT(""))));

	for ( const FProperty* Property :
	      TFieldRange<FProperty>(UDevParamType::StaticClass()) )
	{
		ColumnNamesHeaderRow->AddColumn(
		    SHeaderRow::Column(Property->GetFName())
		        .DefaultLabel(Property->GetDisplayNameText())
		        .ToolTip(IDocumentation::Get()->CreateToolTip(
		            Property->GetToolTipText(), nullptr, TEXT(""), TEXT("")))

		);
		//
	}

	ColumnNamesHeaderRow->AddColumn(
	    SHeaderRow::Column(DevParamColumnExpansion)
	        .DefaultLabel(LOCTEXT("ColumnNamesHeaderRow_Other", "Other"))
	        .ToolTip(IDocumentation::Get()->CreateToolTip(
	            LOCTEXT("ColumnNamesHeaderRow_OtherTooltip", "Other Parameter"),
	            nullptr,
	            TEXT(""),
	            TEXT("")))

	);
}

void SDevParamEditor::UpdateVisibleRows()
{
	//
	if ( !DevParamEditor.IsValid() )
	{
		return;
	}

	UDevParamDataAsset* DevParamEdited = DevParamEditor.Pin()->GetDevParamEdited();

	VisibleRows.Empty();

	for ( UDevParamType* ParamType : DevParamEdited->Params )
	{
		auto& NewData =
		    VisibleRows.Add_GetRef(MakeShared<FDevParamEditorRowListViewData>());

		NewData->ParamType = ParamType;
	}

	if ( CellsListView.IsValid() )
	{
		CellsListView->RequestListRefresh();
	}
}

// パラメータ変更通知
void SDevParamEditor::OnChangedDevParam()
{
	OnChangedDevParamAsset();
}

// パラメータの変更通知
void SDevParamEditor::OnChangedDevParamAsset()
{
	UDevParamType* SelectedItem = DevParamEditor.Pin()->GetSelectItem();
	UpdateVisibleRows();

	auto HasDevParam =
	    [SelectedItem](const FDevParamEditorRowListViewDataPtr& ViewData)
	{
		return ViewData->ParamType == SelectedItem;
	};

	auto* Result = VisibleRows.FindByPredicate(HasDevParam);

	if ( Result )
	{
		CellsListView->SetSelection(*Result);
	}
}

#undef LOCTEXT_NAMESPACE