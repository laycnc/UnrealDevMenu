// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"
class FUnrealDevMenuEditor;

struct FDevMenuLibraryTabSummoner : public FWorkflowTabFactory
{
public:
	FDevMenuLibraryTabSummoner(TSharedPtr<FUnrealDevMenuEditor> InDevMenuEditor);

	virtual TSharedRef<SWidget> CreateTabBody(
	    const FWorkflowTabSpawnInfo& Info) const override;

protected:
	TWeakPtr<FUnrealDevMenuEditor> DevMenuEditor;
};

struct FDevMenuHierarchyTabSummoner : public FWorkflowTabFactory
{
public:
	FDevMenuHierarchyTabSummoner(TSharedPtr<FUnrealDevMenuEditor> InDevMenuEditor);

	virtual TSharedRef<SWidget> CreateTabBody(
	    const FWorkflowTabSpawnInfo& Info) const override;

protected:
	TWeakPtr<FUnrealDevMenuEditor> DevMenuEditor;
};

struct FDevMenuDetailsSummoner : public FWorkflowTabFactory
{
public:
	FDevMenuDetailsSummoner(TSharedPtr<FUnrealDevMenuEditor> InDevMenuEditor);

	virtual TSharedRef<SWidget> CreateTabBody(
	    const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(
	    const FWorkflowTabSpawnInfo& Info) const override;

protected:
	TWeakPtr<FUnrealDevMenuEditor> DevMenuEditor;
};
