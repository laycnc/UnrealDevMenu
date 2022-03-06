// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "DevMenuFactory.generated.h"

/**
 * 
 */
UCLASS()
class UDevMenuFactory : public UFactory
{
	GENERATED_UCLASS_BODY()
	
		
	// UFactory interface
	virtual UObject* FactoryCreateNew(UClass*           Class,
	                                  UObject*          InParent,
	                                  FName             Name,
	                                  EObjectFlags      Flags,
	                                  UObject*          Context,
	                                  FFeedbackContext* Warn) override;
	// End of UFactory interface


};
