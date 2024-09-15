#pragma once

#include "ViciCore/include/Networking/SimplePacket.hpp"
#include "ViciServer/include/Entities/ServerPlayer.hpp"
#include <enet/enet.h>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>
#include <unordered_map>

namespace Networking {
class ServerPlayerManager {
public:
    ServerPlayerManager() = delete;

    /**
     * @brief Send initial player data to the newly connected player
     * This data consists of their data from their previous login,
     * or default data if they are new
     * @param peer The peer to send the data to
     * @param packet The packet containing the data
     * @return void
     */
    static void sendInitialPlayerData(ENetPeer *peer, SimplePacket &packet);

    /**
     * @brief Spawn a player for a specific player.
     * For example, if a player enters a level, they need to see
     * all the players already on that level.
     * @param idToSpawn The connection id of the player to spawn
     * @param spawnForId The connection id of the player to spawn for
     * @return void
     */
    static void spawnPlayer(uint32_t idToSpawn, uint32_t spawnForId);

    /**
     * @brief Despawn a player for a specific player.
     * For example, if a player leaves a level, they need to stop
     * seeing all the players on that level.
     * @param idToDespawn The connection id of the player to despawn
     * @param despawnForId The connection id of the player to despawn for
     * @return void
     */
    static void despawnPlayer(uint32_t idToDespawn, uint32_t despawnForId);

    /**
     * @brief Handle a player disconnecting from the server.
     * General cleanup tasks are done here.
     * @param id The connection id of the player that disconnected
     * @return void
     */
    static void onPlayerDisconnect(uint32_t id);

    /**
     * @brief Update the position of a player.
     * Notified when a player moves, forwards that information
     * to all other players that can currently see the player
     * @param id The connection id of the player to update
     * @param packet The packet containing the new position
     * @return void
     */
    static void updatePlayerPos(uint32_t id, SimplePacket &packet);

    /**
     * @brief Update the animation of a player.
     * Notified when a player changes their animation, forwards
     * that information to all other players that can currently
     * see the player
     * @param id The connection id of the player to update
     * @param packet The packet containing the new animation's name
     * @return void
     */
    static void updatePlayerAni(uint32_t id, SimplePacket &packet);

    /**
     * @brief Update the direction of a player.
     * Notified when a player changes their direction, forwards
     * that information to all other players that can currently
     * see the player
     * @param id The connection id of the player to update
     * @param packet The packet containing the new direction
     * @return void
     */
    static void updatePlayerDir(uint32_t id, SimplePacket &packet);

    /**
     * @brief Add a player to a level.
     * Notified when a player enters a level, forwards that
     * information to all other players that should now see
     * the player
     * @param id The connection id of the player to add
     * @param levelName The name of the level to add the player to
     * @return void
     */
    static void addToLevel(uint32_t id, std::string_view levelName);

    /**
     * @brief Remove a player from a level.
     * Notified when a player leaves a level, forwards that
     * information to all other players that should no longer
     * see the player
     * @param id The connection id of the player to remove
     * @param levelName The name of the level to remove the player from
     * @return void
     */
    static void removeFromLevel(uint32_t id, std::string_view levelName);

    /**
     * @brief Start watching a level.
     * Notified when a player starts watching a level.
     * Players on that level should now be visible to the player
     * @param id The connection id of the player watching the level
     * @param packet The packet containing the name of the level
     * @return void
     */
    static void startWatchingLevel(uint32_t id, SimplePacket &packet);

    /**
     * @brief Stop watching a level.
     * Notified when a player stops watching a level.
     * Players on that level should no longer be visible to the player
     * @param id The connection id of the player watching the level
     * @param packet The packet containing the name of the level
     * @return void
     */
    static void stopWatchingLevel(uint32_t id, SimplePacket &packet);

    /**
     * @brief Update the level of a player.
     * Notified when a player changes levels, forwards that
     * information to all other players that can currently see
     * the player or should now see the player
     * @param id The connection id of the player to update
     * @param packet The packet containing the new level
     * @return void
     */
    static void updatePlayerLevel(uint32_t id, SimplePacket &packet);

    /**
     * @brief Update the clientW (client-writeable) attributes
     * of a player. Notified when a player changes their clientW
     * attributes, forwards that information to all other players
     * that can currently see the player
     * @param id The connection id of the player to update
     * @param json The json containing the new clientW attributes
     * @return void
     */
    static void updatePlayerClientW(uint32_t id, nlohmann::json &json);

    /**
     * @brief Update the camera zoom of a player.
     * Notified when a player changes their camera zoom, kept
     * on the server for storage and send in the initial player
     * data on next login
     * @param id The connection id of the player to update
     * @param packet The packet containing the new camera zoom
     * @return void
     */
    static void updatePlayerCameraZoom(uint32_t id, SimplePacket &packet);

    /**
     * @brief Get the players on a level.
     * Returns a copy of the internally maintained set of players.
     * Returned vector is safe from any race conditions.
     * @param levelName The name of the level to get the players on
     * @return std::vector<uint32_t> The connection ids of the players on the level
     */
    static std::vector<uint32_t> getPlayersOnLevel(std::string_view levelName);

    /**
     * @brief Get the players watching a level.
     * Returns a copy of the internally maintained set of players.
     * Returned vector is safe from any race conditions.
     * @param levelName The name of the level to get the players watching
     * @return std::vector<uint32_t> The connection ids of the players watching the level
     */
    static std::vector<uint32_t> getPlayersWatchingLevel(std::string_view levelName);

    /**
     * @brief Get a player by their connection id.
     * Commonly used as a helper method in other methods.
     * @param playerId The connection id of the player to get
     * @return Entities::ServerPlayer* The player with the given id
     */
    static Entities::ServerPlayer *getPlayer(uint32_t playerId);

private:
    // all players on the server
    static std::unordered_map<uint32_t, std::unique_ptr<Entities::ServerPlayer>> _players;

    // all players' connection ids on the server
    static std::unordered_map<uint32_t, ENetPeer *> _peers;

    // mutex for player data
    static std::recursive_mutex _playerMutex;

    // players on each level
    static std::unordered_map<std::string, std::set<uint32_t>> _playersOnLevel;

    // players watching each level
    static std::unordered_map<std::string, std::set<uint32_t>> _playersWatchingLevel;

    /**
     * @brief Get the players on a level.
     * The returned set reference is not safe from race conditions
     * if mutexes are not properly used.
     * Commonly used as a helper method in other methods.
     * @param levelName The name of the level to get the players on
     * @return std::set<uint32_t>& The connection ids of the players on the level
     */
    static std::set<uint32_t> &_getPlayersOnLevel(std::string_view levelName);

    /**
     * @brief Get the players watching a level.
     * The returned set reference is not safe form race conditions
     * if mutexes are not properly used.
     * Commonly used as a helper method in other methods.
     * @param levelName The name of the level to get the players watching
     * @return std::set<uint32_t>& The connection ids of the players watching the level
     */
    static std::set<uint32_t> &_getPlayersWatchingLevel(std::string_view levelName);
};
} // namespace Networking