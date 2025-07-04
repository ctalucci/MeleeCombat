// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include <Modules/ModuleManager.h>
#include <CoreMinimal.h>

class FAnimStateTracingModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};