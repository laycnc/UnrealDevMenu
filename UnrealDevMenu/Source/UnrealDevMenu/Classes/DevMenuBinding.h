// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBlueprintFunctionLibrary> LibraryClass;

	UPROPERTY(EditAnywhere)
	FName FunctionName;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UField>> FunctionArgsType;
};
