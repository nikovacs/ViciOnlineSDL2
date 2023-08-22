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

		StartWatchingLevel,
		StopWatchingLevel,
		
		MAX_CHANNELS
	};
}