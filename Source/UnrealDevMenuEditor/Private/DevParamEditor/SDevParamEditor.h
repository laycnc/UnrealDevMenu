// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "Widgets/SWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Misc/NotifyHook.h"
#include "Widgets/Input/SComboBox.h"
#include "DevParamEditorDefine.h"

class UDevParamType;
class FDevParamEditor;

class SDevParamListViewRow
    : public SMultiColumnTableRow<FDevParamEditorRowListViewDataPtr>
{
public:
	SLATE_BEGIN_ARGS(SDevParamListViewRow)
	    : _IsEditable(true)
	{
	}
	/** The owning object. This allows us access to the actual data table being edited as well as some other API functions. */
	SLATE_ARGUMENT(TSharedPtr<FDevParamEditor>, DevParamEditor)
	/** The row we're working with to allow us to get naming information. */
	SLATE_ARGUMENT(FDevParamEditorRowListViewDataPtr, RowDataPtr)
	SLATE_ARGUMENT(bool, IsEditable)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments&                 InArgs,
	               const TSharedRef<STableViewBase>& InOwnerTableView);

	virtual TSharedRef<SWidget> GenerateWidgetForColumn(
	    const FName& ColumnName) override;

	TSharedRef<SWidget> MakeCellWidget(const int32  InRowIndex,
	                                   const FName& InColumnId);


private:
	TWeakPtr<FDevParamEditor>         DevParamEditor;
	FDevParamEditorRowListViewDataPtr RowData;
};

class SDevParamEditor : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDevParamEditor) {}
	SLATE_END_ARGS()

	SDevParamEditor();
	virtual ~SDevParamEditor();

public:
	void Construct(const FArguments&           InArgs,
	               TSharedRef<FDevParamEditor> InDevMenuEditor);

	// 行ウィジェットの作成
	TSharedRef<ITableRow> MakeRowWidget(
	    FDevParamEditorRowListViewDataPtr InRowDataPtr,
	    const TSharedRef<STableViewBase>& OwnerTable);

	// 選択の切り替わり
	void OnRowSelectionChanged(FDevParamEditorRowListViewDataPtr InNewSelection,
	                           ESelectInfo::Type                 InSelectInfo);

	// ヘッダーのリフレッシュ
	void RefreshHeaderRow();
	void UpdateVisibleRows();

    // パラメータ変更通知
	void OnChangedDevParam();

	// パラメータの変更通知
	void OnChangedDevParamAsset();

private:
	// 検索ボックス
	TSharedPtr<SSearchBox> SearchBoxWidget;

	// CellsListViewに表示されるヘッダー
	TSharedPtr<SHeaderRow> ColumnNamesHeaderRow;

	/** List view responsible for showing the rows in VisibleRows for each entry in AvailableColumns */
	TSharedPtr<SListView<FDevParamEditorRowListViewDataPtr>> CellsListView;

	// 表示されるパラメータタイプ
	TArray<FDevParamEditorRowListViewDataPtr> VisibleRows;

	TWeakPtr<FDevParamEditor> DevParamEditor;

private:
	FDelegateHandle OnChangedDevParamAssetHandle;
	FDelegateHandle OnChangedDevParamHandle;
};