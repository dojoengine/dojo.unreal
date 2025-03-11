//
//  GeneratedModels.h
//  dojo_starter_ue5 (Mac)
//
//  Created by Corentin Cailleaud on 21/10/2024.
//  Copyright © 2024 Epic Games, Inc. All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DojoModule.h"
#include "Account.generated.h"

USTRUCT(BlueprintType)
struct FAccount
{
    GENERATED_BODY()

    Account *account;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Address;
};
