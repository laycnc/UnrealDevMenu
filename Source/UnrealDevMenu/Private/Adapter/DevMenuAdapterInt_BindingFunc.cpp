// Fill out your copyright notice in the Description page of Project Settings.

#include "Adapter/DevMenuAdapterInt_BindingFunc.h"
#include "DevMenuSubsystem.h"

UDevMenuAdapterInt_BindingFunc::UDevMenuAdapterInt_BindingFunc(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
	Setter.FunctionArgsType.Add(TEXT("int32"));
	Setter.FunctionArgsType.Add(TEXT("UObject*"));

	Getter.FunctionArgsType.Add(TEXT("UObject*"));
	Getter.FunctionArgsType.Add(TEXT("int32"));
}

// 値が設定された
void UDevMenuAdapterInt_BindingFunc::SetValue_Implementation(
    UDevMenuSubsystem* InSubsystem,
    int32              NewValue) const
{
	struct FArgs
	{
		int32          NewValue;
		const UObject* WorldContext;
	};

	FArgs Args        = {};
	Args.WorldContext = InSubsystem;
	Args.NewValue     = NewValue;

	// 関数経由で実行
	Setter.Invoke(&Args);
}

// 値の取得
int32 UDevMenuAdapterInt_BindingFunc::GetValue_Implementation(
    UDevMenuSubsystem* InSubsystem) const
{

	struct FArgs
	{
		const UObject* WorldContext;
		int32          Result;
	};

	FArgs Args        = {};
	Args.WorldContext = InSubsystem;

	// イベントの実行
	Getter.Invoke(&Args);

	return Args.Result;
}