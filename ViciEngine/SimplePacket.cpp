#include "SimplePacket.hpp"

namespace Networking {
	SimplePacket::SimplePacket(ENetPacket* packet) {
		loadFromPacket(packet);
	}

	SimplePacket::~SimplePacket() {
		if (_packet) {
			enet_packet_destroy(_packet);
		}
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
			if (_packet) {
				enet_packet_destroy(_packet);
			}
			_packet = enet_packet_create(data(), size(), flag);
			_packetNeedsRecreation = false;
		}
		return _packet;
	}

	void SimplePacket::loadFromPacket(ENetPacket* packet) {
		_data.clear();
		_pos = 0;
		_packet = packet;
		_packetNeedsRecreation = false;
		const size_t length = packet->dataLength;
		for (size_t i{ 0 }; i < length; i++) {
			_data.push_back(packet->data[i]);
		}
	}
}