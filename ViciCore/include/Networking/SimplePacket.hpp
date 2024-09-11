#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <enet/enet.h>
#include <bit>
#include "ViciCore/include/Networking/UdpChannels.hpp"

namespace Networking {
	/**
	* @brief A simple packet class that can be used to store data and send it over the network.
	*
	* This class is used to store data in a packet and send it over the network. It can store
	* any type of data and send it over the network. It can also be used to receive data from
	* the network and extract the data from the packet.
	*
	* Values must be added to the packet in the same order they are expected to be read from the packet.
	*
	* @note When constructed with an ENetPacket, the class takes ownership of the packet and will
	* destroy it when the class is destroyed or the packet is regenerated.
	*
	* @note This class should not store any pointers or references to data. That data will not be
	* and cannot be sent over the network.
	*/
	class SimplePacket {
	public:
		SimplePacket() = default;
		SimplePacket(ENetPacket* packet);
		virtual ~SimplePacket();

		SimplePacket(const SimplePacket&) = delete;
		SimplePacket(SimplePacket&&) = delete;

		template <typename T>
		void add(const T& value) {
			if constexpr (std::endian::native == std::endian::little) {
				for (int i{ sizeof(T) - 1 }; i >= 0; i--) {
					_data.push_back((value >> (i * 8)) & 0xFF);
				}
			}
			else if constexpr (std::endian::native == std::endian::big) {
				for (size_t i{ 0 }; i < sizeof(T); i++) {
					_data.push_back((value >> (i * 8)) & 0xFF);
				}
			}
			else {
				throw std::runtime_error("Endianness of this machine is not supported");
			}
			_pos += sizeof(T);
			_packetNeedsRecreation = true;
		}
	
		template <>
		void add(const float& value) {
			add(*(reinterpret_cast<const uint32_t*>(&value)));
		}
		
		template <>
		void add(const double& value) {
			add(*(reinterpret_cast<const uint64_t*>(&value)));
		}

		template <>
		void add(const std::string& str) {
			add(static_cast<uint32_t>(str.size()));
			for (const char& c : str) {
				_data.push_back(c);
			}
			_pos += str.size();
			_packetNeedsRecreation = true;
		}

		template <typename T>
		inline T get() {
			T value{ 0 };
			// data is always stored as big endian in the packet
			if constexpr (std::endian::native == std::endian::little) {
				// bytes must be read in reverse order
				char bytes[sizeof(T)]{};
				for (int i{ sizeof(T) - 1 }; i >= 0; i--) {
					bytes[sizeof(T) - 1 - i] = _data[_pos + i];
				}
				value = *reinterpret_cast<T*>(bytes);
			}
			else if constexpr (std::endian::native == std::endian::big) {
				// bytes can be read in the same order they were written
				for (size_t i{ 0 }; i < sizeof(T); i++) {
					value = *reinterpret_cast<T*>(_data.data() + _pos);
				}
			}
			else {
				throw std::runtime_error("Endianness of this machine is not supported");
			}

			_pos += sizeof(T);
			return value;
			}

		template <>
		inline float get() {
			uint32_t value = get<uint32_t>();
			return *(reinterpret_cast<float*>(&value));
		}

		template <>
		inline double get() {
			uint64_t value = get<uint64_t>();
			return *(reinterpret_cast<double*>(&value));
		}

		template <>
		inline std::string get() {
			uint32_t size = get<uint32_t>();
			std::string str{ _data.data() + _pos, size };
			_pos += size;
			return str;
		}

		void reset();
		void seek(size_t pos = 0);
		const void* data() const;
		size_t size() const;

		friend class UdpClient;
		friend class UdpServer;

	private:
		ENetPacket* createEnetPacket(Networking::UdpChannels channel, ENetPacketFlag flag);
		void loadFromPacket(ENetPacket* packet);
		void abandonPacket();

		std::vector<char> _data{};
		size_t _pos{ 0 };

		ENetPacket* _packet{ nullptr };
		bool _packetNeedsRecreation{ true };
	};
}