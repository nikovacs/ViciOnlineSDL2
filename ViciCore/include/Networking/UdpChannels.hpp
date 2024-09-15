#pragma once

namespace Networking {
	enum UdpChannels {
		Texture,
		Json,
		Level,
		Script,
		String,

		handshake,
		initialPlayerData,
		connectionAccepted,

		SpawnPlayer,
		DespawnPlayer,
		UpdatePlayerCameraZoom,
		UpdatePlayerPos,
		UpdatePlayerAni,
		UpdatePlayerDir,
		UpdatePlayerLevel,
		UpdatePlayerWorld,

		StartWatchingLevel,
		StopWatchingLevel,

		UpdateClientW,
		UpdateClientR,

		LoadScriptForPlayer,
		UnloadScriptForPlayer,
		
		MAX_CHANNELS
	};
}