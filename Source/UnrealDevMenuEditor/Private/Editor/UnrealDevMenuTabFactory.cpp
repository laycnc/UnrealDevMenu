#include "UnrealDevMenuTabFactory.h"
#include "EditorStyleSet.h"
#include "Styling/AppStyle.h"
#include "Widgets/Docking/SDockTab.h"
#include "UnrealDevMenuEditor.h"
#include "SDevMenuHierarchy.h"

//////////////////////////////////////////////////////////////////////////
// FDevMenuLibraryTabSummoner

#define LOCTEXT_NAMESPACE "FDevMenuLibraryTabSummoner"

FDevMenuLibraryTabSummoner::FDevMenuLibraryTabSummoner(
    TSharedPtr<FUnrealDevMenuEditor> InDevMenuEditor)
    : FWorkflowTabFactory(FUnrealDevMenuEditorTabs::LibraryID, InDevMenuEditor)
    , DevMenuEditor(InDevMenuEditor)
{
	TabLabel = LOCTEXT("LibraryTabLabel", "Library");
	TabIcon  = FSlateIcon(FEditorStyle::GetStyleSetName(), "Kismet.Tabs.Palette");

	bIsSingleton = true;

	ViewMenuDescription = LOCTEXT("SlateHierarchy_ViewMenu_Desc", "Hierarchy");
	ViewMenuTooltip =
	    LOCTEXT("SlateHierarchy_ViewMenu_ToolTip", "Show the Hierarchy");
}

TSharedRef<SWidget> FDevMenuLibraryTabSummoner::CreateTabBody(
    const FWorkflowTabSpawnInfo& Info) const
{
	return SNew(SBox);
}

#undef LOCTEXT_NAMESPACE

//////////////////////////////////////////////////////////////////////////
// FDevMenuHierarchyTabSummoner

#define LOCTEXT_NAMESPACE "FDevMenuHierarchyTabSummoner"

FDevMenuHierarchyTabSummoner::FDevMenuHierarchyTabSummoner(
    TSharedPtr<FUnrealDevMenuEditor> InDevMenuEditor)
    : FWorkflowTabFactory(FUnrealDevMenuEditorTabs::HierarchyID, InDevMenuEditor)
    , DevMenuEditor(InDevMenuEditor)
{
	TabLabel = LOCTEXT("SlateHierarchyTabLabel", "Hierarchy");
	TabIcon  = FSlateIcon(FEditorStyle::GetStyleSetName(), "Kismet.Tabs.Palette");

	bIsSingleton = true;

	ViewMenuDescription = LOCTEXT("SlateHierarchy_ViewMenu_Desc", "Hierarchy");
	ViewMenuTooltip =
	    LOCTEXT("SlateHierarchy_ViewMenu_ToolTip", "Show the Hierarchy");
}

TSharedRef<SWidget> FDevMenuHierarchyTabSummoner::CreateTabBody(
    const FWorkflowTabSpawnInfo& Info) const
{
	return SNew(SDevMenuHierarchyView, DevMenuEditor.Pin())
	    .OnChangeHierarchyItem(DevMenuEditor.Pin().Get(),
	                           &FUnrealDevMenuEditor::OnChangeHierarchyItem);
}

#undef LOCTEXT_NAMESPACE

//////////////////////////////////////////////////////////////////////////
// FDevMenuDetailsSummoner

#define LOCTEXT_NAMESPACE "FDevMenuDetailsSummoner"

FDevMenuDetailsSummoner::FDevMenuDetailsSummoner(
    TSharedPtr<FUnrealDevMenuEditor> InDevMenuEditor)
    : FWorkflowTabFactory(FUnrealDevMenuEditorTabs::DetailsID, InDevMenuEditor)
    , DevMenuEditor(InDevMenuEditor)
{
	TabLabel = LOCTEXT("DevMenuDetailsLabel", "Details");
	TabIcon =
	    FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details");

	bIsSingleton = true;

	ViewMenuDescription = LOCTEXT("DevMenuDetailsView", "Details");
	ViewMenuTooltip = LOCTEXT("DevMenuDetailsView_ToolTip", "Show the details view");
}

TSharedRef<SWidget> FDevMenuDetailsSummoner::CreateTabBody(
    const FWorkflowTabSpawnInfo& Info) const
{
	check(DevMenuEditor.IsValid());
	return DevMenuEditor.Pin()->SpawnDetails();
}

#undef LOCTEXT_NAMESPACE
