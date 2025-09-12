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

// RAII wrapper for FieldElement arrays
class FFieldElementArrayWrapper
{
private:
    FieldElement* Data;
    size_t Size;

public:
    FFieldElementArrayWrapper(size_t InSize) : Size(InSize)
    {
        if (Size > 0)
        {
            Data = static_cast<FieldElement*>(FMemory::Malloc(sizeof(FieldElement) * Size));
            FMemory::Memzero(Data, sizeof(FieldElement) * Size);
        }
        else
        {
            Data = nullptr;
        }
    }

    ~FFieldElementArrayWrapper()
    {
        if (Data)
        {
            FMemory::Free(Data);
        }
    }

    // Delete copy constructor and assignment
    FFieldElementArrayWrapper(const FFieldElementArrayWrapper&) = delete;
    FFieldElementArrayWrapper& operator=(const FFieldElementArrayWrapper&) = delete;

    // Move semantics
    FFieldElementArrayWrapper(FFieldElementArrayWrapper&& Other) noexcept
        : Data(Other.Data), Size(Other.Size)
    {
        Other.Data = nullptr;
        Other.Size = 0;
    }

    FieldElement* Get() { return Data; }
    const FieldElement* Get() const { return Data; }
    size_t GetSize() const { return Size; }

    FieldElement& operator[](size_t Index)
    {
        check(Index < Size);
        return Data[Index];
    }
};

typedef void (*ControllerAccountCallback)(struct ControllerAccount*);
//typedef void (*ControllerUrlCallback)(const char *);
typedef void (*EntityUpdateCallback)(struct FieldElement, struct CArrayStruct);

class DOJO_API FDojoModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    static ToriiClient *CreateToriiClient(const char *torii_url, const char *world_str);

    static ResultPageEntity GetEntities(ToriiClient *client, int limit, const char *cursor);

    static struct ResultSubscription OnEntityUpdate(ToriiClient *client, const char *query_str, void *user_data, EntityUpdateCallback callback);

    static void ExecuteRaw(Account *account, const char *to, const char *selector, const TArray<std::string> &feltsStr);

    static void ExecuteFromOutside(ControllerAccount *account, const char *to, const char *selector, const TArray<std::string> &feltsStr);

    static Account *CreateAccount(const char *rpc_url, const char *address, const char *private_key);

    static Account *CreateBurner(const char *rpc_url, Account *master_account);

    static FString AccountAddress(Account *account);

    static FString ControllerAccountAddress(ControllerAccount *account);

    static void ControllerGetAccountOrConnect(const char* rpc_url, const char* chain_id, const struct Policy *policies, size_t nb_policies, ControllerAccountCallback callback);

//    static void ControllerGetAccountOrConnectMobile(const char* rpc_url, const char* chain_id, const struct Policy *policies, size_t nb_policies, ControllerAccountCallback callback, ControllerUrlCallback url_callback);

    static void ControllerConnect(const char* rpc_url, const struct Policy *policies, size_t nb_policies, ControllerAccountCallback callback);

//    static void ControllerConnectMobile(const char* rpc_url, const struct Policy *policies, size_t nb_policies, ControllerAccountCallback callback, ControllerUrlCallback url_callback);
//
    static void SubscriptionCancel(struct Subscription *subscription);

    static void AccountFree(struct Account *account);

    static void EntityFree(struct Entity *entity);

    static void ModelFree(struct Struct *model);

    static void TyFree(struct Ty *ty);

    static void CArrayFree(void *data, int len);

    static FString bytes_to_fstring(const uint8_t* data, size_t length, bool addPrefix = true);

    static void string_to_bytes(const std::string& hex_str, uint8_t* out_bytes, size_t max_bytes);

    static struct ResultCArrayFieldElement SerializeByteArray(const FString& str);

};
