#include "DbConnectionPool.hpp"
#include <iostream>
namespace Vici {
	DbConnectionPool::DbConnectionPool(std::string_view host, int port, std::string_view db, std::string_view user, std::string_view pass, int minAvailableConnections) :
		_host{ host }, _port{ port }, _db{ db }, _user{ user }, _pass{ pass },
		_minAvailableConnections{ minAvailableConnections }
	{
		_connectionString = "host=" + _host + " port=" + std::to_string(_port) + " dbname=" + _db + " user=" + _user + " password=" + _pass;
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

	DbResultsJSWrapper DbConnectionPool::exec(std::string_view sql) {
		pqxx::connection* conn = borrowConnection();
		pqxx::nontransaction ntx(*conn);
		pqxx::result result = ntx.exec(sql);
		ntx.abort();
		returnConnection(conn);
		return result;
	}

	void DbConnectionPool::_createConnections(int count) {
		for (int i = 0; i < count; i++) {
			_availableConnections.emplace_back(std::make_unique<pqxx::connection>(_connectionString));
		}
	}
}