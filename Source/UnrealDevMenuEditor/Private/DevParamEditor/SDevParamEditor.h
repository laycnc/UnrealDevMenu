// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "Widgets/SWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Misc/NotifyHook.h"
#include "Widgets/Input/SComboBox.h"

class FDevParamEditor;

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

private:
	// 検索ボックス
	TSharedPtr<SSearchBox> SearchBoxWidget;

	TWeakPtr<FDevParamEditor> DevParamEditor;
};