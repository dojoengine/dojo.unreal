//
//  ControllerAccount.h
//  ue5dojostarter (Mac)
//
//  Created by Corentin Cailleaud on 24/02/2025.
//  Copyright © 2025 Epic Games, Inc. All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DojoModule.h"
#include "ControllerAccount.generated.h"

USTRUCT(BlueprintType)
struct FControllerAccount
{
    GENERATED_BODY()

    dojo_bindings::ControllerAccount *account;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Address;
};
