// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealDevMenuEditor.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "SSingleObjectDetailsPanel.h"
#include "DevMenuEditorApplicationModes.h"
#include "PropertyEditorModule.h"
#include "DetailsViewArgs.h"
//
#include "DevMenu.h"

#define LOCTEXT_NAMESPACE "FUnrealDevMenuEditor"

//////////////////////////////////////////////////////////////////////////

const FName UnrealDevMenuEditorAppName = FName(TEXT("UnrealDevMenuEditorApp"));

//////////////////////////////////////////////////////////////////////////

const FName FUnrealDevMenuEditorTabs::DetailsID(TEXT("Details"));
const FName FUnrealDevMenuEditorTabs::HierarchyID(TEXT("Hierarchy"));
const FName FUnrealDevMenuEditorTabs::LibraryID(TEXT("Library"));

//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////
// SDevMenuPropertiesTabBody

class SDevMenuPropertiesTabBody : public SSingleObjectDetailsPanel
{
public:
	SLATE_BEGIN_ARGS(SDevMenuPropertiesTabBody) {}
	SLATE_END_ARGS()

private:
	// Pointer back to owning sprite editor instance (the keeper of state)
	TWeakPtr<FUnrealDevMenuEditor> DevMenuEditorPtr;
public:
	void Construct(const FArguments&                InArgs,
	               TSharedPtr<FUnrealDevMenuEditor> InDevMenuEditor)
	{
		DevMenuEditorPtr = InDevMenuEditor;

		SSingleObjectDetailsPanel::Construct(
		    SSingleObjectDetailsPanel::FArguments()
		        .HostCommandList(InDevMenuEditor->GetToolkitCommands())
		        .HostTabManager(InDevMenuEditor->GetTabManager()),
		    /*bAutomaticallyObserveViaGetObjectToObserve=*/true,
		    /*bAllowSearch=*/true);
	}

	// SSingleObjectDetailsPanel interface
	virtual UObject* GetObjectToObserve() const override
	{
		return DevMenuEditorPtr.Pin()->GetDevMenuEdited();
	}

	virtual TSharedRef<SWidget> PopulateSlot(
	    TSharedRef<SWidget> PropertyEditorWidget) override
	{
		return SNew(SVerticalBox) +
		       SVerticalBox::Slot().FillHeight(1)[PropertyEditorWidget];
	}
	// End of SSingleObjectDetailsPanel interface
};

//////////////////////////////////////////////////////////////////////////
// UnrealDevMenuEditor

/** Modes in mode switcher */
const FName FUnrealDevMenuEditor::DevMenuMode = TEXT("DevMenuMode");

void FUnrealDevMenuEditor::InitDevMenuEditor(
    const EToolkitMode::Type        Mode,
    const TSharedPtr<IToolkitHost>& InitToolkitHost,
    UDevMenu*                       InitDevMenu)
{
	GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->CloseOtherEditors(
	    InitDevMenu, this);
	DevMenuEdited = InitDevMenu;

	// Initialize the asset editor and spawn nothing (dummy layout)
	InitAssetEditor(Mode,
	                InitToolkitHost,
	                UnrealDevMenuEditorAppName,
	                FTabManager::FLayout::NullLayout,
	                /*bCreateDefaultStandaloneMenu=*/true,
	                /*bCreateDefaultToolbar=*/true,
	                InitDevMenu);

	AddApplicationMode(
	    DevMenuMode,
	    MakeShareable(new FDevMenuEditorApplicationModes(SharedThis(this))));

	// 現在のモードを切り替える
	SetCurrentMode(DevMenuMode);
}

FUnrealDevMenuEditor::FUnrealDevMenuEditor() {}

// IToolkit interface
void FUnrealDevMenuEditor::RegisterTabSpawners(
    const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}
void FUnrealDevMenuEditor::UnregisterTabSpawners(
    const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
}
// End of IToolkit interface

// FAssetEditorToolkit
FName FUnrealDevMenuEditor::GetToolkitFName() const
{
	return FName("UnrealDevMenuEditor");
}

FText FUnrealDevMenuEditor::GetBaseToolkitName() const
{
	return LOCTEXT("FUnrealDevMenuEditorAppLabel", "Dev Menu Editor");
}

FText FUnrealDevMenuEditor::GetToolkitName() const
{
	return FText::FromString(DevMenuEdited->GetName());
}

FText FUnrealDevMenuEditor::GetToolkitToolTipText() const
{
	return FAssetEditorToolkit::GetToolTipTextForObject(DevMenuEdited);
}

FLinearColor FUnrealDevMenuEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}

FString FUnrealDevMenuEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("UnrealDevMenuEditor");
}

// End of FAssetEditorToolkit

// FGCObject interface
void FUnrealDevMenuEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(DevMenuEdited);
}

// End of FGCObject interface

//~ Begin FNotifyHook Interface

void FUnrealDevMenuEditor::NotifyPostChange(
    const FPropertyChangedEvent& PropertyChangedEvent,
    FProperty*                   PropertyThatChanged)
{
}

// End of FNotifyHook

TSharedRef<SWidget> FUnrealDevMenuEditor::SpawnDetails()
{
	FPropertyEditorModule& PropertyEditorModule =
	    FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings  = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.bHideSelectionTip = true;
	DetailsViewArgs.NotifyHook        = this;
	DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

	// Set Objects
	DetailsView->SetObject(DevMenuEdited);
	//DetailsView->SetEnabled(
	//    TAttribute<bool>::Create(TAttribute<bool>::FGetter::CreateSP(
	//        this, &FUnrealDevMenuEditor::CanEditWithDebuggerActive)));

	return DetailsView.ToSharedRef();
}

#undef LOCTEXT_NAMESPACE
