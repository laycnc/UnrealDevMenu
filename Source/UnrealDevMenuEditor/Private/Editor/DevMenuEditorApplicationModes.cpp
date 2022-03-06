#include "DevMenuEditorApplicationModes.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"
#include "UnrealDevMenuEditor.h"
#include "UnrealDevMenuTabFactory.h"

FDevMenuEditorApplicationModes::FDevMenuEditorApplicationModes(
    TSharedPtr<FUnrealDevMenuEditor> InDevMenuEditor)
    : FApplicationMode(FUnrealDevMenuEditor::DevMenuMode)

{
	DevMenuEditor = InDevMenuEditor;

	EditorTabFactories.RegisterFactory(
	    MakeShareable(new FDevMenuLibraryTabSummoner(InDevMenuEditor)));
	EditorTabFactories.RegisterFactory(
	    MakeShareable(new FDevMenuHierarchyTabSummoner(InDevMenuEditor)));
	EditorTabFactories.RegisterFactory(
	    MakeShareable(new FDevMenuDetailsSummoner(InDevMenuEditor)));

	// clang-format off
	TabLayout = FTabManager::NewLayout("Standalone_UnrealDevMenuEditor_Layout_v2")
			->AddArea
			(
				FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
				->Split
				(
					FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
					->SetSizeCoefficient(0.9f)
					->Split
					(
						FTabManager::NewSplitter() ->SetOrientation(Orient_Vertical)
						->SetSizeCoefficient(0.3f)
						->Split
						(
							FTabManager::NewStack()
							->SetSizeCoefficient(0.6f)
                           ->AddTab(FUnrealDevMenuEditorTabs::LibraryID, ETabState::OpenedTab)
						)
						->Split
						(
							FTabManager::NewStack()
							->SetSizeCoefficient(0.4f)
                            ->AddTab(FUnrealDevMenuEditorTabs::HierarchyID, ETabState::OpenedTab)
						)
					)
					->Split
					(
						FTabManager::NewSplitter() ->SetOrientation(Orient_Vertical)
						->SetSizeCoefficient(0.3f)
						->Split
						(
							FTabManager::NewStack()
							->SetSizeCoefficient(0.6f)
						)
						->Split
						(
							FTabManager::NewStack()
							->SetSizeCoefficient(0.4f)
                            ->AddTab(FUnrealDevMenuEditorTabs::DetailsID, ETabState::OpenedTab)
						)
					)
				)
			);

	// clang-format on
}

void FDevMenuEditorApplicationModes::RegisterTabFactories(
    TSharedPtr<class FTabManager> InTabManager)
{
	// Mode-specific setup
	DevMenuEditor.Pin()->PushTabFactories(EditorTabFactories);

	FApplicationMode::RegisterTabFactories(InTabManager);
}

void FDevMenuEditorApplicationModes::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();
}

void FDevMenuEditorApplicationModes::PostActivateMode()
{
	FApplicationMode::PostActivateMode();
}
