// Fill out your copyright notice in the Description page of Project Settings.

#include "DevMenuItem/Input/DevMenuInputText.h"
#include "DevMenuSubsystem.h"
#include "Adapter/DevMenuAdapterString.h"
#include "DevMenuUtility.h"

UDevMenuInputText::UDevMenuInputText(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

// メニューの更新処理
void UDevMenuInputText::UpdateMenu(UDevMenuSubsystem& InSubsystem) const
{
#if WITH_IMGUI

	if ( !IsValid(TargetValue) )
	{
		// TargetValueが未設定なのでエラーを出して終了
		FDevMenuUtility::MessageLog(this,
		                            FText::FromString("TargetValue : invalid"));
		ImGui::Text("[Error]");

		return;
	}

	// 現在の値を取得する
	char                    Buffer[128]  = "[Error]";
	static constexpr size_t BufferSize   = IM_ARRAYSIZE(Buffer);
	FString                 CurrentValue = TargetValue->GetValue(&InSubsystem);
	FTCHARToUTF8            Conv(*CurrentValue);

	if ( BufferSize >= Conv.Length() )
	{
		FMemory::Memcpy(Buffer, Conv.Get(), Conv.Length());
	}

	if ( ImGui::InputText(TCHAR_TO_UTF8(*Label.ToString()), Buffer, BufferSize) )
	{
		// 値の更新があったので反映
		TargetValue->SetValue(&InSubsystem, UTF8_TO_TCHAR(Buffer));
	}

	if ( ImGui::IsItemHovered() )
	{
		// ホバー中なのでツールチップを表示する
		if ( !Tooltip.IsEmpty() )
		{
			ImGui::SetTooltip(TCHAR_TO_UTF8(*Tooltip.ToString()));
		}
	}

#endif
}
