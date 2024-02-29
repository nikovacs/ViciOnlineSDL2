#include "DbJSWrapper.hpp"
#include <v8pp/convert.hpp>

namespace Vici {
	DbResultsJSWrapper::DbResultsJSWrapper(pqxx::result result) : _result{ result } {}
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
		return getString(_result.column_number(columnName));
	}

	v8::Local<v8::Value> DbResultsJSWrapper::getString(int columnIndex) {
		auto value = _current[columnIndex];
		if (value.is_null()) {
			return v8::Null(v8::Isolate::GetCurrent());
		}
		return v8pp::to_v8(v8::Isolate::GetCurrent(), value.as<std::string>());
	}

	v8::Local<v8::Value> DbResultsJSWrapper::getInt(std::string columnName) {
		return getInt(_result.column_number(columnName));
	}

	v8::Local<v8::Value> DbResultsJSWrapper::getInt(int columnIndex) {
		auto value = _current[columnIndex];
		if (value.is_null()) {
			return v8::Null(v8::Isolate::GetCurrent());
		}
		return v8pp::to_v8(v8::Isolate::GetCurrent(), value.as<int>());
	}

	v8::Local<v8::Value> DbResultsJSWrapper::getFloat(std::string columnName) {
		return getFloat(_result.column_number(columnName));
	}

	v8::Local<v8::Value> DbResultsJSWrapper::getFloat(int columnIndex) {
		auto value = _current[columnIndex];
		if (value.is_null()) {
			return v8::Null(v8::Isolate::GetCurrent());
		}
		return v8pp::to_v8(v8::Isolate::GetCurrent(), value.as<double>());
	}

	v8::Local<v8::Value> DbResultsJSWrapper::getBool(std::string columnName) {
		return getBool(_result.column_number(columnName));
	}

	v8::Local<v8::Value> DbResultsJSWrapper::getBool(int columnIndex) {
		auto value = _current[columnIndex];
		if (value.is_null()) {
			return v8::Null(v8::Isolate::GetCurrent());
		}
		return v8pp::to_v8(v8::Isolate::GetCurrent(), value.as<bool>());
	}

	v8::Local<v8::Value> DbResultsJSWrapper::getJson(std::string columnName) {
		return getJson(_result.column_number(columnName));
	}

	v8::Local<v8::Value> DbResultsJSWrapper::getJson(int columnIndex) {
		auto value = _current[columnIndex];
		if (value.is_null()) {
			return v8::Null(v8::Isolate::GetCurrent());
		}
		return v8pp::json_parse(v8::Isolate::GetCurrent(), value.as<std::string>());
	}

	v8::Local<v8::Value> DbResultsJSWrapper::getArray(std::string columnName) {
		return getArray(_result.column_number(columnName));
	}

	v8::Local<v8::Value> DbResultsJSWrapper::getArray(int columnIndex) {
		auto value = _current[columnIndex];
		if (value.is_null()) {
			return v8::Null(v8::Isolate::GetCurrent());
		}
		return v8pp::json_parse(v8::Isolate::GetCurrent(), value.as<std::string>()).As<v8::Array>();
	}


	// DbTransactionJSWrapper
	DbTransactionJSWrapper::DbTransactionJSWrapper() {
		// borrow a connection from the connection pool
	}
	DbTransactionJSWrapper::~DbTransactionJSWrapper() {
		delete _tnx;
		// release the connection to the connection pool
	}

	DbResultsJSWrapper DbTransactionJSWrapper::exec(std::string sql) {
		return DbResultsJSWrapper(_tnx->exec(sql));
	}

	void DbTransactionJSWrapper::commit() {
		_tnx->commit();
	}
}