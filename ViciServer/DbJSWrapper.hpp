#pragma once

#include <string>
#include <v8.h>
#include <v8pp/json.hpp>
#include <pqxx/pqxx>
#include <v8pp/convert.hpp>
#include <memory>
#include <optional>
#include <iostream>

namespace Vici {
	class DbResultsJSWrapper {
	public:
		DbResultsJSWrapper(pqxx::result result);
		virtual ~DbResultsJSWrapper();
		bool next();
		bool hasNext();
		bool isEmpty();

		template <typename T>
		v8::Local<v8::Value> getValue(std::string columnName) {
			std::optional<pqxx::field> opt = _getValue(columnName);
			if (!opt.has_value()) {
				return v8::Null(v8::Isolate::GetCurrent());
			}
			return v8pp::to_v8(v8::Isolate::GetCurrent(), opt.value().as<T>());
		}
		v8::Local<v8::Value> getBigInt(std::string columnName);
		v8::Local<v8::Value> getJson(std::string columnName);
		v8::Local<v8::Value> getArray(std::string columnName);
	private:
		pqxx::result _result;
		pqxx::result::iterator _current;

		std::optional<pqxx::field> _getValue(std::string& columnName);
	};


	/*class DbTransactionJSWrapper {
	public:
		DbTransactionJSWrapper();
		virtual ~DbTransactionJSWrapper();
		DbResultsJSWrapper exec(std::string sql);
		void commit();
	private:
		std::unique_ptr<pqxx::work> _tnx{ nullptr };
		pqxx::connection* _cnx{ nullptr };
	};*/
}