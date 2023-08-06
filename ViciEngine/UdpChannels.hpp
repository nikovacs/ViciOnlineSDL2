#pragma once

namespace Networking {
	enum UdpChannels {
		Texture,
		Animation,
		Level,
		Script,
		
		SpawnPlayer,
		DespawnPlayer,
		UpdatePlayerPos,
		UpdatePlayerAniHard,
		UpdatePlayerAniSoft,
		UpdatePlayerDir,
		
		MAX_CHANNELS
	};
}