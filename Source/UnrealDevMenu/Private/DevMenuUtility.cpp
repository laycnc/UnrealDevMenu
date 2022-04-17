// Fill out your copyright notice in the Description page of Project Settings.

#include "DevMenuUtility.h"

#include "Misc/UObjectToken.h"
#include "Logging/TokenizedMessage.h"
#include "Logging/MessageLog.h"
#if WITH_EDITOR
	#include "Kismet2\KismetEditorUtilities.h"
#endif

#define LOCTEXT_NAMESPACE "UDevMenuSubsystem"

namespace
{
	struct Local
	{
		static void OnMessageLogLinkActivated(
		    const class TSharedRef<IMessageToken>& Token)
		{
			if ( Token->GetType() == EMessageToken::Object )
			{
				const TSharedRef<FUObjectToken> UObjectToken =
				    StaticCastSharedRef<FUObjectToken>(Token);
				if ( UObjectToken->GetObject().IsValid() )
				{
#if WITH_EDITOR
					FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(
					    UObjectToken->GetObject().Get());

					// todo: アセットエディターを開けるようにする

#endif
				}
			}
		}
	};

	FText GetDisplayText(const UObject* ErrorObj)
	{
		if ( IsValid(ErrorObj) )
		{
			FString Hierarchy = ErrorObj->GetName();

			for ( UObject* Obj = ErrorObj->GetOuter(); Obj; Obj = Obj->GetOuter() )
			{
				if ( Obj )
				{
					Hierarchy =
					    FString::Printf(TEXT("%s/%s"), *Obj->GetName(), *Hierarchy);
				}
			}

			return FText::FromString(Hierarchy);
		}

		return FText::GetEmpty();
	}

} // namespace

void FDevMenuUtility::MessageLog(const UObject* ErrorObj, const FText ErrorText)
{

	// declared as its own variable since it's flushed (logs pushed to std output) on destruction
	// we want the full message constructed before it's logged
	TSharedRef<FTokenizedMessage> Message =
	    FTokenizedMessage::Create(EMessageSeverity::Error);
	Message->AddToken(FTextToken::Create(
	    LOCTEXT("RuntimeErrorMessageFmt", "DevMenu Runtime Error: ")));

	Message->AddToken(
	    FUObjectToken::Create(ErrorObj, GetDisplayText(ErrorObj))
	        ->OnMessageTokenActivated(FOnMessageTokenActivated::CreateStatic(
	            &Local::OnMessageLogLinkActivated)));

	Message->AddToken(FTextToken::Create(ErrorText));

	FMessageLog("PIE").AddMessage(Message);
}

#undef LOCTEXT_NAMESPACE