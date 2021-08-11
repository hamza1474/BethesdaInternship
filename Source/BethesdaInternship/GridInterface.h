// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GridInterface.generated.h"

class ATile;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGridInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BETHESDAINTERNSHIP_API IGridInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Utility)
    TArray<ATile*> GetTileGridArray();
};
