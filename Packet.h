#pragma once
struct gamepacket_t
{
private:
	int index = 0, len = 0;
	BYTE* packet_data = new BYTE[61];
public:
	gamepacket_t(int delay = 0, int NetID = -1) {
		len = 61;
		int MessageType = 0x4, PacketType = 0x1, CharState = 0x8;
		memset(packet_data, 0, 61);
		memcpy(packet_data, &MessageType, 4);
		memcpy(packet_data + 4, &PacketType, 4);
		memcpy(packet_data + 8, &NetID, 4);
		memcpy(packet_data + 16, &CharState, 4);
		memcpy(packet_data + 24, &delay, 4);
	};
	~gamepacket_t() {
		delete[] packet_data;
	}
	void Insert(string a) {
		BYTE* data = new BYTE[len + 2 + a.length() + 4];
		memcpy(data, packet_data, len);
		delete[] packet_data;
		packet_data = data;
		data[len] = index;
		data[len + 1] = 0x2;
		int str_len = (int)a.length();
		memcpy(data + len + 2, &str_len, 4);
		memcpy(data + len + 6, a.data(), str_len);
		len = len + 2 + (int)a.length() + 4;
		index++;
		packet_data[60] = index;
	}
	void Insert(int a) {
		BYTE* data = new BYTE[len + 2 + 4];
		memcpy(data, packet_data, len);
		delete[] packet_data;
		packet_data = data;
		data[len] = index;
		data[len + 1] = 0x9;
		memcpy(data + len + 2, &a, 4);
		len = len + 2 + 4;
		index++;
		packet_data[60] = index;
	}
	void Insert(unsigned int a) {
		BYTE* data = new BYTE[len + 2 + 4];
		memcpy(data, packet_data, len);
		delete[] packet_data;
		packet_data = data;
		data[len] = index;
		data[len + 1] = 0x5;
		memcpy(data + len + 2, &a, 4);
		len = len + 2 + 4;
		index++;
		packet_data[60] = index;
	}
	void Insert(float a) {
		BYTE* data = new BYTE[len + 2 + 4];
		memcpy(data, packet_data, len);
		delete[] packet_data;
		packet_data = data;
		data[len] = index;
		data[len + 1] = 0x1;
		memcpy(data + len + 2, &a, 4);
		len = len + 2 + 4;
		index++;
		packet_data[60] = index;
	}
	void Insert(float a, float b) {
		BYTE* data = new BYTE[len + 2 + 8];
		memcpy(data, packet_data, len);
		delete[] packet_data;
		packet_data = data;
		data[len] = index;
		data[len + 1] = 0x3;
		memcpy(data + len + 2, &a, 4);
		memcpy(data + len + 6, &b, 4);
		len = len + 2 + 8;
		index++;
		packet_data[60] = index;
	}
	void Insert(float a, float b, float c) {
		BYTE* data = new BYTE[len + 2 + 12];
		memcpy(data, packet_data, len);
		delete[] packet_data;
		packet_data = data;
		data[len] = index;
		data[len + 1] = 0x4;
		memcpy(data + len + 2, &a, 4);
		memcpy(data + len + 6, &b, 4);
		memcpy(data + len + 10, &c, 4);
		len = len + 2 + 12;
		index++;
		packet_data[60] = index;
	}
	void CreatePacket(ENetPeer* peer) {
		ENetPacket* packet = enet_packet_create(packet_data, len, 1);
		enet_peer_send(peer, 0, packet);
	}
};

PlayerMoving* unpackPlayerMoving(BYTE* data) {
	PlayerMoving* dataStruct = new PlayerMoving;
	memcpy(&dataStruct->packetType, data, 4);
	memcpy(&dataStruct->netID, data + 4, 4);
	memcpy(&dataStruct->characterState, data + 12, 4);
	memcpy(&dataStruct->plantingTree, data + 20, 4);
	memcpy(&dataStruct->x, data + 24, 4);
	memcpy(&dataStruct->y, data + 28, 4);
	memcpy(&dataStruct->XSpeed, data + 32, 4);
	memcpy(&dataStruct->YSpeed, data + 36, 4);
	memcpy(&dataStruct->punchX, data + 44, 4);
	memcpy(&dataStruct->punchY, data + 48, 4);
	return dataStruct;
}
BYTE* get_struct(ENetPacket* packet) {
	const unsigned int packetLenght = (unsigned int)packet->dataLength;
	BYTE* result = nullptr;
	if (packetLenght >= 0x3C) {
		BYTE* packetData = packet->data;
		result = packetData + 4;
		if (*static_cast<BYTE*>(packetData + 16) & 8) {
			if (packetLenght < (unsigned int)*reinterpret_cast<int*>(packetData + 56) + 60)
				result = nullptr;
		}
		else {
			int zero = 0;
			memcpy(packetData + 56, &zero, 4);
		}
	}
	return result;
}

void SendPacketRaw112(int a1, void* packetData, size_t packetDataSize, void* a4, ENetPeer* peer, int packetFlag)
{
	ENetPacket* p;

	if (peer) // check if we have it setup
	{
		if (a1 == 4 && *((BYTE*)packetData + 12) & 8)
		{
			p = enet_packet_create(0, packetDataSize + *((DWORD*)packetData + 13) + 5, packetFlag);
			int four = 4;
			memcpy(p->data, &four, 4);
			memcpy((char*)p->data + 4, packetData, packetDataSize);
			memcpy((char*)p->data + packetDataSize + 4, a4, *((DWORD*)packetData + 13));
			enet_peer_send(peer, 0, p);
		}
		else
		{
			p = enet_packet_create(0, packetDataSize + 5, packetFlag);
			memcpy(p->data, &a1, 4);
			memcpy((char*)p->data + 4, packetData, packetDataSize);
			enet_peer_send(peer, 0, p);
		}
	}
	delete (char*)packetData;
}
void SendPacketRaw1(int a1, void* packetData, size_t packetDataSize, void* a4, ENetPeer* peer, int packetFlag, int delay)
{
	ENetPacket* p;

	if (peer) // check if we have it setup
	{
		if (a1 == 4 && *((BYTE*)packetData + 12) & 8)
		{

			p = enet_packet_create(0, packetDataSize + *((DWORD*)packetData + 13) + 5, packetFlag);
			int four = 4;
			memcpy(p->data, &four, 4);
			memcpy((char*)p->data + 4, packetData, packetDataSize);
			memcpy((char*)p->data + packetDataSize + 4, a4, *((DWORD*)packetData + 13));
			int deathFlag = 0x19;
			memcpy(p->data + 24, &delay, 4);
			memcpy(p->data + 56, &deathFlag, 4);
			enet_peer_send(peer, 0, p);
		}
		else
		{
			p = enet_packet_create(0, packetDataSize + 5, packetFlag);
			memcpy(p->data, &a1, 4);
			memcpy((char*)p->data + 4, packetData, packetDataSize);
			int deathFlag = 0x19;
			memcpy(p->data + 24, &delay, 4);
			memcpy(p->data + 56, &deathFlag, 4);
			enet_peer_send(peer, 0, p);
		}
	}
	delete (char*)packetData;
}
namespace variants {
	void barrel(ENetPeer* peer, int netid, int x, int y, int delay) {
		PlayerMoving data;
		data.packetType = 17;
		data.netID = netid;
		data.x = x;
		data.y = y;
		data.characterState = 0;
		data.plantingTree = 0;
		data.XSpeed = 4;
		data.YSpeed = 1;
		data.punchX = 0;
		data.punchY = 0;
		SendPacketRaw1(4, packPlayerMoving(&data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE, delay);
	}
	void CrashTheGameClient(ENetPeer* peer) {
		gamepacket_t p;
		p.Insert("CrashTheGameClient");
		p.CreatePacket(peer);
	}
	void OnRequestWorldSelectMenu(ENetPeer* peer, string output) {
		gamepacket_t p;
		p.Insert("OnRequestWorldSelectMenu");
		p.Insert(output);
		p.CreatePacket(peer);
	}
	void OnParticleEffect(ENetPeer* peer, float x, float y, int id, bool all = false, string name = "", int delay = 0) {
		gamepacket_t p(delay);
		p.Insert("OnParticleEffect");
		p.Insert(id);
		p.Insert(x + 5, y + 5);
		if (all) {
			for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer)
			{
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
				if (pInfo(currentPeer)->world == name) {
					p.CreatePacket(currentPeer);
				}
			}
		}
		else p.CreatePacket(peer);
	}
	void OnSetPos(ENetPeer* peer, int netID, float x, float y, int delay = 0) {
		PlayerMoving data;
		data.packetType = 0;
		data.characterState = 0;
		data.netID = netID;
		data.x = x;
		data.y = x;
		data.punchX = -1;
		data.punchY = -1;
		data.plantingTree = -1;
		SendPacketRaw112(4, packPlayerMoving(&data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
		gamepacket_t p(delay, netID);
		p.Insert("OnSetPos");
		p.Insert(x, y);
		p.CreatePacket(peer);
	}
	void OnSendLog(ENetPeer* enetPeer, string text, int type)
	{
		if (enetPeer)
		{
			ENetPacket* v3 = enet_packet_create(0, text.length() + 5, 1);
			memcpy(v3->data, &type, 4);
			memcpy((v3->data) + 4, text.c_str(), text.length());
			if (enet_peer_send(enetPeer, 0, v3) != 0) {
				enet_packet_destroy(v3);
			}
		}
	}
	void OnAddNotification(ENetPeer* peer, string text, string interfaces, string audio, int delay = 0) {
		gamepacket_t p(delay);
		p.Insert("OnAddNotification");
		p.Insert(interfaces);
		p.Insert(text);
		p.Insert(audio);
		p.CreatePacket(peer);
	}
	void OnTalkBubble(ENetPeer* peer, int netID, string text, int chatColor = 0, bool overlay = false, int delay = 0, bool overlay2 = false) {
		gamepacket_t p(delay);
		p.Insert("OnTalkBubble");
		p.Insert(netID);
		p.Insert(text);
		p.Insert(chatColor == 2 ? 2 : (overlay2 == true ? 1 : 0));
		p.Insert((overlay == true ? 1 : 0));
		p.CreatePacket(peer);
	}
	void SetHasAccountSecured(ENetPeer* peer, bool secured = false) {
		gamepacket_t p(0);
		p.Insert("SetHasAccountSecured");
		p.Insert(secured ? 1 : 0);
		p.CreatePacket(peer);
	}
	void OnDialogRequest(ENetPeer* peer, string text, int delay = 0) {
		gamepacket_t p(delay);
		p.Insert("OnDialogRequest");
		p.Insert("" + text);//last pos
		p.CreatePacket(peer);
	}
	void OnTextOverlay(ENetPeer* peer, string text, int delay = 0) {
		gamepacket_t p(delay);
		p.Insert("OnTextOverlay");
		p.Insert(text);
		p.CreatePacket(peer);
	}
	void OnSendPingRequest(ENetPeer* peer) {
		int intdata = rand() % 100000;
		PlayerMoving data;
		data.packetType = 22;
		data.plantingTree = intdata;
		SendPacketRaw112(4, packPlayerMoving(&data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
	}
	void OnSendPingReply(ENetPeer* peer, PlayerMoving* datas) {
		int intdata = datas->plantingTree;
		PlayerMoving data;
		data.packetType = 22;
		data.plantingTree = intdata;
		SendPacketRaw112(4, packPlayerMoving(&data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
	}
	void OnSpawn(ENetPeer* peer, string name, string country, int netID, int userID, float x, float y, int invis, int mstate, int smstate, bool local, int level = 1, int delay = 0) {
		gamepacket_t p(delay);
		p.Insert("OnSpawn");
		p.Insert(
			"spawn|avatar"
			"\nnetID|" + to_string(netID) +
			"\nuserID|" + to_string(userID) +
			"\ncolrect|0|0|20|30"
			"\nposXY|" + to_string(x) + "|" + to_string(y) +
			"\nname|````" + name + " `w(`2" + to_string(level) + "`w)"
			"\ncountry|" + country +
			"\ninvis|" + to_string(invis) +
			"\nmstate|" + to_string(mstate) +
			"\nsmstate|" + to_string(smstate) +
			(local == true ? "\nonlineID|\ntype|local" : "\n"));
		p.CreatePacket(peer);
	}
	void OnChangePureBeingMode(ENetPeer* peer, int netID, int mode) {
		gamepacket_t p(0, netID);
		p.Insert("OnChangePureBeingMode");
		p.Insert(mode);
		p.CreatePacket(peer);
	}
	void OnPlayPositioned(ENetPeer* peer, int netID, string file, int delay = 0)
	{
		gamepacket_t p(delay, netID);
		p.Insert("OnPlayPositioned");
		p.Insert(file);
		p.CreatePacket(peer);
	}
	void OnNameChanged(ENetPeer* peer, int netID, string name, bool all = false) {
		gamepacket_t p(0, netID);
		p.Insert("OnNameChanged");
		p.Insert(name);
		p.CreatePacket(peer);
		if (all) {
			for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world.empty() or pInfo(currentPeer)->tankIDName.empty()) continue;
				if (pInfo(peer)->world == pInfo(currentPeer)->world)
					p.CreatePacket(currentPeer);

			}
		}
	}
	void OnConsoleMessage(ENetPeer* peer, string text, bool all = false, int dly = 0) {
		gamepacket_t p(dly);
		p.Insert("OnConsoleMessage");
		p.Insert("`o" + text);
		if (!all) p.CreatePacket(peer);
		else {
			for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
				p.CreatePacket(currentPeer);
			}
		}
	}
	void OnPlaySound(ENetPeer* peer, string file, int delay = 0) {
		variants::OnSendLog(peer, "action|play_sfx\nfile|" + file + "\ndelayMS|" + to_string(delay), 3);
	}
	void OnParticleEffect(ENetPeer* peer, int effect, int size, int netid, int x, int y, int delay) {
		PlayerMoving data;
		data.packetType = 17;
		data.netID = netid;
		data.x = x;
		data.y = y;
		data.characterState = 0;
		data.plantingTree = 0;
		data.XSpeed = size;
		data.YSpeed = effect;
		data.punchX = 0;
		data.punchY = 0;
		SendPacketRaw1(4, packPlayerMoving(&data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE, delay);
	}
}
