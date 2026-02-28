#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
using namespace RE;

namespace SavingOnHorse {
    TESObjectCELL* g_QASmokeCell = nullptr;
    void OnGameLoaded() {
        const auto playerRef = PlayerCharacter::GetSingleton();
        if (!playerRef) {
            return;
        }
        if (!playerRef->IsOnMount()) {
            return;
        }
        RE::NiPointer<RE::Actor> mountPtr;
        if (!playerRef->GetMount(mountPtr)) {
            return;
        }
        const auto mountRef = mountPtr.get();
        if (!mountRef) {
            return;
        }
        if (!g_QASmokeCell) {
            auto* dataHandler = RE::TESDataHandler::GetSingleton();
            if (dataHandler) {
                g_QASmokeCell = dataHandler->LookupForm<RE::TESObjectCELL>(0x32AE7, "Skyrim.esm");
            }
        }
        if (!g_QASmokeCell) {
            return;
        }
        playerRef->CenterOnCell(g_QASmokeCell);
        playerRef->MoveTo(mountRef);
    }

    SKSEPluginLoad(const SKSE::LoadInterface* skse) {
        SKSE::Init(skse);
        SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* message) {
            if (message->type == SKSE::MessagingInterface::kPostLoadGame) {
                OnGameLoaded();
            }
        });
        return true;
    }
}
