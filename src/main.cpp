#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

namespace
{
	using ActivateFunc_t = bool (*)(RE::TESObjectBOOK*, RE::TESObjectREFR*, RE::TESObjectREFR*, std::uint8_t, RE::TESBoundObject*, std::int32_t);

	ActivateFunc_t OriginalActivate = nullptr;

	void SendSkillBookReadEvent(RE::TESObjectBOOK* a_book)
	{
		auto* eventSource = SKSE::GetModCallbackEventSource();
		if (!eventSource) {
			SKSE::log::warn("ModCallbackEventSource not available, cannot send skill book event.");
			return;
		}

		SKSE::ModCallbackEvent modEvent{
			.eventName = "TM_SBE_OnSkillBookRead",
			.strArg = "",
			.numArg = 0.0f,
			.sender = a_book
		};
		eventSource->SendEvent(&modEvent);

		SKSE::log::debug("Sent TM_SBE_OnSkillBookRead event for book 0x{:08X}", a_book->GetFormID());
	}

	bool Hook_Activate(
		RE::TESObjectBOOK* a_book,
		RE::TESObjectREFR* a_targetRef,
		RE::TESObjectREFR* a_activatorRef,
		std::uint8_t a_arg3,
		RE::TESBoundObject* a_object,
		std::int32_t a_targetCount)
	{
		auto& flags = a_book->data.flags;
		const bool wasSkillBook = flags.any(RE::OBJ_BOOK::Flag::kAdvancesActorValue);

		if (wasSkillBook) {
			flags.reset(RE::OBJ_BOOK::Flag::kAdvancesActorValue);
		}

		const bool result = OriginalActivate(a_book, a_targetRef, a_activatorRef, a_arg3, a_object, a_targetCount);

		if (wasSkillBook) {
			flags.set(RE::OBJ_BOOK::Flag::kAdvancesActorValue);
			SendSkillBookReadEvent(a_book);
		}

		return result;
	}

	void Install()
	{
		SKSE::log::info("Installing hooks...");

		REL::Relocation<std::uintptr_t> vtable{ RE::VTABLE_TESObjectBOOK[7] };

		OriginalActivate = REL::stl::unrestricted_cast<ActivateFunc_t>(
			vtable.write_vfunc(0, REL::stl::unrestricted_cast<std::uintptr_t>(&Hook_Activate)));

		SKSE::log::info("Hooks installed.");
	}
}

SKSEPluginInfo(
	.Version = { 1, 1, 0, 0 },
	.Name = "TM_SkillbooksEnhcancer",
	.Author = "Thu'mundus",
	.RuntimeCompatibility = SKSE::VersionIndependence::AddressLibrary
);

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse);

	Install();

	SKSE::log::info("TM_SkillbooksEnhcancer loaded.");
	return true;
}
