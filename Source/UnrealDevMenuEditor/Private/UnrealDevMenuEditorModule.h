// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
struct FGraphNodeClassHelper;

class IUnrealDevMenuEditorModule : public IModuleInterface
{
public:
	// クラスキャッシュを取得する
	virtual TSharedRef<FGraphNodeClassHelper> GetMenuItemClassCache() = 0;
};
