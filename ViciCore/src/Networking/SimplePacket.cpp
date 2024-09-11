#include <iostream>
#include "ViciCore/include/Networking/SimplePacket.hpp"

namespace Networking {
	SimplePacket::SimplePacket(ENetPacket* packet) {
		loadFromPacket(packet);
	}

	SimplePacket::~SimplePacket() {
		abandonPacket();
	}

	void SimplePacket::reset() {
		_data.clear();
		_pos = 0;
	}

	void SimplePacket::seek(size_t pos) {
		_pos = pos;
	}

	const void* SimplePacket::data() const {
		return reinterpret_cast<const void*>(_data.data());
	}

	size_t SimplePacket::size() const {
		return _data.size();
	}

	ENetPacket* SimplePacket::createEnetPacket(Networking::UdpChannels channel, ENetPacketFlag flag) {
		if (_packetNeedsRecreation) {
			abandonPacket();
			_packet = enet_packet_create(data(), size(), flag);
			++_packet->referenceCount;
			_packetNeedsRecreation = false;
		}
		return _packet;
	}

	void SimplePacket::loadFromPacket(ENetPacket* packet) {
		_data.clear();
		_pos = 0;

		_packet = packet;
		++_packet->referenceCount;
		_packetNeedsRecreation = false;

		const size_t length = packet->dataLength;
		for (size_t i{ 0 }; i < length; i++) {
			_data.push_back(packet->data[i]);
		}
	}

	void SimplePacket::abandonPacket() {
		if (!_packet) return;

		if (_packet->referenceCount == 1) {
			// if this instance of the class is the only one holding a reference to the packet, destroy it
			enet_packet_destroy(_packet);
		}
		else {
			// otherwise, just decrement the reference count
			// whatever else is holding a reference to the packet will destroy it
			--_packet->referenceCount;
		}
		_packet = nullptr;
	}
}