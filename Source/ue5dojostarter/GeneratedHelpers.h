//
//  GeneratedHelpers.hpp
//  dojo_starter_ue5 (Mac)
//
//  Created by Corentin Cailleaud on 21/10/2024.
//  Copyright © 2024 Epic Games, Inc. All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DojoModule.h"
#include "Account.h"
#include "GeneratedHelpers.generated.h"

UCLASS(BlueprintType)
class UDojoModel : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DojoModelType;
};

UCLASS(BlueprintType)
class UDojoModelMoves : public UDojoModel
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Player;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int Remaining;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int LastDirection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool CanMove;
};

UCLASS(BlueprintType)
class UDojoModelPosition : public UDojoModel
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Player;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Vec;
};

UCLASS()
class AGeneratedHelpers : public AActor
{
    GENERATED_BODY()
    
private:
    ToriiClient *toriiClient;
    
    static const TMap<FString, FString> ContractsAddresses;
    
    bool subscribed;
    
    struct Subscription *subscription;
    
    static AGeneratedHelpers* Instance;
    
    void ControllerAccountCallback(ControllerAccount *account);

    static void ControllerCallbackProxy(ControllerAccount *account);
    
    static void CallbackProxy(struct FieldElement key, struct CArrayStruct models);

    UDojoModel* parsePositionModel(struct Struct* model);

    UDojoModel* parseMovesModel(struct Struct* model);

    void ParseModelsAndSend(struct CArrayStruct *models);
    
    void ExecuteFromOutside(const FControllerAccount& account, const FString& to, const FString& selector, const FString& calldataParameter);

    void ExecuteRawDeprecated(const FAccount& account, const FString& to, const FString& selector, const FString& calldataParameter);

public:
    AGeneratedHelpers();
    ~AGeneratedHelpers();
    
    AGeneratedHelpers* GetGlobalInstance();
    void SetGlobalInstance(AGeneratedHelpers* instance);
    
    UFUNCTION(BlueprintCallable)
    void Connect(const FString& torii_url, const FString& world);
    
    UFUNCTION(BlueprintCallable)
    void FetchExistingModels();
    
    UFUNCTION(BlueprintCallable)
    void SubscribeOnDojoModelUpdate();
    
    UFUNCTION(BlueprintCallable)
    FAccount CreateAccountDeprecated(const FString& rpc_url, const FString& address, const FString& private_key);
    
    UFUNCTION(BlueprintCallable)
    FAccount CreateBurnerDeprecated(const FString& rpc_url, const FString& address, const FString& private_key);
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDojoModelUpdated, UDojoModel*, Model);

    UPROPERTY(BlueprintAssignable)
    FOnDojoModelUpdated OnDojoModelUpdated;
    
    // EXECUTE CALL
    UFUNCTION(BlueprintCallable, Category = "Calls")
    void CallSpawn(const FAccount& account);
    
    UFUNCTION(BlueprintCallable, Category = "Calls")
    void CallMove(const FAccount& account, int direction);
    
    // CONTROLLER
    UFUNCTION(BlueprintCallable)
    void ControllerGetAccountOrConnect(const FString& rpc_url, const FString& chain_id, const FString& actions_contract);

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDojoControllerAccount, struct FControllerAccount, Account);
    
    UPROPERTY(BlueprintAssignable)
    FOnDojoControllerAccount FOnDojoControllerAccount;
   
    // EXECUTE CONTROLLER CALL
    UFUNCTION(BlueprintCallable, Category = "Calls")
    void CallControllerSpawn(const FControllerAccount& account);
    
    UFUNCTION(BlueprintCallable, Category = "Calls")
    void CallControllerMove(const FControllerAccount& account, int direction);
};
