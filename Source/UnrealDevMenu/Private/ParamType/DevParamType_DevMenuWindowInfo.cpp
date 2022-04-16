// Fill out your copyright notice in the Description page of Project Settings.

#include "ParamType/DevParamType_DevMenuWindowInfo.h"
#include "DevMenuSubsystem.h"

UDevParamType_DevMenuWindowInfo::UDevParamType_DevMenuWindowInfo(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
	StructType = FDevMenuSubWindowInfo::StaticStruct();
	DefaultValuePropertyName =
	    GET_MEMBER_NAME_CHECKED(UDevParamType_DevMenuWindowInfo, DefaultValue);
}
