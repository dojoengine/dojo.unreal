//
//  DojoModule.h
//  dojo_starter_ue5 (Mac)
//
//  Created by Corentin Cailleaud on 20/10/2024.
//  Copyright © 2024 Epic Games, Inc. All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "GameFramework/Actor.h"
#include "dojo.h"
#include "ControllerAccount.h"

using namespace dojo_bindings;

typedef void (*ControllerAccountCallback)(struct ControllerAccount*);
typedef void (*EntityUpdateCallback)(struct FieldElement, struct CArrayStruct);

class DOJO_API FDojoModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    
    static ToriiClient *CreateToriiClient(const char *torii_url, const char *world_str);
    
    static ResultCArrayEntity GetEntities(ToriiClient *client, const char *query);
    
    static struct ResultSubscription OnEntityUpdate(ToriiClient *client, const char *query_str, void *user_data, EntityUpdateCallback callback);
    
    static void ExecuteRaw(Account *account, const char *to, const char *selector, const std::vector<std::string> &feltsStr);

    static void ExecuteFromOutside(ControllerAccount *account, const char *to, const char *selector, const std::vector<std::string> &feltsStr);

    static Account *CreateAccount(const char *rpc_url, const char *address, const char *private_key);
    
    static Account *CreateBurner(const char *rpc_url, Account *master_account);
    
    static FString AccountAddress(Account *account);
    
    static void ControllerGetAccountOrConnect(const char* rpc_url, const char* chain_id, const struct Policy *policies, size_t nb_policies, ControllerAccountCallback callback);

    static void SubscriptionCancel(struct Subscription *subscription);
    
    static void AccountFree(struct Account *account);
    
    static void EntityFree(struct Entity *entity);
    
    static void ModelFree(struct Struct *model);

    static void TyFree(struct Ty *ty);

    static void CArrayFree(void *data, int len);
};
