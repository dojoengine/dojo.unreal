//
//  DojoModule.cpp
//  dojo_starter_ue5 (Mac)
//
//  Created by Corentin Cailleaud on 20/10/2024.
//  Copyright © 2024 Epic Games, Inc. All rights reserved.
//

#include "DojoModule.h"
#include <cstring>
#include <string>
#include <iomanip>
#include <sstream>
#include <memory>

using namespace dojo_bindings;

IMPLEMENT_MODULE(FDojoModule, Dojo)

void FDojoModule::StartupModule()
{
}

void FDojoModule::ShutdownModule()
{

}

FString FDojoModule::bytes_to_fstring(const uint8_t* data, size_t length, bool addPrefix) {
    if (data == nullptr || length == 0)
        return TEXT("0x");

    FString result = addPrefix ? TEXT("0x") : TEXT("");
    for (size_t i = 0; i < length; ++i) {
        result += FString::Printf(TEXT("%02x"), data[i]);
    }
    return result;
}

void FDojoModule::string_to_bytes(const std::string& hex_str, uint8_t* out_bytes, size_t max_bytes) {
    // Skip "0x" prefix if present
    size_t start_idx = (hex_str.substr(0, 2) == "0x") ? 2 : 0;

    // Calculate actual string length without prefix
    size_t hex_length = hex_str.length() - start_idx;

    // Handle odd number of characters by assuming leading zero
    bool is_odd = hex_length % 2 != 0;
    size_t num_bytes = (hex_length + is_odd) / 2;

    // Ensure we don't overflow the output buffer
    if (num_bytes > max_bytes) {
        return;
//        throw std::runtime_error("Input hex string too long for output buffer");
    }

    size_t out_idx = 0;

    // Handle first nibble separately if we have odd number of characters
    if (is_odd) {
        std::string nibble = hex_str.substr(start_idx, 1);
        out_bytes[out_idx++] = static_cast<uint8_t>(std::stoul(nibble, nullptr, 16));
    }

    // Process two hex digits at a time
    for (size_t i = is_odd ? 1 : 0; i < hex_length; i += 2) {
        std::string byte_str = hex_str.substr(start_idx + i, 2);
        out_bytes[out_idx++] = static_cast<uint8_t>(std::stoul(byte_str, nullptr, 16));
    }
}

ToriiClient *FDojoModule::CreateToriiClient(const char *torii_url, const char *world_str)
{
    UE_LOG(LogTemp, Log, TEXT("Connecting Torii Client..."));
    UE_LOG(LogTemp, Log, TEXT("Torii URL: %hs"), torii_url);
    UE_LOG(LogTemp, Log, TEXT("World: %hs"), world_str);

    FieldElement world;
    FDojoModule::string_to_bytes(world_str, world.data, 32);

    ResultToriiClient resClient = client_new(torii_url, "/ip4/127.0.0.1/tcp/9090", world);
    if (resClient.tag == ErrToriiClient)
    {
        UE_LOG(LogTemp, Log, TEXT("Failed to create client: %hs"), resClient.err.message);
        return nullptr;
    }
    ToriiClient *client = resClient.ok;
    UE_LOG(LogTemp, Log, TEXT("Torii Client connected!"));
    return client;
}

ResultCArrayEntity FDojoModule::GetEntities(ToriiClient *client, const char *query_str)
{
    // TODO: handle query
    struct Query query;
    memset(&query, 0, sizeof(query));
    query.limit = 100;
    query.offset = 0;
    query.clause.tag = NoneClause;
    query.dont_include_hashed_keys = true;
    if (client == nullptr) {
        ResultCArrayEntity array;
        array.tag = OkCArrayEntity;
        array.ok.data_len = 0;
        return array;
    }
    return client_entities(client, &query);
}

void FDojoModule::ControllerGetAccountOrConnect(const char* rpc_url, const char* chain_id, const struct Policy *policies, size_t nb_policies, ControllerAccountCallback callback)
{
    FieldElement chain_id_felt;
    FDojoModule::string_to_bytes(chain_id, chain_id_felt.data, 32);
//    ResultControllerAccount resAccount = controller_account(policies, nb_policies, chain_id_felt);
//    if (resAccount.tag == ErrControllerAccount) {
        controller_connect(rpc_url, policies, nb_policies, callback);
        return;
//    }
//    ControllerAccount *account = resAccount.ok;
    //TODO: sometimes the account is not deployed on Slot if the Katana has been redeployed for example, need to find a way to validate the account
//    callback(account);
}

Account *FDojoModule::CreateAccount(const char* rpc_url, const char *player_address, const char *private_key_str)
{
    UE_LOG(LogTemp, Log, TEXT("EXECUTE RAW : create provider"));

    ResultProvider resProvider = provider_new(rpc_url);
    if (resProvider.tag == ErrProvider) {
        UE_LOG(LogTemp, Log, TEXT("Failed to create provider: %hs"), resProvider.err.message);
        return nullptr;
    }
    Provider *provider = resProvider.ok;

    FieldElement private_key;
    FDojoModule::string_to_bytes(private_key_str, private_key.data, 32);

    UE_LOG(LogTemp, Log, TEXT("EXECUTE RAW : create account"));

    ResultAccount resAccount = account_new(provider, private_key, player_address);
    if (resAccount.tag == ErrAccount) {
        UE_LOG(LogTemp, Log, TEXT("Failed to create account: %hs"), resAccount.err.message);
        return nullptr;
    }
    Account *account = resAccount.ok;
    provider_free(provider);
    return account;
}

Account *FDojoModule::CreateBurner(const char* rpc_url, Account *master_account)
{
    UE_LOG(LogTemp, Log, TEXT("EXECUTE RAW : Create burner"));

    UE_LOG(LogTemp, Log, TEXT("EXECUTE RAW : create provider"));

    ResultProvider resProvider = provider_new(rpc_url);
    if (resProvider.tag == ErrProvider) {
        UE_LOG(LogTemp, Log, TEXT("Failed to create provider: %hs"), resProvider.err.message);
        return nullptr;
    }
    Provider *provider = resProvider.ok;

    FieldElement burner_signer = signing_key_new();
    ResultAccount resBurner = account_deploy_burner(provider, master_account, burner_signer);
    if (resBurner.tag == ErrAccount)
    {
        UE_LOG(LogTemp, Log, TEXT("Failed to create burner: %hs"), resBurner.err.message);
        return nullptr;
    }
    Account *account = resBurner.ok;
    provider_free(provider);
    return account;
}

void FDojoModule::ExecuteRaw(Account *account, const char *to, const char *selector, const std::vector<std::string>& feltsStr)
{
    struct FieldElement actions;
    FDojoModule::string_to_bytes(to, actions.data, 32);

    struct FieldElement *felts = nullptr;
    int nbFelts = feltsStr.size();

    if (nbFelts > 0) {
        felts = reinterpret_cast<struct FieldElement *>(malloc(sizeof(*felts) * nbFelts));
        memset(felts, 0, sizeof(*felts) * nbFelts);
        for (int i = 0; i < nbFelts; i++) {
            FDojoModule::string_to_bytes(feltsStr[i].c_str(), felts[i].data, 32);
        }
    }

    struct Call call = {
        .to = actions,
        .selector = selector,
        .calldata = {
            .data_len = nbFelts,
            .data = felts
        }
    };

    account_execute_raw(account, &call, 1);

    if (felts) {
        free(felts);
    }
}

void FDojoModule::ExecuteFromOutside(ControllerAccount *account, const char *to, const char *selector, const std::vector<std::string>& feltsStr)
{
    struct FieldElement actions;
    FDojoModule::string_to_bytes(to, actions.data, 32);

    struct FieldElement *felts = nullptr;
    int nbFelts = feltsStr.size();

    if (nbFelts > 0) {
        felts = reinterpret_cast<struct FieldElement *>(malloc(sizeof(*felts) * nbFelts));
        memset(felts, 0, sizeof(*felts) * nbFelts);
        for (int i = 0; i < nbFelts; i++) {
            FDojoModule::string_to_bytes(feltsStr[i].c_str(), felts[i].data, 32);
        }
    }

    struct Call call = {
        .to = actions,
        .selector = selector,
        .calldata = {
            .data_len = nbFelts,
            .data = felts
        }
    };

    controller_execute_from_outside(account, &call, 1);

    if (felts) {
        free(felts);
    }
}

FString FDojoModule::AccountAddress(Account *account)
{
    return FDojoModule::bytes_to_fstring(account_address(account).data, 32);
}

FString FDojoModule::ControllerAccountAddress(ControllerAccount *account)
{
    return FDojoModule::bytes_to_fstring(controller_address(account).data, 32);
}

struct ResultSubscription FDojoModule::OnEntityUpdate(ToriiClient *client, const char *query_str, void *user_data, EntityUpdateCallback callback)
{
    return client_on_entity_state_update(client, nullptr, 0, callback);
}

void FDojoModule::SubscriptionCancel(struct Subscription *subscription)
{
    subscription_cancel(subscription);
}

void FDojoModule::AccountFree(struct Account *account)
{
    account_free(account);
}

void FDojoModule::EntityFree(struct Entity *entity)
{
    entity_free(entity);
}

void FDojoModule::ModelFree(struct Struct *model)
{
    model_free(model);
}

void FDojoModule::TyFree(struct Ty *ty)
{
    ty_free(ty);
}

void FDojoModule::CArrayFree(void *data, int len)
{
    carray_free(data, len);
}
