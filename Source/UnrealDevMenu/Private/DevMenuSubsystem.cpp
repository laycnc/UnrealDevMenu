// Fill out your copyright notice in the Description page of Project Settings.

#include "DevMenuSubsystem.h"
#include "Engine/World.h"

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

void UDevMenuSubsystem::RegisterInstance(FName                 Id,
                                              UDevMenuInstanceBase* InInstance)
{
	//
	InstanceMap.Add(Id, InInstance);
}

UDevMenuInstanceBase* UDevMenuSubsystem::FindInstance(FName Id) const
{
	if ( auto* Result = InstanceMap.Find(Id) )
	{
		return *Result;
	}
	return nullptr;
}

void UDevMenuSubsystem::RegisterWindow(const UDevMenu& InWindowDevMenu)
{
	WindowDevMenus.Add(&InWindowDevMenu);
	VariableInfo.WindowVariables.FindOrAdd(InWindowDevMenu.Id);
}

FDevMenuSubWindowInfo* UDevMenuSubsystem::GetWindowVariable(FName Id)
{
	if ( auto* Result = VariableInfo.WindowVariables.Find(Id) )
	{
		return Result;
	}
	return nullptr;
}

bool UDevMenuSubsystem::GetVariable(FName Id, int32& OutValue) const
{
	if ( auto* Result = VariableInfo.IntVariables.Find(Id) )
	{
		OutValue = *Result;
		return true;
	}
	return false;
}

bool UDevMenuSubsystem::GetVariable(FName Id, float& OutValue) const
{
	if ( auto* Result = VariableInfo.FloatVariables.Find(Id) )
	{
		OutValue = *Result;
		return true;
	}
	return false;
}

bool UDevMenuSubsystem::GetVariable(FName Id, FName& OutValue) const
{
	if ( auto* Result = VariableInfo.NameVariables.Find(Id) )
	{
		OutValue = *Result;
		return true;
	}
	return false;
}

bool UDevMenuSubsystem::GetVariable(FName Id, FString& OutValue) const
{
	if ( auto* Result = VariableInfo.StringVariables.Find(Id) )
	{
		OutValue = *Result;
		return true;
	}
	return false;
}

void UDevMenuSubsystem::SetVariable(FName Id, int32 NewValue)
{
	int32& Value = VariableInfo.IntVariables.FindOrAdd(Id);
	if ( Value != NewValue )
	{
		Value = NewValue;
	}
}

void UDevMenuSubsystem::SetVariable(FName Id, float NewValue)
{
	float& Value = VariableInfo.FloatVariables.FindOrAdd(Id);
	if ( Value != NewValue )
	{
		Value = NewValue;
	}
}

void UDevMenuSubsystem::SetVariable(FName Id, FName NewValue)
{
	FName& Value = VariableInfo.NameVariables.FindOrAdd(Id);
	if ( Value != NewValue )
	{
		Value = NewValue;
	}
}

void UDevMenuSubsystem::SetVariable(FName Id, const FString& NewValue)
{
	FString& Value = VariableInfo.StringVariables.FindOrAdd(Id);
	if ( Value != NewValue )
	{
		Value = NewValue;
	}
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
