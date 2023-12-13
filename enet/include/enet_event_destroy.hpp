#pragma once
#include <utility>
using namespace std;

vector<string> freeCallbacks; // ?
mutex enetSafe;

void enet_event_destroy(ENetEvent event) {
	ENetPacket* packet = event.packet;
	if (packet == 0) return;
	if (packet->freeCallback != NULL) (*packet->freeCallback) (packet);
	if (not (packet->flags & ENET_PACKET_FLAG_NO_ALLOCATE) and packet->data != NULL)
	{
		if (packet->dataLength > 3u and *(packet->data) < 3) 
		{
			enetSafe.lock(); 
			freeCallbacks.emplace_back(string((char*)(packet->data + 4))); 
			enetSafe.unlock();
		}
		enet_free(packet->data);
	}
	enet_free(packet);
}