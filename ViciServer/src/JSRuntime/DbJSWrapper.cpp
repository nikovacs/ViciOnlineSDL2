#include "ViciServer/include/JSRuntime/DbJSWrapper.hpp"
#include "ViciServer/include/ViciServer.hpp"

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

	v8::Local<v8::Value> DbResultsJSWrapper::getBigInt(std::string columnName) {
		std::optional<pqxx::field> opt{ _getValue(columnName) };
		if (!opt.has_value()) {
			return v8::Null(v8::Isolate::GetCurrent());
		}
		return v8::BigInt::New(v8::Isolate::GetCurrent(), opt.value().as<int64_t>());
	}

	v8::Local<v8::Value> DbResultsJSWrapper::getJson(std::string columnName) {
		std::optional<pqxx::field> opt{ _getValue(columnName) };
		if (!opt.has_value()) {
			return v8::Null(v8::Isolate::GetCurrent());
		}
		return v8pp::json_parse(v8::Isolate::GetCurrent(), opt.value().as<std::string>());
	}

	std::optional<pqxx::field> DbResultsJSWrapper::_getValue(std::string& columnName) {
		auto value = _current[_result.column_number("\"" + columnName + "\"")];
		if (value.is_null()) {
			return std::nullopt;
		}
		return value;
	}


	//// DbTransactionJSWrapper
	//DbTransactionJSWrapper::DbTransactionJSWrapper() {
	//	_cnx = ViciServer::instance->getDbPool().borrowConnection();
	//	_tnx = std::make_unique<pqxx::work>(*_cnx);
	//}


	//DbTransactionJSWrapper::~DbTransactionJSWrapper() {
	//	_tnx->abort();
	//	_tnx.reset();
	//	ViciServer::instance->getDbPool().returnConnection(_cnx);
	//}

	//DbResultsJSWrapper DbTransactionJSWrapper::exec(std::string sql) {
	//	return DbResultsJSWrapper(_tnx->exec(sql));
	//}

	//void DbTransactionJSWrapper::commit() {
	//	_tnx->commit();
	//}
}