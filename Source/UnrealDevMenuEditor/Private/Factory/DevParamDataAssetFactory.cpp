// Fill out your copyright notice in the Description page of Project Settings.

#include "Factory/DevParamDataAssetFactory.h"
#include "ParamType/DevParamDataAsset.h"

UDevParamDataAssetFactory::UDevParamDataAssetFactory(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
	bCreateNew     = true;
	bEditAfterNew  = true;
	SupportedClass = UDevParamDataAsset::StaticClass();
}

// UFactory interface
UObject* UDevParamDataAssetFactory::FactoryCreateNew(UClass*           Class,
                                                     UObject*          InParent,
                                                     FName             Name,
                                                     EObjectFlags      Flags,
                                                     UObject*          Context,
                                                     FFeedbackContext* Warn)
{
	UDevParamDataAsset* NewAsset = NewObject<UDevParamDataAsset>(
	    InParent, Class, Name, Flags | RF_Transactional);

	return NewAsset;
}

// End of UFactory interface
