// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Docking/TabManager.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
class FUnrealDevMenuEditor;

/** Application mode for main behavior tree editing mode */
class FDevMenuEditorApplicationModes : public FApplicationMode
{
public:
	FDevMenuEditorApplicationModes(TSharedPtr<FUnrealDevMenuEditor> InDevMenuEditor);

	virtual void RegisterTabFactories(
	    TSharedPtr<class FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;

protected:
	TWeakPtr<FUnrealDevMenuEditor> DevMenuEditor;

	// Set of spawnable tabs in behavior tree editing mode
	FWorkflowAllowedTabSet EditorTabFactories;
};
