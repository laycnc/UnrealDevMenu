// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class FDevMenuUtility
{
public:
	static UNREALDEVMENU_API void MessageLog(const UObject* ErrorObj,
	                                         const FText    ErrorText);
};
