// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Toolkits/AssetEditorToolkit.h"

struct FGraphNodeClassHelper;

class IUnrealDevMenuEditorModule : public IModuleInterface
{
public:
	// メニューアイテムクラスキャッシュを取得する
	virtual TSharedRef<FGraphNodeClassHelper> GetMenuItemClassCache() = 0;
	// DevParamクラスキャッシュを取得する
	virtual TSharedRef<FGraphNodeClassHelper> GetDevParamClassCache() = 0;
	virtual TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager()
	    const = 0;
};
