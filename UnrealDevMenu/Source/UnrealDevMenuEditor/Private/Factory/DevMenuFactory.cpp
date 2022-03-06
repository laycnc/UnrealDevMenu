// Fill out your copyright notice in the Description page of Project Settings.

#include "Factory/DevMenuFactory.h"
#include "DevMenu.h"

UDevMenuFactory::UDevMenuFactory(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
	bCreateNew     = true;
	bEditAfterNew  = true;
	SupportedClass = UDevMenu::StaticClass();
}

// UFactory interface
UObject* UDevMenuFactory::FactoryCreateNew(UClass*           Class,
                                           UObject*          InParent,
                                           FName             Name,
                                           EObjectFlags      Flags,
                                           UObject*          Context,
                                           FFeedbackContext* Warn)
{
	UDevMenu* NewAsset =
	    NewObject<UDevMenu>(InParent, Class, Name, Flags | RF_Transactional);

	return NewAsset;
}

// End of UFactory interface
