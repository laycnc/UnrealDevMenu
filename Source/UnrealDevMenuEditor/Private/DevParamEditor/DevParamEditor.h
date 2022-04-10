// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "UObject/GCObject.h"
#include "EditorUndoClient.h"
#include "Framework/Docking/TabManager.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class IDetailsView;
class UDevParamDataAsset;

//////////////////////////////////////////////////////////////////////////

struct FDevParamEditorTabs
{
	// Tab identifiers
	static const FName DetailsID;
	static const FName DevParamID;
};

//////////////////////////////////////////////////////////////////////////
// FDevParamEditor

class FDevParamEditor
    : public FWorkflowCentricApplication
    , public FEditorUndoClient
    , public FGCObject
    , public FNotifyHook
{

public:
	/** Modes in mode switcher */
	static const FName DevParamMode;

public:
	void InitDevMenuEditor(const EToolkitMode::Type        Mode,
	                       const TSharedPtr<IToolkitHost>& InitToolkitHost,
	                       UDevParamDataAsset*             InitDevParamDataAsset);

public:
	FDevParamEditor();
	~FDevParamEditor();

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

	// FEditorUndoClient interface
	virtual void PostUndo(bool bSuccess) override;
	virtual void PostRedo(bool bSuccess) override;
	// End of FEditorUndoClient

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

	UDevParamDataAsset* GetDevParamEdited() const
	{
		return DevParamDataAssetEdited;
	}

	TSharedRef<FUICommandList> GetHierarchyCommandList() const;

	DECLARE_MULTICAST_DELEGATE(FOnChangedMenu);

	// メニューが更新された
	FOnChangedMenu OnChangedMenu;

public:
	TSharedRef<SWidget> SpawnDetails();
	void                OnChangeHierarchyItem(UObject* NewObject);

    void OnAddClicked();

private:
	// 拡張ツールバー
	void ExtendToolbar(TSharedPtr<FExtender> Extender);
    // ツールバーを拡張分で埋める
	void FillToolbar(FToolBarBuilder& ToolbarBuilder);
	void GeneratedMenuItemClasses();

	//////////////////////////////////////////////////////////////////////////
	// Commands

	// コマンドの初期化
	void InitializeCommands();

private:
	UDevParamDataAsset* DevParamDataAssetEdited = nullptr;

	/** The details view we use to display the blackboard */
	TSharedPtr<IDetailsView> DetailsView;

	// Hierarchyのコマンドリスト
	TSharedPtr<FUICommandList> HierarchyCommandList;
};