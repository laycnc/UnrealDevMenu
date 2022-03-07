// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "UObject/GCObject.h"
#include "Framework/Docking/TabManager.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class IDetailsView;
class UDevMenu;

//////////////////////////////////////////////////////////////////////////

struct FUnrealDevMenuEditorTabs
{
	// Tab identifiers
	static const FName DetailsID;
	static const FName HierarchyID;
	static const FName LibraryID;
};

//////////////////////////////////////////////////////////////////////////
// UnrealDevMenuEditor

class FUnrealDevMenuEditor
    : public FWorkflowCentricApplication
    , public FGCObject
    , public FNotifyHook
{

public:
	/** Modes in mode switcher */
	static const FName DevMenuMode;

public:
	void InitDevMenuEditor(const EToolkitMode::Type        Mode,
	                       const TSharedPtr<IToolkitHost>& InitToolkitHost,
	                       UDevMenu*                       InitDevMenu);

public:
	FUnrealDevMenuEditor();

	// IToolkit interface
	virtual void RegisterTabSpawners(
	    const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(
	    const TSharedRef<FTabManager>& InTabManager) override;
	// End of IToolkit interface

	// FAssetEditorToolkit
	virtual FName        GetToolkitFName() const override;
	virtual FText        GetBaseToolkitName() const override;
	virtual FText        GetToolkitName() const override;
	virtual FText        GetToolkitToolTipText() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString      GetWorldCentricTabPrefix() const override;
	//virtual FString      GetDocumentationLink() const override;
	// End of FAssetEditorToolkit

	// FGCObject interface
	virtual void    AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override
	{
		return TEXT("FUnrealDevMenuEditor");
	}
	// End of FGCObject interface

	//~ Begin FNotifyHook Interface
	virtual void NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent,
	                              FProperty* PropertyThatChanged) override;
	// End of FNotifyHook

	UDevMenu* GetDevMenuEdited() const
	{
		return DevMenuEdited;
	}
public:
	TSharedRef<SWidget> SpawnDetails();
	void                OnChangeHierarchyItem(UObject* NewObject);

private:
	UDevMenu* DevMenuEdited = nullptr;

	/** The details view we use to display the blackboard */
	TSharedPtr<IDetailsView> DetailsView;
};