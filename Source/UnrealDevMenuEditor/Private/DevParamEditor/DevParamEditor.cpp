// Copyright Epic Games, Inc. All Rights Reserved.

#include "DevParamEditor.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "SSingleObjectDetailsPanel.h"
#include "PropertyEditorModule.h"
#include "DetailsViewArgs.h"
#include "UnrealDevMenuEditorModule.h"
#include "AIGraphTypes.h"
#include "DevMenuItemBase.h"
#include "Framework/Commands/GenericCommands.h"
#include "Algo/AllOf.h"
#include "ParamType/DevParamDataAsset.h"
#include "DevParamEditorApplicationModes.h"

#define LOCTEXT_NAMESPACE "FDevParamEditor"

//////////////////////////////////////////////////////////////////////////

const FName DevParamEditorAppName = FName(TEXT("UnrealDevMenuEditorApp"));

//////////////////////////////////////////////////////////////////////////

const FName FDevParamEditorTabs::DetailsID(TEXT("Details"));
const FName FDevParamEditorTabs::DevParamID(TEXT("DevParamID"));

//////////////////////////////////////////////////////////////////////////
// UnrealDevMenuEditor

/** Modes in mode switcher */
const FName FDevParamEditor::DevParamMode = TEXT("DevParamMode");

void FDevParamEditor::InitDevMenuEditor(
    const EToolkitMode::Type        Mode,
    const TSharedPtr<IToolkitHost>& InitToolkitHost,
    UDevParamDataAsset*             InitDevParamDataAsset)
{
	//GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->CloseOtherEditors(
	//    InitDevMenu, this);

	DevParamDataAssetEdited = InitDevParamDataAsset;

	// キャッシュを作成する
	GeneratedMenuItemClasses();

	// コマンドを初期化
	InitializeCommands();

	// Initialize the asset editor and spawn nothing (dummy layout)
	InitAssetEditor(Mode,
	                InitToolkitHost,
	                DevParamEditorAppName,
	                FTabManager::FLayout::NullLayout,
	                /*bCreateDefaultStandaloneMenu=*/true,
	                /*bCreateDefaultToolbar=*/true,
	                InitDevParamDataAsset);

	IUnrealDevMenuEditorModule& DevMenuEditorModule =
	    FModuleManager::LoadModuleChecked<IUnrealDevMenuEditorModule>(
	        "UnrealDevMenuEditor");

	// ツールバーの拡張
	TSharedPtr<FExtender> ToolbarExtender =
	    DevMenuEditorModule.GetToolBarExtensibilityManager()->GetAllExtenders(
	        GetToolkitCommands(), GetEditingObjects());
	ExtendToolbar(ToolbarExtender);

	AddToolbarExtender(ToolbarExtender);

	AddApplicationMode(
	    DevParamMode,
	    MakeShareable(new FDevParamEditorApplicationModes(SharedThis(this))));

	// 現在のモードを切り替える
	SetCurrentMode(DevParamMode);
}

FDevParamEditor::FDevParamEditor()
{
	if ( UEditorEngine* Editor = Cast<UEditorEngine>(GEngine) )
	{
		Editor->RegisterForUndo(this);
	}
}

FDevParamEditor ::~FDevParamEditor()
{
	if ( UEditorEngine* Editor = Cast<UEditorEngine>(GEngine) )
	{
		Editor->UnregisterForUndo(this);
	}
}

// IToolkit interface
void FDevParamEditor::RegisterTabSpawners(
    const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}
void FDevParamEditor::UnregisterTabSpawners(
    const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
}
// End of IToolkit interface

// FAssetEditorToolkit
FName FDevParamEditor::GetToolkitFName() const
{
	return FName("UnrealDevMenuEditor");
}

FText FDevParamEditor::GetBaseToolkitName() const
{
	return LOCTEXT("FUnrealDevMenuEditorAppLabel", "Dev Menu Editor");
}

FText FDevParamEditor::GetToolkitName() const
{
	return FText::FromString(DevParamDataAssetEdited->GetName());
}

FText FDevParamEditor::GetToolkitToolTipText() const
{
	return FAssetEditorToolkit::GetToolTipTextForObject(DevParamDataAssetEdited);
}

FLinearColor FDevParamEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}

FString FDevParamEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("UnrealDevMenuEditor");
}

// End of FAssetEditorToolkit

// FEditorUndoClient interface
void FDevParamEditor::PostUndo(bool bSuccess)
{
	if ( bSuccess )
	{
		OnChangedMenu.Broadcast();
	}
}
void FDevParamEditor::PostRedo(bool bSuccess)
{
	if ( bSuccess )
	{
		OnChangedMenu.Broadcast();
	}
}
// End of FEditorUndoClient

// FGCObject interface
void FDevParamEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(DevParamDataAssetEdited);
}

// End of FGCObject interface

//~ Begin FNotifyHook Interface

void FDevParamEditor::NotifyPostChange(
    const FPropertyChangedEvent& PropertyChangedEvent,
    FProperty*                   PropertyThatChanged)
{
	if ( PropertyThatChanged == nullptr )
	{
		return;
	}
	const FString ChangeOwnerClassName =
	    PropertyThatChanged->GetOwner<UStruct>()->GetName();
	if ( ChangeOwnerClassName == TEXT("DevMenu") )
	{
		if ( PropertyThatChanged->GetFName() == TEXT("Items") )
		{
			// メニュー変更を呼ぶ
			OnChangedMenu.Broadcast();
			return;
		}
	}

	if ( ChangeOwnerClassName == TEXT("DevMenuGroup") )
	{
		if ( PropertyThatChanged->GetFName() == TEXT("Items") )
		{
			// メニュー変更を呼ぶ
			OnChangedMenu.Broadcast();
			return;
		}
	}
}

// End of FNotifyHook

TSharedRef<SWidget> FDevParamEditor::SpawnDetails()
{
	FPropertyEditorModule& PropertyEditorModule =
	    FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings  = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.bHideSelectionTip = true;
	DetailsViewArgs.NotifyHook        = this;
	DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

	// Set Objects
	DetailsView->SetObject(DevParamDataAssetEdited);
	//DetailsView->SetEnabled(
	//    TAttribute<bool>::Create(TAttribute<bool>::FGetter::CreateSP(
	//        this, &FDevParamEditor::CanEditWithDebuggerActive)));

	return DetailsView.ToSharedRef();
}

void FDevParamEditor::OnChangeHierarchyItem(UObject* NewObject)
{
	//
	if ( DetailsView.IsValid() )
	{
		DetailsView->SetObject(NewObject);
	}
}

// 拡張ツールバー
void FDevParamEditor::ExtendToolbar(TSharedPtr<FExtender> Extender)
{
	Extender->AddToolBarExtension(
	    "Asset",
	    EExtensionHook::After,
	    GetToolkitCommands(),
	    FToolBarExtensionDelegate::CreateSP(this, &FDevParamEditor::FillToolbar));
}

// ツールバーを拡張分で埋める
void FDevParamEditor::FillToolbar(FToolBarBuilder& ToolbarBuilder)
{
	ToolbarBuilder.BeginSection("DevParamCommands");
	{
		ToolbarBuilder.AddToolBarButton(
		    FUIAction(
		        FExecuteAction::CreateSP(this, &FDevParamEditor::OnAddClicked)),
		    NAME_None,
		    LOCTEXT("AddIconText", "Add"),
		    LOCTEXT("AddRowToolTip", "Add a new row to the Data Table"),
		    FSlateIcon(FEditorStyle::GetStyleSetName(), "Icons.Plus"));
	}
}

void FDevParamEditor::GeneratedMenuItemClasses()
{
	IUnrealDevMenuEditorModule& Module =
	    FModuleManager::GetModuleChecked<IUnrealDevMenuEditorModule>(
	        TEXT("UnrealDevMenuEditor"));
	TSharedRef<FGraphNodeClassHelper> ClassCache = Module.GetMenuItemClassCache();
	TArray<FGraphNodeClassData>       NodeClasses;
	ClassCache->GatherClasses(UDevMenuItemBase::StaticClass(), NodeClasses);
}

void FDevParamEditor::OnAddClicked()
{
	//
}

//////////////////////////////////////////////////////////////////////////
// Commands

TSharedRef<FUICommandList> FDevParamEditor::GetHierarchyCommandList() const
{
	return HierarchyCommandList.ToSharedRef();
}

// コマンドの初期化
void FDevParamEditor::InitializeCommands()
{
	//
	HierarchyCommandList = MakeShared<FUICommandList>();
}

#undef LOCTEXT_NAMESPACE
