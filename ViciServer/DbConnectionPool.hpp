#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <pqxx/pqxx>
#include <memory>
#include "DbJSWrapper.hpp"

namespace Vici {
	class DbConnectionPool {
	public:
		DbConnectionPool(std::string_view host, int port, std::string_view db, std::string_view user, std::string_view pass, int minAvailableConnections);
		~DbConnectionPool() = default;
		pqxx::connection* borrowConnection();
		void returnConnection(pqxx::connection* conn);
		DbResultsJSWrapper exec(std::string_view sql);
	private:
		void _createConnections(int count);

		std::string _host{};
		int _port{ 0 };
		std::string _db{};
		std::string _user{};
		std::string _pass{};
		std::string _connectionString{};
		int _minAvailableConnections{ 0 };

		std::vector<std::unique_ptr<pqxx::connection>> _availableConnections{};
		std::vector<std::unique_ptr<pqxx::connection>> _inUseConnections{};
	 };
}