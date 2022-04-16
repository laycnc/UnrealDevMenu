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
#include "ParamType/DevParamType.h"

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
		OnChangedDevParam.Broadcast();
	}
}
void FDevParamEditor::PostRedo(bool bSuccess)
{
	if ( bSuccess )
	{
		OnChangedDevParam.Broadcast();
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
	bIsLockDetails = true;
	// プロパティが更新されたので更新
	OnChangedDevParam.Broadcast();

	bIsLockDetails = false;
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
		ToolbarBuilder.AddComboButton(
		    FUIAction(),
		    FOnGetContent::CreateRaw(this,
		                             &FDevParamEditor::GenAddDevParamMenuCategory),
		    LOCTEXT("AddIconText", "Add"),
		    LOCTEXT("AddRowToolTip", "Add a new row to the DevParam"),
		    FSlateIcon(FEditorStyle::GetStyleSetName(), "Icons.Plus"));

		ToolbarBuilder.AddToolBarButton(
		    FUIAction(
		        FExecuteAction::CreateSP(this, &FDevParamEditor::OnRemoveClicked),
		        FCanExecuteAction::CreateSP(this,
		                                    &FDevParamEditor::CanEditDevParam)),
		    NAME_None,
		    LOCTEXT("RemoveRowIconText", "Remove"),
		    LOCTEXT("RemoveRowToolTip",
		            "Remove the currently selected row from the Data Table"),
		    FSlateIcon(FEditorStyle::GetStyleSetName(), "Icons.Delete"));
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

// DevParam追加メニューを作成する
TSharedRef<SWidget> FDevParamEditor::GenAddDevParamMenuCategory()
{
	FMenuBuilder MenuBuilder(true, nullptr);

	IUnrealDevMenuEditorModule& Module =
	    FModuleManager::GetModuleChecked<IUnrealDevMenuEditorModule>(
	        TEXT("UnrealDevMenuEditor"));
	TSharedRef<FGraphNodeClassHelper> DevParamClassCache =
	    Module.GetDevParamClassCache();
	TArray<FGraphNodeClassData> NodeClasses;
	DevParamClassCache->GatherClasses(UDevParamType::StaticClass(), NodeClasses);

	const FString DefaultCategory = TEXT("Default");

	TArray<FString> Categorys;
	for ( FGraphNodeClassData& NodeClass : NodeClasses )
	{
		if ( NodeClass.IsAbstract() )
		{
			// 抽象型はスルーする
			continue;
		}

		FString CurrentCategory = NodeClass.GetCategory().ToString();
		if ( CurrentCategory.IsEmpty() )
		{
			// カテゴリが未設定の場合はDefaultにする
			CurrentCategory = DefaultCategory;
		}

		Categorys.AddUnique(CurrentCategory);
	}

	for ( const FString& Category : Categorys )
	{
		auto OnOpen = [=](FMenuBuilder& MenuBuilder)
		{
			GenAddDevParamMenu(MenuBuilder, Category);
		};

		MenuBuilder.AddSubMenu(
		    FText::FromString(Category),
		    FText::GetEmpty(),
		    FNewMenuDelegate::CreateRaw(
		        this, &FDevParamEditor::GenAddDevParamMenu, Category));
	}

	return MenuBuilder.MakeWidget();
}

// DevParam追加メニューを作成する
void FDevParamEditor::GenAddDevParamMenu(FMenuBuilder& MenuBuilder, FString Category)
{
	IUnrealDevMenuEditorModule& Module =
	    FModuleManager::GetModuleChecked<IUnrealDevMenuEditorModule>(
	        TEXT("UnrealDevMenuEditor"));
	TSharedRef<FGraphNodeClassHelper> DevParamClassCache =
	    Module.GetDevParamClassCache();
	TArray<FGraphNodeClassData> NodeClasses;
	DevParamClassCache->GatherClasses(UDevParamType::StaticClass(), NodeClasses);

	const FString DefaultCategory = TEXT("Default");

	for ( FGraphNodeClassData& NodeClass : NodeClasses )
	{
		if ( NodeClass.IsAbstract() )
		{
			// 抽象型はスルーする
			continue;
		}

		FString CurrentCategory = NodeClass.GetCategory().ToString();
		if ( CurrentCategory.IsEmpty() )
		{
			// カテゴリが未設定の場合はDefaultにする
			CurrentCategory = DefaultCategory;
		}

		// カテゴリが一致するか調べる
		if ( CurrentCategory != Category )
		{
			continue;
		}

		FString DisplayName = NodeClass.GetDisplayName();
		if ( DisplayName.IsEmpty() )
		{
			DisplayName = NodeClass.GetClassName();
		}

		if ( NodeClass.IsBlueprint() )
		{
			// ブループリントの場合は名称が長いので、短縮させる
			FString Rhs;
			if ( DisplayName.Split(FString(TEXT(".")), nullptr, &Rhs) )
			{
				DisplayName = Rhs;
			}
		}

		FText Tooltip = NodeClass.GetClass()->GetToolTipText();

		//
		MenuBuilder.AddMenuEntry(
		    FText::FromString(DisplayName),
		    Tooltip,
		    FSlateIcon(),
		    FUIAction(FExecuteAction::CreateRaw(
		        this, &FDevParamEditor::AddDevParamMenu, NodeClass)));
	}
}

// DevParam追加メニューを作成する
void FDevParamEditor::AddDevParamMenu(FGraphNodeClassData NewDevParamClassData)
{
	UClass* NewClass = NewDevParamClassData.GetClass();
	if ( NewClass && DevParamDataAssetEdited )
	{
		// 選ばれた項目を作成して追加する
		UDevParamType* NewDevParamType =
		    NewObject<UDevParamType>(DevParamDataAssetEdited, NewClass);

		DevParamDataAssetEdited->Params.Add(NewDevParamType);

		// 編集済み扱いにする
		DevParamDataAssetEdited->Modify();

		// 変更を通知
		OnChangedDevParamAsset.Broadcast();
	}
}

void FDevParamEditor::OnRemoveClicked()
{
	// 選択されている項目があれば削除を行う
	if ( DevParamDataAssetEdited->Params.Contains(SelectedItem) )
	{
		DevParamDataAssetEdited->Params.Remove(SelectedItem.Get());

		SetSelectItem(nullptr);

		// 編集済み扱いにする
		DevParamDataAssetEdited->Modify();

		// 変更を通知
		OnChangedDevParamAsset.Broadcast();
	}
}

bool FDevParamEditor::CanEditDevParam() const
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Commands

TSharedRef<FUICommandList> FDevParamEditor::GetHierarchyCommandList() const
{
	return HierarchyCommandList.ToSharedRef();
}

FSlateColor FDevParamEditor::GetDevParamColor(
    TWeakObjectPtr<UDevParamType> ViewParamType) const
{
	if ( SelectedItem.IsValid() && ViewParamType.IsValid() )
	{
		if ( SelectedItem == ViewParamType )
		{
			return FSlateColor(FColorList::Orange);
		}
	}
	return FSlateColor::UseForeground();
}

void FDevParamEditor::SetSelectItem(TWeakObjectPtr<UDevParamType> NewSelectedItem)
{
	if ( NewSelectedItem.IsValid() )
	{
		SelectedItem = NewSelectedItem;

		if ( DetailsView.IsValid() && !bIsLockDetails )
		{
			DetailsView->SetObject(SelectedItem.Get());
		}

		return;
	}

	if ( !bIsLockDetails )
	{
		DetailsView->SetObject(nullptr);
	}
	SelectedItem = nullptr;
}

UDevParamType* FDevParamEditor::GetSelectItem() const
{
	if ( SelectedItem.IsValid() )
	{
		return SelectedItem.Get();
	}
	return nullptr;
}

// コマンドの初期化
void FDevParamEditor::InitializeCommands()
{
	//
	HierarchyCommandList = MakeShared<FUICommandList>();
}

#undef LOCTEXT_NAMESPACE
