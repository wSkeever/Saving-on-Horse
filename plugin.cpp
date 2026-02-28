#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include <unordered_set>
using namespace RE;

namespace SavingOnHorse {

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
        playerRef->CenterOnCell("QASmoke");
        playerRef->MoveTo(mountRef);
    }

    SKSEPluginLoad(const SKSE::LoadInterface* skse) {
        SKSE::Init(skse);
        SKSE::AllocTrampoline(14);
        SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* message) {
            if (message->type == SKSE::MessagingInterface::kPostLoadGame) {
                OnGameLoaded();
            }
        });
        return true;
    }
}
