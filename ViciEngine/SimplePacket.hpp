#pragma once

#include <vector>
#include <string>
#include <enet/enet.h>
#include "UdpChannels.hpp"

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
		void add(T& value) {
			for (size_t i{ 0 }; i < sizeof(T); i++) {
				_data.push_back((value >> (i * 8)) & 0xFF);
			}
			_pos += sizeof(T);
			_packetNeedsRecreation = true;
		}

		void add(const std::string& str) {
			for (const char& c : str) {
				_data.push_back(c);
			}
			_data.push_back('\0');
			_pos += str.size() + 1;
			_packetNeedsRecreation = true;
		}

		template <typename T>
		inline T get() {
			T value{ 0 };
			for (size_t i{ 0 }; i < sizeof(T); i++) {
				value |= static_cast<T>(_data[_pos + i]) << (i * 8);
			}
			_pos += sizeof(T);
			return value;
		}

		inline std::string getString() {
			std::string str{};
			while (_data[_pos] != '\0') {
				str.push_back(_data[_pos]);
				_pos++;
			}
			_pos++;
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

		std::vector<char> _data{};
		size_t _pos{ 0 };

		ENetPacket* _packet{ nullptr };
		bool _packetNeedsRecreation{ true };
	};
}