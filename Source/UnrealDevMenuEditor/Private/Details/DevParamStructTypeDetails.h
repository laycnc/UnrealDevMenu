// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class FDevParamStructTypeCustomization : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
	// Instanced で作成されたオブジェクトか？
	bool bInstancedObject = false;
	//
	TWeakObjectPtr<UObject>    OuterObject;
	TSharedPtr<FStructOnScope> DefaultValueStructData;
	TSharedPtr<FStructOnScope> SampleValueStructData;
};