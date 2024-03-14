#include "DbAsyncQueryRunner.hpp"
#include "ViciServer.hpp"
#include "DbJSWrapper.hpp"
#include <v8pp/convert.hpp>
#include <v8pp/class.hpp>
#include <v8pp/throw_ex.hpp>
#include <functional>

namespace Vici {
	DbAsyncQueryRunner::DbAsyncQueryRunner(std::unique_ptr<DbConnectionPool> connectionPool) : _connectionPool{ std::move(connectionPool) } {}

	void DbAsyncQueryRunner::processInProgress() {
		v8::Isolate* isolate{ ViciServer::instance->getIsolate() };
		isolate->Enter(); // Called from outside of v8, so we must manually enter the isolate

		std::vector<size_t> toRemove{};
		for (size_t i{ 0 }; i < _inProgressQueries.size(); i++) {
			auto& future = _inProgressQueries[i].first;
			auto& v8Resolver = _inProgressQueries[i].second;
			if (future->wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
				continue;
			}
			toRemove.push_back(i);

			v8::HandleScope handleScope{ isolate };
			v8::Local<v8::Context> ctx = _futureToContext[_hashFuture(*future)].Get(isolate);
			_futureToContext.erase(_hashFuture(*future));
			v8::Local<v8::Promise::Resolver> resolver = v8Resolver.Get(isolate);

			pqxx::result result;
			try {
				result = future->get();
				resolver->Resolve(ctx, v8pp::to_v8(isolate, DbResultsJSWrapper{ result }));
			}
			catch (const std::exception& e) {
				v8::Local<v8::String> v8ErrorText = v8pp::to_v8(isolate, e.what());
				v8::Local<v8::Value> v8Error = v8::Exception::Error(v8ErrorText);
				resolver->Reject(ctx, v8Error);
			}
		}

		// handle the removal of completed queries
		std::sort(toRemove.begin(), toRemove.end(), std::greater<size_t>{});
		for (size_t index : toRemove) {
			_inProgressQueries.erase(_inProgressQueries.begin() + index);
		}
		isolate->Exit();
	}

	v8::Local<v8::Promise> DbAsyncQueryRunner::runQuery(std::string_view sql, v8::Local<v8::Context> ctx) {
		v8::Context::Scope contextScope{ ctx };

		auto resolver = v8::Promise::Resolver::New(ctx).ToLocalChecked();
		auto future = std::async(std::launch::async, &DbAsyncQueryRunner::_runQueryAsync, this, sql);
		auto v8Promise = resolver->GetPromise();
		std::unique_ptr<std::future<pqxx::result>> uniqueFuture{ std::make_unique<std::future<pqxx::result>>(std::move(future)) };


		_futureToContext.emplace(_hashFuture(*uniqueFuture), v8::Global<v8::Context>{ctx->GetIsolate(), ctx});
		_inProgressQueries.push_back({ std::move(uniqueFuture), v8::Global<v8::Promise::Resolver>{ctx->GetIsolate(), resolver}});

		return v8Promise;
	}

	pqxx::result DbAsyncQueryRunner::_runQueryAsync(std::string_view sql) {
		pqxx::connection* conn = _connectionPool->borrowConnection();
		pqxx::nontransaction ntx(*conn);
		pqxx::result result = ntx.exec(sql);
		ntx.abort();
		_connectionPool->returnConnection(conn);
		return result;
	}

	size_t DbAsyncQueryRunner::_hashFuture(const std::future<pqxx::result>& future) {
		std::hash hash = std::hash<const std::future<pqxx::result>*>{};
		return hash(&future);
	}
}