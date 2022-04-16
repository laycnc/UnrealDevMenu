// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class UDevParamType;
class FDevParamEditor;

struct FDevParamEditorRowListViewData
{

	TWeakObjectPtr<UDevParamType> ParamType;
};

using FDevParamEditorRowListViewDataPtr = TSharedPtr<FDevParamEditorRowListViewData>;

