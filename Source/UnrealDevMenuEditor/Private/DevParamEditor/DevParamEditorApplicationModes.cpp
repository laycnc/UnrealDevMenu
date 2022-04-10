#include "DevParamEditorApplicationModes.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"
#include "DevParamEditor.h"
#include "DevParamTabFactory.h"

FDevParamEditorApplicationModes::FDevParamEditorApplicationModes(
    TSharedPtr<FDevParamEditor> InDevParamEditor)
    : FApplicationMode(FDevParamEditor::DevParamMode)

{
	DevParamEditor = InDevParamEditor;

	EditorTabFactories.RegisterFactory(
	    MakeShareable(new FDevParamEditorSummoner(InDevParamEditor.ToSharedRef())));
	EditorTabFactories.RegisterFactory(
	    MakeShareable(new FDevParamDetailsSummoner(InDevParamEditor.ToSharedRef())));

	// clang-format off
	TabLayout = FTabManager::NewLayout("Standalone_DevParamEditor_Layout_v1")
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
                           ->AddTab(FDevParamEditorTabs::DevParamID, ETabState::OpenedTab)
						)
						->Split
						(
							FTabManager::NewStack()
							->SetSizeCoefficient(0.4f)
                            ->AddTab(FDevParamEditorTabs::DetailsID, ETabState::OpenedTab)
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
						)
					)
				)
			);

	// clang-format on
}

void FDevParamEditorApplicationModes::RegisterTabFactories(
    TSharedPtr<class FTabManager> InTabManager)
{
	// Mode-specific setup
	DevParamEditor.Pin()->PushTabFactories(EditorTabFactories);

	FApplicationMode::RegisterTabFactories(InTabManager);
}

void FDevParamEditorApplicationModes::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();
}

void FDevParamEditorApplicationModes::PostActivateMode()
{
	FApplicationMode::PostActivateMode();
}
