#include "DbJSWrapper.hpp"
#include <v8pp/convert.hpp>
#include "ViciServer.hpp"
#include <pqxx/pqxx>

namespace Vici {
	DbResultsJSWrapper::DbResultsJSWrapper(pqxx::result result) : _result{ result }, _current{ _result.cbegin() } {}
	DbResultsJSWrapper::~DbResultsJSWrapper() {}
	bool DbResultsJSWrapper::next() {
		if (_result.empty() || _current == _result.end()) {
			return false;
		}

		_current++;
		return _current != _result.end();
	}

	bool DbResultsJSWrapper::hasNext() {
		if (_result.empty() || _current == _result.end()) {
			return false;
		}

		return _current + 1 != _result.end();
	}

	bool DbResultsJSWrapper::isEmpty() {
		return _result.empty();
	}

	v8::Local<v8::Value> DbResultsJSWrapper::getString(std::string columnName) {
		auto value = _current[_result.column_number(columnName)];
		if (value.is_null()) {
			return v8::Null(v8::Isolate::GetCurrent());
		}
		return v8pp::to_v8(v8::Isolate::GetCurrent(), value.as<std::string>());
	}

	v8::Local<v8::Value> DbResultsJSWrapper::getInt(std::string columnName) {
		auto value = _current[_result.column_number(columnName)];
		if (value.is_null()) {
			return v8::Null(v8::Isolate::GetCurrent());
		}
		return v8pp::to_v8(v8::Isolate::GetCurrent(), value.as<int>());
	}

	v8::Local<v8::Value> DbResultsJSWrapper::getFloat(std::string columnName) {
		auto value = _current[_result.column_number(columnName)];
		if (value.is_null()) {
			return v8::Null(v8::Isolate::GetCurrent());
		}
		return v8pp::to_v8(v8::Isolate::GetCurrent(), value.as<double>());
	}

	v8::Local<v8::Value> DbResultsJSWrapper::getBool(std::string columnName) {
		auto value = _current[_result.column_number(columnName)];
		if (value.is_null()) {
			return v8::Null(v8::Isolate::GetCurrent());
		}
		return v8pp::to_v8(v8::Isolate::GetCurrent(), value.as<bool>());
	}

	v8::Local<v8::Value> DbResultsJSWrapper::getJson(std::string columnName) {
		auto value = _current[_result.column_number(columnName)];
		if (value.is_null()) {
			return v8::Null(v8::Isolate::GetCurrent());
		}
		return v8pp::json_parse(v8::Isolate::GetCurrent(), value.as<std::string>());
	}

	v8::Local<v8::Value> DbResultsJSWrapper::getArray(std::string columnName) {
		auto value = _current[_result.column_number(columnName)];
		if (value.is_null()) {
			return v8::Null(v8::Isolate::GetCurrent());
		}
		return v8pp::json_parse(v8::Isolate::GetCurrent(), value.as<std::string>()).As<v8::Array>();
	}


	// DbTransactionJSWrapper
	DbTransactionJSWrapper::DbTransactionJSWrapper() {
		_cnx = ViciServer::instance->getDbPool().borrowConnection();
		_tnx = std::make_unique<pqxx::work>(*_cnx);
	}


	DbTransactionJSWrapper::~DbTransactionJSWrapper() {
		_tnx->abort();
		_tnx.reset();
		ViciServer::instance->getDbPool().returnConnection(_cnx);
	}

	DbResultsJSWrapper DbTransactionJSWrapper::exec(std::string sql) {
		return DbResultsJSWrapper(_tnx->exec(sql));
	}

	void DbTransactionJSWrapper::commit() {
		_tnx->commit();
	}
}