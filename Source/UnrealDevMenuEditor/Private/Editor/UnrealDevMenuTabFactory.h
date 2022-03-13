// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"
class FUnrealDevMenuEditor;

struct FDevMenuLibraryTabSummoner : public FWorkflowTabFactory
{
public:
	FDevMenuLibraryTabSummoner(TSharedRef<FUnrealDevMenuEditor> InDevMenuEditor);

	virtual TSharedRef<SWidget> CreateTabBody(
	    const FWorkflowTabSpawnInfo& Info) const override;

protected:
	TWeakPtr<FUnrealDevMenuEditor> DevMenuEditor;
};

struct FDevMenuHierarchyTabSummoner : public FWorkflowTabFactory
{
public:
	FDevMenuHierarchyTabSummoner(TSharedRef<FUnrealDevMenuEditor> InDevMenuEditor);

	virtual TSharedRef<SWidget> CreateTabBody(
	    const FWorkflowTabSpawnInfo& Info) const override;

protected:
	TWeakPtr<FUnrealDevMenuEditor> DevMenuEditor;
};

struct FDevMenuDetailsSummoner : public FWorkflowTabFactory
{
public:
	FDevMenuDetailsSummoner(TSharedRef<FUnrealDevMenuEditor> InDevMenuEditor);

	virtual TSharedRef<SWidget> CreateTabBody(
	    const FWorkflowTabSpawnInfo& Info) const override;

protected:
	TWeakPtr<FUnrealDevMenuEditor> DevMenuEditor;
};
