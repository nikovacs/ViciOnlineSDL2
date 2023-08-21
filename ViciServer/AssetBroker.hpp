#pragma once

#include <unordered_map>
#include <string>
#include <enet/enet.h>
#include <nlohmann/json.hpp>
#include "../ViciEngine/AssetTransfer.hpp"

namespace Networking {
	class AssetBroker : public AssetTransfer {
	public:
		static void sendFile(ENetEvent& event);
	};
}