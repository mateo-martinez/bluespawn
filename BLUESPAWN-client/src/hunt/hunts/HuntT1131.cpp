#include "hunt/hunts/HuntT1131.h"
#include "hunt/RegistryHunt.h"

#include "util/log/Log.h"
#include "util/configurations/Registry.h"

using namespace Registry;

namespace Hunts {
	HuntT1131::HuntT1131() : Hunt(L"T1131 - Authentication Package") {
		dwCategoriesAffected = (DWORD) Category::Configurations;
		dwSourcesInvolved = (DWORD) DataSource::Registry;
		dwTacticsUsed = (DWORD) Tactic::Persistence;
	}

	std::vector<std::reference_wrapper<Detection>> HuntT1131::RunHunt(const Scope& scope){
		HUNT_INIT();

		auto safeAuthPackages = okAuthPackages;
		auto safeNotifPackages = okNotifPackages;
		for(auto& detection : CheckValues(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Lsa", {
			{ L"Authentication Packages", std::move(safeAuthPackages), false, CheckMultiSzSubset },
			{ L"Notification Packages", std::move(safeNotifPackages), false, CheckMultiSzSubset },
		})){
			REGISTRY_DETECTION(detection);
		}

		HUNT_END();
	}

	std::vector<std::unique_ptr<Event>> HuntT1131::GetMonitoringEvents() {
		std::vector<std::unique_ptr<Event>> events;

		events.push_back(std::make_shared<RegistryEvent>(RegistryKey{ HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Lsa" }));

		return events;
	}
}