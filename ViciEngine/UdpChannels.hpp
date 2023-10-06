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

		UpdatePlayerAttr,
		
		MAX_CHANNELS
	};
}