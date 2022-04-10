// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Docking/TabManager.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
class FDevParamEditor;

/** Application mode for main behavior tree editing mode */
class FDevParamEditorApplicationModes : public FApplicationMode
{
public:
	FDevParamEditorApplicationModes(TSharedPtr<FDevParamEditor> InDevParamEditor);

	virtual void RegisterTabFactories(
	    TSharedPtr<class FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;

protected:
	TWeakPtr<FDevParamEditor> DevParamEditor;

	// Set of spawnable tabs in behavior tree editing mode
	FWorkflowAllowedTabSet EditorTabFactories;
};
