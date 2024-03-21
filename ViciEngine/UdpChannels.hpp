#pragma once

namespace Networking {
	enum UdpChannels {
		Texture,
		Animation,
		Level,
		Script,
		String,

		handshake,
		initialPlayerData,
		SpawnPlayer,
		DespawnPlayer,
		UpdatePlayerCameraZoom,
		UpdatePlayerPos,
		UpdatePlayerAni,
		UpdatePlayerDir,
		UpdatePlayerLevel,
		UpdatePlayerWorld,

		LoadScript,
		UnloadScript,

		StartWatchingLevel,
		StopWatchingLevel,

		UpdateClientW,
		UpdateClientR,
		
		MAX_CHANNELS
	};
}