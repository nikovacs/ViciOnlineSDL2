#pragma once

#include <string>
#include <v8.h>
#include <v8pp/json.hpp>
#include <pqxx/pqxx>

namespace Vici {
	class DbResultsJSWrapper {
	public:
		DbResultsJSWrapper(pqxx::result result);
		virtual ~DbResultsJSWrapper();
		bool next();
		bool hasNext();
		bool isEmpty();

		v8::Local<v8::Value> getString(std::string columnName);
		v8::Local<v8::Value> getString(int columnIndex);
		v8::Local<v8::Value> getInt(std::string columnName);
		v8::Local<v8::Value> getInt(int columnIndex);
		v8::Local<v8::Value> getFloat(std::string columnName);
		v8::Local<v8::Value> getFloat(int columnIndex);
		v8::Local<v8::Value> getBool(std::string columnName);
		v8::Local<v8::Value> getBool(int columnIndex);
		v8::Local<v8::Value> getJson(std::string columnName);
		v8::Local<v8::Value> getJson(int columnIndex);
		v8::Local<v8::Value> getArray(std::string columnName);
		v8::Local<v8::Value> getArray(int columnIndex);
	private:
		pqxx::result _result;
		pqxx::result::iterator _current;
	};


	class DbTransactionJSWrapper {
	public:
		DbTransactionJSWrapper();
		virtual ~DbTransactionJSWrapper();
		DbResultsJSWrapper exec(std::string sql);
		void commit();
	private:
		pqxx::work* _tnx{ nullptr };
		pqxx::connection* _cnx{ nullptr };
	};
}