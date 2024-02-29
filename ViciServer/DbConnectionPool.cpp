#include "DbConnectionPool.hpp"

namespace Vici {
	DbConnectionPool::DbConnectionPool(std::string_view host, std::string_view db, std::string_view user, std::string_view pass, int minAvailableConnections) :
		_host{ host }, _db{ db }, _user{ user }, _pass{ pass },
		_minAvailableConnections{ minAvailableConnections }
	{
		_connectionString = "host=" + _host + " dbname=" + _db + " user=" + _user + " password=" + _pass;
		_createConnections(_minAvailableConnections);
	}

	pqxx::connection* DbConnectionPool::borrowConnection() {
		if (_availableConnections.empty()) {
			_createConnections(1);
		}

		// move a connection from available to in use
		auto conn = std::move(_availableConnections.back());
		_availableConnections.pop_back();
		_inUseConnections.push_back(std::move(conn));

		return _inUseConnections.back().get();
	}

	void DbConnectionPool::returnConnection(pqxx::connection* conn) {
		auto it = std::find_if(_inUseConnections.begin(), _inUseConnections.end(), [conn](const std::unique_ptr<pqxx::connection>& c) {
			return c.get() == conn;
		});

		if (it != _inUseConnections.end()) {
			_availableConnections.push_back(std::move(*it));
			_inUseConnections.erase(it);
		}
	}

	void DbConnectionPool::_createConnections(int count) {
		for (int i = 0; i < count; i++) {
			_availableConnections.emplace_back(std::make_unique<pqxx::connection>(_connectionString));
		}
	}
}