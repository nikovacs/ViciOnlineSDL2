#pragma once

namespace Networking {
	enum UdpChannels {
		Texture,
		Animation,
		Level,
		Script,
		
		initialPlayerData,
		SpawnPlayer,
		DespawnPlayer,
		UpdatePlayerPos,
		UpdatePlayerAniHard,
		UpdatePlayerAniSoft,
		UpdatePlayerDir,

		StartWatchingLevel,
		StopWatchingLevel,
		
		MAX_CHANNELS
	};
}