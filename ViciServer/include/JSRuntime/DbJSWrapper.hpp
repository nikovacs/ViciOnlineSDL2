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
		
		template <typename T>
		v8::Local<v8::Value> getArrayOf(std::string columnName) {
			std::optional<pqxx::field> opt{ _getValue(columnName) };
			if (!opt.has_value()) {
				return v8::Null(v8::Isolate::GetCurrent());
			}
			pqxx::array_parser arr = opt.value().as_array();

			v8::EscapableHandleScope scope(v8::Isolate::GetCurrent());
			v8::Local<v8::Array> v8Arr = v8::Array::New(v8::Isolate::GetCurrent());

			for (uint32_t i = 0;; i++) {
				std::pair<pqxx::array_parser::juncture, std::string> pair{ arr.get_next() };
				if (pair.first == pqxx::array_parser::juncture::done) {
					break;
				}
				if (pair.first == pqxx::array_parser::juncture::row_start || pair.first == pqxx::array_parser::juncture::row_end) {
					i--;
					continue;
				}
				if (pair.first == pqxx::array_parser::juncture::null_value) {
					v8Arr->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), i, v8::Null(v8::Isolate::GetCurrent()));
					continue;
				}
				v8Arr->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), i, v8pp::to_v8(v8::Isolate::GetCurrent(), convertFromString<T>(pair.second)));
			}

			return scope.Escape(v8Arr);
		}

		template<>
		v8::Local<v8::Value> getArrayOf<int64_t>(std::string columnName) {
			std::optional<pqxx::field> opt{ _getValue(columnName) };
			if (!opt.has_value()) {
				return v8::Null(v8::Isolate::GetCurrent());
			}
			pqxx::array_parser arr = opt.value().as_array();
			std::vector<int64_t> vec;
			for (uint32_t i = 0;; i++) {
				std::pair<pqxx::array_parser::juncture, std::string> pair{ arr.get_next() };
				if (pair.first == pqxx::array_parser::juncture::done) {
					break;
				}
				if (pair.first == pqxx::array_parser::juncture::row_start || pair.first == pqxx::array_parser::juncture::row_end) {
					i--;
					continue;
				}
				if (pair.first == pqxx::array_parser::juncture::null_value) {
					vec.push_back(-999); // completely arbitrary value, BigInt64Array does not support null values
					continue;
				}
				vec.push_back(std::stoll(pair.second));
			}

			v8::EscapableHandleScope scope(v8::Isolate::GetCurrent());

			v8::Local<v8::ArrayBuffer> buffer = v8::ArrayBuffer::New(v8::Isolate::GetCurrent(), sizeof(int64_t)*vec.size());
			v8::Local<v8::BigInt64Array> v8Arr = v8::BigInt64Array::New(buffer, static_cast<size_t>(0), vec.size());

			for (uint32_t i = 0; i < vec.size(); i++) {
				v8Arr->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), i, v8::BigInt::New(v8::Isolate::GetCurrent(), vec.at(i)));
			}
			return scope.Escape(v8Arr);
		}

	private:
		pqxx::result _result;
		pqxx::result::iterator _current;

		std::optional<pqxx::field> _getValue(std::string& columnName);

		template <typename T>
		T convertFromString(std::string& str);

		template <>
		std::string convertFromString<std::string>(std::string& str) {
			return str;
		}

		template <>
		inline int16_t convertFromString<int16_t>(std::string& str) {
			return std::stoi(str);
		}

		template <>
		inline int32_t convertFromString<int32_t>(std::string& str) {
			return std::stoi(str);
		}

		template <>
		inline int64_t convertFromString<int64_t>(std::string& str) {
			return std::stoll(str);
		}

		template <>
		inline float convertFromString<float>(std::string& str) {
			return std::stof(str);
		}

		template <>
		inline double convertFromString<double>(std::string& str) {
			return std::stod(str);
		}

		template <>
		inline bool convertFromString<bool>(std::string& str) {
			return str == "t";
		}

		template <>
		inline v8::Local<v8::Value> convertFromString<v8::Local<v8::Value>>(std::string& str) {
			std::cout << "Converting string to json: " << str << std::endl;
			return v8pp::json_parse(v8::Isolate::GetCurrent(), str);
		}
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