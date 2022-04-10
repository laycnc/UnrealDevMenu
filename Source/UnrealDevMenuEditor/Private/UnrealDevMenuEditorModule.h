// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Toolkits/AssetEditorToolkit.h"

struct FGraphNodeClassHelper;

class IUnrealDevMenuEditorModule : public IModuleInterface
{
public:
	// クラスキャッシュを取得する
	virtual TSharedRef<FGraphNodeClassHelper> GetMenuItemClassCache() = 0;
	virtual TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager()
	    const = 0;
};
