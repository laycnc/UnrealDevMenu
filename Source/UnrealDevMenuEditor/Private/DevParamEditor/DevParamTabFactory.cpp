#include "DevParamTabFactory.h"
#include "EditorStyleSet.h"
#include "Styling/AppStyle.h"
#include "Widgets/Docking/SDockTab.h"
#include "DevParamEditor.h"
#include "DevParamEditor/SDevParamEditor.h"

#define LOCTEXT_NAMESPACE "DevParamSummonerFactory"

//////////////////////////////////////////////////////////////////////////
// FDevParamEditorSummoner

FDevParamEditorSummoner::FDevParamEditorSummoner(
    TSharedRef<FDevParamEditor> InDevParamEditor)
    : FWorkflowTabFactory(FDevParamEditorTabs::DevParamID, InDevParamEditor)
    , DevParamEditor(InDevParamEditor)
{
	TabLabel = LOCTEXT("DevMenuDetailsLabel", "Dev Param");
	TabIcon =
	    FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details");

	bIsSingleton = true;

	ViewMenuDescription = LOCTEXT("DevParamEditView", "Dev Param Edit");
	ViewMenuTooltip     = LOCTEXT("DevParamEditView_ToolTip", "Dev Param Edit");
}

TSharedRef<SWidget> FDevParamEditorSummoner::CreateTabBody(
    const FWorkflowTabSpawnInfo& Info) const
{
	return SNew(SDevParamEditor, DevParamEditor.Pin().ToSharedRef());
}

//////////////////////////////////////////////////////////////////////////
// FDevParamDetailsSummoner

FDevParamDetailsSummoner::FDevParamDetailsSummoner(
    TSharedRef<FDevParamEditor> InDevParamEditor)
    : FWorkflowTabFactory(FDevParamEditorTabs::DetailsID, InDevParamEditor)
    , DevParamEditor(InDevParamEditor)
{
	TabLabel = LOCTEXT("DevParamDetailsLabel", "Details");
	TabIcon =
	    FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details");

	bIsSingleton = true;

	ViewMenuDescription = LOCTEXT("DevParamDetailsView", "Details");
	ViewMenuTooltip =
	    LOCTEXT("DevParamDetailsView_ToolTip", "Show the details view");
}

TSharedRef<SWidget> FDevParamDetailsSummoner::CreateTabBody(
    const FWorkflowTabSpawnInfo& Info) const
{
	check(DevParamEditor.IsValid());
	return DevParamEditor.Pin()->SpawnDetails();
}

#undef LOCTEXT_NAMESPACE
