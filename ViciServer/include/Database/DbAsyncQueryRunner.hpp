#pragma once

#include <unordered_map>
#include <vector>
#include <future>
#include <pqxx/pqxx>
#include <mutex>
#include <v8.h>
#include <memory>
#include <v8pp/context.hpp>
#include "ViciServer/include/Database/DbConnectionPool.hpp"

namespace Vici {
	class DbAsyncQueryRunner {
	public:
		DbAsyncQueryRunner(std::unique_ptr<DbConnectionPool> connectionPool);
		void processInProgress();
		v8::Local<v8::Promise> runQuery(std::string_view sql, v8::Local<v8::Context> ctx); // returns a promise that resolves to a result object
		//v8::Local<v8::Promise> beginTransaction(); // returns a promise that resolves to a transaction object
		//v8::Local<v8::Promise> runQueryInTransaction(std::string_view sql, pqxx::work& tnx); // returns a promise that resolves to a result object
	private:
		std::vector<std::pair<std::unique_ptr<std::future<pqxx::result>>, v8::Global<v8::Promise::Resolver>>> _inProgressQueries{};
		std::unordered_map<size_t, v8::Global<v8::Context>> _futureToContext{};
		std::unique_ptr<DbConnectionPool> _connectionPool{ nullptr };

		DbAsyncQueryRunner(const DbAsyncQueryRunner&) = delete;
		DbAsyncQueryRunner& operator=(const DbAsyncQueryRunner&) = delete;
		DbAsyncQueryRunner(DbAsyncQueryRunner&&) = delete;
		DbAsyncQueryRunner& operator=(DbAsyncQueryRunner&&) = delete;

		pqxx::result _runQueryAsync(std::string sql);
		//pqxx::result _runQueryInTransactionAsync(std::string_view sql, pqxx::work& tnx);
		size_t _hashFuture(const std::future<pqxx::result>& future);
	};
}