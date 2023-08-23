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
		UpdatePlayerAni,
		UpdatePlayerDir,
		UpdatePlayerLevel,
		UpdatePlayerWorld,

		StartWatchingLevel,
		StopWatchingLevel,
		
		MAX_CHANNELS
	};
}