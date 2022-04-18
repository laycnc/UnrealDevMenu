// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
class UBlueprintFunctionLibrary;
#include "DevMenuBinding.generated.h"

/**
 * 
 */
USTRUCT()
struct UNREALDEVMENU_API FDevMenuBinding
{
	GENERATED_BODY();

public:
	void Invoke(void* InParam) const;

public:
	UPROPERTY(EditAnywhere, Category = "DevMenu")
	TSubclassOf<UBlueprintFunctionLibrary> LibraryClass;

	UPROPERTY(EditAnywhere, Category = "DevMenu")
	FName FunctionName;

	UPROPERTY(EditAnywhere, Category = "DevMenu")
	TArray<FString> FunctionArgsType;
};
