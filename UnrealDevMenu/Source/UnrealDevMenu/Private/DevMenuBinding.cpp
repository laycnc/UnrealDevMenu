// Fill out your copyright notice in the Description page of Project Settings.

#include "DevMenuBinding.h"

void FDevMenuBinding::Invoke(void* InParam) const
{
	// イベントの実行
	UFunction* CallbackFunction = LibraryClass->FindFunctionByName(FunctionName);

	if ( !IsValid(CallbackFunction) )
	{
		return;
	}
	UObject* LibraryObject = const_cast<UObject*>(GetDefault<UObject>(LibraryClass));

	if ( !IsValid(LibraryObject) )
	{
		return;
	}

	// リフレクション経由で関数を呼び出す
	LibraryObject->ProcessEvent(CallbackFunction, InParam);
}