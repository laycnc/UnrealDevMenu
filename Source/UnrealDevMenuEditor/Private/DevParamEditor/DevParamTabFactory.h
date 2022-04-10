// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"
class FDevParamEditor;

/**
 * デバッグパラメータのエディタータブ召喚者
 */
struct FDevParamEditorSummoner : public FWorkflowTabFactory
{
public:
	FDevParamEditorSummoner(TSharedRef<FDevParamEditor> InDevParamEditor);

	virtual TSharedRef<SWidget> CreateTabBody(
	    const FWorkflowTabSpawnInfo& Info) const override;

protected:
	TWeakPtr<FDevParamEditor> DevParamEditor;
};

/**
 * デバッグパラメータの詳細タブ召喚者
 */
struct FDevParamDetailsSummoner : public FWorkflowTabFactory
{
public:
	FDevParamDetailsSummoner(TSharedRef<FDevParamEditor> InDevParamEditor);

	virtual TSharedRef<SWidget> CreateTabBody(
	    const FWorkflowTabSpawnInfo& Info) const override;

protected:
	TWeakPtr<FDevParamEditor> DevParamEditor;
};
