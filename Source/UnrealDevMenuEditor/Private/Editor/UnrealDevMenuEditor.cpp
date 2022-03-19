// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealDevMenuEditor.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "SSingleObjectDetailsPanel.h"
#include "DevMenuEditorApplicationModes.h"
#include "PropertyEditorModule.h"
#include "DetailsViewArgs.h"
#include "UnrealDevMenuEditorModule.h"
#include "AIGraphTypes.h"
#include "DevMenuItem/DevMenuItemBase.h"
#include "Framework/Commands/GenericCommands.h"
#include "Algo/AllOf.h"
#include "DevMenu.h"

#define LOCTEXT_NAMESPACE "FUnrealDevMenuEditor"

//////////////////////////////////////////////////////////////////////////

const FName UnrealDevMenuEditorAppName = FName(TEXT("UnrealDevMenuEditorApp"));

//////////////////////////////////////////////////////////////////////////

const FName FUnrealDevMenuEditorTabs::DetailsID(TEXT("Details"));
const FName FUnrealDevMenuEditorTabs::HierarchyID(TEXT("Hierarchy"));
const FName FUnrealDevMenuEditorTabs::LibraryID(TEXT("Library"));

//////////////////////////////////////////////////////////////////////////
// UnrealDevMenuEditor

/** Modes in mode switcher */
const FName FUnrealDevMenuEditor::DevMenuMode = TEXT("DevMenuMode");

void FUnrealDevMenuEditor::InitDevMenuEditor(
    const EToolkitMode::Type        Mode,
    const TSharedPtr<IToolkitHost>& InitToolkitHost,
    UDevMenu*                       InitDevMenu)
{
	//GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->CloseOtherEditors(
	//    InitDevMenu, this);

	DevMenuEdited = InitDevMenu;

	// キャッシュを作成する
	GeneratedMenuItemClasses();

	// コマンドを初期化
	InitializeCommands();

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

FUnrealDevMenuEditor::FUnrealDevMenuEditor()
{
	if ( UEditorEngine* Editor = Cast<UEditorEngine>(GEngine) )
	{
		Editor->RegisterForUndo(this);
	}
}

FUnrealDevMenuEditor ::~FUnrealDevMenuEditor()
{
	if ( UEditorEngine* Editor = Cast<UEditorEngine>(GEngine) )
	{
		Editor->UnregisterForUndo(this);
	}
}

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

// FEditorUndoClient interface
void FUnrealDevMenuEditor::PostUndo(bool bSuccess)
{
	if ( bSuccess )
	{
		OnChangedMenu.Broadcast();
	}
}
void FUnrealDevMenuEditor::PostRedo(bool bSuccess)
{
	if ( bSuccess )
	{
		OnChangedMenu.Broadcast();
	}
}
// End of FEditorUndoClient

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

void FUnrealDevMenuEditor::OnChangeHierarchyItem(UObject* NewObject)
{
	//
	if ( DetailsView.IsValid() )
	{
		DetailsView->SetObject(NewObject);
	}
}

TArray<TSharedPtr<FDevMenuItemViewModel>>& FUnrealDevMenuEditor::
    GetMenuItemViewModel()
{
	return MenuItemClasses;
}

void FUnrealDevMenuEditor::GeneratedMenuItemClasses()
{

	IUnrealDevMenuEditorModule& Module =
	    FModuleManager::GetModuleChecked<IUnrealDevMenuEditorModule>(
	        TEXT("UnrealDevMenuEditor"));
	TSharedRef<FGraphNodeClassHelper> ClassCache = Module.GetMenuItemClassCache();
	TArray<FGraphNodeClassData>       NodeClasses;
	ClassCache->GatherClasses(UDevMenuItemBase::StaticClass(), NodeClasses);

	TArray<FString> Categorys;

	// カテゴリ情報を収集する
	for ( FGraphNodeClassData& Data : NodeClasses )
	{
		FText Category = Data.GetCategory();

		// 含まれているかチェック
		if ( !Categorys.Contains(Category.ToString()) )
		{
			Categorys.Add(Category.ToString());

			// 初回だけカテゴリViewModelを作成する
			MenuItemClasses.Add(MakeShareable(
			    new FDevMenuItemCategoryViewModel(SharedThis(this), Category)));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Commands

TSharedRef<FUICommandList> FUnrealDevMenuEditor::GetHierarchyCommandList() const
{
	return HierarchyCommandList.ToSharedRef();
}

// コマンドの初期化
void FUnrealDevMenuEditor::InitializeCommands()
{
	//
	HierarchyCommandList = MakeShared<FUICommandList>();

	// delete commands
	HierarchyCommandList->MapAction(
	    FGenericCommands::Get().Delete,
	    FExecuteAction::CreateSP(this, &FUnrealDevMenuEditor::DeleteSelectItem),
	    FCanExecuteAction::CreateSP(this,
	                                &FUnrealDevMenuEditor::CanDeleteSelectItem));
}

// 選択した項目を削除
void FUnrealDevMenuEditor::DeleteSelectItem()
{
	FScopedTransaction Transaction(LOCTEXT("DeleteItem", "Delete Item"));
	bool               bDeleteItem     = false;
	const auto&        SelectedObjects = DetailsView->GetSelectedObjects();
	for ( const auto& Obj : SelectedObjects )
	{
		UDevMenuItemBase* Item = Cast<UDevMenuItemBase>(Obj.Get());
		if ( Item )
		{
			if ( DevMenuEdited->RemoveMenuItem(Item) )
			{
				bDeleteItem = true;
			}
		}
	}
	if ( bDeleteItem )
	{
		// 削除が行われたのでメニューをクリアする
		OnChangedMenu.Broadcast();
	}
	else
	{
		Transaction.Cancel();
	}
}

// 選択した項目を削除出来るか？
bool FUnrealDevMenuEditor::CanDeleteSelectItem() const
{
	if ( DetailsView.IsValid() )
	{
		const auto& SelectedObjects = DetailsView->GetSelectedObjects();

		// 選択している項目がUDevMenuItemBaseのみであるか確認する
		return Algo::AllOf(SelectedObjects,
		                   [](const auto& InObj) -> bool
		                   {
			                   return InObj->IsA(UDevMenuItemBase::StaticClass());
		                   });
	}
	return false;
}

#undef LOCTEXT_NAMESPACE
