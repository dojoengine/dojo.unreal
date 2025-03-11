//
//  GeneratedHelpers.cpp
//  dojo_starter_ue5 (Mac)
//
//  Created by Corentin Cailleaud on 21/10/2024.
//  Copyright © 2024 Epic Games, Inc. All rights reserved.
//

#include "GeneratedHelpers.h"
#include <string>
#include <iomanip>
#include <sstream>
#include <memory>
#include "Async/Async.h"

const TMap<FString, FString> AGeneratedHelpers::ContractsAddresses = {
    {TEXT("actions"), TEXT("0x062f6a8b1bc8f4b45985eaff3b00f85f370d6eb8dcb038072f3bf34fba0dd855")},
};
AGeneratedHelpers* AGeneratedHelpers::Instance = nullptr;

AGeneratedHelpers::AGeneratedHelpers()
{
    Instance = this;
    subscribed = false;
}

AGeneratedHelpers::~AGeneratedHelpers()
{
    if (subscribed) {
        FDojoModule::SubscriptionCancel(subscription);
    }
}

void AGeneratedHelpers::Connect(const FString& torii_url, const FString& world)
{
    std::string torii_url_string = std::string(TCHAR_TO_UTF8(*torii_url));
    std::string world_string = std::string(TCHAR_TO_UTF8(*world));
    toriiClient = FDojoModule::CreateToriiClient(torii_url_string.c_str(), world_string.c_str());
    UE_LOG(LogTemp, Log, TEXT("Torii Client initialized."));
}

static FString bytes_to_fstring(const uint8_t* data, size_t length, bool addPrefix = true) {
    if (data == nullptr || length == 0)
        return TEXT("0x");

    FString result = addPrefix ? TEXT("0x") : TEXT("");
    for (size_t i = 0; i < length; ++i) {
        result += FString::Printf(TEXT("%02x"), data[i]);
    }
    return result;
}

FAccount AGeneratedHelpers::CreateAccountDeprecated(const FString& rpc_url, const FString& address, const FString& private_key)
{
    FAccount account;

    std::string rpc_url_string = std::string(TCHAR_TO_UTF8(*rpc_url));
    std::string address_string = std::string(TCHAR_TO_UTF8(*address));
    std::string private_key_string = std::string(TCHAR_TO_UTF8(*private_key));
    account.account = FDojoModule::CreateAccount(rpc_url_string.c_str(), address_string.c_str(), private_key_string.c_str());
    account.Address = address;
    return account;
}

FAccount AGeneratedHelpers::CreateBurnerDeprecated(const FString& rpc_url, const FString& address, const FString& private_key)
{
    FAccount account;

    std::string rpc_url_string = std::string(TCHAR_TO_UTF8(*rpc_url));
    std::string address_string = std::string(TCHAR_TO_UTF8(*address));
    std::string private_key_string = std::string(TCHAR_TO_UTF8(*private_key));
    Account *master_account = FDojoModule::CreateAccount(rpc_url_string.c_str(), address_string.c_str(), private_key_string.c_str());
    if (master_account == nullptr) {
        account.Address = UTF8_TO_TCHAR("0x0");
        return account;
    }
    account.account = FDojoModule::CreateBurner(rpc_url_string.c_str(), master_account);
    if (account.account == nullptr) {
        account.Address = UTF8_TO_TCHAR("0x0");
        return account;
    }
    account.Address = FDojoModule::AccountAddress(account.account);
    return account;
}

static void string_to_bytes(const std::string& hex_str, uint8_t* out_bytes, size_t max_bytes) {
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

void AGeneratedHelpers::ControllerGetAccountOrConnect(const FString& rpc_url, const FString& chain_id, const FString& actions_contract)
{
    std::string rpc_url_string = std::string(TCHAR_TO_UTF8(*rpc_url));
    std::string chain_id_string = std::string(TCHAR_TO_UTF8(*chain_id));
    std::string actions_contract_string = std::string(TCHAR_TO_UTF8(*actions_contract));

    FieldElement actionsContract;
    string_to_bytes(actions_contract_string, actionsContract.data, 32);
    struct Policy policies[] = {
        { actionsContract, "spawn", "Spawn" },
        { actionsContract, "move", "Move" },
    };
    int nbPolicies = 2;

    FDojoModule::ControllerGetAccountOrConnect(rpc_url_string.c_str(), chain_id_string.c_str(), policies, nbPolicies, ControllerCallbackProxy);
}

void AGeneratedHelpers::ControllerCallbackProxy(ControllerAccount *account)
{
    if (!Instance) return;
    Instance->ControllerAccountCallback(account);
}

void AGeneratedHelpers::ControllerAccountCallback(ControllerAccount *account)
{
    // Going back to Blueprint thread to broadcast the account
    Async(EAsyncExecution::TaskGraphMainThread, [this, account]() {
        FControllerAccount controllerAccount;
        controllerAccount.account = account;
        FOnDojoControllerAccount.Broadcast(controllerAccount);
    });
}

void AGeneratedHelpers::ExecuteRawDeprecated(const FAccount& account, const FString& to, const FString& selector, const FString& calldataParameter)
{
    Async(EAsyncExecution::Thread, [this, to, selector, calldataParameter]()
    {
        std::vector<std::string> felts;
        if (strcmp(TCHAR_TO_UTF8(*calldataParameter), "") != 0) {
            TArray<FString> Out;
            calldataParameter.ParseIntoArray(Out,TEXT(","),true);
            for (int i = 0; i < Out.Num(); i++) {
                std::string felt = TCHAR_TO_UTF8(*Out[i]);
                felts.push_back(felt);
            }
        }
        FDojoModule::ExecuteRaw(account, TCHAR_TO_UTF8(*to), TCHAR_TO_UTF8(*selector), felts);
    });
}

void AGeneratedHelpers::ExecuteFromOutside(const FControllerAccount& account, const FString& to, const FString& selector, const FString& calldataParameter)
{
    Async(EAsyncExecution::Thread, [this, account, to, selector, calldataParameter]()
    {
        std::vector<std::string> felts;
        if (strcmp(TCHAR_TO_UTF8(*calldataParameter), "") != 0) {
            TArray<FString> Out;
            calldataParameter.ParseIntoArray(Out,TEXT(","),true);
            for (int i = 0; i < Out.Num(); i++) {
                std::string felt = TCHAR_TO_UTF8(*Out[i]);
                felts.push_back(felt);
            }
        }
        FDojoModule::ExecuteFromOutside(account.account, TCHAR_TO_UTF8(*to), TCHAR_TO_UTF8(*selector), felts);
    });
}

void AGeneratedHelpers::FetchExistingModels()
{
    Async(EAsyncExecution::Thread, [this]()
          {
        ResultCArrayEntity resEntities = FDojoModule::GetEntities(toriiClient, "{ not used }");
        if (resEntities.tag == ErrCArrayEntity) {
            UE_LOG(LogTemp, Log, TEXT("Failed to fetch entities: %hs"), resEntities.err.message);
            return;
        }
        CArrayEntity *entities = &resEntities.ok;

        for (int i = 0; i < entities->data_len; i++) {
            CArrayStruct* models = &entities->data[i].models;
            this->ParseModelsAndSend(models);
        }
        FDojoModule::CArrayFree(entities->data, entities->data_len);
    });
}

void AGeneratedHelpers::SubscribeOnDojoModelUpdate()
{
    UE_LOG(LogTemp, Log, TEXT("Subscribing to entity update."));
    if (subscribed) {
        UE_LOG(LogTemp, Log, TEXT("Warning: cancelled, already subscribed."));
        return;
    }
    if (toriiClient == nullptr) {
        UE_LOG(LogTemp, Log, TEXT("Error: ."));
        return;
    }
    subscribed = true;
    struct ResultSubscription res = FDojoModule::OnEntityUpdate(toriiClient, "{}", nullptr, CallbackProxy);
    subscription = res.ok;
}

void AGeneratedHelpers::CallbackProxy(struct FieldElement key, struct CArrayStruct models)
{
    if (!Instance) return;
    Instance->ParseModelsAndSend(&models);
}

UDojoModel* AGeneratedHelpers::parsePositionModel(struct Struct* model)
{
    UDojoModelPosition* Model = NewObject<UDojoModelPosition>();
    CArrayMember* members = &model->children;
    
    for (int k = 0; k < members->data_len; k++) {
        Member* member = &members->data[k];
        if (strcmp("player", member->name) == 0) {
            Model->Player = bytes_to_fstring(member->ty->primitive.contract_address.data, 32);
            FDojoModule::TyFree(member->ty);
        } else if (strcmp("vec", member->name) == 0) {
            Model->Vec = FVector(member->ty->struct_.children.data[0].ty->primitive.u32, member->ty->struct_.children.data[1].ty->primitive.u32, 0);
            FDojoModule::TyFree(member->ty->struct_.children.data[0].ty);
            FDojoModule::TyFree(member->ty->struct_.children.data[1].ty);
        }
    }
    FDojoModule::CArrayFree(members->data, members->data_len);
    return Model;
}

UDojoModel* AGeneratedHelpers::parseMovesModel(struct Struct* model)
{
    UDojoModelMoves* Model = NewObject<UDojoModelMoves>();
    CArrayMember* members = &model->children;
    
    for (int k = 0; k < members->data_len; k++) {
        Member* member = &members->data[k];
        if (strcmp("player", member->name) == 0) {
            Model->Player = bytes_to_fstring(member->ty->primitive.contract_address.data, 32);
            FDojoModule::TyFree(member->ty);
        } else if (strcmp("remaining", member->name) == 0) {
            Model->Remaining = member->ty->primitive.u8;
            FDojoModule::TyFree(member->ty);
        } else if (strcmp("last_direction", member->name) == 0) {
            Model->LastDirection = 0;
            FDojoModule::TyFree(member->ty);
        } else if (strcmp("can_move", member->name) == 0) {
            Model->CanMove = member->ty->primitive.u8 == 0;
            FDojoModule::TyFree(member->ty);
        }
    }
    FDojoModule::CArrayFree(members->data, members->data_len);
    return Model;
}

void AGeneratedHelpers::ParseModelsAndSend(struct CArrayStruct* models)
{
    if (!models || !models->data)
    {
        UE_LOG(LogTemp, Warning, TEXT("ParseModelsAndSend: Invalid input models"));
        return;
    }

    TArray<UDojoModel*> ParsedModels;
    ParsedModels.Reserve(models->data_len);

    for (int32 Index = 0; Index < models->data_len; ++Index)
    {
        const char* ModelName = models->data[Index].name;
        if (!ModelName)
        {
            UE_LOG(LogTemp, Warning, TEXT("ParseModelsAndSend: Encountered null model name at index %d"), Index);
            continue;
        }

        UDojoModel* ParsedModel = nullptr;
        if (strcmp(ModelName, "dojo_starter-Position") == 0)
        {
            ParsedModel = AGeneratedHelpers::parsePositionModel(&models->data[Index]);
        }
        else if (strcmp(ModelName, "dojo_starter-Moves") == 0)
        {
            ParsedModel = AGeneratedHelpers::parseMovesModel(&models->data[Index]);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ParseModelsAndSend: Unknown model type %s"), UTF8_TO_TCHAR(ModelName));
            continue;
        }

        if (ParsedModel)
        {
            ParsedModel->DojoModelType = ModelName;
            ParsedModels.Add(ParsedModel);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ParseModelsAndSend: Failed to parse model %s"), UTF8_TO_TCHAR(ModelName));
        }
    }

    if (ParsedModels.Num() > 0)
    {
        AsyncTask(ENamedThreads::GameThread, [this, ParsedModels = MoveTemp(ParsedModels)]()
        {
            for (UDojoModel* Model : ParsedModels)
            {
                if (IsValid(Model))
                {
                    OnDojoModelUpdated.Broadcast(Model);
                }
            }
        });
    }

    // Cleanup
    if (models->data)
    {
        FDojoModule::CArrayFree(models->data, models->data_len);
    }
}

// FUNCTIONS

void AGeneratedHelpers::CallSpawn(const FAccount& account)
{
    this->ExecuteRaw(account, this->ContractsAddresses["actions"], TEXT("spawn"), TEXT(""));
}

void AGeneratedHelpers::CallMove(const FAccount& account, int direction)
{
    FString args;
    args += "0x000000000000000000000000000000000000000000000000000000000000000";
    args += FString::FromInt(direction);
    this->ExecuteRaw(account, this->ContractsAddresses["actions"], TEXT("move"), args);
}

// CONTROLLER FUNCTIONS

void AGeneratedHelpers::CallControllerSpawn(const FControllerAccount& account)
{
    this->ExecuteFromOutside(account, this->ContractsAddresses["actions"], TEXT("spawn"), TEXT(""));
}

void AGeneratedHelpers::CallControllerMove(const FControllerAccount& account, int direction)
{
    FString args;
    args += "0x000000000000000000000000000000000000000000000000000000000000000";
    args += FString::FromInt(direction);
    this->ExecuteFromOutside(account, this->ContractsAddresses["actions"], TEXT("move"), args);
}
