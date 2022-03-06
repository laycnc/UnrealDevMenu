// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

class FDevMenuBindingCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	virtual void CustomizeHeader(
	    TSharedRef<IPropertyHandle>      PropertyHandle,
	    FDetailWidgetRow&                HeaderRow,
	    IPropertyTypeCustomizationUtils& CustomizationUtils) override;

	virtual void CustomizeChildren(
	    TSharedRef<IPropertyHandle>      PropertyHandle,
	    IDetailChildrenBuilder&          ChildBuilder,
	    IPropertyTypeCustomizationUtils& CustomizationUtils) override;

private:
	void CheckLibraryFunctionName(TSharedPtr<IPropertyHandle> LibraryClassProperty,
	                              TSharedPtr<IPropertyHandle> FunctionNameProperty);

private:
	TArray<TWeakObjectPtr<UField>> FunctionArgsType;
};