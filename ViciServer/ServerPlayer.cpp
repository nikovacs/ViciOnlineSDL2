#include "ServerPlayer.hpp"
#include "AssetBroker.hpp"
#include <filesystem>

namespace fs = std::filesystem;

namespace Entities {
	ServerPlayer::ServerPlayer(std::string_view username, std::string_view playerId, uint32_t id, std::string_view animation, std::string_view world,
		int dir, int x, int y, float zoom, nlohmann::json* clientW, nlohmann::json* clientR)
		: _username{ username }, _playerId{ playerId }, _connectionId{ id }, _animation{ animation }, _world{ world }, _cameraZoom{ zoom }, Entity(clientW, clientR) {
		setPosition(x, y);
		_dir = dir;
	}

	ServerPlayer::~ServerPlayer() {
		nlohmann::json playerData{};
		playerData["x"] = _x;
		playerData["y"] = _y;
		playerData["w"] = _width;
		playerData["h"] = _height;
		playerData["dir"] = _dir;
		playerData["animation"] = _animation;
		playerData["world"] = _world;
		playerData["cameraZoom"] = _cameraZoom;
		playerData["clientW"] = *_clientW.getUnderlyingJson();
		playerData["clientR"] = *_clientR.getUnderlyingJson();

		// create playerData/playerId.json if it does not exist
		std::string strPath = "playerData/" + _playerId + ".json";
		fs::path path{ strPath };
		fs::create_directories(path.remove_filename());

		std::ofstream file(strPath);
		file << playerData.dump();
		file.close();
	}

	void ServerPlayer::setLevel(std::string_view level) {
		_level = level;
	}

	std::string_view ServerPlayer::getLevel() {
		return _level;
	}

	void ServerPlayer::setCameraZoom(float zoom) {
		_cameraZoom = zoom;
	}

	float ServerPlayer::getCameraZoom() {
		return _cameraZoom;
	}
	
	void ServerPlayer::setAni(std::string_view animation) {
		_animation = animation;
	}

	std::string_view ServerPlayer::getAni() {
		return _animation;
	}

	void ServerPlayer::setWorld(std::string_view world) {
		_world = world;
	}

	std::string_view ServerPlayer::getWorld() {
		return _world;
	}

	void ServerPlayer::startWatchingLevel(std::string_view lvl) {
		_levelsWatching.insert(lvl.data());
	}

	const std::set<std::string>& ServerPlayer::getLevelsWatching() {
		return _levelsWatching;
	}

	void ServerPlayer::stopWatchingLevel(std::string_view lvl) {
		_levelsWatching.erase(lvl.data());
	}

	std::string_view ServerPlayer::getUsername() {
		return _username;
	}
}