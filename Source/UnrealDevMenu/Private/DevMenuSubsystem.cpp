// Fill out your copyright notice in the Description page of Project Settings.

#include "DevMenuSubsystem.h"
#include "Engine/World.h"
#include "DevMenu.h"

// USubsystem implementation Begin
void UDevMenuSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

#if WITH_IMGUI

	if ( GetWorld()->WorldType == EWorldType::Game ||
	     GetWorld()->WorldType == EWorldType::PIE )
	{
		static bool bInitialize = false;
		if ( !bInitialize )
		{
			ImGuiIO& io = ImGui::GetIO();
			io.Fonts->Clear();
			io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc",
			                             18.0f,
			                             nullptr,
			                             io.Fonts->GetGlyphRangesJapanese());
			io.Fonts->Build();
			bInitialize = true;
		}

		// ImGuiのWorld Delegateに処理を登録
		FImGuiDelegates::OnWorldDebug(GetWorld())
		    .AddUObject(this, &UDevMenuSubsystem::ImGuiTick);
	}

	FImGuiModule::Get().GetProperties().SetInputEnabled(true);

#endif // WITH_IMGUI
}

void UDevMenuSubsystem::Deinitialize()
{
	Super::Deinitialize();

#if WITH_IMGUI
	if ( GetWorld()->WorldType == EWorldType::Game ||
	     GetWorld()->WorldType == EWorldType::PIE )
	{
		// ImGuiのWorld Delegateに登録した処理を解除
		FImGuiDelegates::OnWorldDebug(GetWorld()).RemoveAll(this);
	}
#endif // WITH_IMGUI
}
// USubsystem implementation End

// UTickableWorldSubsystem implementation Begin

void UDevMenuSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TStatId UDevMenuSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UDevMenu, STATGROUP_Tickables);
}

// UTickableWorldSubsystem implementation End

void UDevMenuSubsystem::InitializeMenu(UDevMenu* InMenuAsset)
{
	RootDevMenu = InMenuAsset;
	InMenuAsset->GeneratedInstance(*this);
}

void UDevMenuSubsystem::RegisterWindow(const UDevMenu& InWindowDevMenu)
{
	WindowDevMenus.Add(&InWindowDevMenu);
	WindowVariables.FindOrAdd(InWindowDevMenu.Id);
}

FDevMenuSubWindowInfo* UDevMenuSubsystem::GetWindowVariable(FName Id)
{
	if ( auto* Result = WindowVariables.Find(Id) )
	{
		return Result;
	}
	return nullptr;
}

void UDevMenuSubsystem::ImGuiTick()
{
#if WITH_IMGUI
	if ( ImGui::BeginMainMenuBar() )
	{
		RootDevMenu->UpdateMenu(*this);
		ImGui::EndMainMenuBar();
	}

	for ( auto& Window : WindowDevMenus )
	{
		if ( auto* WindowInfo = GetWindowVariable(Window->Id) )
		{
			Window->UpdateMenuWindow(*this, *WindowInfo);
		}
	}

#endif // WITH_IMGUI
}
