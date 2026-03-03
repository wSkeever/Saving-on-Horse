#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
using namespace RE;
using namespace SKSE;

namespace SavingOnHorse {
    TESObjectCELL* g_QASmokeCell = nullptr;

    void HandlePlayer() {
        const auto playerRef = PlayerCharacter::GetSingleton();
        if (!playerRef) {
            return;
        }
        if (!playerRef->IsOnMount()) {
            return;
        }
        NiPointer<Actor> mountPtr;
        if (!playerRef->GetMount(mountPtr)) {
            return;
        }
        const auto mountRef = mountPtr.get();
        if (!mountRef) {
            return;
        }
        if (!g_QASmokeCell) {
            auto* dataHandler = TESDataHandler::GetSingleton();
            if (dataHandler) {
                g_QASmokeCell = dataHandler->LookupForm<TESObjectCELL>(0x32AE7, "Skyrim.esm");
            }
        }
        if (!g_QASmokeCell) {
            return;
        }
        playerRef->CenterOnCell(g_QASmokeCell);
        playerRef->MoveTo(mountRef);
    }

    void HandleFollowers() {
        const auto playerRef = PlayerCharacter::GetSingleton();
        if (!playerRef) {
            return;
        }
        if (!playerRef->IsOnMount()) {
            return;
        }
        const auto processLists = ProcessLists::GetSingleton();
        if (!processLists) {
            return;
        }
        const auto highActorHandles = processLists->highActorHandles;
        for (auto& actorHandle : highActorHandles) {
            if (!actorHandle) {
                continue;
            }
            const auto actorPtr = actorHandle.get();
            if (!actorPtr) {
                continue;
            }
            if (!actorPtr->IsOnMount()) {
                continue;
            }
            actorPtr->PutActorOnMountQuick();
        }
    }

    void OnGameLoaded() {
        HandleFollowers();
        HandlePlayer();
    }

    SKSEPluginLoad(const LoadInterface* skse) {
        Init(skse);
        GetMessagingInterface()->RegisterListener([](MessagingInterface::Message* message) {
            if (message->type == MessagingInterface::kPostLoadGame) {
                OnGameLoaded();
            }
        });
        return true;
    }
}
