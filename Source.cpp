#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <signal.h>
#include "enet/include/enet.h"
#include "include/nlohmann/json.hpp"
#include "include/proton/rtparam.hpp"
#include "include/HTTPRequest.hpp"
#include "Item.h"
#include "Base.h"
#include "Player.h"
#include "Packet.h"
#include "Guilds.h"
#include "skStr.h"
#include "World.h"
#pragma comment(lib, "Ws2_32.lib")
BOOL WINAPI ConsoleHandler(DWORD dwType)
{
	switch (dwType) {
	case CTRL_LOGOFF_EVENT: case CTRL_SHUTDOWN_EVENT: case CTRL_CLOSE_EVENT:
	{
		trigger_save_(false);
		return TRUE;
	}
	default:
	{
		break;
	}
	}
	return FALSE;
}


/*
void ubitoken_reset() {
	if (ubitoken_time - time(nullptr) < 0 && ubi_sold_3 >= ubi_item_3) {
		ubi_sold_3 = 0;
		ubitoken_time = time(nullptr) + 3600;
	}
}*/


int ancientprice(int z) {
	switch (z) {
	case 5078: case 5080: case 5084: case 5082: case 7166:
		return -50;
	case 5126: case 5144: case 5162: case 5180: case 7168:
		return -75;
	case 5128: case 5146: case 5164: case 5182: case 7170:
		return -100;
	case 5130: case 5148: case 5166: case 5184: case 7172:
		return -125;
	case 5132: case 5150: case 5168: case 5186: case 7174:
		return -200;
	case 5134: case 5152: case 5170: case 5188: case 9212:
		return -200;
	default:
		return 0;
	}
}

string ancientdialog(ENetPeer* peer, int ril) {
	int price = abs(ancientprice(ril)), ewe = inventory_contains(peer, 1796);
	return "\nadd_textbox|`2- " + items[pInfo(peer)->ances].name + " (OK!)|" + (ewe >= price ? "\nadd_textbox|`2- " + to_string(price) + " Diamond Locks (OK!)" : "\nadd_textbox|`o- " + to_string(price) + " Diamond Locks (" + to_string(ewe) + "/" + to_string(price) + ")") + "|\nadd_spacer|small|\nadd_smalltext|`1The upgraded item will be untradeable|" + (ewe >= price ? "\nadd_button|ancientaltar|`0Complete the ritual" : "") + "|\nend_dialog|tolol12|Return||\nadd_quick_exit";
}
void loop_worlds() {
	if (server_port != main_port && server_port != 17098) return;
	if (f_saving_ == false) {
		long long ms_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		if (beach_party_game) {
			if (last_beach_event != 0 and (last_beach_event - ms_time <= 0 || beach_players.size() == 0)) {
				string name_ = "BEACHPARTYGAME";
				vector<BYTE*>blocks;
				for (int i_ = 0; i_ < change_id_beach.size(); i_++) blocks.push_back(packBlockType(3, 8676, change_id_beach[i_] % 100, change_id_beach[i_] / 100));
				vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
				if (p != worlds.end()) {
					World* world_ = &worlds[p - worlds.begin()];
					for (uint16_t i_ = 4400; i_ < 5300; i_++) if (world_->blocks[i_].fg != 12252)blocks.push_back(packBlockType(3, world_->blocks[i_].fg = 12252, (i_ % 100), (i_ / 100)));
				}
				if (beach_players.size() != 0) {
					gamepacket_t p;
					p.Insert("OnEndMission");
					for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != "BEACHPARTYGAME") continue;
						for (auto& b : blocks)send_raw(currentPeer, 4, b, 56, ENET_PACKET_FLAG_RELIABLE);
						if (find(beach_players.begin(), beach_players.end(), pInfo(currentPeer)->tankIDName) != beach_players.end()) {
							gamepacket_t p;
							p.Insert("OnTalkBubble"), p.Insert(pInfo(currentPeer)->netID), p.Insert("The game is over!"), p.Insert(0), p.Insert(1), p.CreatePacket(currentPeer);
							pInfo(currentPeer)->c_x = 0, pInfo(currentPeer)->c_y = 0;
							SendRespawn(currentPeer, true, 0, 1);
							p.CreatePacket(currentPeer);
						}
					}
					beach_players.clear();
				}
				else {
					for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != "BEACHPARTYGAME") continue;
						for (auto& b : blocks)send_raw(currentPeer, 4, b, 56, ENET_PACKET_FLAG_RELIABLE);
					}
				}
				last_beach_event = 0;
				for (auto& b : blocks) free(b);
				blocks.clear();
			}
		}
		if (last_time2_ - ms_time <= 0 && Server_Security.restart_server_status) {
			gamepacket_t p;
			p.Insert("OnConsoleMessage"), p.Insert("`4Global System Message``: Restarting server for update in `4" + to_string(Server_Security.restart_server_time) + "`` minutes");
			for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
				packet_(currentPeer, "action|play_sfx\nfile|audio/ogg/suspended.ogg\ndelayMS|700");
				p.CreatePacket(currentPeer);
			}
			Server_Security.restart_server_time -= 1;
			if (Server_Security.restart_server_time == 0) {
				last_time2_ = ms_time + 10000, Server_Security.restart_server_status_seconds = true, Server_Security.restart_server_status = false;
				Server_Security.restart_server_time = 50;
			}
			else last_time2_ = ms_time + 60000;
		}
		if (Server_Security.restart_server_status_seconds && last_time2_ - ms_time <= 0) {
			bool save_ = false, send_now = false;
			gamepacket_t p;
			p.Insert("OnConsoleMessage"), p.Insert("`4Global System Message``: Restarting server for update in `4" + (Server_Security.restart_server_time > 0 ? to_string(Server_Security.restart_server_time) : "ZERO") + "`` seconds" + (Server_Security.restart_server_time > 0 ? "" : "! Should be back up in a minute or so. BYE!") + "");
			for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
				p.CreatePacket(currentPeer);
				send_now = true;
			}
			if (Server_Security.restart_server_time > 0) save_ = false;
			else save_ = true;
			last_time2_ = ms_time + 10000;
			if (save_ && send_now) {
				Server_Security.restart_server_status_seconds = false;
				trigger_save_(false);
			}
			else  Server_Security.restart_server_time -= 10;
		}
		if (last_time - ms_time <= 0) {
			if (Hide_N_Seek.last_hide_event != 0 and (Hide_N_Seek.last_hide_event - ms_time <= 0 || Hide_N_Seek.hide_players.size() == Hide_N_Seek.seeker.size())) finish_hide();
			else if (Hide_N_Seek.hide_time - time(nullptr) <= 0 && Hide_N_Seek.hide_time != 0) {
				Hide_N_Seek.hide_time = 0;
				gamepacket_t p3, pe;
				p3.Insert("OnSetMissionTimer"), p3.Insert(Hide_N_Seek.hide_gamemode_time - 30);
				pe.Insert("OnEndMission");
				string start = Hide_N_Seek.hidenseekworld;
				for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
					if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
					if (pInfo(currentPeer)->seeker) {
						pe.CreatePacket(currentPeer);
						Hide_N_Seek.seeker_start = true;
						pInfo(currentPeer)->ghost = false;
						pInfo(currentPeer)->invis = false;
						update_clothes_value(currentPeer);
						join_world(currentPeer, start);
						p3.CreatePacket(currentPeer);
					}
					else if (pInfo(currentPeer)->hider) {
						Hide_N_Seek.total_players++;
						pe.CreatePacket(currentPeer);
						string worldas = Hide_N_Seek.hidenseekworld;
						if (pInfo(currentPeer)->world != worldas) join_world(currentPeer, worldas);
						gamepacket_t p;
						p.Insert("OnTalkBubble"), p.Insert(pInfo(currentPeer)->netID);
						p.Insert("Hiding time is over.. Seeker is coming!"), p.Insert(0), p.Insert(0), p.CreatePacket(currentPeer);
						p3.CreatePacket(currentPeer);
						Hide_N_Seek.hider_start = true;
					}
				}
			}
			if (Hide_N_Seek.wait_players_time - time(nullptr) <= 0 && Hide_N_Seek.wait_players_time != 0 && Hide_N_Seek.last_hide_event == 0) {
				Hide_N_Seek.seeker.clear();
				vector<int> types = { 7874, 454, 988,3818, 2962, 554 , 1766, 6 };
				Hide_N_Seek.hidenseekworld = Hide_N_Seek.hide_worlds[rand() % Hide_N_Seek.hide_worlds.size()];
				if (Hide_N_Seek.hidenseekworld == "HIDENSEEK2") types = { 1004 , 884 , 1042 , 846 , 170 };
				else if (Hide_N_Seek.hidenseekworld == "HIDENSEEK3") types = { 1446 , 482 , 954 , 1100 , 188 };
				else if (Hide_N_Seek.hidenseekworld == "HIDENSEEK4") types = { 1044 , 1042 , 5196 , 652 , 16, 1046, 688 };
				string world = Hide_N_Seek.hidenseekworld;
				Hide_N_Seek.seeker.push_back(to_lower(Hide_N_Seek.hide_players[rand() % Hide_N_Seek.hide_players.size()]));
				if (Hide_N_Seek.hide_players.size() > 6) {
					string random_seeker = to_lower(Hide_N_Seek.hide_players[rand() % Hide_N_Seek.hide_players.size()]);
					if (find(Hide_N_Seek.seeker.begin(), Hide_N_Seek.seeker.end(), random_seeker) != Hide_N_Seek.seeker.end()) {
						random_seeker = to_lower(Hide_N_Seek.hide_players[rand() % Hide_N_Seek.hide_players.size()]);
					}
					else Hide_N_Seek.seeker.push_back(to_lower(Hide_N_Seek.hide_players[rand() % Hide_N_Seek.hide_players.size()]));
				}
				Hide_N_Seek.hide_time = time(nullptr) + 20;
				gamepacket_t p2c;
				p2c.Insert("OnConsoleMessage");
				Hide_N_Seek.last_hide_event = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() + (Hide_N_Seek.hide_gamemode_time * 1000);
				bool found_seaker = false;
				for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
					if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
					if (find(Hide_N_Seek.seeker.begin(), Hide_N_Seek.seeker.end(), to_lower(pInfo(currentPeer)->tankIDName)) != Hide_N_Seek.seeker.end()) {
						found_seaker = true;
						break;
					}
				}
				string seekers = join(Hide_N_Seek.seeker, ", ");
				gamepacket_t p3;
				p3.Insert("OnSetMissionTimer");
				p3.Insert(30);
				if (found_seaker == false) p2c.Insert("The game is canceled because the seeker left!");
				else p2c.Insert("CP:_PL:0_OID:_CT:[S]_ `5***`` `9[HIDE AND SEEK]: The game has started! There is " + to_string(Hide_N_Seek.hide_players.size() - Hide_N_Seek.seeker.size()) + " hiders and "+to_string(Hide_N_Seek.seeker.size()) + " Seeker (" + seekers + ")!");
				for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
					if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
					if (find(Hide_N_Seek.hide_players.begin(), Hide_N_Seek.hide_players.end(), pInfo(currentPeer)->tankIDName) != Hide_N_Seek.hide_players.end()) {
						gamepacket_t pt;
						pt.Insert("OnTalkBubble"), pt.Insert(pInfo(currentPeer)->netID);
						if (found_seaker == false) 	pt.Insert("The game is canceled because the seeker left!");
						else {
							int remove = -1;
							if (modify_inventory(currentPeer, 10016, remove) != 0) exit_(currentPeer);
							else {
								if (find(Hide_N_Seek.seeker.begin(), Hide_N_Seek.seeker.end(), to_lower(pInfo(currentPeer)->tankIDName)) != Hide_N_Seek.seeker.end()) {
									pInfo(currentPeer)->seeker = true;
									gamepacket_t p;
									p.Insert("OnAddNotification"), p.Insert("interface/large/special_event.rttex"), p.Insert("`wYou are seeker!``"), p.Insert("audio/cumbia_horns.wav"), p.Insert(0), p.CreatePacket(currentPeer);
									pt.Insert("The game has started and your ticket has been removed from your inventory! You are seeker! Players are hiding for 20 seconds.");
								}
								else {
									pInfo(currentPeer)->hider = true;
									pInfo(currentPeer)->hiden_clothing = types[rand() % types.size()];
									pt.Insert("The game has started and your ticket has been removed from your inventory! You have 20 seconds to hide!");
									join_world(currentPeer, world);
								}
								p3.CreatePacket(currentPeer);
							}
						}
						p2c.CreatePacket(currentPeer);
						pt.Insert(0), pt.Insert(1), pt.CreatePacket(currentPeer);
					}
				}
				if (found_seaker == false) Hide_N_Seek.hide_players.clear();
			}
			if (Honors_Update.last_honors_reset - ms_time <= 0) {
				top_wls_leaderboard();
				account_rid_detect.clear();
				time_t currentTime;
				time(&currentTime);
				const auto localTime = localtime(&currentTime);
				const auto Hour = localTime->tm_hour; const auto Min = localTime->tm_min; const auto Sec = localTime->tm_sec; const auto Year = localTime->tm_year + 1900; const auto Day = localTime->tm_mday; const auto Month = localTime->tm_mon + 1;
				if (Hour >= 6 and Hour < 15) {
					DaylightDragon.param1 = 0, DaylightDragon.param2 = 0, DaylightDragon.param3 = 1, DaylightDragon.param4 = 5, DaylightDragon.param5 = 0, DaylightDragon.param6 = 2;
				}
				if (Hour >= 15 and Hour < 18) {
					DaylightDragon.param1 = 1, DaylightDragon.param2 = 0, DaylightDragon.param3 = 1, DaylightDragon.param4 = 5, DaylightDragon.param5 = 0, DaylightDragon.param6 = 0;
				}
				if (Hour >= 18 and Hour <= 0 or Hour > 0 and Hour < 6) {
					DaylightDragon.param1 = 2, DaylightDragon.param2 = 0, DaylightDragon.param3 = 1, DaylightDragon.param4 = 5, DaylightDragon.param5 = 0, DaylightDragon.param6 = 1;
				}
				honors_reset();
				Honors_Update.last_honors_reset = ms_time + 7200000;
				cout << "LEADERBOARD RESTART" << endl;
			}
			/*
			if (bonanza_time - time(nullptr) <= 0 && bonanza_item_current != 0) {
				bonanza_sold = 0;
				bonanza_item_current = 0;
			}*/
			if (current_event - time(nullptr) <= 0 && can_event == false) {
				clear_event();
			}
			if (next_event - time(nullptr) <= 0 && can_event == true) {
				start_event();
			}
			if (Crypto_Update.crypto_time - time(nullptr) <= 0) {
				vector<int> added;
				janeway_.janeway_item.clear();
				janeway_.janeway_payout = janeway_.random_janeway_payout[rand() % janeway_.random_janeway_payout.size()];
				for (int i = 0; i < 5; i++) {
					int random_item = rand() % janeway_.janeway_items.size();
					if (find(added.begin(), added.end(), janeway_.janeway_items[random_item].first) == added.end()) {
						added.push_back(janeway_.janeway_items[random_item].first);
						janeway_.janeway_item.push_back(make_pair(janeway_.janeway_items[random_item].first, janeway_.janeway_items[random_item].second));
						janeway_.janeway_item.push_back(make_pair(janeway_.janeway_items[random_item].first + 1, janeway_.janeway_items[random_item].second * 4));
					}
				}
				Crypto_Update.crypto_list.clear();
				Crypto_Update.crypto_sale_list.clear();
				Crypto_Update.crypto_list = "\ntext_scaling_string|Crypto Currency|";
				string info = "";
				std::ifstream ifs("db/crypto_prices.json");
				json j = json::parse(ifs);
				info += "\nBitcoin|" + j["Bitcoin"].get<string>() + "\n";
				info += "Ethereum|" + j["Ethereum"].get<string>() + "\n";
				info += "Litecoin|" + j["Litecoin"].get<string>();
				vector<string> d_ = explode("\n", info);
				for (int i = 1; i < d_.size(); i++) {
					string crypto_name = explode("|", d_[i])[0], color = "";
					int crypto_price = atoi(explode("|", d_[i])[1].c_str()), item_id = 0, crypto_sales = crypto_price * 0.9;
					for (int i = 0; i < Crypto_Update.crypto.size(); i++) {
						if (Crypto_Update.crypto[i].first == crypto_name) {
							if (crypto_price >= Crypto_Update.crypto[i].second) color = "`2rise``";
							else color = "`4drop``";
							Crypto_Update.crypto[i].second = crypto_price;
							Crypto_Update.crypto_sale[i].second = crypto_sales;
							Crypto_Update.crypto_sale_list += "\nadd_button_with_icon|sell_" + Crypto_Update.crypto[i].first + "|" + Crypto_Update.crypto[i].first + "|staticPurpleFrame|" + to_string(item_crypto(Crypto_Update.crypto[i].first)) + "|" + to_string(crypto_sales) + "|";
							Crypto_Update.crypto_list += "\nadd_button_with_icon|buy_" + Crypto_Update.crypto[i].first + "|" + Crypto_Update.crypto[i].first + " (" + color + ")|staticYellowFrame|" + to_string(item_crypto(Crypto_Update.crypto[i].first)) + "|" + to_string(crypto_price) + "|";
						}
					}
				}
				Crypto_Update.crypto_time = time(nullptr) + 6000;
			}
			if (World_Stuff.last_world_menu - ms_time <= 0) {
				World_Stuff.active_world_list.clear();
				World_Stuff.active_world_list = "";
				load_config();
				for (uint8_t i = 0; i < (World_Stuff.top_active_worlds.size() > 20 ? 20 : World_Stuff.top_active_worlds.size()); i++) {
					uint8_t w_cz = 0;
					for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != World_Stuff.top_active_worlds[i].second) continue;
						w_cz++;
					}
					World_Stuff.active_world_list += "\nadd_floater|" + World_Stuff.top_active_worlds[i].second + "|" + to_string(w_cz) + "|0.4" + (i == 0 ? "5" : "2") + "|3529161471";
				}
				if (World_Stuff.active_world_list.empty()) {
					vector<string> active_worlds;
					for (uint8_t i = 0; i < (worlds.size() >= 18 ? 18 : worlds.size()); i++) {
						World world_ = worlds[rand() % worlds.size()];
						if (find(active_worlds.begin(), active_worlds.end(), world_.name) == active_worlds.end()) {
							//0.5
							World_Stuff.active_world_list += "\nadd_floater|" + world_.name + "|0|0.42|3529161471";
							active_worlds.push_back(world_.name);
						}
					}
				}
				if (World_Stuff.active_world_list.empty())World_Stuff.active_world_list = "\nadd_floater|START|0|0.5|3529161471";
				World_Stuff.top_active_worlds.clear();
				World_Stuff.last_world_menu = ms_time + 150000;
			}
		}
		if (last_rainbow_reset - ms_time <= 0) {
			rainbow_color++;
			if (rainbow_color > 24) rainbow_color = 0;
			last_rainbow_reset = ms_time + 2000;
		}
		if (last_autofarm - ms_time <= 0) {
			for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED || currentPeer->data == NULL || pInfo(currentPeer)->world.empty()) continue;
				if (pInfo(currentPeer)->cheater_settings & Gtps3::SETTINGS_0 && pInfo(currentPeer)->disable_cheater == 0) {
					if (pInfo(currentPeer)->last_used_block != 0 && pInfo(currentPeer)->autofarm_x != -1) {
						for (int i = 0; i < pInfo(currentPeer)->autofarm_slot; i++) {
							if (i > 3 and pInfo(currentPeer)->hand != 13700) continue;
							player_punch(currentPeer, pInfo(currentPeer)->last_used_block, pInfo(currentPeer)->autofarm_x + (pInfo(currentPeer)->backwards ? i * -1 : i), pInfo(currentPeer)->autofarm_y, pInfo(currentPeer)->x, pInfo(currentPeer)->y, true);
						}
					}
				}
			}
		}
		if (last_autofarm - ms_time <= 0) last_autofarm = ms_time + 530;
		if (last_time - ms_time <= 0) {
			for (int a = 0; a < World_Stuff.t_worlds.size(); a++) {
				string name = World_Stuff.t_worlds[a];
				vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name](const World& a) { return a.name == name; });
				if (p != worlds.end()) {
					World* world = &worlds[p - worlds.begin()];
					world->fresh_world = true;
					if (world->machines.size() == 0 && world->npc.size() == 0 && world->special_event == false) {
						World_Stuff.t_worlds.erase(World_Stuff.t_worlds.begin() + a);
						a--;
						if (get_players_world(world->name) == 0) save_world(world->name, true);
						continue;
					}
					
					if (world->special_event) {
						if (world->last_special_event + 30000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
							gamepacket_t p, p2;
							p.Insert("OnAddNotification"), p.Insert("interface/large/special_event.rttex"), p.Insert("`2" + items[world->special_event_item].event_name + ":`` " + (items[world->special_event_item].event_total == 1 ? "`oTime's up! Nobody found it!``" : "`oTime's up! " + to_string(world->special_event_item_taken) + " of " + to_string(items[world->special_event_item].event_total) + " items found.``") + ""), p.Insert("audio/cumbia_horns.wav"), p.Insert(0);
							p2.Insert("OnConsoleMessage"), p2.Insert("`2" + items[world->special_event_item].event_name + ":`` " + (items[world->special_event_item].event_total == 1 ? "`oTime's up! Nobody found it!``" : "`oTime's up! " + to_string(world->special_event_item_taken) + " of " + to_string(items[world->special_event_item].event_total) + " items found.``") + "");
							for (ENetPeer* currentPeer_event = server->peers; currentPeer_event < &server->peers[server->peerCount]; ++currentPeer_event) {
								if (currentPeer_event->state != ENET_PEER_STATE_CONNECTED or currentPeer_event->data == NULL or pInfo(currentPeer_event)->world != world->name) continue;
								p.CreatePacket(currentPeer_event), p2.CreatePacket(currentPeer_event);
								PlayerMoving data_{};
								for (int i_ = 0; i_ < world->drop_new.size(); i_++) {
									if (find(world->world_event_items.begin(), world->world_event_items.end(), world->drop_new[i_][0]) != world->world_event_items.end()) {
										BYTE* raw1_ = PackBlockUpdate(14, 0, 0, 0, 0, 0, 0, 0, world->drop_new[i_][2], 0, 0, 0, 0, 0);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != world->name) continue;
											send_raw(currentPeer, 4, raw1_, 56, ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw1_;
										world->drop_new.erase(world->drop_new.begin() + i_);
										i_--;
									}
								}
							}
							world->world_event_items.clear();
							world->last_special_event = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count(), world->special_event_item = 0, world->special_event_item_taken = 0, world->special_event = false;
						}
						continue;
					}
					/*
					for (int i_ = 0; i_ < (world->npc.size() > 25 ? 25 : world->npc.size()); i_++) {
						WorldNPC* npc = &world->npc[i_];
						if (npc->last_ - time(nullptr) > 0 || not npc->enabled || not world->blocks[npc->x + (npc->y * 100)].enabled) continue;
						int block = world->blocks[npc->x + (npc->y * 100)].fg;
						npc->last_ = time(nullptr) + npc->rate_of_fire;
						if (world->blocks[npc->x + (npc->y * 100)].fg == 4344 or world->blocks[npc->x + (npc->y * 100)].fg == 8020) {
							npc->uid = world->npc_uid++;
							BYTE* raw1_ = PackBlockUpdate(34, 0, npc->x * 32 + 16, npc->y * 32 + (block == 8020 ? 6 : 16), npc->x * 32 + 16, npc->y * 32 + (block == 8020 ? 6 : 16), (int)npc->kryptis, 0, 0, 0, npc->projectile_speed, (block == 8020 ? 15 : 8), npc->uid, 2);
							for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != world->name) continue;
								send_raw(currentPeer, 4, raw1_, 56, ENET_PACKET_FLAG_RELIABLE);
							}
							delete[] raw1_;
							npc->started_moving = ms_time;
							if (world->npc_uid > 120)  world->npc_uid = 0;
						}
						else world->blocks[npc->x + (npc->y * 100)].enabled = false;
					}*/

					for (int i_ = 0; i_ < world->machines.size(); i_++) {
						WorldMachines* machine = &world->machines[i_];
						if (world->blocks[machine->x + (machine->y * 100)].pr <= 0 or not world->blocks[machine->x + (machine->y * 100)].enabled or machine->target_item == 0) {
							if (items[world->blocks[machine->x + (machine->y * 100)].fg].blockType == BlockTypes::AUTO_BLOCK) {
								world->machines.erase(world->machines.begin() + i_);
								i_--;
							}
							continue;
						}
						WorldBlock* itemas = &world->blocks[machine->x + (machine->y * 100)];
						int ySize = world->blocks.size() / 100, xSize = world->blocks.size() / ySize;
						if (itemas->pr > 0) {
							if (machine->last_ - ms_time > 0) break;
							if (itemas->fg == 5958) machine->last_ = ms_time + (machine->target_item * 60000);
							else machine->last_ = ms_time + 1500;

							if (itemas->fg == 5958) {
								if ((itemas->shelf_1 == 0 && itemas->shelf_2 == 0 && itemas->shelf_3 == 0) or itemas->enabled == false) {
									for (int i_ = 0; i_ < world->machines.size(); i_++) {
										if (world->machines[i_].x == machine->x and world->machines[i_].y == machine->y) {
											gamepacket_t p;
											p.Insert("OnSetCurrentWeather");
											p.Insert(world->weather = 0);
											for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
												if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
												if (pInfo(currentPeer)->world == world->name) p.CreatePacket(currentPeer);
											}
											world->machines.erase(world->machines.begin() + i_);
											break;
										}
									}
								}
								else {
									if (itemas->shelf_3 == false && world->weather == 40) world->weather = 38;
									if (itemas->shelf_2 == false && world->weather == 39) world->weather = 40;
									if (itemas->shelf_1 == false && world->weather == 38) world->weather = 39;
									if (itemas->shelf_1 && world->weather == 38) {
										if (itemas->shelf_2) world->weather = 39;
										else if (itemas->shelf_3) world->weather = 40;
									}
									else if (itemas->shelf_2 && world->weather == 39) {
										if (itemas->shelf_3) world->weather = 40;
										else if (itemas->shelf_1) world->weather = 38;
									}
									else if (itemas->shelf_3 && world->weather == 40) {
										if (itemas->shelf_1) world->weather = 38;
										else if (itemas->shelf_2) world->weather = 39;
									}
									gamepacket_t p;
									p.Insert("OnSetCurrentWeather");
									p.Insert(world->weather);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(currentPeer)->world == world->name) p.CreatePacket(currentPeer);
									}
								}
							}
							else if (itemas->fg == 6952 or (itemas->fg == 6954 && itemas->build_only == false)) {
								int itemas_ = (itemas->fg == 6954 ? machine->target_item - 1 : machine->target_item);
								vector<WorldBlock>::iterator p = find_if(world->blocks.begin(), world->blocks.end(), [&](const WorldBlock& a) { return a.fg == itemas_ or a.bg == itemas_; });
								if (p != world->blocks.end()) {
									WorldBlock* block_ = &world->blocks[p - world->blocks.begin()];
									int size = p - world->blocks.begin(), x_ = size % xSize, y_ = size / xSize;
									if (items[itemas_].blockType == BlockTypes::BACKGROUND and block_->fg != 0) continue;
									BYTE* raw1_ = PackBlockUpdate(17, 0x8, x_ * 32 + 16, y_ * 32 + 16, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0);
									BYTE* raw2_ = PackBlockUpdate(36, 0x8, x_ * 32 + 16, y_ * 32 + 16, 0, 0, 0, 110, 0, 0, 0, 0, 0, 0);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != world->name) continue;
										send_raw(currentPeer, 4, raw1_, 56, ENET_PACKET_FLAG_RELIABLE);
										send_raw(currentPeer, 4, raw2_, 56, ENET_PACKET_FLAG_RELIABLE);
									}
									delete[] raw1_;
									delete[] raw2_;
									itemas->pr--;
									if (itemas->pr <= 0) {
										PlayerMoving data_{};
										data_.packetType = 5, data_.punchX = machine->x, data_.punchY = machine->y, data_.characterState = 0x8;
										BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world, itemas));
										BYTE* blc = raw + 56;
										form_visual(blc, *itemas, *world, NULL, false);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(currentPeer)->world == world->name) {
												send_raw(currentPeer, 4, raw, 112 + alloc_(world, itemas), ENET_PACKET_FLAG_RELIABLE);
											}
										}
										delete[] raw, blc;
									}
									if (block_->hp == -1) {
										block_->hp = items[itemas_].breakHits;
										block_->lp = ms_time;
									}
									block_->hp -= 1;
									if (block_->hp == 0) {
										if (items[itemas_].max_gems != 0) {
											int maxgems = items[itemas_].max_gems;
											if (itemas_ == 120) maxgems = 50;
											int c_ = rand() % (maxgems + 1);
											if (c_ != 0) {
												bool no_seed = false, no_gems = false, no_block = false;
												if (itemas_ == 2242 or itemas_ == 2244 or itemas_ == 2246 or itemas_ == 2248 or itemas_ == 2250 or itemas_ == 542) no_seed = true, no_block = true;
												else {
													for (int i_ = 0; i_ < world->drop_new.size(); i_++) {
														if (abs(world->drop_new[i_][4] - y_ * 32) <= 16 and abs(world->drop_new[i_][3] - x_ * 32) <= 16) {
															if (world->drop_new[i_][0] == 112 and items[itemas_].rarity < 8) {
																no_gems = true;
															}
															else {
																no_seed = true, no_block = true;
															}
														}
													}
												}
												int chanced = 0;
												if (thedaytoday == 2) chanced = 5;
												if (rand() % 100 < 8) {
													WorldDrop drop_block_{};
													drop_block_.id = itemas_, drop_block_.count = 1, drop_block_.x = (x_ * 32) + rand() % 17, drop_block_.y = (y_ * 32) + rand() % 17;
													if (not use_mag(world, drop_block_, x_, y_) and not no_block) {
														dropas_(world, drop_block_);
													}
												}
												else if (rand() % 100 < (items[itemas_].newdropchance + chanced)) {
													WorldDrop drop_seed_{};
													drop_seed_.id = itemas_ + 1, drop_seed_.count = 1, drop_seed_.x = (x_ * 32) + rand() % 17, drop_seed_.y = (y_ * 32) + rand() % 17;
													if (not use_mag(world, drop_seed_, x_, y_) and not no_seed) {
														dropas_(world, drop_seed_);
													}
												}
												else if (not no_gems) {
													drop_rare_item(world, NULL, itemas_, x_, y_, false);
													gems_(NULL, world, c_, x_ * 32, y_ * 32, itemas_);
												}
											}
										}
										reset_(block_, x_, y_, world);
										PlayerMoving data_{};
										data_.packetType = 5, data_.punchX = x_, data_.punchY = y_, data_.characterState = 0x8;
										BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world, block_));
										BYTE* blc = raw + 56;
										form_visual(blc, *block_, *world, NULL, false);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(currentPeer)->world == world->name) {
												send_raw(currentPeer, 4, raw, 112 + alloc_(world, block_), ENET_PACKET_FLAG_RELIABLE);
											}
										}
										delete[] raw, blc;
									}
									else {
										BYTE* raw1_ = PackBlockUpdate(0x8, 0x0, x_, y_, 0, 0, 0, -1, 6, x_, y_, 0, 0, 0);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != world->name) continue;
											send_raw(currentPeer, 4, raw1_, 56, ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw1_;
									}
								}
							}
							else if (itemas->fg == 6950 or (itemas->fg == 6954 && itemas->build_only)) {
								vector<WorldBlock>::iterator p = find_if(world->blocks.begin(), world->blocks.end(), [&](const WorldBlock& a) { return a.fg == machine->target_item; });
								if (p != world->blocks.end()) {
									int a_ = p - world->blocks.begin();
									long long times_ = time(nullptr);
									uint32_t laikas = uint32_t((times_ - world->blocks[a_].planted <= items[world->blocks[a_].fg].growTime ? times_ - world->blocks[a_].planted : items[world->blocks[a_].fg].growTime));
									if (items[world->blocks[a_].fg].blockType == BlockTypes::SEED and laikas == items[world->blocks[a_].fg].growTime) {
										int x_ = a_ % xSize, y_ = a_ / xSize;
										WorldBlock* block_ = &world->blocks[x_ + (y_ * 100)];
										int drop_count = items[block_->fg - 1].rarity == 1 ? (items[block_->fg - 1].farmable ? (rand() % 6) + 5 : (rand() % block_->fruit) + 1) : items[block_->fg - 1].farmable ? (rand() % 6) + 4 : (rand() % block_->fruit) + 1;
										if (harvest_seed(world, block_, x_, y_, drop_count, -1)) {

										}
										else if (world->weather == 8 and rand() % 300 < 2) {
											WorldDrop drop_block_{};
											drop_block_.id = 3722, drop_block_.count = 1, drop_block_.x = x_ * 32 + rand() % 17, drop_block_.y = y_ * 32 + rand() % 17;
											dropas_(world, drop_block_);
											BYTE* raw1_ = PackBlockUpdate(0x11, 0, drop_block_.x, drop_block_.y, 0, 108, 0, 0, 0, 0, 0, 0, 0, 0);
											for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
												if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != world->name) continue;
												send_raw(currentPeer, 4, raw1_, 56, ENET_PACKET_FLAG_RELIABLE);
											}
											delete[] raw1_;
										}
										if (drop_count != 0) drop_rare_item(world, NULL, machine->target_item - 1, x_, y_, true);
										BYTE* raw1_ = PackBlockUpdate(17, 0x8, x_ * 32 + 16, y_ * 32 + 16, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0);
										BYTE* raw2_ = PackBlockUpdate(36, 0x8, x_ * 32 + 16, y_ * 32 + 16, 0, 0, 0, 109, 0, 0, 0, 0, 0, 0);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != world->name) continue;
											send_raw(currentPeer, 4, raw1_, 56, ENET_PACKET_FLAG_RELIABLE);
											send_raw(currentPeer, 4, raw2_, 56, ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw1_;
										delete[] raw2_;
										itemas->pr--;
										if (itemas->pr <= 0) {
											PlayerMoving data_{};
											data_.packetType = 5, data_.punchX = machine->x, data_.punchY = machine->y, data_.characterState = 0x8;
											BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world, itemas));
											BYTE* blc = raw + 56;
											form_visual(blc, *itemas, *world, NULL, false);
											for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
												if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
												if (pInfo(currentPeer)->world == world->name) {
													send_raw(currentPeer, 4, raw, 112 + alloc_(world, itemas), ENET_PACKET_FLAG_RELIABLE);
												}
											}
											delete[] raw, blc;
										}
									}
								}
							}
						}
					}
				}
			}
			for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED || currentPeer->data == NULL or pInfo(currentPeer)->tankIDName.empty() or pInfo(currentPeer)->world.empty()) continue;
				if (pInfo(currentPeer)->hand == 3578 || pInfo(currentPeer)->face == 3576) {
					if (pInfo(currentPeer)->hand_torch + 60000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
						int got = 0;
						if (pInfo(currentPeer)->hand == 3578) {
							if (pInfo(currentPeer)->hand_torch != 0) {
								modify_inventory(currentPeer, 3578, got);
								if (got - 1 >= 1) {
									gamepacket_t p;
									p.Insert("OnTalkBubble"), p.Insert(pInfo(currentPeer)->netID), p.Insert("`4My torch went out, but I have " + to_string(got - 1) + " more!``"), p.Insert(0), p.Insert(0), p.CreatePacket(currentPeer);
								}
								modify_inventory(currentPeer, 3578, got = -1);
							}
						}
						else if (pInfo(currentPeer)->face == 3576) {
							modify_inventory(currentPeer, 3306, got = -1);
						}
						pInfo(currentPeer)->hand_torch = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
					}
				}
				else if (pInfo(currentPeer)->hand == 2204 or pInfo(currentPeer)->hand == 2558 and pInfo(currentPeer)->x != -1 and pInfo(currentPeer)->y != -1) {
					if (pInfo(currentPeer)->random_geiger_time < 100) {
						pInfo(currentPeer)->random_geiger_time++;
					}
					else {
						pInfo(currentPeer)->random_geiger_time = 0;
						pInfo(currentPeer)->geiger_x = (rand() % 100) * 32;
						pInfo(currentPeer)->geiger_y = (rand() % 54) * 32;
					}
					int hands_ = pInfo(currentPeer)->hand;
					if (not has_playmod2(pInfo(currentPeer), 10)) {
						if (pInfo(currentPeer)->geiger_x == -1 and pInfo(currentPeer)->geiger_y == -1) {
							pInfo(currentPeer)->geiger_x = (rand() % 100) * 32;
							pInfo(currentPeer)->geiger_y = (rand() % 54) * 32;
						}
						int a_ = pInfo(currentPeer)->geiger_x + ((pInfo(currentPeer)->geiger_y * 100) / 32), b_ = pInfo(currentPeer)->x + ((pInfo(currentPeer)->y * 100) / 32), diff = abs(a_ - b_) / 32;
						if (diff < 30) {
							int t_ = 1500;
							if (diff >= 6) t_ = 1350;
							else if (diff < 15) t_ = 1000;
							else if (diff <= 1) t_ = 2500;
							if (pInfo(currentPeer)->geiger_time + t_ < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
								pInfo(currentPeer)->geiger_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
								PlayerMoving data_{};
								data_.packetType = 17, data_.characterState = 0x8, data_.x = pInfo(currentPeer)->x + 10, data_.y = pInfo(currentPeer)->y + 16, data_.XSpeed = (diff >= 30 ? 0 : (diff >= 15 ? 1 : 2)), data_.YSpeed = 114;
								BYTE* raw = packPlayerMoving(&data_);
								send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
								delete[] raw;
								if (diff <= 1) {
									pInfo(currentPeer)->geiger_x = -1, pInfo(currentPeer)->geiger_y = -1;
									int give_back_geiger = items[pInfo(currentPeer)->hand].geiger_give_back;
									{
										int c_ = -1;
										modify_inventory(currentPeer, pInfo(currentPeer)->hand, c_);
										int c_2 = 1;
										if (modify_inventory(currentPeer, give_back_geiger, c_2) != 0) {
											string name_ = pInfo(currentPeer)->world;
											vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
											if (p != worlds.end()) {
												World* world_ = &worlds[p - worlds.begin()];
												world_->fresh_world = true;
												WorldDrop drop_block_{};
												drop_block_.id = give_back_geiger, drop_block_.count = 1, drop_block_.x = pInfo(currentPeer)->x + rand() % 17, drop_block_.y = pInfo(currentPeer)->y + rand() % 17;
												dropas_(world_, drop_block_);
											}
										}
										int seconds = 1800;
										if (thedaytoday == 3) seconds = 600;
										pInfo(currentPeer)->hand = give_back_geiger;
										update_clothes(currentPeer);
										add_playmod(currentPeer, 10, seconds);
										packet_(currentPeer, "action|play_sfx\nfile|audio/dialog_confirm.wav\ndelayMS|0");
									}
									if (pInfo(currentPeer)->lwiz_step == 13) {
										if (pInfo(currentPeer)->lwiz_quest == 5 || pInfo(currentPeer)->lwiz_quest == 6 || pInfo(currentPeer)->lwiz_quest == 7 || pInfo(currentPeer)->lwiz_quest == 8) {
											add_lwiz_points(currentPeer, 1);
										}
									}
									if (pInfo(currentPeer)->grow4good_geiger < pInfo(currentPeer)->grow4good_geiger2 && pInfo(currentPeer)->grow4good_geiger != -1) daily_quest(currentPeer, false, "geiger", 1);
									add_event_xp(currentPeer, 1, "geiger");
									int give_times = 1;
									if (pInfo(currentPeer)->gp) {
										if (complete_gpass_task(currentPeer, "Geiger")) give_times++;
									}
									for (int i = 0; i < give_times; i++) {
										int item_ = items[hands_].randomitem[rand() % items[hands_].randomitem.size()], c_ = 1;
										if (item_ == 1486) if (pInfo(currentPeer)->lwiz_step == 6) add_lwiz_points(currentPeer, 1);
										if (item_ == 1486 && pInfo(currentPeer)->C_QuestActive && pInfo(currentPeer)->C_QuestKind == 11 && pInfo(currentPeer)->C_QuestProgress < pInfo(currentPeer)->C_ProgressNeeded) {
											pInfo(currentPeer)->C_QuestProgress += 1;
											if (pInfo(currentPeer)->C_QuestProgress >= pInfo(currentPeer)->C_ProgressNeeded) {
												pInfo(currentPeer)->C_QuestProgress = pInfo(currentPeer)->C_ProgressNeeded;
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(currentPeer)->netID);
												p.Insert("`9Ring Quest task complete! Go tell the Ringmaster!");
												p.Insert(0), p.Insert(0);
												p.CreatePacket(currentPeer);
											}
										}
										/*
										if (item_ == 13158) {
											c_ = 5;
											ubi_sold_3 += 5;
										}*/
										if (modify_inventory(currentPeer, item_, c_) != 0) {
											string name_ = pInfo(currentPeer)->world;
											vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
											if (p != worlds.end()) {
												World* world_ = &worlds[p - worlds.begin()];
												world_->fresh_world = true;
												WorldDrop drop_block_{};
												drop_block_.id = item_, drop_block_.count = 1, drop_block_.x = pInfo(currentPeer)->x + rand() % 17, drop_block_.y = pInfo(currentPeer)->y + rand() % 17;
												dropas_(world_, drop_block_);
											}
										}
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(currentPeer)->netID);
										p.Insert("I found `21 " + items[item_].name + "``!" + (hands_ == 2558 ? " But now I lost it in my basket!" : "") + "");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(currentPeer);
										gamepacket_t p2;
										p2.Insert("OnConsoleMessage");
										p2.Insert(get_player_nick(currentPeer) + " found `21 " + items[item_].name + "``!");
										PlayerMoving data_{};
										data_.packetType = 19, data_.plantingTree = 0, data_.netID = 0;
										data_.punchX = item_;
										data_.x = pInfo(currentPeer)->x + 10, data_.y = pInfo(currentPeer)->y + 16;
										int32_t to_netid = pInfo(currentPeer)->netID;
										BYTE* raw = packPlayerMoving(&data_);
										raw[3] = 5;
										memcpy(raw + 8, &to_netid, 4);
										for (ENetPeer* currentPeer2 = server->peers; currentPeer2 < &server->peers[server->peerCount]; ++currentPeer2) {
											if (currentPeer2->state != ENET_PEER_STATE_CONNECTED or currentPeer2->data == NULL) continue;
											if (pInfo(currentPeer2)->world == pInfo(currentPeer)->world) {
												send_raw(currentPeer2, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
												p2.CreatePacket(currentPeer2);
											}
										}
										delete[]raw;
									}
									gamepacket_t p;
									p.Insert("OnParticleEffect");
									p.Insert(48);
									p.Insert((float)pInfo(currentPeer)->x + 10, (float)pInfo(currentPeer)->y + 16);
									p.CreatePacket(currentPeer);
								}
							}
						}
					}
				}
				if (pInfo(currentPeer)->hider == false && (pInfo(currentPeer)->rb or pInfo(currentPeer)->valentine && pInfo(currentPeer)->name_time + 250 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count())) {
					if (pInfo(currentPeer)->vip == 0) pInfo(currentPeer)->rb = false;
					else {
						pInfo(currentPeer)->name_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
						string msg2 = "", nick = fixchar4(get_player_nick(currentPeer));
						if (pInfo(currentPeer)->rb) {
							if (pInfo(currentPeer)->is_legend && pInfo(currentPeer)->d_name.empty()) msg2 += random_color[rainbow_color] + nick;
							else {
								for (int i = 0; i < nick.length(); i++) msg2 += random_color[i + rainbow_color] + nick[i];
							}
						}
						else {
							vector <string> random_letter{ "4", "p" };
							if (pInfo(currentPeer)->is_legend && pInfo(currentPeer)->d_name.empty()) msg2 += "`" + random_letter[rand() % random_letter.size()] + nick;
							else for (int i = 0; i < nick.length(); i++) msg2 += "`" + random_letter[rand() % random_letter.size()] + nick[i];
						}
						nick_update_2(currentPeer, NULL, msg2);
					}
				}
				if (pInfo(currentPeer)->save_time + 300000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
					if (pInfo(currentPeer)->save_time != 0) {
						pInfo(currentPeer)->opc++;
						if (pInfo(currentPeer)->hand == 10384) pInfo(currentPeer)->opc += 2;
						if (pInfo(currentPeer)->gp || pInfo(currentPeer)->hand == 10384 || pInfo(currentPeer)->hair == 9542 || pInfo(currentPeer)->hair == 9984 || pInfo(currentPeer)->hair == 9920 || pInfo(currentPeer)->necklace == 9964 || pInfo(currentPeer)->necklace == 10176 || pInfo(currentPeer)->pants == 9782 || pInfo(currentPeer)->hand == 9880 || pInfo(currentPeer)->hand == 10020 || pInfo(currentPeer)->hand == 9974 || pInfo(currentPeer)->hand == 9918 || pInfo(currentPeer)->hand == 10290 || pInfo(currentPeer)->hand == 9916 || pInfo(currentPeer)->hand == 9914 || pInfo(currentPeer)->hand == 9766 || pInfo(currentPeer)->hand == 9772 || pInfo(currentPeer)->hand == 9908) pInfo(currentPeer)->opc++;
						add_honors(pInfo(currentPeer)->world, pInfo(currentPeer)->world_owner);
						if (pInfo(currentPeer)->grow4good_30mins < 30) daily_quest(currentPeer, false, "30mins", 10);
						loop_save(currentPeer);
						add_top_player(to_lower(pInfo(currentPeer)->tankIDName));
					}
					pInfo(currentPeer)->save_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
				}
				if (pInfo(currentPeer)->World_Timed - time(nullptr) == 60 && pInfo(currentPeer)->WorldTimed && pInfo(currentPeer)->tankIDName != pInfo(currentPeer)->world_owner) {
					gamepacket_t p;
					p.Insert("OnTalkBubble"); p.Insert(pInfo(currentPeer)->netID); p.Insert("Your access to this world will expire in less than a minute!"); p.Insert(0); p.Insert(0); p.CreatePacket(currentPeer);
				}
				else if (pInfo(currentPeer)->World_Timed - time(nullptr) < 0 && pInfo(currentPeer)->WorldTimed && pInfo(currentPeer)->tankIDName != pInfo(currentPeer)->world_owner) {
					exit_(currentPeer);
				}
				if (pInfo(currentPeer)->fishing_used != 0) {
					if (pInfo(currentPeer)->last_fish_catch + pInfo(currentPeer)->fish_seconds < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count() && rand() % 100 < (pInfo(currentPeer)->hand == 6258 ? 20 : pInfo(currentPeer)->hand == 3010 ? 15 : 10)) {
						PlayerMoving data_{};
						data_.packetType = 17, data_.netID = 34, data_.YSpeed = 34, data_.x = pInfo(currentPeer)->f_x * 32 + 16, data_.y = pInfo(currentPeer)->f_y * 32 + 16;
						pInfo(currentPeer)->last_fish_catch = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
						BYTE* raw = packPlayerMoving(&data_);
						gamepacket_t p3(0, pInfo(currentPeer)->netID);
						p3.Insert("OnPlayPositioned"), p3.Insert("audio/splash.wav");
						for (ENetPeer* currentPeer_event = server->peers; currentPeer_event < &server->peers[server->peerCount]; ++currentPeer_event) {
							if (currentPeer_event->state != ENET_PEER_STATE_CONNECTED or currentPeer_event->data == NULL or pInfo(currentPeer_event)->world != pInfo(currentPeer)->world) continue;
							send_raw(currentPeer_event, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE), p3.CreatePacket(currentPeer_event);
						}
						delete[] raw;
						if (pInfo(currentPeer)->cheater_settings & Gtps3::SETTINGS_16 && pInfo(currentPeer)->disable_cheater == 0) {
							int bait = pInfo(currentPeer)->fishing_used, fx = pInfo(currentPeer)->f_x, fy = pInfo(currentPeer)->f_y;
							stop_fishing(currentPeer, false, "");
							player_punch(currentPeer, bait, fx, fy, fx * 32, fy * 32);
						}
					}
				}
				if (pInfo(currentPeer)->world != "TRADE" && pInfo(currentPeer)->world != Hide_N_Seek.hidenseekworld) {
					vector<pair<int, string>>::iterator p = find_if(World_Stuff.top_active_worlds.begin(), World_Stuff.top_active_worlds.end(), [&](const pair < int, string>& element) { return element.second == pInfo(currentPeer)->world; });
					if (p != World_Stuff.top_active_worlds.end()) World_Stuff.top_active_worlds[p - World_Stuff.top_active_worlds.begin()].first++;
					else World_Stuff.top_active_worlds.push_back(make_pair(1, pInfo(currentPeer)->world));
				}
				/*
				if (pInfo(currentPeer)->world == "GROWGANOTH") {
					if (last_growganoth_time - ms_time <= 0) {
						last_growganoth_time = ms_time + 15000;
						update_growganoth(currentPeer);
					}
				}*/
				{
					if (last_firehouse - ms_time <= 0) {
						vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [&](const World& a) { return a.name == pInfo(currentPeer)->world; });
						if (p != worlds.end()) {
							World* world = &worlds[p - worlds.begin()];
							world->fresh_world = true;
							vector<WorldBlock>::iterator p3 = find_if(world->blocks.begin(), world->blocks.end(), [&](const WorldBlock& a) { return a.fg == 3072; });
							if (p3 != world->blocks.end()) {
								vector<WorldBlock>::iterator p2 = find_if(world->blocks.begin(), world->blocks.end(), [&](const WorldBlock& a) { return a.flags & 0x10000000; });
								if (p2 != world->blocks.end()) {
									int x_ = int(p2 - world->blocks.begin()) % 100, y_ = int(p2 - world->blocks.begin()) / 100;
									apply_tile_visual(world, &world->blocks[x_ + (y_ * 100)], x_, y_, 0x10000000, true);
								}
							}
							if (last_fire_time - ms_time <= 0) {
								if (world->total_fires < 150) {
									vector<WorldBlock>::iterator p2 = find_if(world->blocks.begin(), world->blocks.end(), [&](const WorldBlock& a) { return a.flags & 0x10000000 && (world->fire_try > 10 ? a.applied_fire == true : a.applied_fire == false); });
									if (p2 != world->blocks.end()) {
										int x_ = int(p2 - world->blocks.begin()) % 100, y_ = int(p2 - world->blocks.begin()) / 100;
										vector<int> random_xy{ 1, 0, -1, 0 };
										int randomx = 0, randomy = 0;
										if (rand() % 2 < 1) randomx = x_ + random_xy[rand() % random_xy.size()], randomy = y_;
										else randomx = x_, randomy = y_ + random_xy[rand() % random_xy.size()];

										if (randomx > 0 && randomx < world->max_x && randomy > 0 && randomy < world->max_y) {
											bool has_fire = world->blocks[randomx + (randomy * 100)].flags & 0x10000000, has_water = world->blocks[randomx + (randomy * 100)].flags & 0x04000000;
											if (world->blocks[randomx + (randomy * 100)].fg != 0 && has_fire == false && has_water == false && items[world->blocks[randomx + (randomy * 100)].fg].blockType != BlockTypes::MAIN_DOOR && items[world->blocks[randomx + (randomy * 100)].fg].blockType != BlockTypes::BEDROCK && world->blocks[randomx + (randomy * 100)].fg != 9570) apply_tile_visual(world, &world->blocks[randomx + (randomy * 100)], randomx, randomy, 0x10000000);
											else {
												world->blocks[x_ + (y_ * 100)].fire_try++;
												if (world->blocks[x_ + (y_ * 100)].fire_try >= 8) world->blocks[x_ + (y_ * 100)].applied_fire = true, world->blocks[x_ + (y_ * 100)].fire_try = 0;
											}
										}
									}
									else {
										world->fire_try++;
										if (world->fire_try > 10) world->fire_try = 0;
									}
								}
							}
						}
					}
				}
				long long time_ = time(nullptr);
				for (int i_ = 0; i_ < pInfo(currentPeer)->playmods.size(); i_++) {
					if (pInfo(currentPeer)->playmods[i_].id == 12) {
						if (pInfo(currentPeer)->valentine_time + 2500 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
							pInfo(currentPeer)->valentine_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
							for (ENetPeer* valentine = server->peers; valentine < &server->peers[server->peerCount]; ++valentine) {
								if (valentine->state != ENET_PEER_STATE_CONNECTED or valentine->data == NULL) continue;
								if (pInfo(valentine)->world == pInfo(currentPeer)->world and pInfo(valentine)->tankIDName == pInfo(currentPeer)->playmods[i_].user) {
									if (not pInfo(valentine)->invis and not pInfo(currentPeer)->invis and pInfo(currentPeer)->x != -1 and pInfo(currentPeer)->y != -1 and pInfo(valentine)->x != -1 and pInfo(valentine)->y != -1) {
										gamepacket_t p;
										p.Insert("OnParticleEffect");
										p.Insert(13);
										p.Insert((float)pInfo(valentine)->x + 10, (float)pInfo(valentine)->y + 16);
										p.Insert((float)0), p.Insert((float)pInfo(currentPeer)->netID);
										bool double_send = false;
										for (int i_2 = 0; i_2 < pInfo(valentine)->playmods.size(); i_2++) {
											if (pInfo(valentine)->playmods[i_2].id == 12 and pInfo(valentine)->playmods[i_2].user == pInfo(currentPeer)->tankIDName) {
												double_send = true;
												break;
											}
										}
										gamepacket_t p2;
										p2.Insert("OnParticleEffect");
										p2.Insert(13);
										p2.Insert((float)pInfo(currentPeer)->x + 10, (float)pInfo(currentPeer)->y + 16);
										p2.Insert((float)0), p2.Insert((float)pInfo(valentine)->netID);
										for (ENetPeer* valentine_bc = server->peers; valentine_bc < &server->peers[server->peerCount]; ++valentine_bc) {
											if (valentine_bc->state != ENET_PEER_STATE_CONNECTED or valentine_bc->data == NULL) continue;
											if (pInfo(valentine_bc)->world == pInfo(currentPeer)->world) {
												p.CreatePacket(valentine_bc);
												if (double_send) p2.CreatePacket(valentine_bc);
											}
										}
									}
									break;
								}
							}
						}
					}
					if (pInfo(currentPeer)->playmods[i_].time - time_ < 0) {
						if (pInfo(currentPeer)->playmods[i_].id == 125) {
							if (pInfo(currentPeer)->moderator == 0) {
								pInfo(currentPeer)->mod = 0, pInfo(currentPeer)->tmod = 0;
								if (not pInfo(currentPeer)->d_name.empty()) {
									pInfo(currentPeer)->d_name = "";
									nick_update(currentPeer, NULL);
								}
							}
						}
						else if (pInfo(currentPeer)->playmods[i_].id == 126) pInfo(currentPeer)->vip = 0;
						else if (pInfo(currentPeer)->playmods[i_].id == 127 || pInfo(currentPeer)->playmods[i_].id == 128) exit_(currentPeer);
						else if (pInfo(currentPeer)->playmods[i_].id == 136) pInfo(currentPeer)->hit_by = 0;
						else if (pInfo(currentPeer)->playmods[i_].id == 143) {
							if (pInfo(currentPeer)->cheater_ == 0) {
								pInfo(currentPeer)->cheater_settings = 0;
								pInfo(currentPeer)->chat_prefix.clear();
								autofarm_status(currentPeer);
							}
						}
						packet_(currentPeer, "action|play_sfx\nfile|audio/dialog_confirm.wav\ndelayMS|0");
						gamepacket_t p;
						p.Insert("OnConsoleMessage");
						p.Insert(info_about_playmods[pInfo(currentPeer)->playmods[i_].id - 1][5] + " (`$" + info_about_playmods[pInfo(currentPeer)->playmods[i_].id - 1][3] + "`` mod removed)");
						p.CreatePacket(currentPeer);
						pInfo(currentPeer)->playmods.erase(pInfo(currentPeer)->playmods.begin() + i_);
						update_clothes_value(currentPeer);
						update_clothes(currentPeer);
						break;
					}
				}
			}
			if (last_time - ms_time <= 0) last_time = ms_time + 1450;
			if (last_firehouse - ms_time <= 0) last_firehouse = ms_time + 2000000;
			if (last_fire_time - ms_time <= 0)last_fire_time = ms_time + 2000000;
		}
	}
}

bool isASCII(const std::string& s)
{
	return !std::any_of(s.begin(), s.end(), [](char c) {
		return static_cast<unsigned char>(c) > 127;
	});
}

std::string get_str_between_two_str(const std::string& s,
	const std::string& start_delim,
	const std::string& stop_delim)
{
	unsigned first_delim_pos = s.find(start_delim);
	unsigned end_pos_of_first_delim = first_delim_pos + start_delim.length();
	unsigned last_delim_pos = s.find(stop_delim);

	return s.substr(end_pos_of_first_delim,
		last_delim_pos - end_pos_of_first_delim);
}

void thesaver() {
	while (true) {
		Sleep(300000 * 2);
		cout << "AUTO SAVING..." << endl;
		trigger_save2();
	}
}

int main(int argc, char* argv[]) {
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);
	today_day = newtime.tm_mday;
	/*
	{
		ifstream ifs("db/volcanic_capes.json");
		if (ifs.is_open()) {
			json j;
			ifs >> j;
			Cinco_De_Mayo.volcanic_cape = j["v1"].get<int>();
			Cinco_De_Mayo.volcanic_wings = j["v2"].get<int>();
			Cinco_De_Mayo.volcanic_pauldrons = j["v4"].get<int>();
			if (today_day != j["v3"].get<int>()) {
				Cinco_De_Mayo.volcanic_cape = 0;
				Cinco_De_Mayo.volcanic_wings = 0;
				Cinco_De_Mayo.volcanic_pauldrons = 0;
			}
		}
	}*/
	{
		ifstream ifs("db/server_events.json");
		if (ifs.is_open()) {
			json j;
			ifs >> j;
			mails = (!(j.find("mail") != j.end()) ? mails : j["mail"].get<vector<pair<string, string>>>());
			world_rating = (!(j.find("rate") != j.end()) ? world_rating : j["rate"].get<vector<WorldRate>>());
			top_basher = (!(j.find("2") != j.end()) ? top_basher : j["2"].get<vector<pair<long long int, string>>>());

			top_wls = (!(j.find("top_wls") != j.end()) ? top_wls : j["top_wls"].get<vector<pair<long long int, string>>>());
			vector<pair<long long int, string>> top_gls;
			for (int i = 0; i < top_wls.size(); i++) {
				int wls = top_wls[i].first;
				vector<pair<long long int, string>>::iterator pz = find_if(top_gls.begin(), top_gls.end(), [&](const pair < long long int, string>& element) { return  to_lower(element.second) == to_lower(top_wls[i].second); });
				if (pz != top_gls.end()) top_gls[pz - top_gls.begin()].first += wls;
				else top_gls.push_back(make_pair(wls, top_wls[i].second));
			}

			top_wls = top_gls;

			total_wls_recycled = (!(j.find("total_wls_recycled") != j.end()) ? total_wls_recycled : j["total_wls_recycled"].get<long long int>());
			wls_event_time = (!(j.find("wls_event_time") != j.end()) ? wls_event_time : j["wls_event_time"].get<long long int>());

			top_basher_winners = (!(j.find("3") != j.end()) ? top_basher_winners : j["3"].get<vector<pair<int, string>>>());
			top_player_points = (!(j.find("top_player_points") != j.end()) ? top_player_points : j["top_player_points"].get<vector<pair<int, string>>>());
			top_old_winners = (!(j.find("4") != j.end()) ? top_old_winners : j["4"].get<string>());
			top_guild = (!(j.find("5") != j.end()) ? top_guild : j["5"].get<vector<pair<long long int, string>>>());
			top_guild_winners = (!(j.find("6") != j.end()) ? top_guild_winners : j["6"].get<vector<pair<int, string>>>());
			top_old_guild_winners = (!(j.find("7") != j.end()) ? top_old_guild_winners : j["7"].get<string>());
			Server_Security.ridbans = (!(j.find("rid") != j.end()) ? Server_Security.ridbans : j["rid"].get<vector<string>>());
			World_Stuff.wotd = (!(j.find("wotd") != j.end()) ? World_Stuff.wotd : j["wotd"].get<vector<string>>());
			current_event = (!(j.find("c_event") != j.end()) ? current_event : j["c_event"].get<long long int>());
			next_event = (!(j.find("n_event") != j.end()) ? next_event : j["n_event"].get<long long int>());
			can_event = (!(j.find("can_event") != j.end()) ? true : j["can_event"].get<bool>());
			event_item = (!(j.find("event_id") != j.end()) ? 6828 : j["event_id"].get<int>());
			old_event_item = (!(j.find("old_event_id") != j.end()) ? 6238 : j["old_event_id"].get<int>());
			angelic_aura = (!(j.find("angelic_aura2") != j.end()) ? angelic_aura : j["angelic_aura2"].get<int>());
			grow_pass_item_price = (!(j.find("helicopter_price2s1") != j.end()) ? grow_pass_item_price : j["helicopter_price2s1"].get<int>());
			zeus_crown = (!(j.find("zeus_crown2") != j.end()) ? zeus_crown : j["zeus_crown2"].get<int>());
			vapor_blade = (!(j.find("vapor_blade") != j.end()) ? vapor_blade : j["vapor_blade"].get<int>());
			dracula_set = (!(j.find("dracula_set") != j.end()) ? dracula_set : j["dracula_set"].get<int>());
			recycling_machine = (!(j.find("recycling_machine2") != j.end()) ? recycling_machine : j["recycling_machine2"].get<int>());
			building_machine = (!(j.find("building_machine2") != j.end()) ? building_machine : j["building_machine2"].get<int>());
			island_blast = (!(j.find("island_blast2") != j.end()) ? island_blast : j["island_blast2"].get<int>());
			cursed_eyes = (!(j.find("cursed_eyes") != j.end()) ? cursed_eyes : j["cursed_eyes"].get<int>());
			e_scepter = (!(j.find("e_scepter") != j.end()) ? e_scepter : j["e_scepter"].get<int>());
			_10938_ = (!(j.find("_10938_") != j.end()) ? _10938_ : j["_10938_"].get<int>());
			phoenix_items_all = (!(j.find("phoenix_items_all") != j.end()) ? all_phoenix_items : j["phoenix_items_all"].get<vector<int>>());


			experience_rayman = (!(j.find("experience_rayman2") != j.end()) ? experience_rayman : j["experience_rayman2"].get<int>());
			laser_light = (!(j.find("laser_light2") != j.end()) ? laser_light : j["laser_light2"].get<int>());
			//halloween_rarity = (!(j.find("halloween_rarity") != j.end()) ? halloween_rarity : j["halloween_rarity"].get<int>());
			//	winterfest_sold = (!(j.find("winterfest_sold") != j.end()) ? 0 : j["winterfest_sold"].get<int>());
			/*
			ubi_sold_1 = (!(j.find("ubi_sold_1") != j.end()) ? 0 : j["ubi_sold_1"].get<int>());
			ubi_sold_2 = (!(j.find("ubi_sold_2") != j.end()) ? 0 : j["ubi_sold_2"].get<int>());
			ubi_sold_3 = (!(j.find("ubi_sold_3") != j.end()) ? 0 : j["ubi_sold_3"].get<int>());
			ubi_sold_4 = (!(j.find("ubi_sold_4") != j.end()) ? 0 : j["ubi_sold_4"].get<int>());
			ubi_sold_5 = (!(j.find("ubi_sold_5") != j.end()) ? 0 : j["ubi_sold_5"].get<int>());
			ubi_sold_6 = (!(j.find("ubi_sold_6") != j.end()) ? 0 : j["ubi_sold_6"].get<int>());
			ubi_sold_7 = (!(j.find("ubi_sold_7") != j.end()) ? 0 : j["ubi_sold_7"].get<int>());
			ubi_sold_8 = (!(j.find("ubi_sold_8") != j.end()) ? 0 : j["ubi_sold_8"].get<int>());
			ubi_sold_9 = (!(j.find("ubi_sold_9") != j.end()) ? 0 : j["ubi_sold_9"].get<int>());
			ubi_sold_10 = (!(j.find("ubi_sold_10") != j.end()) ? 0 : j["ubi_sold_10"].get<int>());*/
			//planted = (!(j.find("planted") != j.end()) ? 0 : j["planted"].get<int>());
			//total_pineapple_eaten = (!(j.find("total_pineapple_eaten") != j.end()) ? 5000000 : j["total_pineapple_eaten"].get<int>());
			Crypto_Update.crypto_sold = (!(j.find("crypto_sold") != j.end()) ? 0 : j["crypto_sold"].get<int>());
			Crypto_Update.crypto_bought = (!(j.find("crypto_bought") != j.end()) ? 0 : j["crypto_bought"].get<int>());
			Server_Security.buy_enabled = (!(j.find("buy_enabled") != j.end()) ? Server_Security.buy_enabled : j["buy_enabled"].get<bool>());
			Server_Security.logs = (!(j.find("logs") != j.end()) ? Server_Security.logs : j["logs"].get<vector<pair<string, string>>>());
			/*
			bonanza_item = (!(j.find("bonanza_item") != j.end()) ? bonanza_item : j["bonanza_item"].get<int>());
			bonanza_sold = (!(j.find("bonanza_sold") != j.end()) ? bonanza_sold : j["bonanza_sold"].get<int>());
			bonanza_item_current = (!(j.find("bonanza_item_current") != j.end()) ? bonanza_item_current : j["bonanza_item_current"].get<int>());
			bonanza_time = (!(j.find("bonanza_time") != j.end()) ? bonanza_time : j["bonanza_time"].get<long long int>());*/
			//	crypto = (!(j.find("crypto_1") != j.end()) ? crypto : j["crypto_1"].get<vector<pair<string, int>>>());
			//	crypto_sale = (!(j.find("crypto_2") != j.end()) ? crypto_sale : j["crypto_2"].get<vector<pair<string, int>>>());
			Crypto_Update.crypto_history = (!(j.find("crypto_3") != j.end()) ? Crypto_Update.crypto_history : j["crypto_3"].get<vector<string>>());
			//ubitoken_time = (!(j.find("ubi_token_time") != j.end()) ? 0 : j["ubi_token_time"].get<long long int>());
			/*winterfest_gift = (!(j.find("winterfest_gift") != j.end()) ? 0 : j["winterfest_gift"].get<long long int>());
			winterfest_rarity_250 = (!(j.find("winterfest_rarity_250") != j.end()) ? 0 : j["winterfest_rarity_250"].get<int>());
			winterfest_rarity = (!(j.find("winterfest_rarity") != j.end()) ? 0 : j["winterfest_rarity"].get<int>());
			winterfest_total_toys = (!(j.find("winterfest_total_toys") != j.end()) ? 0 : j["winterfest_total_toys"].get<int>());*/
			Balloon_Warz.balloon_warz = (!(j.find("balloon_warz2") != j.end()) ? Balloon_Warz.balloon_warz : j["balloon_warz2"].get<vector<pair<int, int>>>());
			Balloon_Warz.top_balloon = (!(j.find("ballon_list2") != j.end()) ? Balloon_Warz.top_balloon : j["ballon_list2"].get<vector<pair<int, string>>>());
			Balloon_Warz.top_balloon_winners = (!(j.find("balloon_winners") != j.end()) ? Balloon_Warz.top_balloon_winners : j["balloon_winners"].get<vector<pair<int, string>>>());
			role_price.cheater_price2 = (!(j.find("cheater_role3") != j.end()) ? role_price.cheater_price2 : j["cheater_role3"].get<int>());
			role_price.lrayman_price = (!(j.find("lrayman_price") != j.end()) ? role_price.lrayman_price : j["lrayman_price"].get<int>());
			top_points = (!(j.find("top_today" + to_string(today_day)) != j.end()) ? top_points : j["top_today" + to_string(today_day)].get<vector<pair<int, string>>>());
			top_yesterday = (!(j.find("top_today" + to_string(today_day - 1)) != j.end()) ? top_yesterday : j["top_today" + to_string(today_day - 1)].get<vector<pair<int, string>>>());
			top_overall = (!(j.find("top_overall") != j.end()) ? top_overall : j["top_overall"].get<vector<pair<int, string>>>());
		}
		ifs.close();
	}
	/*
	for (const auto& entry : fs::directory_iterator("worlds")) {
		if (!fs::is_directory(entry.path())) {
			string world_text = explode("_.", entry.path().filename().string())[0];
			string path2_ = "worlds/" + world_text + "_.json";
			get_world(world_text);
			save_world(world_text, true);
			cout << world_text << endl;
		}
	}*/
	current_uid = atoll(read_file("guilduid.txt").c_str());
	for (const auto& entry : fs::directory_iterator("guilds")) {
		if (!fs::is_directory(entry.path())) {
			json guild_read;
			ifstream read_guild(entry.path(), ifstream::binary);
			read_guild >> guild_read;
			read_guild.close();
			Guild new_guild{};
			new_guild.guild_id = guild_read["guild_id"].get<int>();
			new_guild.guild_name = guild_read["guild_name"].get<string>();
			new_guild.guild_description = guild_read["guild_description"].get<string>();
			new_guild.guild_mascot = guild_read["guild_mascot"].get<vector<uint16_t>>();
			new_guild.guild_level = guild_read["guild_level"].get<uint16_t>();
			new_guild.guild_xp = guild_read["guild_xp"].get<uint32_t>();
			new_guild.guild_world = guild_read["guild_world"].get<string>();
			new_guild.guild_members = guild_read["guild_members"].get<vector<GuildMember>>();
			new_guild.guild_logs = guild_read["guild_logs"].get<vector<GuildLog>>();
			guilds.push_back(new_guild);
		//	cout << "Loaded guild -> " << new_guild.guild_id << " >> " << new_guild.guild_name << endl;
		}
	}
	BOOL ret = SetConsoleCtrlHandler(ConsoleHandler, TRUE);
	srand(unsigned int(time(nullptr)));
	
	system("cls");
	server_port = 53181;
	refresh_wotd_worlds();
	if (init_enet(server_port) == -1) cout << "enet klaida" << endl;
	else {
		if (server_port == 53181) domain = "https://reugtps.ddns.net/", main_port = 53181;
		if (items_dat() == -1) cout << "items.dat klaida" << endl;
		else cout << setGems(items.size()) << " items loaded, with HASH: " << item_hash << endl;
		cout << "[Server]" << "|PORT:" << server_port << "|MAX ITEMS : " << setGems(items.size()) << "| " << endl;
	}
	Server_Security.up_time_ = time(nullptr);
	for (int i = 0; i < updates.size(); i++) {
		update_List += "\nadd_spacer|small|\nadd_textbox| `7[```9/```0]`` `$" + updates[i] + "``|left|";
		if (i < 20) update_news_list += "\nadd_label_with_icon|small|" + updates[i] + "|left|24|";
		
	}
	load_config();
	load_item_price();
	ENetEvent event;
	while (true) {
		while (enet_host_service(server, &event, 3000) > 0 && (f_saving_ == false and auto_save == false)) {
			if (last_onl < 500) loop_worlds();
			ENetPeer* peer = event.peer;
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT:
			{
				char clientConnection[16];
				enet_address_get_host_ip(&peer->address, clientConnection, 16);
				int logged = 0;
				if (Server_Security.login_time + 6500 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
					Server_Security.login_count = 0;
					Server_Security.update_item_data = 0;
					Server_Security.login_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
				}
				if (Server_Security.login_count < 40 || Server_Security.update_item_data < 15) {
					for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
						if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or clientConnection != pInfo(currentPeer)->ip) continue;
						logged++;
					}
				}
				if (logged >= 3 || Server_Security.login_count > 40 || Server_Security.update_item_data > 15) {
					failed_login(peer, "CT:[S]_ `4OOPS:`` Too many people logging in at once. Please press `5CANCEL`` and try again in a few seconds.");
					break;
				}
				/*
				else {
					vector<pair<string, long long int>>::iterator p = find_if(Server_Security.banned_ip_temporary.begin(), Server_Security.banned_ip_temporary.end(), [&](const pair < string, long long int>& element) { return element.first == clientConnection; });
					if (p != Server_Security.banned_ip_temporary.end()) if (Server_Security.banned_ip_temporary[p - Server_Security.banned_ip_temporary.begin()].second + CONNECT_TIME > (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
						failed_login(peer, "CT:[S]_ `4OOPS:`` Too many people logging in at once. Please press `5CANCEL`` and try again in a few seconds.");
						break;
					}
				}*/
				Server_Security.login_count++;
				peer->data = new Player;
				send_(peer, 1, nullptr, 0);
				variants::OnConsoleMessage(peer, "`oConnected To `2"+server_name+"`` [`wV4.40].");
				pInfo(peer)->ip = clientConnection;
				pInfo(peer)->id = peer->connectID;
				break;
			}
			case ENET_EVENT_TYPE_RECEIVE:
			{
				if (pInfo(peer)->all_packets_time + 1000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
					pInfo(peer)->all_packets = 0;
					pInfo(peer)->all_packets_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
				}
				else {
					if (pInfo(peer)->all_packets >= 560) {
						enet_peer_disconnect_later(peer, 0);
						break;
					}
					else pInfo(peer)->all_packets++;
				}
				string cch = "";
				if (message_(event.packet) == 2 or message_(event.packet) == 3) {
					cch = text_(event.packet);
					if (cch.size() < 5) break;
					if (cch == "action|getDRAnimations\n" || cch == "action|refresh_player_tribute_data\n" || cch.size() > (pInfo(peer)->lastwrenchb == 10374 ? 100000 : 1024) || not isASCII(cch)) {
						enet_packet_destroy(event.packet);
						continue;
					}
					if (pInfo(peer)->lpps + 1000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
						pInfo(peer)->pps = 0;
						pInfo(peer)->lpps = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
					}
					else {
						pInfo(peer)->pps++;
						if (pInfo(peer)->pps >= 16) {
							enet_peer_disconnect_later(peer, 0);
							break;
						}
					}
					if (message_(event.packet) == 3) {
						if (pInfo(peer)->bypass == false) break;
						if (pInfo(peer)->trading_with != -1) cancel_trade(peer, false);
					}
				}
				switch (message_(event.packet)) {
				case 2:
				{
					if (cch.find("player_chat=") != string::npos) break;
					cchs2 = pInfo(peer)->tankIDName + "(" + pInfo(peer)->world + "): " + cch;
					/*
					if (pInfo(peer)->bypass == false) {
						if (cch == "action|enter_game\n" || cch.find("requestedName|") != string::npos || cch.find("tankIDName|") != string::npos || cch.find("action|dialog_return\ndialog_name|puzzle_captcha_submit\n") != string::npos || cch == "action|getDRAnimations\n" || cch == "action|refresh_player_tribute_data\n" || cch.find("action|dialog_return\ndialog_name|2fa\nverificationcode|") != string::npos) {

						}
						else {
							 add_modlogs(peer, pInfo(peer)->name_color + pInfo(peer)->tankIDName, "`6 [" + pInfo(peer)->ip + " SHADOW BANNED BY SYSTEM (PACKET BYPASS 2)", "");
							cout << "banned for bypassing packets 2>> " << pInfo(peer)->tankIDName << " | " <<  cch << endl;
							Server_Security.banned_ip_temporary.push_back(make_pair(pInfo(peer)->ip, (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()));
							enet_peer_disconnect_later(peer, 0);
						}
					}
					*/
					if (not Server_Security.log_player.empty() && Server_Security.log_player == to_lower(pInfo(peer)->tankIDName)) {
						cout << "LOGGING: " << pInfo(peer)->tankIDName << " | " << "DIALOG/TEXT" << "|" << cch << endl;
					}
					/*
					if (server_port == 2) {
						string log = pInfo(peer)->world + " | " + cch;
						ofstream m;
						m.open("db/logs/"+ pInfo(peer)->tankIDName +".txt", std::ios_base::app), m << log, m.close();
					}*/
					loop_worlds();
					if (pInfo(peer)->requestedName.empty()) {
						if (pInfo(peer)->enter_game != 0 || pInfo(peer)->world != "") enet_peer_disconnect_later(peer, 0);
						else {
							if (!player_login(peer, cch)) {
								break;
							}
						}
					}
					else if (cch.find("action|input") != string::npos) {
						vector<string> t_ = explode("|", cch);
						if (t_.size() < 4) break;
						string msg = explode("\n", t_[3])[0];
						if (msg.length() <= 0 || msg.length() > 120 || msg.empty() || std::all_of(msg.begin(), msg.end(), [](char c) {return std::isspace(c); })) continue;
						for (char c : msg) if (c < 0x20 || c>0x7A) continue;
						space_(msg);
						if (pInfo(peer)->tankIDName.empty() || pInfo(peer)->world.empty() || msg[0] == '`' and msg.size() <= 2) break;
						if (msg[0] == '/') SendCmd(peer, msg);
						else if (msg[0] == '!' && pInfo(peer)->dev) SendCmd2(peer, msg);
						else chat_message(peer, msg);
						break;
					}
					else if (cch.find("buttonClicked|set") != string::npos) {
						if (pInfo(peer)->vip) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() > 3) {
								string button_name = t_[t_.size() - 1];
								replaceAll(button_name, "\n", "");
								if (button_name.empty()) break;
								gamepacket_t p;
								p.Insert("OnTextOverlay");
								if (button_name == "set_addslot") {
									if (pInfo(peer)->set.size() < 10) {
										p.Insert("Added slot!");
										pInfo(peer)->set.push_back({});
									}
									else p.Insert("Maximum slot reached!");
								}
								else if (button_name == "set_removeslot") {
									if (pInfo(peer)->set.size() > 1) {
										p.Insert("Removed slot!");
										pInfo(peer)->set.erase(pInfo(peer)->set.begin() + pInfo(peer)->set.size() - 1);
									}
									else p.Insert("Maximum slots removed!");
								}
								else if (button_name.find("set_save_") != string::npos or button_name.find("set_load_") != string::npos) {
									int slot = atoi(button_name.substr(9, button_name.length() - 9).c_str());
									if (slot < 0 or slot > 10) break;
									if (pInfo(peer)->set.size() >= slot) {
										if (button_name.find("set_save_") != string::npos) {
											pInfo(peer)->set[slot] = { pInfo(peer)->hair , pInfo(peer)->shirt , pInfo(peer)->pants , pInfo(peer)->feet , pInfo(peer)->face , pInfo(peer)->hand , pInfo(peer)->back , pInfo(peer)->mask , pInfo(peer)->necklace , pInfo(peer)->ances };
											p.Insert("Set `#" + to_string(slot + 1) + "`` saved!");
										}
										else if (button_name.find("set_load_") != string::npos) {
											p.Insert("Set `#" + to_string(slot + 1) + "`` loaded!");
											if (pInfo(peer)->set[slot].size() != 0) {
												for (int i = 0; i < pInfo(peer)->set[slot].size(); i++) {
													bool ignore_ = false;
													int c_ = inventory_contains(peer, pInfo(peer)->set[slot][i]);
													if (c_ == 0) ignore_ = true;
													if (i == 0) pInfo(peer)->hair = (ignore_ ? 0 : pInfo(peer)->set[slot][i]);
													else if (i == 1)pInfo(peer)->shirt = (ignore_ ? 0 : pInfo(peer)->set[slot][i]);
													else if (i == 2) pInfo(peer)->pants = (ignore_ ? 0 : pInfo(peer)->set[slot][i]);
													else if (i == 3)  pInfo(peer)->feet = (ignore_ ? 0 : pInfo(peer)->set[slot][i]);
													else if (i == 4) pInfo(peer)->face = (ignore_ ? 0 : pInfo(peer)->set[slot][i]);
													else if (i == 5)  pInfo(peer)->hand = (ignore_ ? 0 : pInfo(peer)->set[slot][i]);
													else if (i == 6) pInfo(peer)->back = (ignore_ ? 0 : pInfo(peer)->set[slot][i]);
													else if (i == 7)  pInfo(peer)->mask = (ignore_ ? 0 : pInfo(peer)->set[slot][i]);
													else if (i == 8) pInfo(peer)->necklace = (ignore_ ? 0 : pInfo(peer)->set[slot][i]);
													else if (i == 9) pInfo(peer)->ances = (ignore_ ? 0 : pInfo(peer)->set[slot][i]);
												}
											}
											update_clothes_value(peer);
											update_clothes(peer);
										}
									}
								}
								p.CreatePacket(peer);
								set_tab(peer);
							}
						}
						break;
					}
					else if (cch.find("action|mod_trade") != string::npos or cch.find("action|rem_trade") != string::npos) {
						vector<string> t_ = explode("|", cch);
						if (t_.size() < 3) break;
						int item_id = atoi(explode("\n", t_[2])[0].c_str()), c_ = inventory_contains(peer, item_id);
						if (c_ == 0) break;
						if (items[item_id].untradeable || items[item_id].blockType == BlockTypes::FISH) {
							gamepacket_t p;
							p.Insert("OnTextOverlay");
							p.Insert("You'd be sorry if you lost that!");
							p.CreatePacket(peer);
							break;
						}
						bool cancel_ = false;
						if (item_id == 1424 || item_id == 5816) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								if (to_lower(world_->owner_name) == to_lower(pInfo(peer)->tankIDName) || pInfo(peer)->superdev) {

								}
								else cancel_ = true;
							}

						}
						if (cancel_) if (pInfo(peer)->trading_with != -1) cancel_trade(peer, false);
						if (c_ == 1 or cch.find("action|rem_trade") != string::npos) {
							mod_trade(peer, item_id, c_, (cch.find("action|rem_trade") != string::npos ? true : false));
							break;
						}
						if (cch.find("action|rem_trade") == string::npos) {
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`2Trade`` `w" + items[item_id].name + "``|left|" + to_string(item_id) + "|\nadd_textbox|`2Trade how many?``|left|\nadd_text_input|count||" + to_string(c_) + "|5|\nembed_data|itemID|" + to_string(item_id) + "\nend_dialog|trade_item|Cancel|OK|");
							p.CreatePacket(peer);
						}
						break;
					}
					else if (cch.find("action|trade_accept") != string::npos) {
						if (pInfo(peer)->trading_with != -1) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 3) break;
							string status_ = explode("\n", t_[2])[0];
							if (status_ != "1" and status_ != "0") break;
							bool f_ = false;
							for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
								if (pInfo(currentPeer)->world == pInfo(peer)->world) {
									if (pInfo(currentPeer)->netID == pInfo(peer)->trading_with and pInfo(peer)->netID == pInfo(currentPeer)->trading_with) {
										string name_ = pInfo(peer)->world;
										vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
										if (p != worlds.end()) {
											World* world_ = &worlds[p - worlds.begin()];
											world_->fresh_world = true;
											if (status_ == "1")
												pInfo(peer)->trade_accept = 1;
											else
												pInfo(peer)->trade_accept = 0;
											if (pInfo(peer)->trade_accept and pInfo(currentPeer)->trade_accept) {
												// check inv space   
												if (not trade_space_check(peer, currentPeer)) {
													pInfo(peer)->trade_accept = 0, pInfo(currentPeer)->trade_accept = 0;
													gamepacket_t p;
													p.Insert("OnTradeStatus");
													p.Insert(pInfo(peer)->netID);
													p.Insert("");
													p.Insert("`o" + get_player_nick(peer) + "``'s offer.``");
													p.Insert(make_trade_offer(pInfo(peer), true) + "locked|0\naccepted|0");
													p.CreatePacket(peer);
													{
														gamepacket_t p;
														p.Insert("OnTradeStatus");
														p.Insert(pInfo(peer)->netID);
														p.Insert("");
														p.Insert("`o" + get_player_nick(peer) + "``'s offer.``");
														p.Insert(make_trade_offer(pInfo(peer), true) + "locked|0\nreset_locks|1\naccepted|0");
														p.CreatePacket(currentPeer);
													}
													f_ = true;
													break;
												}
												else if (not trade_space_check(currentPeer, peer)) {
													pInfo(peer)->trade_accept = 0, pInfo(currentPeer)->trade_accept = 0;
													gamepacket_t p;
													p.Insert("OnTradeStatus");
													p.Insert(pInfo(currentPeer)->netID);
													p.Insert("");
													p.Insert("`o" + get_player_nick(currentPeer) + "``'s offer.``");
													p.Insert(make_trade_offer(pInfo(currentPeer), true) + "locked|0\naccepted|0");
													p.CreatePacket(currentPeer);
													{
														gamepacket_t p;
														p.Insert("OnTradeStatus");
														p.Insert(pInfo(currentPeer)->netID);
														p.Insert("");
														p.Insert("`o" + get_player_nick(currentPeer) + "``'s offer.``");
														p.Insert(make_trade_offer(pInfo(currentPeer), true) + "locked|0\nreset_locks|1\naccepted|0");
														p.CreatePacket(peer);
													}
													f_ = true;
													break;
												}
												{
													gamepacket_t p;
													p.Insert("OnForceTradeEnd");
													p.CreatePacket(peer);
												}
												bool blocked = false;
												for (int i_ = 0; i_ < pInfo(currentPeer)->trade_items.size(); i_++) {
													map<string, int>::iterator it;
													for (auto it = pInfo(currentPeer)->trade_items[i_].begin(); it != pInfo(currentPeer)->trade_items[i_].end(); it++) {
														if (inventory_contains(currentPeer, it->first) == 0) if (pInfo(currentPeer)->trading_with != -1) cancel_trade(currentPeer, false, true), blocked = true;
													}
												}
												for (int i_ = 0; i_ < pInfo(peer)->trade_items.size(); i_++) {
													map<string, int>::iterator it;
													for (auto it = pInfo(peer)->trade_items[i_].begin(); it != pInfo(peer)->trade_items[i_].end(); it++) {
														if (inventory_contains(peer, it->first) == 0) if (pInfo(peer)->trading_with != -1) cancel_trade(peer, false, true), blocked = true;
													}
												}
												if (blocked == false) send_trade_confirm_dialog(peer, currentPeer);
												break;
											}
											gamepacket_t p;
											p.Insert("OnTradeStatus");
											p.Insert(pInfo(peer)->netID);
											p.Insert("");
											p.Insert("`o" + get_player_nick(peer) + "``'s offer.``");
											p.Insert(make_trade_offer(pInfo(peer), true) + "locked|0\naccepted|" + status_);
											p.CreatePacket(peer);
											{
												{
													gamepacket_t p;
													p.Insert("OnTradeStatus");
													p.Insert(pInfo(currentPeer)->netID);
													p.Insert("");
													p.Insert("`o" + get_player_nick(currentPeer) + "``'s offer.``");
													p.Insert("locked|0\nreset_locks|1\naccepted|0");
													p.CreatePacket(currentPeer);
												}
												gamepacket_t p;
												p.Insert("OnTradeStatus");
												p.Insert(pInfo(currentPeer)->netID);
												p.Insert("");
												p.Insert("`o" +get_player_nick(currentPeer) + "``'s offer.``");
												p.Insert("locked|0\naccepted|1");
												p.CreatePacket(currentPeer);
												{
													gamepacket_t p;
													p.Insert("OnTradeStatus");
													p.Insert(pInfo(currentPeer)->netID);
													p.Insert("");
													p.Insert("`o" + get_player_nick(currentPeer) + "``'s offer.``");
													p.Insert(make_trade_offer(pInfo(currentPeer), true) + "locked|0\nreset_locks|1\naccepted|0");
													p.CreatePacket(currentPeer);
												}
												{
													gamepacket_t p;
													p.Insert("OnTradeStatus");
													p.Insert(pInfo(peer)->netID);
													p.Insert("");
													p.Insert("`o" + get_player_nick(peer) + "``'s offer.``");
													p.Insert(make_trade_offer(pInfo(peer), true) + "locked|0\nreset_locks|1\naccepted|" + status_);
													p.CreatePacket(currentPeer);
												}
											}
										}
										f_ = true;
										break;
									}
								}
							} if (not f_) {
								if (status_ == "1")
									pInfo(peer)->trade_accept = 1;
								else
									pInfo(peer)->trade_accept = 0;
							}
						}
						break;
					}
					else if (cch == "action|trade_cancel\n") cancel_trade(peer);
					if (pInfo(peer)->trading_with == -1) {
						/*
						if (cch == "action|claimprogressbar\n" || cch == "action|dialog_return\ndialog_name|fruit_mixer_choose_dialog\nslot|0|\nbuttonClicked|goto_maindialog\n\n" || cch == "action|dialog_return\ndialog_name|fruit_mixer_choose_dialog\nslot|1|\nbuttonClicked|goto_maindialog\n\n" || cch == "action|dialog_return\ndialog_name|fruit_mixer_choose_dialog\nslot|2|\nbuttonClicked|goto_maindialog\n\n") {
							send_fruit_mixer(peer, 500);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|fruit_mixer_dialog\nbuttonClicked|blend\n\n") {
							int total = pInfo(peer)->fruit_1_c + pInfo(peer)->fruit_2_c + pInfo(peer)->fruit_3_c;
							if (total == 50) {
								vector<int> list{ 13498, 13568, 13570 };

								if (pInfo(peer)->fruit_1 == 13502 || pInfo(peer)->fruit_2 == 13502 || pInfo(peer)->fruit_3 == 13502) {
									list.insert(list.end(), { 13528, 13486, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526, 13502, 13526 });
								}
								if (pInfo(peer)->fruit_1 == 13540 || pInfo(peer)->fruit_2 == 13540 || pInfo(peer)->fruit_3 == 13540) {
									list.insert(list.end(), { 13532, 13488, 13520, 13518,13520, 13518,13520, 13518,13520, 13518,13520, 13518,13520, 13518,13520, 13518,13520, 13518,13520, 13518,13520, 13518,13520, 13518,13520, 13518,13520, 13518,13520, 13518,13520, 13518, 13520, 13518,13520, 13518,13520, 13518,13520, 13518,13520, 13518,13520, 13518,13520, 13518,13520, 13518,13520, 13518,13520, 13518,13520, 13518,13520, 13518,13520, 13518,13520, 13518,13520, 13518 });
								}
								if (pInfo(peer)->fruit_1 == 13542 || pInfo(peer)->fruit_2 == 13542 || pInfo(peer)->fruit_3 == 13542) {
									list.insert(list.end(), { 13534, 13490, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514, 13516, 13514 });
								}
								if (pInfo(peer)->fruit_1 == 13538 || pInfo(peer)->fruit_2 == 13538 || pInfo(peer)->fruit_3 == 13538) {
									list.insert(list.end(), { 13536, 13492, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512, 13510, 13512 });
								}
								if (pInfo(peer)->fruit_1 == 13544 || pInfo(peer)->fruit_2 == 13544 || pInfo(peer)->fruit_3 == 13544) {
									list.insert(list.end(), { 13530, 13494, 13500, 13522, 13524 , 13522, 13524,  13522, 13524 , 13522, 13524,  13522, 13524 , 13522, 13524,  13522, 13524 , 13522, 13524,  13522, 13524 , 13522, 13524,  13522, 13524 , 13522, 13524, 13522, 13524 , 13522, 13524 , 13522, 13524,  13522, 13524 , 13522, 13524,  13522, 13524 , 13522, 13524,  13522, 13524 , 13522, 13524,  13522, 13524 , 13522, 13524,  13522, 13524 , 13522, 13524, 13522, 13524 });
								}
								int free = get_free_slots(pInfo(peer)), slot = 1, getcount = 1, inventoryfull = 0;
								if (free >= slot) {
										int itemid = list[rand() % list.size()];
										if (itemid == 13502 || itemid == 13526 || itemid == 13520 || itemid == 13518 || itemid == 13516 || itemid == 13514 || itemid == 13510 || itemid == 13512 || itemid == 13524) getcount = 8;
										else if (itemid == 13522) getcount = 20;
										else getcount = 1;

										int itemcount = 0;
										modify_inventory(peer, itemid, itemcount);
										if (itemcount + getcount > 200) inventoryfull = 1;
									
									if (inventoryfull == 0) {
										pInfo(peer)->fruit_1 = 0, pInfo(peer)->fruit_2 = 0, pInfo(peer)->fruit_3 = 0;
										pInfo(peer)->fruit_1_c = 0, pInfo(peer)->fruit_2_c = 0, pInfo(peer)->fruit_3_c = 0;
										int give_ = getcount;
											modify_inventory(peer, itemid, getcount);
										gamepacket_t p, p2;
										p.Insert("OnConsoleMessage"), p.Insert("You received `2"+to_string(give_) + " "+items[itemid].ori_name + "`` from blending the fruits."), p.CreatePacket(peer);
										p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("You received `2" + to_string(give_) + " " + items[itemid].ori_name + "`` from blending the fruits."), p2.Insert(0), p2.Insert(1), p2.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|fruit_mixer_dialog\nbuttonClicked|spp_slot_btn_") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 3) break;
							string button_name = explode("\n", t_[3])[0].c_str();
							int slot = 1;
							if (button_name == "spp_slot_btn_0") {
								slot = 1;
								if (pInfo(peer)->fruit_1_c != 0) {
									int give_back = pInfo(peer)->fruit_1_c;
									if (modify_inventory(peer, pInfo(peer)->fruit_1, give_back) == 0) {
										pInfo(peer)->fruit_1 = 0;
										pInfo(peer)->fruit_1_c = 0;
									}
									else console_msg(peer, "No inventory space.");
								}
							}
							else if (button_name == "spp_slot_btn_1") {
								slot = 2;
								if (pInfo(peer)->fruit_2_c != 0) {
									int give_back = pInfo(peer)->fruit_2_c;
									if (modify_inventory(peer, pInfo(peer)->fruit_2, give_back) == 0) {
										pInfo(peer)->fruit_2 = 0;
										pInfo(peer)->fruit_2_c = 0;
									}
									else console_msg(peer, "No inventory space.");
								}
							}
							else if (button_name == "spp_slot_btn_2") {
								slot = 3;
								if (pInfo(peer)->fruit_3_c != 0) {
									int give_back = pInfo(peer)->fruit_3_c;
									if (modify_inventory(peer, pInfo(peer)->fruit_3, give_back) == 0) {
										pInfo(peer)->fruit_3 = 0;
										pInfo(peer)->fruit_3_c = 0;
									}
									else console_msg(peer, "No inventory space.");
								}
							}
							int i_2734 = inventory_contains(peer, 2734), i_13538 = inventory_contains(peer, 13538), i_13540 = inventory_contains(peer, 13540), i_13542 = inventory_contains(peer, 13542), i_13544 = inventory_contains(peer, 13544);
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("add_label_with_icon|big|`wSuper Fruit Mixing Machine``|left|13548|\nadd_spacer|small|\nadd_textbox|Select a tropical fruit to put in `2Slot " + to_string(slot) + "``:|left|\nadd_spacer|small|\ntext_scaling_string|Defibrillators|\nadd_button_with_icon|" + (i_2734 == 0 || pInfo(peer)->fruit_1 == 2734 || pInfo(peer)->fruit_2 == 2734 || pInfo(peer)->fruit_3 == 2734 ? "" : "2734") + "|Pineapple|" + (i_2734 == 0 || pInfo(peer)->fruit_1 == 2734 || pInfo(peer)->fruit_2 == 2734 || pInfo(peer)->fruit_3 == 2734  ? "frame,disabled" : "staticPurpleFrame") + "|2734|" + to_string(i_2734) + "|\nadd_custom_margin|x:5;y:0|\nadd_button_with_icon|" + (i_13538 == 0 || pInfo(peer)->fruit_1 == 13538 || pInfo(peer)->fruit_2 == 13538 || pInfo(peer)->fruit_3 == 13538 ? "" : "13538") + "|Watermelon|" + (i_13538 == 0 || pInfo(peer)->fruit_1 == 13538 || pInfo(peer)->fruit_2 == 13538 || pInfo(peer)->fruit_3 == 13538 ? "frame,disabled" : "staticPurpleFrame") + "|13538|" + to_string(i_13538) + "|\nadd_custom_margin|x:5;y:0|\nadd_button_with_icon|" + (i_13540 == 0 || pInfo(peer)->fruit_1 == 13540 || pInfo(peer)->fruit_2 == 13540 || pInfo(peer)->fruit_3 == 13540 ? "" : "13540") + "|Kiwi|" + (i_13540 == 0 || pInfo(peer)->fruit_1 == 13540 || pInfo(peer)->fruit_2 == 13540 || pInfo(peer)->fruit_3 == 13540 ? "frame,disabled" : "staticPurpleFrame") + "|13540|" + to_string(i_13540) + "|\nadd_custom_margin|x:5;y:0|\nadd_button_with_icon|" + (i_13542 == 0 || pInfo(peer)->fruit_1 == 13542 || pInfo(peer)->fruit_2 == 13542 || pInfo(peer)->fruit_3 == 13542 ? "" : "13542") + "|Papaya|" + (i_13542 == 0 || pInfo(peer)->fruit_1 == 13542 || pInfo(peer)->fruit_2 == 13542 || pInfo(peer)->fruit_3 == 13542 ? "frame,disabled" : "staticPurpleFrame") + "|13542|" + to_string(i_13542) + "|\nadd_custom_margin|x:5;y:0|\nadd_button_with_icon|" + (i_13544 == 0 || pInfo(peer)->fruit_1 == 13544 || pInfo(peer)->fruit_2 == 13544 || pInfo(peer)->fruit_3 == 13544 ? "" : "13544") + "|Dragon Fruit|" + (i_13544 == 0 || pInfo(peer)->fruit_1 == 13544 || pInfo(peer)->fruit_2 == 13544 || pInfo(peer)->fruit_3 == 13544 ? "frame,disabled" : "staticPurpleFrame") + "|13544|" + to_string(i_13544) + "|\nadd_custom_margin|x:5;y:0|\nadd_button_with_icon||END_LIST|noflags|0||\nadd_custom_margin|x:0;y:10|\nadd_spacer|small|\nadd_button|goto_maindialog|Back|noflags|\nembed_data|slot|" + to_string(slot) + "\nend_dialog|fruit_mixer_choose_dialog|||");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|fruit_mixer_choose_dialog") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 5) break;
							int slot = atoi(explode("\n", t_[3])[0].c_str()), item = atoi(explode("\n", t_[5])[0].c_str()), has = inventory_contains(peer, item);
							if (item == 2734 || item == 13538 || item == 13540 || item == 13542 || item == 13544) {
								if (slot < 0 || slot > 3 || item < 0 || item > items.size() || has == 0) break;
								int total = pInfo(peer)->fruit_1_c + pInfo(peer)->fruit_2_c + pInfo(peer)->fruit_3_c;
								int only_need = (total == 0 ? 48 : 50 - total);
								if (total == 0) {
									only_need = 48;
								}
								else {
									int normal = 51;
									if (pInfo(peer)->fruit_1_c != 0) normal -= pInfo(peer)->fruit_1_c;
									else normal -= 1;

									if (pInfo(peer)->fruit_2_c != 0) normal -= pInfo(peer)->fruit_2_c;
									else normal -= 1;
									if (pInfo(peer)->fruit_3_c != 0) normal -= pInfo(peer)->fruit_3_c;
									else normal -= 1;
									only_need = normal;
								}
								int so_put = (only_need != 0 ? (has >= only_need ? only_need : has) : 0);
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("add_label_with_icon|big|`wSuper Fruit Mixing Machine``|left|13548|\nadd_textbox|How many `2" + items[item].ori_name + "`` would you like to put in the machine?|left|\nadd_textbox|You currently have `2" + to_string(has) + "`` in your inventory.|left|\nadd_text_input|count||" + to_string(so_put) + "|3|\nembed_data|slot|" + to_string(slot) + "\nembed_data|itemID|" + to_string(item) + "\nadd_spacer|small|\nadd_button|goto_choose_fruit_dialog|Back|noflags|\nadd_custom_button|add_fruit|textLabel:Put in Slot " + to_string(slot) + ";anchor:_button_goto_choose_fruit_dialog;left:1;margin:40,0;|\nadd_spacer|small|\nend_dialog|tropical_fruits_count_dialog|||");
								p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|tropical_fruits_count_dialog") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 8) break;
							int slot = atoi(explode("\n", t_[3])[0].c_str()), item = atoi(explode("\n", t_[5])[0].c_str()), has = inventory_contains(peer, item);
							string btn = explode("\n", t_[7])[0].c_str();
							if (btn == "goto_choose_fruit_dialog") {
							}
							else {
								if (item == 2734 || item == 13538 || item == 13540 || item == 13542 || item == 13544) {
									if (atoi(explode("\n", t_[8])[0].c_str()) <= 0 || atoi(explode("\n", t_[8])[0].c_str()) > 48) break;
									int add_ = atoi(explode("\n", t_[8])[0].c_str()) * -1;
									if (slot < 0 || slot > 3 || item < 0 || item > items.size() || has == 0 || abs(add_) <= 0) break;
									int total = pInfo(peer)->fruit_1_c + pInfo(peer)->fruit_2_c + pInfo(peer)->fruit_3_c;
									int only_need = (total == 0 ? 48 : 50 - total);
									if (total == 0) {
										only_need = 48;
									}
									else {
										int normal = 51;
										if (pInfo(peer)->fruit_1_c != 0) normal -= pInfo(peer)->fruit_1_c;
										else normal -= 1;

										if (pInfo(peer)->fruit_2_c != 0) normal -= pInfo(peer)->fruit_2_c;
										else normal -= 1;
										if (pInfo(peer)->fruit_3_c != 0) normal -= pInfo(peer)->fruit_3_c;
										else normal -= 1;
										only_need = normal;
									}
									if (abs(add_) > only_need) add_ = only_need * -1;
									if (slot == 1) {
										pInfo(peer)->fruit_1 = item;
										pInfo(peer)->fruit_1_c += abs(add_);
									}
									else if (slot == 2) {
										pInfo(peer)->fruit_2 = item;
										pInfo(peer)->fruit_2_c += abs(add_);
									}
									else if (slot == 3) {
										pInfo(peer)->fruit_3 = item;
										pInfo(peer)->fruit_3_c += abs(add_);
									}
									modify_inventory(peer, item, add_);
								}
							}
							send_fruit_mixer(peer);
							break;
						}*/
						if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|newbie\n\n") {
							{
								gamepacket_t p(7500), p2, p3;
								p.Insert("OnClearNPCMessage"), p.CreatePacket(peer);
								p2.Insert("OnAddNotification"), p2.Insert("interface/tutorial/tut07_create_world.rttex"), p2.Insert("`wLet's create a worlds! You can create a worlds by enter a random worlds and lock it!``"), p2.Insert("audio/tip_start.wav"), p2.Insert(1), p2.CreatePacket(peer);
								p3.Insert("OnPersistentNPCMessage"), p3.Insert(4032), p3.Insert("Welcome to Growtopia Private Server! Type /help to see all commands.\n`2Pickup`` your newbie rewards and start the journey!"), p3.Insert(1), p3.CreatePacket(peer);
							}
							pInfo(peer)->n = 1;
							if (pInfo(peer)->cc == 0) {
								int itemid = 9160, c_ = 1;
								if (pInfo(peer)->country == "lt") itemid = 9146;
								if (pInfo(peer)->country == "id") itemid = 9148;
								if (pInfo(peer)->country == "tr") itemid = 9150;
								if (pInfo(peer)->country == "ru") itemid = 9152;
								if (pInfo(peer)->country == "us") itemid = 9154;
								if (pInfo(peer)->country == "fi") itemid = 9156;
								if (pInfo(peer)->country == "cn") itemid = 9158;
								if (pInfo(peer)->country == "ae") itemid = 9502;
								if (pInfo(peer)->country == "ro") itemid = 9504;
								if (pInfo(peer)->country == "nl") itemid = 9506;
								if (pInfo(peer)->country == "lb") itemid = 9508;
								if (pInfo(peer)->country == "hu") itemid = 9510;
								if (pInfo(peer)->country == "ua") itemid = 9512;
								if (pInfo(peer)->country == "ph") itemid = 9514;
								if (modify_inventory(peer, itemid, c_) == 0) {
									pInfo(peer)->cc = 1;
									pInfo(peer)->back = itemid;
									pInfo(peer)->hand = 98;
									update_clothes_value(peer);
									update_clothes(peer);
									gamepacket_t p;
									p.Insert("OnDialogRequest");
									p.Insert("set_default_color|`o\nadd_label_with_icon|big|`2" + items[itemid].name + "``|left|" + to_string(itemid) + "|\nadd_smalltext|You received `8" + items[itemid].name + "|left|\nadd_smalltext|`9" + items[itemid].description + "|left|\nend_dialog|||OK|\nadd_quick_exit|\n");
									p.CreatePacket(peer);
								}
							}
							string start = "START";
							join_world(peer, start);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|punish_view") != string::npos) {
							if (pInfo(peer)->vip + pInfo(peer)->mod == 0) break;
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 4) break;
							string button_name = explode("\n", t_[3])[0].c_str();
							if (button_name.find("warp_to_") != string::npos) {
								if (has_playmod2(pInfo(peer), 139)) {
									gamepacket_t p;
									p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("Hmm, you can't do that while cursed."), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
									break;
								}
								string world_name = button_name.substr(8, button_name.length() - 8);
								join_world(peer, world_name);
							}
							else if (button_name == "view_inventory") {
								view_inventory(peer);
							}
							else if (button_name.find("ban_") != string::npos || button_name.find("duc_") != string::npos || button_name.find("cur_") != string::npos || button_name.find("curduc_") != string::npos || button_name == "ridban") {
								if (t_.size() < 5) break;
								if (pInfo(peer)->mod == 1 || pInfo(peer)->dev == 1) {
									string reason = explode("\n", t_[4])[0].c_str();
									if (button_name == "ridban") {
										if (reason.empty()) reason = "no reason";
										string rid, ip, growid;
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
												rid = pInfo(currentPeer)->rid;
												ip = pInfo(currentPeer)->ip;
												growid = pInfo(currentPeer)->tankIDName;
												add_ban_or_mute(currentPeer, 6.307e+7, reason, pInfo(peer)->name_color + pInfo(peer)->tankIDName + "``", 76);
												send_logs("player: " + pInfo(peer)->tankIDName + "" + " RID banned (" + " " + pInfo(currentPeer)->rid + " " + ") - " + " " + pInfo(currentPeer)->tankIDName + "", "Rid Bans");
												add_ridban(currentPeer);
											}
										}
										if (rid != "") {
											for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
												if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->dev or pInfo(peer)->mod or pInfo(peer)->superdev) continue;
												if (pInfo(currentPeer)->rid == rid && pInfo(currentPeer)->ip == ip) {
													add_ban_or_mute(currentPeer, 6.307e+7, "alt of " + growid + " -" + reason, pInfo(peer)->name_color + pInfo(peer)->tankIDName + "``", 76);
													send_logs("player: " + pInfo(peer)->tankIDName + "" + " RID banned (" + " " + pInfo(currentPeer)->rid + " " + ") - " + " `" + pInfo(currentPeer)->tankIDName + "", "Rid Bans");
												}
											}
										}
									}
									else {
										bool online_ = false;
										long long int seconds = atoi(button_name.substr(4, button_name.length() - 4).c_str());
										if (reason.empty() && seconds != 0) {
											gamepacket_t p;
											p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You did not put the reason for this punishment!"), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
											break;
										}
										if (seconds == 729) seconds = 6.307e+7;
										if (seconds == 31) seconds = 2.678e+6;
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
												if (button_name.find("cur_") != string::npos) {
													add_ban_or_mute(currentPeer, seconds, reason, pInfo(peer)->name_color + pInfo(peer)->tankIDName + "``", 139);
													add_modlogs(peer, pInfo(peer)->name_color + pInfo(peer)->tankIDName, "CURSED (" + reason + "): " + pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName + "``", "`#" + ((seconds / 86400 > 0) ? to_string(seconds / 86400) + " days" : (seconds / 3600 > 0) ? to_string(seconds / 3600) + " hours" : (seconds / 60 > 0) ? to_string(seconds / 60) + " minutes" : to_string(seconds) + " seconds"));
												}
												else if (button_name.find("ban_") != string::npos) {
													online_ = true;
													pInfo(peer)->ban_seconds = seconds;
													pInfo(peer)->ban_reason = reason;
													gamepacket_t p;
													p.Insert("OnDialogRequest");
													p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wBan " + pInfo(currentPeer)->tankIDName + "?``|left|278|\nadd_textbox|Time: " + to_playmod_time(seconds) + "|left|\nadd_textbox|Reason: " + reason + "|left|\nend_dialog|ban_player|Cancel|OK|");
													p.CreatePacket(peer);
												}
												else if (button_name.find("curduc_") != string::npos) {
													seconds = 86400;
													add_ban_or_mute(currentPeer, seconds, reason, pInfo(peer)->name_color + pInfo(peer)->tankIDName + "``", 11);
													add_ban_or_mute(currentPeer, seconds, reason, pInfo(peer)->name_color + pInfo(peer)->tankIDName + "``", 139);
													add_modlogs(peer, pInfo(peer)->name_color + pInfo(peer)->tankIDName, "CURSED (" + reason + "): " + pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName + "``", "`#" + ((seconds / 86400 > 0) ? to_string(seconds / 86400) + " days" : (seconds / 3600 > 0) ? to_string(seconds / 3600) + " hours" : (seconds / 60 > 0) ? to_string(seconds / 60) + " minutes" : to_string(seconds) + " seconds"));
												}
												else {
													add_ban_or_mute(currentPeer, seconds, reason, pInfo(peer)->name_color + pInfo(peer)->tankIDName + "``", 11);
													add_modlogs(peer, pInfo(peer)->name_color + pInfo(peer)->tankIDName, "DUCT-TAPED (" + reason + "): " + pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName + "``", "`#" + ((seconds / 86400 > 0) ? to_string(seconds / 86400) + " days" : (seconds / 3600 > 0) ? to_string(seconds / 3600) + " hours" : (seconds / 60 > 0) ? to_string(seconds / 60) + " minutes" : to_string(seconds) + " seconds"));
												}
												break;
											}
										}
										if (button_name.find("ban_") != string::npos && online_ == false && pInfo(peer)->dev) {
											string path_ = "players/" + pInfo(peer)->last_wrenched + "_.json";
											if (_access_s(path_.c_str(), 0) == 0) {
												json r_;
												ifstream f_(path_, ifstream::binary);
												if (f_.fail()) continue;
												f_ >> r_;
												f_.close();
												{
													json f_ = r_["b_t"].get<int>();
													if (seconds == 0) {
														r_["b_t"] = 0;
														r_["b_s"] = 0;
														r_["b_r"] = "";
														r_["b_b"] = "";
														r_["b_t"] = 0;
														add_modlogs(peer, pInfo(peer)->name_color + pInfo(peer)->tankIDName, "UNBANNED: " + pInfo(peer)->last_wrenched + "``", "");
													}
													else {
														if (seconds == 729) r_["b_s"] = seconds = (6.307e+7 * 1000);
														else if (seconds == 31)r_["b_s"] = seconds = (2.678e+6 * 1000);
														else r_["b_s"] = (seconds * 1000);
														r_["b_r"] = reason;
														r_["b_b"] = pInfo(peer)->name_color + pInfo(peer)->tankIDName + "``";
														r_["b_t"] = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
														add_modlogs(peer, pInfo(peer)->name_color + pInfo(peer)->tankIDName, "BANNED (" + reason + "): " + pInfo(peer)->last_wrenched + "``", "`#" + ((seconds / 86400 > 0) ? to_string(seconds / 86400) + " days" : (seconds / 3600 > 0) ? to_string(seconds / 3600) + " hours" : (seconds / 60 > 0) ? to_string(seconds / 60) + " minutes" : to_string(seconds) + " seconds"));
													}
												}
												{
													ofstream f_(path_, ifstream::binary);
													f_ << r_;
													f_.close();
												}
											}
										}
									}
								}
							}
							else if (button_name == "Logs" || button_name == "Trade History" || button_name == "Owned Worlds" || button_name == "Last Worlds" || button_name == "disconnect") {
								bool online_ = false;
								string logger = "";
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->growid == false) continue;
									if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
										if (button_name == "Logs") logger = (pInfo(currentPeer)->bans.size() == 0 ? "This player has clear records / no warning." : join(pInfo(currentPeer)->bans, ", "));
										else if (button_name == "Trade History") logger = join(pInfo(currentPeer)->trade_history, "`o, ``");
										else if (button_name == "Owned Worlds") logger = join(pInfo(currentPeer)->worlds_owned, "`o, ``");
										else if (button_name == "Last Worlds") logger = join(pInfo(currentPeer)->last_visited_worlds, "`o, ``");
										else if (button_name == "disconnect" && pInfo(peer)->superdev) {
											enet_peer_disconnect_later(currentPeer, 0);
											break;
										}
										gamepacket_t p;
										p.Insert("OnDialogRequest");
										p.Insert("set_default_color|`o\nadd_label_with_icon|big|`1Punish / View - " + pInfo(currentPeer)->tankIDName + " (" + pInfo(currentPeer)->d_name + ") - #" + to_string(pInfo(currentPeer)->netID) + "``|left|732|\nadd_progress_bar|`w" + pInfo(currentPeer)->tankIDName + "``|big|Level " + to_string(pInfo(currentPeer)->level) + "|" + to_string(pInfo(currentPeer)->xp) + "|" + to_string(50 * ((pInfo(currentPeer)->level * pInfo(currentPeer)->level) + 2)) + "|(" + to_string(pInfo(currentPeer)->xp) + "/" + to_string(50 * ((pInfo(currentPeer)->level * pInfo(currentPeer)->level) + 2)) + ")|-3669761|\nadd_spacer|small\nadd_textbox|`6" + button_name + "``|left|\nadd_smalltext|" + logger + "|left|\nadd_spacer|small\nend_dialog|punish_view|Cancel||\nadd_quick_exit|");
										p.CreatePacket(peer);
										online_ = true;
										break;
									}
								}
								if (online_ == false) {
									try {
										string name = pInfo(peer)->last_wrenched;
										ifstream ifs("players/" + name + "_.json");
										if (ifs.is_open()) {
											json j;
											ifs >> j;
											if (button_name == "Logs") logger = (j["7bans"].size() == 0 ? "This player has clear records / no warning." : join(j["7bans"], ", "));
											else if (button_name == "Trade History") logger = join(j["t_h"], "`o, ``");
											else if (button_name == "Owned Worlds") logger = join(j["worlds_owned"], "`o, ``");
											else if (button_name == "Last Worlds") logger = join(j["la_wo"], "`o, ``");
											gamepacket_t p;
											p.Insert("OnDialogRequest");
											p.Insert("set_default_color|`o\nadd_label_with_icon|big|`1Punish / View -`` `0" + name + "``|left|732|\nadd_spacer|small\nadd_textbox|`6" + button_name + "``|left|\nadd_smalltext|" + logger + "|left|\nadd_spacer|small\nend_dialog|punish_view|Cancel||\nadd_quick_exit|");
											p.CreatePacket(peer);
										}
									}
									catch (exception) {
										break;
									}
								}
							}
							else if (button_name == "wipe_inventory") {
								if (pInfo(peer)->give_role) {
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
											pInfo(currentPeer)->inv.clear(), pInfo(currentPeer)->bp.clear();
											pInfo(currentPeer)->gems = 0, pInfo(currentPeer)->vip = 0, pInfo(currentPeer)->mod = 0;
											pInfo(currentPeer)->hair = 0;
											pInfo(currentPeer)->shirt = 0;
											pInfo(currentPeer)->pants = 0;
											pInfo(currentPeer)->feet = 0;
											pInfo(currentPeer)->face = 0;
											pInfo(currentPeer)->hand = 0;
											pInfo(currentPeer)->back = 0;
											pInfo(currentPeer)->mask = 0;
											pInfo(currentPeer)->necklace = 0;
											pInfo(currentPeer)->ances = 0;
											update_clothes(currentPeer);
											OnSetGems(currentPeer);
											vector<uint16_t> list{ 454, 682, 3004, 1154,4584, 526, 5666, 340, 3838, 5990 }, list2{ 1008, 866, 872, 928 };
											uint16_t list_s1 = list[rand() % list.size()], list_s2 = list2[rand() % list2.size()];
											pInfo(currentPeer)->inv.push_back({ 18, 1 }), pInfo(currentPeer)->inv.push_back({ 32, 1 }), pInfo(currentPeer)->inv.push_back({ 6336, 1 }), pInfo(currentPeer)->inv.push_back({ 204, 1 }), pInfo(currentPeer)->inv.push_back({ 98, 1 }), pInfo(currentPeer)->inv.push_back({ 954, 20 }), pInfo(currentPeer)->inv.push_back({ list_s1, 50 }), pInfo(currentPeer)->inv.push_back({ 7066, 2 }), pInfo(currentPeer)->inv.push_back({ 3898, 1 }), pInfo(currentPeer)->inv.push_back({ 8430, 1 }), pInfo(currentPeer)->inv.push_back({ list_s2, 1 });
											for (int i_ = 11; i_ <= 16; i_++) pInfo(currentPeer)->inv.push_back({ 0,0 });

											send_inventory(currentPeer);
											pInfo(currentPeer)->gtwl = 0;
										}
									}
								}
							}
							else if (button_name == "login_as") {
								if (pInfo(peer)->give_role) {
									gamepacket_t p;
									p.Insert("SetHasGrowID"), p.Insert(1), p.Insert(pInfo(peer)->last_wrenched), p.Insert(pInfo(peer)->login_pass), p.CreatePacket(peer);
									enet_peer_disconnect_later(peer, 0);
								}
							}
							else if (button_name.find("unbanrid_") != string::npos) {
								if (pInfo(peer)->dev or pInfo(peer)->superdev) {
									SendCmd(peer, "/banrid " + button_name.substr(9, button_name.length() - 9), true);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|warp_to_") != string::npos || cch.find("action|dialog_return\ndialog_name|zz\nbuttonClicked|warp_to_") != string::npos || cch.find("action|dialog_return\ndialog_name|top\nbuttonClicked|warp_to_") != string::npos) {
							if (has_playmod2(pInfo(peer), 139)) {
								gamepacket_t p;
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("Hmm, you can't do that while cursed."), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
								break;
							}
							string world_name = "";
							if (cch.find("action|dialog_return\ndialog_name|zz\nbuttonClicked|warp_to_") != string::npos) world_name = cch.substr(58, cch.length() - 60);
							else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|warp_to_") != string::npos) world_name = cch.substr(56, cch.length() - 58);
							else if (cch.find("action|dialog_return\ndialog_name|top\nbuttonClicked|warp_to_") != string::npos) world_name = cch.substr(59, cch.length() - 61);
							join_world(peer, world_name);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|blast\nname|") != string::npos) {
							if (pInfo(peer)->lastchoosenitem == 830 || pInfo(peer)->lastchoosenitem == 9164 || pInfo(peer)->lastchoosenitem == 8738 || pInfo(peer)->lastchoosenitem == 10380 || pInfo(peer)->lastchoosenitem == 9602 || pInfo(peer)->lastchoosenitem == 942 || pInfo(peer)->lastchoosenitem == 1060 || pInfo(peer)->lastchoosenitem == 1136 || pInfo(peer)->lastchoosenitem == 1402 || pInfo(peer)->lastchoosenitem == 9582 || pInfo(peer)->lastchoosenitem == 1532 || pInfo(peer)->lastchoosenitem == 3562 || pInfo(peer)->lastchoosenitem == 4774 || pInfo(peer)->lastchoosenitem == 7380 || pInfo(peer)->lastchoosenitem == 7588 || pInfo(peer)->lastchoosenitem == 8556) {
								int blast = pInfo(peer)->lastchoosenitem;
								string world = cch.substr(44, cch.length() - 44).c_str();
								replace_str(world, "\n", "");
								transform(world.begin(), world.end(), world.begin(), ::toupper);
								if (find_if(worlds.begin(), worlds.end(), [world](const World& a) { return a.name == world; }) != worlds.end() || not check_blast(world) || _access_s(("worlds/" + world + "_.json").c_str(), 0) == 0) {
									gamepacket_t p;
									p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("That world name already exists. You'll have to be more original. Maybe add some numbers after it?"), p.Insert(0), p.Insert(1), p.CreatePacket(peer);
								}
								else {
									int got = -1;
									if (modify_inventory(peer, blast, got) == 0) {
										create_world_blast(peer, world, blast);
										if (blast == 830) modify_inventory(peer, 834, got = -100);
										join_world(peer, world);
										gamepacket_t p(750), p2(750);
										p.Insert("OnConsoleMessage"), p.Insert("** `5" + pInfo(peer)->tankIDName + " activates a " + items[blast].name + "! ``**"), p.CreatePacket(peer);
										p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("** `5" + pInfo(peer)->tankIDName + " activates a " + items[blast].name + "! ``**"), p2.Insert(0), p2.Insert(1), p2.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|supermusic") != string::npos) {
							try {
								if (cch.find("tilex|") != string::npos or cch.find("tiley|") != string::npos) {
									int x_ = atoi(explode("\n", explode("tilex|", cch)[1])[0].c_str()), y_ = atoi(explode("\n", explode("tiley|", cch)[1])[0].c_str());
									string name_ = pInfo(peer)->world;
									vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
									if (p != worlds.end()) {
										World* world_ = &worlds[p - worlds.begin()];
										WorldBlock* block_ = &world_->blocks.at(x_ + (y_ * 100));
										if (cch.find("buttonClicked|manual") != std::string::npos) {
											gamepacket_t p;
											p.Insert("OnDialogRequest");
											p.Insert("set_default_color|`o\nadd_label_with_icon|big|`w" + items[block_->fg].ori_name + "``|left|" + to_string(block_->fg) + "|\nadd_spacer|small|\nembed_data|tilex|" + to_string(x_) + "\nembed_data|tiley|" + to_string(y_) + "\nadd_textbox|This block will play up to 5 music notes simultaneously when the Sheet Music Marker reaches its position. It can play notes of any pitch.|left|\nadd_textbox|In the `2Volume`` box, enter a volume level for these notes, from 1-100. 100 is the normal volume of music notes.|left|\nadd_textbox|In the `2Notes`` box, enter up to 5 music notes to play. For each note, you enter 3 symbols:|left|\nadd_smalltext|- First, the instrument to play: `2P`` for Piano, `2B`` for Bass, `2S`` for Sax, `2F`` for Flute, `2G`` for Guitar, `2V`` for Violin, `2L`` for Lyre, `2E`` for Electric Guitar, `2T`` for Mexican Trumpet  or `2D`` for Drums.|left|\nadd_smalltext|- Next, the note to play, `2A to G``, as in normal music notation. Lowercase for lower octave, uppercase for higher.|left|\nadd_smalltext|- Last, a `2#`` for a sharp note, a `2-`` for a natural note, or a `2b`` for a flat note.|left|\nadd_spacer|small|\nadd_textbox|Example: `3\"PA# DB- BCb\"`` will simultaneously play an A# on the Piano, a bass drum on the Drums, and a Cb on the Bass.|left|\nadd_smalltext|Spaces are optional, but sure make it easier to read.|left|\nadd_spacer|small|\nadd_text_input|volume|Volume|" + to_string(block_->pr) + "|3|\nadd_text_input|text|Notes|" + block_->txt + "|20|\nend_dialog|supermusic|Cancel|Update|\n");
											p.CreatePacket(peer);
											break;
										}
										else {
											if (cch.find("volume|") != string::npos and cch.find("text|") != string::npos) {
												int Volume = atoi(explode("\n", explode("volume|", cch)[1])[0].c_str());
												string Note = explode("\n", explode("text|", cch)[1])[0], Notes = "", Error = "", Note_Rewrite = "";
												Notes = Note;
												if (Volume < 0) Volume = 0; if (Volume > 100) Volume = 100;
												replaceAll(Notes, " ", "");
												bool Update = true;
												int Note_Length = Notes.size() / 3;
												if (!Note.empty()) {
													if (Notes.size() > 3) {
														for (int i = 0; i < Note_Length; i++) {
															char Note_1 = Notes[0 + (2 * i) + (i > 0 ? i : 0)];
															char Note_2 = Notes[1 + (2 * i) + (i > 0 ? i : 0)];
															char Note_3 = Notes[2 + (2 * i) + (i > 0 ? i : 0)];
															bool Skip_Second_Note = false;
															if (Note_2 == 'A' or Note_2 == 'B' or Note_2 == 'C' or Note_2 == 'D' or Note_2 == 'E' or Note_2 == 'F' or Note_2 == 'G') {
																Skip_Second_Note = true;
															}
															else if (Note_2 == 'a' or Note_2 == 'b' or Note_2 == 'c' or Note_2 == 'd' or Note_2 == 'e' or Note_2 == 'f' or Note_2 == 'g') {
																Skip_Second_Note = true;
															}
															if (Note_3 == NULL) {
																Error = "`4Your last note is missing its modifier (#, -, or b)!``", Update = false;
																break;
															}
															else if (Note_3 != '#' and Note_3 != '-' and Note_3 != 'b') {
																Error = "`4Valid modifiers are -, #, or b!``", Update = false;
																break;
															}
															else if (Note_2 == NULL) {
																Error = "`4Your last note is missing its note!``", Update = false;
																break;
															}
															else if (!Skip_Second_Note and Note_2 != 'A' and Note_2 != 'B' and Note_2 != 'C' and Note_2 != 'D' and Note_2 != 'E' and Note_2 != 'F' and Note_2 != 'G') {
																Error = "`4Notes must be from A to G!``", Update = false;
																break;
															}
															else if (!Skip_Second_Note and Note_2 != 'a' and Note_2 != 'b' and Note_2 != 'c' and Note_2 != 'd' and Note_2 != 'e' and Note_2 != 'f' and Note_2 != 'g') {
																Error = "`4Notes must be from A to G!``", Update = false;
																break;
															}
															else if (Note_1 != 'P' and Note_1 != 'B' and Note_1 != 'S' and Note_1 != 'F' and Note_1 != 'G' and Note_1 != 'V' and Note_1 != 'L' and Note_1 != 'D' and Note_1 != 'T' and Note_1 != 'E') {
																Error = "`4The only valid instruments are P, B, S, F, G, V, L, D, T and E!``", Update = false;
																break;
															}
															for (int i_ = 0; i_ < 3; i_++) {
																Note_Rewrite += Notes[i_ + (2 * i) + (i > 0 ? i : 0)];
															}
															Note_Rewrite += " ";
														}
													}
													else {
														bool Skip_Second_Note = false;
														if (Notes[1] == 'A' or Notes[1] == 'B' or Notes[1] == 'C' or Notes[1] == 'D' or Notes[1] == 'E' or Notes[1] == 'F' or Notes[1] == 'G') {
															Skip_Second_Note = true;
														}
														else if (Notes[1] == 'a' or Notes[1] == 'b' or Notes[1] == 'c' or Notes[1] == 'd' or Notes[1] == 'e' or Notes[1] == 'f' or Notes[1] == 'g') {
															Skip_Second_Note = true;
														}
														if (Notes[2] == NULL) Error = "`4Your last note is missing its modifier (#, -, or b)!``", Update = false;
														else if (Notes[2] != '#' and Notes[2] != '-' and Notes[2] != 'b') {
															Error = "`4Valid modifiers are -, #, or b!``", Update = false;
														}
														else if (Notes[1] == NULL) Error = "`4Your last note is missing its note!``", Update = false;
														else if (!Skip_Second_Note and Notes[1] != 'A' and Notes[1] != 'B' and Notes[1] != 'C' and Notes[1] != 'D' and Notes[0] != 'E' and Notes[0] != 'F' and Notes[0] != 'G') {
															Error = "`4Notes must be from A to G!``", Update = false;
														}
														else if (!Skip_Second_Note and Notes[1] != 'a' and Notes[1] != 'b' and Notes[1] != 'c' and Notes[1] != 'd' and Notes[1] != 'e' and Notes[1] != 'f' and Notes[1] != 'g') {
															Error = "`4Notes must be from A to G!``", Update = false;
														}
														else if (Notes[0] != 'P' and Notes[0] != 'B' and Notes[0] != 'S' and Notes[0] != 'F' and Notes[0] != 'G' and Notes[0] != 'V' and Notes[0] != 'L' and Notes[0] != 'D' and Notes[0] != 'T' and Notes[0] != 'E') {
															Error = "`4The only valid instruments are P, B, S, F, G, V, L, D, T and E!``", Update = false;
														}
														Note_Rewrite = Notes + " ";
													}
												}
												if (Error != "") {
													gamepacket_t p;
													p.Insert("OnDialogRequest");
													p.Insert("set_default_color|`o\nadd_label_with_icon|big|`w" + items[block_->fg].ori_name + "``|left|" + to_string(block_->fg) + "|\nadd_spacer|small|\nembed_data|tilex|" + to_string(x_) + "\nembed_data|tiley|" + to_string(y_) + "\n" + (Error != "" ? "add_textbox|" + Error + "|left|\n" : "") + "add_button|manual|Instructions|noflags|0|0|\nadd_spacer|small|\nadd_text_input|volume|Volume|" + to_string(Volume) + "|3|\nadd_text_input|text|Notes|" + Note + "|20|\nend_dialog|supermusic|Cancel|Update|\n");
													p.CreatePacket(peer);
												}
												if (Update) {
													gamepacket_t p;
													p.Insert("OnTalkBubble"); p.Insert(pInfo(peer)->netID);
													p.Insert("Updated " + items[block_->fg].ori_name + "!"); p.Insert(0), p.Insert(0);
													p.CreatePacket(peer);
													block_->pr = Volume; block_->txt = Note_Rewrite;
													PlayerMoving data_{};
													data_.packetType = 5, data_.punchX = x_, data_.punchY = y_, data_.characterState = 0x8;
													BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, block_));
													BYTE* blc = raw + 56;
													form_visual(blc, *block_, *world_, peer, false, false, x_, y_);
													for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
														if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != world_->name) continue;
														send_raw(currentPeer, 4, raw, 112 + alloc_(world_, block_), ENET_PACKET_FLAG_RELIABLE);
													}
													delete[] raw, blc;
													if (block_->locked) upd_lock(*block_, *world_, peer);
												}
											}
											break;
										}
									}
								}
							}
							catch (...) {
								break;
							}
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|credit_card\nbuttonClicked|deposit\n\n") {
							convert_wls(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|credit_card\nbuttonClicked|withdraw\n\n") {
							convert_wls(peer, true);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|megaphone\nitemID|2480|\nwords|") != string::npos) {
							string text = cch.substr(62, cch.length() - 63).c_str();
							bool cansb = true;
							int c_ = inventory_contains(peer, 2480);
							if (c_ == 0) continue;
							if (has_playmod2(pInfo(peer), 11)) {
								gamepacket_t p;
								p.Insert("OnConsoleMessage");
								p.Insert("`6>> That's sort of hard to do while duct-taped.``");
								p.CreatePacket(peer);
								cansb = false;
								continue;
							}
							if (has_playmod2(pInfo(peer), 13)) {
								int time_ = 0;
								for (PlayMods peer_playmod : pInfo(peer)->playmods) {
									if (peer_playmod.id == 13) {
										time_ = peer_playmod.time - time(nullptr);
										break;
									}
								}
								packet_(peer, "action|log\nmsg|>> (" + to_playmod_time(time_) + " before you can broadcast again)", "");
								break;
							}
							if (cansb) {
								pInfo(peer)->usedmegaphone = 1;
								SendCmd(peer, "/sb " + text, false);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|notebook_edit\nbuttonClicked|cancel") != string::npos || cch.find("action|dialog_return\ndialog_name|notebook_edit\nbuttonClicked|clear") != string::npos) {
							if (cch.find("action|dialog_return\ndialog_name|notebook_edit\nbuttonClicked|clear") != string::npos) 	pInfo(peer)->note = "";
							send_wrench_self(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|rules\n\n") {
							SendCmd(peer, "/rules", false);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|online_status\nbuttonClicked|online_status") != string::npos) {
							if (cch.find("checkbox_status_online|") != string::npos and cch.find("checkbox_status_busy|") != string::npos and cch.find("checkbox_status_away|") != string::npos)  {
								pInfo(peer)->p_status = atoi(explode("\n", explode("checkbox_status_online|", cch)[1])[0].c_str()) == 1 ? 0 : atoi(explode("\n", explode("checkbox_status_busy|", cch)[1])[0].c_str()) == 1 ? 1 : atoi(explode("\n", explode("checkbox_status_away|", cch)[1])[0].c_str()) == 1 ? 2 : 0;
								send_wrench_self(peer);
							}
						break;
						}
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|promote\n\n") {
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wThe " + server_name + " Gazette``|left|5016|\nadd_image_button||" + news_banner + "|bannerlayout|||\nadd_spacer|small|\nadd_image_button|iotm_layout|interface/gtps/discord.rttex|3imageslayout|" + discord_url + "|Do you want to join our Discord Server?|\nadd_image_button|rules|interface/gtps/rules.rttex|3imageslayout|||\nadd_image_button|promote|interface/gtps/features.rttex|3imageslayout|||\nadd_spacer|small|\nadd_image_button||interface/large/gazette/gtps_1.rttex|3imageslayout|||\nadd_image_button||interface/large/gazette/gtps_2.rttex|3imageslayout|||\nadd_image_button||interface/large/gazette/gtps_3.rttex|3imageslayout|||\nadd_spacer|small|\nadd_textbox|`2" + server_name + "`` `$is updated every single day, we do provide huge updates every few weeks but update the game everyday and we apply bug fixes really fast!``|left|\nadd_spacer|small|\nadd_textbox|`5Features`` `0(recently added, updated 2022/11/15)```5:``|left|" + update_List + "\nadd_spacer|small|\nadd_image_button|close|interface/large/gtps_continue.rttex|3imageslayout|||\nend_dialog|gazette|\nadd_quick_exit|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|notebook_edit\nbuttonClicked|save\n\npersonal_note|") != string::npos) {
							string btn = cch.substr(81, cch.length() - 82).c_str();
							if (btn.length() > 128) continue;
							pInfo(peer)->note = btn;
							send_wrench_self(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|3898\nbuttonClicked|") != string::npos || cch == "action|dialog_return\ndialog_name|zurgery_back\nbuttonClicked|53785\n\n" || cch == "action|dialog_return\ndialog_name|zurgery_purchase\nbuttonClicked|chc4_1\n\n" || cch == "action|dialog_return\ndialog_name|wolf_back\nbuttonClicked|53785\n\n" || cch == "action|dialog_return\ndialog_name|wolf_purchase\nbuttonClicked|chc5_1\n\n" || cch == "action|dialog_return\ndialog_name|zombie_back\nbuttonClicked|53785\n\n") {
							string btn = cch.substr(52, cch.length() - 52).c_str();
							bool fail = false;
							if (cch == "action|dialog_return\ndialog_name|zurgery_back\nbuttonClicked|53785\n\n" || cch == "action|dialog_return\ndialog_name|wolf_back\nbuttonClicked|53785\n\n" || cch == "action|dialog_return\ndialog_name|zombie_back\nbuttonClicked|53785\n\n") btn = "53785";
							if (cch == "action|dialog_return\ndialog_name|zurgery_purchase\nbuttonClicked|chc4_1\n\n") btn = "chc4_1";
							if (cch == "action|dialog_return\ndialog_name|wolf_purchase\nbuttonClicked|chc5_1\n\n") btn = "chc5_1";
							replace_str(btn, "\n", "");
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							if (btn == "12345") p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wCrazy Jim's Quest Emporium``|left|3902|\nadd_textbox|HEEEEYYY there Growtopian! I'm Crazy Jim, and my quests are so crazy they're KERRRRAAAAZZY!! And that is clearly very crazy, so please, be cautious around them. What can I do ya for, partner?|left|\nadd_button|chc1_1|Daily Quest|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
							else if (btn == "53785") p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSales-Man``|left|4358|\nadd_textbox|It is I, Sales-Man, savior of the wealthy! Let me rescue you from your riches. What would you like to buy today?|left|\nadd_button|chc4_1|Surgery Items|noflags|0|0|\nadd_button|chc5_1|Wolfworld Items|noflags|0|0|\nadd_button|chc3_1|Zombie Defense Items|noflags|0|0|\nadd_button|chc2_1|Blue Gem Lock|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
							else if (btn == "chc1_1") {
								if (pInfo(peer)->dd == 0) {
									int haveitem1 = 0, haveitem2 = 0, received = 0;
									modify_inventory(peer, item1, haveitem1);
									modify_inventory(peer, item2, haveitem2);
									if (haveitem1 >= item1c && haveitem2 >= item2c) received = 1;
									p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wCrazy Jim's Daily Quest``|left|3902|\nadd_textbox|I guess some people call me Crazy Jim because I'm a bit of a hoarder. But I'm very particular about what I want! And today, what I want is this:|left|\nadd_label_with_icon|small|`2" + to_string(item1c) + " " + items[item1].name + "|left|" + to_string(item1) + "|\nadd_smalltext|and|left|\nadd_label_with_icon|small|`2" + to_string(item2c) + " " + items[item2].name + "|left|" + to_string(item2) + "|\nadd_spacer|small|\nadd_smalltext|You shove all that through the phone (it works, I've tried it), and I will hand you one of the `2Growtokens`` from my personal collection!  But hurry, this offer is only good until midnight, and only one `2Growtoken`` per person!|left|\nadd_spacer|small|\nadd_smalltext|`6(You have " + to_string(haveitem1) + " " + items[item1].name + " and " + to_string(haveitem2) + " " + items[item2].name + ")``|left|\nadd_spacer|small|" + (received == 1 ? "\nadd_button|turnin|Turn in items|noflags|0|0|" : "") + "\nadd_spacer|small|\nadd_button|12345|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
								}
								else p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wCrazy Jim's Daily Quest``|left|3902|\nadd_textbox|You've already completed my Daily Quest for today! Call me back after midnight to hear about my next cravings.|left|\nadd_button|12345|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
							}
							else if (btn == "turnin") {
								if (pInfo(peer)->dd == 0) {
									int haveitem1 = 0, haveitem2 = 0, received = 0, remove = -1, remove2 = -1, giveitem = 1;
									modify_inventory(peer, item1, haveitem1);
									modify_inventory(peer, item2, haveitem2);
									if (haveitem1 >= item1c && haveitem2 >= item2c) received = 1;
									if (received == 1) {
										if (thedaytoday == 4) giveitem = 3;
										if (has_playmod2(pInfo(peer), 112)) {
											if (rand() % 100 < 25) giveitem *= 2;
										}
										if (pInfo(peer)->gp) {
											if (complete_gpass_task(peer, "Growtoken")) giveitem++;
										}
										if (pInfo(peer)->C_QuestActive && pInfo(peer)->C_QuestKind == 11 && pInfo(peer)->C_QuestProgress < pInfo(peer)->C_ProgressNeeded) {
											pInfo(peer)->C_QuestProgress += giveitem;
											if (pInfo(peer)->C_QuestProgress >= pInfo(peer)->C_ProgressNeeded) {
												pInfo(peer)->C_QuestProgress = pInfo(peer)->C_ProgressNeeded;
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("`9Ring Quest task complete! Go tell the Ringmaster!");
												p.Insert(0), p.Insert(0);
												p.CreatePacket(peer);
											}
										}
										int gots = giveitem;
										if (pInfo(peer)->lwiz_step == 6) add_lwiz_points(peer, giveitem);
										pInfo(peer)->dd = 1;
										modify_inventory(peer, item1, remove *= item1c);
										modify_inventory(peer, item2, remove2 *= item2c);
										modify_inventory(peer, 1486, giveitem);
										{
											{
												string name_ = pInfo(peer)->world;
												vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
												if (p != worlds.end()) {
													World* world_ = &worlds[p - worlds.begin()];
													world_->fresh_world = true;
													PlayerMoving data_{};
													data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16;
													data_.packetType = 19, data_.plantingTree = 500;
													data_.punchX = 1486, data_.punchY = pInfo(peer)->netID;
													int32_t to_netid = pInfo(peer)->netID;
													BYTE* raw = packPlayerMoving(&data_);
													raw[3] = 5;
													memcpy(raw + 8, &to_netid, 4);
													for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
														if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
														if (pInfo(currentPeer)->world == world_->name) {
															send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
														}
													}
													delete[] raw;
												}
											}
											/*
											if (pInfo(peer)->halloween_ptask_1 == 0) {
												if (pInfo(peer)->hand == halloween_quest || pInfo(peer)->necklace == halloween_quest || pInfo(peer)->back == halloween_quest || pInfo(peer)->face == halloween_quest || pInfo(peer)->mask == halloween_quest || pInfo(peer)->hair == halloween_quest || pInfo(peer)->feet == halloween_quest || pInfo(peer)->shirt == halloween_quest || pInfo(peer)->pants == halloween_quest) {
													add_halloween_point(peer, 1, true);
												}
											}*/
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("[`6You jammed " + to_string(item1c) + " " + items[item1].name + " and " + to_string(item2c) + " " + items[item2].name + " into the phone, and " + to_string(gots) + " `2Growtoken`` popped out!``]");
											p.CreatePacket(peer);
										}
									}
								}
								else {
									p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wCrazy Jim's Daily Quest``|left|3902|\nadd_textbox|You've already completed my Daily Quest for today! Call me back after midnight to hear about my next cravings.|left|\nadd_button|12345|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
									p.CreatePacket(peer);
								}
							}
							else if (btn == "chc2_1") {
								int c_ = 0;
								modify_inventory(peer, 1796, c_);
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wBlue Gem Lock``|left|7188|\nadd_textbox|Excellent! I'm happy to sell you a Blue Gem Lock in exchange for 100 Diamond Lock..|left|\nadd_smalltext|`6You have " + to_string(c_) + " Diamond Lock.``|left|" + (c_ >= 100 ? "\nadd_button|chc2_2_1|Thank you!|noflags|0|0|" : "") + "\nadd_button|53785|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
							}
							else if (btn == "chc2_2_1") {
								int c7188 = 0, c1796 = 0, additem = 0;
								modify_inventory(peer, 1796, c1796);
								if (c1796 < 100) continue;
								modify_inventory(peer, 7188, c7188);
								if (c7188 >= 200) {
									gamepacket_t p;
									p.Insert("OnTalkBubble");
									p.Insert(pInfo(peer)->netID);
									p.Insert("You don't have room in your backpack!");
									p.Insert(0), p.Insert(1);
									p.CreatePacket(peer);
									{
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("You don't have room in your backpack!");
										p.CreatePacket(peer);
									}
									fail = true;
									continue;
								}
								if (c1796 >= 100) {
									if (get_free_slots(pInfo(peer)) >= 2) {
										int cz_ = 1;
										if (modify_inventory(peer, 1796, additem = -100) == 0) {
											modify_inventory(peer, 7188, additem = 1);
											{
												PlayerMoving data_{};
												data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16;
												data_.packetType = 19, data_.plantingTree = 500;
												data_.punchX = 7188, data_.punchY = pInfo(peer)->netID;
												int32_t to_netid = pInfo(peer)->netID;
												BYTE* raw = packPlayerMoving(&data_);
												raw[3] = 5;
												memcpy(raw + 8, &to_netid, 4);
												for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
													if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
													if (pInfo(currentPeer)->world == pInfo(peer)->world) {
														send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
													}
												}
												delete[] raw;
												gamepacket_t p;
												p.Insert("OnConsoleMessage");
												p.Insert("[`6You spent 100 Diamond Lock to get 1 Blue Gem Lock``]");
												p.CreatePacket(peer);
											}
										}
										else {
											fail = true;
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("You don't have room in your backpack!");
											p.Insert(0), p.Insert(1);
											p.CreatePacket(peer);
											{
												gamepacket_t p;
												p.Insert("OnConsoleMessage");
												p.Insert("You don't have room in your backpack!");
												p.CreatePacket(peer);
											}
											continue;
										}
										int c_ = 0;
										modify_inventory(peer, 1796, c_);
										p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wBlue Gem Lock``|left|7188|\nadd_textbox|Excellent! I'm happy to sell you a Blue Gem Lock in exchange for 100 Diamond Lock..|left|\nadd_smalltext|`6You have " + to_string(c_) + " Diamond Lock.``|left|" + (c_ >= 100 ? "\nadd_button|chc2_2_1|Thank you!|noflags|0|0|" : "") + "\nadd_button|53785|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
									}
									else {
										fail = true;
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("You don't have room in your backpack!");
										p.Insert(0), p.Insert(1);
										p.CreatePacket(peer);
										{
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("You don't have room in your backpack!");
											p.CreatePacket(peer);
										}
										continue;
									}
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("You don't have enough inventory space!");
									p.CreatePacket(peer);
									fail = true;
								}
							}
							else if (btn == "chc3_1") {
								int zombie_brain = 0, pile = 0, total = 0;
								modify_inventory(peer, 4450, zombie_brain);
								modify_inventory(peer, 4452, pile);
								total += zombie_brain + (pile * 100);
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSales-Man: Zombie Defense``|left|4358|\nadd_textbox|Excellent! I'm happy to sell you Zombie Defense supplies in exchange for Zombie Brains.|left|\nadd_smalltext|You currently have " + setGems(total) + " Zombie Brains.|left|\nadd_spacer|small|\ntext_scaling_string|5,000ZB|\n" + zombie_list + "\nadd_button_with_icon||END_LIST|noflags|0||\nadd_button|53785|Back|noflags|0|0|\nend_dialog|zombie_back|Hang Up||\n");
								p.CreatePacket(peer);
							}
							else if (btn == "chc4_1") {
								int zombie_brain = 0, pile = 0, total = 0;
								modify_inventory(peer, 4298, zombie_brain);
								modify_inventory(peer, 4300, pile);
								total += zombie_brain + (pile * 100);
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSales-Man: Surgery``|left|4358|\nadd_textbox|Excellent! I'm happy to sell you rare and precious Surgery prizes in exchange for Caduceus medals.|left|\nadd_smalltext|You currently have " + setGems(total) + " Caducei.|left|\nadd_spacer|small|\ntext_scaling_string|50,000ZB|\n" + surgery_list + "\nadd_button_with_icon||END_LIST|noflags|0||\nadd_button|53785|Back|noflags|0|0|\nend_dialog|zurgery_back|Hang Up||\n");
								p.CreatePacket(peer);
							}
							else if (btn == "chc5_1") {
								int zombie_brain = 0, pile = 0, total = 0;
								modify_inventory(peer, 4354, zombie_brain);
								modify_inventory(peer, 4356, pile);
								total += zombie_brain + (pile * 100);
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSales-Man: Wolfworld``|left|4358|\nadd_textbox|Excellent! I'm happy to sell you rare and precious Woflworld prizes in exchange for Wolf Tickets.|left|\nadd_smalltext|You currently have " + setGems(total) + " Wolf Tickets.|left|\nadd_spacer|small|\ntext_scaling_string|50,000WT|\n" + wolf_list + "\nadd_button_with_icon||END_LIST|noflags|0||\nadd_button|53785|Back|noflags|0|0|\nend_dialog|wolf_back|Hang Up||\n");
								p.CreatePacket(peer);
							}
							else p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wDisconnected``|left|774|\nadd_textbox|The number you have tried to reach is disconnected. Please check yourself before you wreck yourself.|left|\nend_dialog|3898|Hang Up||\n");
							if (btn != "turnin" && fail == false) p.CreatePacket(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|top\nbuttonClicked|wotwlistback\n\n" || cch == "action|dialog_return\ndialog_name|top\n") {
							SendCmd(peer, "/top", true);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|shopgemsconfirm\ngemspurchase|") != string::npos) {
							int gems = atoi(cch.substr(62, cch.length() - 62).c_str());
							if (gems <= 0) break;
							pInfo(peer)->offergems = gems;
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase gems``|left|9436|\nadd_spacer|small|\nadd_textbox|`2Purchase`` `9" + setGems(pInfo(peer)->offergems * 1000) + " Gems`` for `9" + to_string(gems) + " World Locks?``|\nadd_button|shopmoneybuy|`0Purchase``|NOFLAGS|0|0|\nadd_button||`0Cancel``|NOFLAGS|0|0|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopmoneybuy\n\n") {
							if (pInfo(peer)->offergems <= 0) break;
							if (pInfo(peer)->gtwl >= pInfo(peer)->offergems) {
								{
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("You got `0" + setGems(pInfo(peer)->offergems * 1000) + "`` Gems!");
									p.CreatePacket(peer);
								}
								packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
								OnSetGems(peer, (pInfo(peer)->offergems * 1000));
								pInfo(peer)->gtwl -= pInfo(peer)->offergems;
							}
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|playerportal\nbuttonClicked|socialportal\n\n"  || cch.find("action|friends") != string::npos) {
							send_social(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|socialportal\n\n") {
							shop_tab(peer, "tab1_opc_shop");
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|socialportal\nbuttonClicked|tradehistory\n\n") {
							string trade_history = "";
							for (int i = 0; i < pInfo(peer)->trade_history.size(); i++) trade_history += "\nadd_spacer|small|\nadd_smalltext|" + pInfo(peer)->trade_history[i] + "|left|";
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|small|" + pInfo(peer)->tankIDName + "'s Trade History|left|242|" + (pInfo(peer)->trade_history.size() == 0 ? "\nadd_spacer|small|\nadd_smalltext|Nothing to show yet.|left|" : trade_history) + "\nadd_spacer|small|\nadd_button|socialportal|Back|noflags|0|0|\nadd_button||Close|noflags|0|0|\nadd_quick_exit|\nend_dialog|playerportal|||");
							p.CreatePacket(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopgrowtoken\n\n" || cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopxp\n\n" || cch == "action|dialog_return\ndialog_name|top\nbuttonClicked|toplist\n\n" || cch == "action|dialog_return\ndialog_name|top\nbuttonClicked|yesterdaylist\n\n" || cch == "action|dialog_return\ndialog_name|top\nbuttonClicked|overalllist\n\n" || cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopmoney\n\n" || cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shoprank\n\n" || cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopitems\n\n" || cch == "action|dialog_return\ndialog_name|socialportal\nbuttonClicked|onlinepointhub\n\n" || cch == "action|dialog_return\ndialog_name|gazette\nbuttonClicked|onlinepointhub\n\n" || cch == "action|opc_shop\n") {
							gamepacket_t p((cch == "action|opc_shop\n" ? 500 : 0));
							p.Insert("OnDialogRequest");
							if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shoprank\n\n") {
								string ranks_more = "";
								if (domain == "https://gold-ps.com/") ranks_more = "\nadd_button_with_icon|" + a + (pInfo(peer)->dev ? "" : "shopcrank_dev") + "|`8Developer Role`` `0(``" + (pInfo(peer)->dev ? "`2OWNED" : "`9" + to_string(role_price.dev_price) + "WL``") + "`0)``|noflags|278|\nadd_button_with_icon|" + a +  (pInfo(peer)->ghost_role ? "" : "shopcrank_ghost") + "|`b/ghost`` `0(``" + (pInfo(peer)->ghost_role ? "`2OWNED" : "`9" + to_string(role_price.ghost_price) + "WL``") + "`0)``|noflags|278|";
								p.Insert(a + "set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase roles``|left|278|\n\nadd_spacer|small|\n\nadd_textbox|Choose which role do you want to purchase?|left|\nadd_smalltext|Note: We are only selling 7 roles for real growtopia payment.|left|\ntext_scaling_string|1,100,000,000,000OPC|\nadd_spacer|small|"+ ranks_more +"\nadd_button_with_icon|" + (pInfo(peer)->cheater_ ? "" : "shopcrank_cheater") + "|`cCheater`` `0(``" + (pInfo(peer)->cheater_ ? "`2OWNED" : "`9" + to_string(role_price.cheater_price + role_price.cheater_price2) + "WL``") + "`0)``|noflags|9726|\nadd_button_with_icon|" + (has_playmod2(pInfo(peer), 143) ? "" : "shopcrank_cheater7days") + "|`cCheater 7 days`` `0(``" + (has_playmod2(pInfo(peer), 143) ? "`2OWNED" : "`9" + to_string(role_price.cheater7days_price) + "WL``") + "`0)``|noflags|9726|\nadd_button_with_icon|" + (pInfo(peer)->all_in ? "" : "shopcrank_allin") + "|`pAll-In Pass`` `0(``" + (pInfo(peer)->all_in ? "`2OWNED" : "`9" + to_string(role_price.allin_price) + "WL``") + "`0)``|noflags|11816|\nadd_button_with_icon|" + (pInfo(peer)->moderator ? "" : "shopcrank_moderator") + "|`bModerator`` `0(``" + (pInfo(peer)->moderator ? "`2OWNED" : "`9" + to_string(role_price.moderator_price) + "WL``") + "`0)``|noflags|278|\nadd_button_with_icon|shopcrank_mod|`#Guardian`` `0(``" + (pInfo(peer)->mod ? "`2OWNED" : "`9" + to_string(role_price.mod_price) + "WL``") + "`0)``|noflags|276|\nadd_button_with_icon|" + (pInfo(peer)->gp ? "" : "shopcrank_growpass") + "|`9Grow Pass`` `0(``" + (pInfo(peer)->gp ? "`2OWNED``" : "`9" + to_string(role_price.growpass_price) + "WL``") + "`0)``|noflags|11304|\nadd_button_with_icon|shopcrank_vip_permament|`eVIP`` `0(``" + (pInfo(peer)->vip ? "`2OWNED``" : "`9" + to_string(role_price.vip_price_permament) + "WL``") + "`0)``|noflags|9938|\nadd_button_with_icon|shopcrank_vip|`eVIP`` 31 days`0(``" + (pInfo(peer)->vip ? "`2OWNED``" : "`9" + to_string(role_price.vip_price) + "WL``") + "`0)``|noflags|9938|\nadd_button_with_icon|" + (pInfo(peer)->glo ? "" : "shopcrank_glory") + "|`9Road to GLory`` `0(``" + (pInfo(peer)->glo ? "`2OWNED``" : "`9" + to_string(role_price.glory_price) + "WL``") + "`0)``|noflags|9436|"/*"\nadd_button_with_icon|shoprankgrowpass1|`rGrowPass LVL1`` `0(```9200Wls```0)``|noflags|9222|\nadd_button_with_icon|shoprankgrowpass2|`rGrowPass LVL2`` `0(```9400Wls```0)``|noflags|9222|\nadd_button_with_icon|shoprankgrowpass3|`rGrowPass LVL3`` `0(```9500Wls```0)``|noflags|9222|"*/"||\nadd_button_with_icon||END_LIST|noflags|0||\nadd_button|shop|`0Back``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|chc0|Close|noflags|0|0|\nnend_dialog|gazette||OK|");

							}
							if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopitems\n\n") p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase items``|left|9474|\n\nadd_spacer|small|\n\nadd_textbox|Choose which item do you want to purchase?|left|\nadd_smalltext|Note: We are only selling those items for premium wl, if you want any item to be added contact our support.|left|\nadd_spacer|small|\ntext_scaling_string|50,000OPC|" + shop_list + "||\nadd_button_with_icon||END_LIST|noflags|0||\nadd_button|shop|`0Back``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|chc0|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							if (cch == "action|dialog_return\ndialog_name|socialportal\nbuttonClicked|onlinepointhub\n\n" || cch == "action|dialog_return\ndialog_name|gazette\nbuttonClicked|onlinepointhub\n\n" || cch == "action|opc_shop\n") p.Insert("set_default_color|`o\nadd_label_with_icon|big|`0Online Point Hub``|left|10668|\nadd_spacer|small|\nadd_textbox|Welcome to `pOnline Point Currency HUB``! Do you have any OPC? You can buy items from me with them.|left|\nadd_smalltext|`2You can earn 1 OPC every 10 minutes just by playing the game.``|left|\nadd_smalltext|OPC Item weekly list sponsored by Abdu|left|\nadd_spacer|small|\nadd_textbox|You have `p" + setGems(pInfo(peer)->opc) + " Online Point Currency``.|left|\ntext_scaling_string|50,000OPC|" + opc_list + "||\nadd_button_with_icon||END_LIST|noflags|0||\nadd_button|socialportal|Back|noflags|0|0|\nadd_button|chc0|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopmoney\n\n")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase gems``|left|9436|\n\nadd_spacer|small|\nadd_textbox|You have `2" + setGems(pInfo(peer)->gtwl) + "`` Premium World Locks, how much you want to spend wls for gems? (Enter wl amount):|\nadd_textbox|Rate: `21000``/`2WL``|\nadd_smalltext|Note: You can spend your gems for awesome items or cool packs.|left|\nadd_text_input|gemspurchase|WL||30|\nend_dialog|shopgemsconfirm|Cancel|`0Check price``|\n");
							if (cch == "action|dialog_return\ndialog_name|top\nbuttonClicked|toplist\n\n")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`8Top Worlds Of Today``|left|394|\nadd_spacer|" + top_list + "\nadd_button|wotwlistback|`oBack`|NOFLAGS|0|0|\nend_dialog|top|Close||\n");
							if (cch == "action|dialog_return\ndialog_name|top\nbuttonClicked|yesterdaylist\n\n")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`5Top Worlds Of Yesterday``|left|394|\nadd_spacer|" + top_yesterday_list + "\nadd_button|wotwlistback|`oBack`|NOFLAGS|0|0|\nend_dialog|top|Close||\n");
							if (cch == "action|dialog_return\ndialog_name|top\nbuttonClicked|overalllist\n\n")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Top Worlds Of All Time``|left|394|\nadd_spacer|" + top_overall_list + "\nadd_button|wotwlistback|`oBack`|NOFLAGS|0|0|\nend_dialog|top|Close||\n");
							if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopxp\n\n")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase XP``|left|1488|\nadd_spacer|small|\n\nadd_textbox|Choose which potion you want to purchase:|left|\nadd_smalltext|Note: You can purchase XP here, for example '100' potion will give you 100xp.|left|\ntext_scaling_string|5,000OPC|\nadd_spacer|small|\nadd_button_with_icon|shopxp_100|`02 WL``|noflags|1488|100|\nadd_button_with_icon|shopxp_500|`010 WL``|noflags|1488|500|\nadd_button_with_icon|shopxp_1000|`020 WL``|noflags|1488|1000|\nadd_button_with_icon|shopxp_2500|`050 WL``|noflags|1488|2500|\nadd_button_with_icon|shopxp_5000|`0100 WL``|noflags|1488|5000|\nadd_button_with_icon|shopxp_10000|`0200 WL``|noflags|1488|10000|\nadd_button_with_icon|shopxp_100000|`02000 WL``|noflags|1488|100000|\nadd_button_with_icon||END_LIST|noflags|0||\n\nadd_spacer|\nadd_button|shop|`0Back``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|chc0|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopgrowtoken\n\n")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase Growtoken``|left|1486|\nadd_spacer|small|\n\nadd_textbox|Choose how many tokens do you want to purchase:|left|\nadd_smalltext|Note: You can spend tokens for super rare items.|left|\nadd_spacer|small|\ntext_scaling_string|5,000OPC|\nadd_button_with_icon|shopgtoken_1|`0100 WL``|noflags|1486|1|\nadd_button_with_icon|shopgtoken_5|`0500 WL``|noflags|1486|5|\nadd_button_with_icon|shopgtoken_10|`01000 WL``|noflags|1486|10|\nadd_button_with_icon|shopgtoken_25|`02500 WL``|noflags|1486|25|\nadd_button_with_icon|shopgtoken_50|`05000 WL``|noflags|1486|50|\nadd_button_with_icon|shopgtoken_100|`010,000 WL``|noflags|1486|100|\nadd_button_with_icon||END_LIST|noflags|0||\n\nadd_spacer|\nadd_button|shop|`0Back``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|chc0|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							//if (cch == "action|claimprogressbar\n")p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wAbout Valentine's Event``|left|384|\nadd_spacer|small|\nadd_textbox|During Valentine's Week you will gain points for opening Golden Booty Chests. Claim enough points to earn bonus rewards.|left|\nadd_spacer|small|\nadd_textbox|Current Progress: " + to_string(pInfo(peer)->booty_broken) + "/100|left|\nadd_spacer|small|\nadd_textbox|Reward:|left|\nadd_label_with_icon|small|Super Golden Booty Chest|left|9350|\nadd_smalltext|             - 4x chance of getting a Golden Heart Crystal when opening!|left|" + (pInfo(peer)->booty_broken >= 100 ? "\nadd_spacer|small|\nadd_button|claimreward|Claim Reward|no_flags|0|0|" : "") + "\nend_dialog|valentines_quest||OK|\n");
							p.CreatePacket(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|top\nbuttonClicked|toprated\n\n") {
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wTop Rated Worlds``|left|3802|\nadd_textbox|Select a category to view.|left|\nadd_button|cat1|Adventure|noflags|0|0|\nadd_button|cat2|Art|noflags|0|0|\nadd_button|cat3|Farm|noflags|0|0|\nadd_button|cat4|Game|noflags|0|0|\nadd_button|cat5|Information|noflags|0|0|\nadd_button|cat6|Parkour|noflags|0|0|\nadd_button|cat7|Roleplay|noflags|0|0|\nadd_button|cat8|Shop|noflags|0|0|\nadd_button|cat9|Social|noflags|0|0|\nadd_button|cat10|Storage|noflags|0|0|\nadd_button|cat11|Story|noflags|0|0|\nadd_button|cat12|Trade|noflags|0|0|\nadd_button|cat13|Guild|noflags|0|0|\nadd_button|cat14|Puzzle|noflags|0|0|\nadd_button|cat15|Music|noflags|0|0|\nend_dialog|toprated|Nevermind||");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|toprated\nbuttonClicked|cat") != string::npos) {
							int world_rateds = atoi(cch.substr(59, cch.length() - 59).c_str());
							if (world_rateds > world_rate_types.size()) break;
							string worlds = "";
							for (int i = 0; i < world_rate_types[world_rateds].size(); i++) {
								string world = world_rate_types[world_rateds][i].substr(0, world_rate_types[world_rateds][i].find("|")), rate = world_rate_types[world_rateds][i].substr(world_rate_types[world_rateds][i].find("|") + 1);
								worlds += "\nadd_button|warp_to_" + world + "|#" + to_string(i + 1) + "``. `8"+ world +"`` ("+rate+")|noflags|0|0|";
							}
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wTop 100 "+(world_category(world_rateds)) + " Worlds``|left|3802|" + worlds + "\nend_dialog|top|Nevermind|Back|\n");
							p.CreatePacket(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|top\nbuttonClicked|wotw\n\n") {
						string wotd_text = "";
						if (World_Stuff.wotd.size() == 0) wotd_text = "\nadd_smalltext|The list should update in few minutes.|\nadd_spacer|small|";
						else {
							vector<string> wotd_reverse = World_Stuff.wotd, a_;
							sort(wotd_reverse.begin(), wotd_reverse.end());
							reverse(top_basher.begin(), top_basher.end());
							for (int i = 0; i < wotd_reverse.size(); i++) {
								a_ = explode("|", wotd_reverse[i]);
								wotd_text += "\nadd_button|warp_to_" + a + a_[0].c_str() + "|`w#" + to_string(wotd_reverse.size() - i) + "`` " + a_[0].c_str() + " by `#" + a_[1].c_str() + "``|noflags|0|0|";
							}
						}
						gamepacket_t p;
						p.Insert("OnDialogRequest");
						p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`$World Of The Week Winners``|left|394|\nadd_spacer|" + wotd_text + "\nadd_button|wotwlistback|`oBack`|NOFLAGS|0|0|\nend_dialog|top|Close||\n");
						p.CreatePacket(peer);
						break;
						}
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopsubscribtion\n\n") {
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase subscribtion``|left|8188|\n\nadd_spacer|small|\n\nadd_textbox|Choose which subscribtion do you want to purchase?|left|\nadd_smalltext|Note: We are only selling 5 subscribtions for premium world locks.|left|\ntext_scaling_string|100,000,000,000OPC|\nadd_spacer|small|\nadd_button_with_icon|shop_price_9266|`p1-Day Subscribtion``|noflags|9266|\nadd_button_with_icon|shop_price_6856|`p3-Day Subscribtion``|noflags|6856|\nadd_button_with_icon|shop_price_6858|`p14-Day Subscribtion``|noflags|6858|\nadd_button_with_icon|shop_price_6860|`p30-Day Subscribtion``|noflags|6860|\nadd_button_with_icon|shop_price_8188|`p1-Year Subscribtion``|noflags|8188|\nadd_button_with_icon||END_LIST|noflags|0||\nadd_button|shop|`0Back``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|chc0|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|shopcrank_") != string::npos) {
							string role = cch.substr(58, cch.length() - 60).c_str();
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							if (role == "mod") p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase Guardian``|left|18|"/*"\nadd_image_button||interface/large/gtps_mod_banner.rttex|bannerlayout|flag_frames:1,1,0,0|flag_surfsize:985,256|"*/"\nadd_spacer|small|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `3" + to_string(role_price.mod_price) + " `9World Locks``|left|\nadd_smalltext|Duration: `7[```431 days```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_smalltext|Extra: `4Get extra items only for Mods from giveaways!``|left|\nadd_textbox|`6Rules:``|left|\nadd_smalltext|`e1.`` `rDo not abuse your role.``|left|\nadd_smalltext|`e2.`` `rIf you are going to ban people, make sure to have screenshots/video for proof.``|left|\nadd_smalltext|`e3.`` `rSharing account will result in account loss.``|left|\nadd_smalltext|`e4.`` `rTrying to sell your account will result in ip-ban!``|left|\nadd_spacer|small|\n\nadd_textbox|`6Commands:``|left|\nadd_smalltext|`eAll commands are displayed in /help ``|left|\nadd_spacer|" + a + (credit_payment ? "\nadd_url_button|comment|Purchase with Credit Card/PayPal|noflags|"+domain+"purchase/|Open premium wls shop?|0|0|" : "") + "\nadd_button|shopbuyrank_mod|`0Purchase|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							else if (role == "moderator") p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase Moderator``|left|18|"/*"\nadd_image_button||interface/large/gtps_mod_banner.rttex|bannerlayout|flag_frames:1,1,0,0|flag_surfsize:985,256|"*/"\nadd_spacer|small|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `3" + to_string(role_price.moderator_price) + " `9World Locks``|left|\nadd_smalltext|Duration: `7[```4PERMAMENT```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_smalltext|Extra: `4Get extra items only for Mods from giveaways!``|left|\nadd_textbox|`6Rules:``|left|\nadd_smalltext|`e1.`` `rDo not abuse your role.``|left|\nadd_smalltext|`e2.`` `rIf you are going to ban people, make sure to have screenshots/video for proof.``|left|\nadd_smalltext|`e3.`` `rSharing account will result in account loss.``|left|\nadd_smalltext|`e4.`` `rTrying to sell your account will result in ip-ban!``|left|\nadd_spacer|small|\n\nadd_textbox|`6Commands:``|left|\nadd_smalltext|`eAll commands are displayed in /help ``|left|\nadd_spacer|" + a + (credit_payment ? "\nadd_url_button|comment|Purchase with Credit Card/PayPal|noflags|"+domain+"purchase/|Open premium wls shop?|0|0|" : "") + "\nadd_button|shopbuyrank_moderator|`0Purchase|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							else if (role == "vip") p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase VIP 31 days``|left|18|\nadd_spacer|small|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `3" + to_string(role_price.vip_price) + " `9World Locks``|left|\nadd_smalltext|Duration: `7[```431 days```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_smalltext|Extra: `4Get extra items only for VIPS from giveaways!``|left|\nadd_textbox|`6Rules:``|left|\nadd_smalltext|`e1.`` `rInpersonating someone with name changing will result in ban!``|left|\nadd_smalltext|`e2.`` `rSharing account will result in account loss.``|left|\nadd_smalltext|`e4.`` `rTrying to sell your account will result in ip-ban!``|left|\nadd_spacer|small|\n\nadd_textbox|`6Commands:``|left|\nadd_smalltext|`eAll commands are displayed in /vhelp (vip help)``|left|\nadd_spacer|" + a + (credit_payment ? "\nadd_url_button|comment|Purchase with Credit Card/PayPal|noflags|" + web_url + "purchase/|Open premium wls shop?|0|0|" : "") + "\nadd_button|shopbuyrank_vip|`0Purchase|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							else if (role == "vip_permament") p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase VIP``|left|18|\nadd_spacer|small|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `3" + to_string(role_price.vip_price_permament) + " `9World Locks``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_smalltext|Extra: `4Get extra items only for VIPS from giveaways!``|left|\nadd_textbox|`6Rules:``|left|\nadd_smalltext|`e1.`` `rInpersonating someone with name changing will result in ban!``|left|\nadd_smalltext|`e2.`` `rSharing account will result in account loss.``|left|\nadd_smalltext|`e4.`` `rTrying to sell your account will result in ip-ban!``|left|\nadd_spacer|small|\n\nadd_textbox|`6Commands:``|left|\nadd_smalltext|`eAll commands are displayed in /vhelp (vip help)``|left|\nadd_spacer|" + a + (credit_payment ? "\nadd_url_button|comment|Purchase with Credit Card/PayPal|noflags|" + web_url + "purchase/|Open premium wls shop?|0|0|" : "") + "\nadd_button|shopbuyrank_vip_permament|`0Purchase|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							else if (role == "glory")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase Road to Glory``|left|18|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `3" + to_string(role_price.glory_price) + " `9World Locks``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_smalltext|Extra: `7RECEIVE INSTANTLY 100,000 GEMS - LEVEL UP & EARN UP TO 1,600,000 GEMS (save up 600+wls)``|left|\nadd_textbox|`6Rules:``|left|\nadd_smalltext|`e1.`` `rDo not abuse your role.``|left|\nadd_smalltext|`e2.`` `rIf you are going to ban people, make sure to have screenshots/video for proof.``|left|\nadd_smalltext|`e3.`` `rSharing account will result in account loss.``|left|\nadd_smalltext|`e4.`` `rTrying to sell your account will result in ip-ban!``|left|\nadd_spacer|small|\n\nadd_textbox|`6Commands:``|left|\nadd_smalltext|`eAll commands are displayed in /help ``|left|\nadd_spacer|" + a + (credit_payment ? "\nadd_url_button|comment|Purchase with Credit Card/PayPal|noflags|"+domain+"purchase/|Open premium wls shop?|0|0|" : "") + "\nadd_button|shopbuyrank_glory|`0Purchase|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							else if (role == "growpass")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase Grow Pass``|left|18|\nadd_spacer|small|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `3" + to_string(role_price.growpass_price) + " `9World Locks``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_smalltext|Extra: `7Receive daily items everyday, get 2x opc points, receive newest coolest growotpia items, use /buy <item> command``, `4Get extra items only for Grow-Pass from giveaways!``|left|\n\nadd_textbox|`6Rules:``|left|\nadd_smalltext|`e1.`` `rDo not abuse your role.``|left|\nadd_smalltext|`e2.`` `rIf you are going to ban people, make sure to have screenshots/video for proof.``|left|\nadd_smalltext|`e3.`` `rSharing account will result in account loss.``|left|\nadd_smalltext|`e4.`` `rTrying to sell your account will result in ip-ban!``|left|\nadd_spacer|small|\n\nadd_textbox|`6Commands:``|left|\nadd_smalltext|`eAll commands are displayed in /help ``|left|\nadd_spacer|" + a + (credit_payment ? "\nadd_url_button|comment|Purchase with Credit Card/PayPal|noflags|"+domain+"purchase/|Open premium wls shop?|0|0|" : "") + "\nadd_button|shopbuyrank_growpass|`0Purchase|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							else if (role == "allin")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase All-In Pass``|left|18|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `3" + to_string(role_price.allin_price) + " `9World Locks``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_smalltext|Extra: `7Permament deposit 2x bonus and some small rewards!``|left|\n\nadd_textbox|`6Rules:``|left|\nadd_smalltext|`e1.`` `rDo not abuse your role.``|left|\nadd_smalltext|`e2.`` `rIf you are going to ban people, make sure to have screenshots/video for proof.``|left|\nadd_smalltext|`e3.`` `rSharing account will result in account loss.``|left|\nadd_smalltext|`e4.`` `rTrying to sell your account will result in ip-ban!``|left|\nadd_spacer|small|\n\nadd_textbox|`6Commands:``|left|\nadd_smalltext|`eAll commands are displayed in /help ``|left|\nadd_textbox|Purchasing this will give you an `5awesome prizes``:|left|\nadd_label_with_icon|small|100 Cashback Coupon|left|10394|\nadd_label_with_icon|small|5 Experience Potion|left|1488|\nadd_label_with_icon|small|5 Curse Wand|left|278|\nadd_label_with_icon|small|5 Fire Wand|left|276|\nadd_label_with_icon|small|5 Freeze Wand|left|274|\nadd_spacer|" + a + (credit_payment ? "\nadd_url_button|comment|Purchase with Credit Card/PayPal|noflags|"+domain+"purchase/|Open premium wls shop?|0|0|" : "") + "\nadd_button|shopbuyrank_allin|`0Purchase|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							else if (role == "cheater")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase Cheater``|left|18|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `3" + to_string(role_price.cheater_price + role_price.cheater_price2) + "`` `9World Locks``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_smalltext|Extra: `4THIS ROLE PRICE RISES 10DL EACH PURCHASE!`` `7Use Cheats such as: Autofarm/Antibounce/Super Speed/Double Jump/Fast drop/Chat Prefix, you will have access to Cheats tab/dialog``|left|\n\nadd_textbox|`6Rules:``|left|\nadd_smalltext|`e1.`` `rDo not abuse your role.``|left|\nadd_smalltext|`e2.`` `rIf you are going to ban people, make sure to have screenshots/video for proof.``|left|\nadd_smalltext|`e3.`` `rSharing account will result in account loss.``|left|\nadd_smalltext|`e4.`` `rTrying to sell your account will result in ip-ban!``|left|\nadd_spacer|small|\n\nadd_textbox|`6Commands:``|left|\nadd_smalltext|`eAll commands are displayed in /help ``|left|\nadd_spacer|" + a + (credit_payment ? "\nadd_url_button|comment|Purchase with Credit Card/PayPal|noflags|" + web_url + "purchase/|Open premium wls shop?|0|0|" : "") + "\nadd_button|shopbuyrank_cheater|`0Purchase|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							else if (role == "cheater7days")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase Cheater for 7 days``|left|18|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `3" + to_string(role_price.cheater7days_price) + "`` `9World Locks``|left|\nadd_smalltext|Duration: `7[```47 days```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_smalltext|Extra: `7Use Cheats such as: Autofarm/Antibounce/Super Speed/Double Jump/Fast drop/Chat Prefix, you will have access to Cheats tab/dialog``|left|\n\nadd_textbox|`6Rules:``|left|\nadd_smalltext|`e1.`` `rDo not abuse your role.``|left|\nadd_smalltext|`e2.`` `rIf you are going to ban people, make sure to have screenshots/video for proof.``|left|\nadd_smalltext|`e3.`` `rSharing account will result in account loss.``|left|\nadd_smalltext|`e4.`` `rTrying to sell your account will result in ip-ban!``|left|\nadd_spacer|small|\n\nadd_textbox|`6Commands:``|left|\nadd_smalltext|`eAll commands are displayed in /help ``|left|\nadd_spacer|" + a + (credit_payment ? "\nadd_url_button|comment|Purchase with Credit Card/PayPal|noflags|" + web_url + "purchase/|Open premium wls shop?|0|0|" : "") + "\nadd_button|shopbuyrank_cheater7days|`0Purchase|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							else if (role == "dev" && domain == "https://gold-ps.com/")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase Developer``|left|18|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `3" + to_string(role_price.dev_price) + "`` `9World Locks``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_textbox|`6Rules:``|left|\nadd_smalltext|`e1.`` `rDo not abuse your role.``|left|\nadd_smalltext|`e2.`` `rIf you are going to ban people, make sure to have screenshots/video for proof.``|left|\nadd_smalltext|`e3.`` `rSharing account will result in account loss.``|left|\nadd_smalltext|`e4.`` `rTrying to sell your account will result in ip-ban!``|left|\nadd_spacer|small|\n\nadd_textbox|`6Commands:``|left|\nadd_smalltext|`eAll commands are displayed in /help ``|left|\nadd_spacer|" + a + (credit_payment ? "\nadd_url_button|comment|Purchase with Credit Card/PayPal|noflags|" + web_url + "purchase/|Open premium wls shop?|0|0|" : "") + "\nadd_button|shopbuyrank_dev|`0Purchase|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							else if (role == "ghost" && domain == "https://gold-ps.com/")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase /Ghost``|left|18|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `3" + to_string(role_price.ghost_price) + "`` `9World Locks``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_textbox|`6Rules:``|left|\nadd_smalltext|`e1.`` `rDo not abuse your role.``|left|\nadd_smalltext|`e2.`` `rIf you are going to ban people, make sure to have screenshots/video for proof.``|left|\nadd_smalltext|`e3.`` `rSharing account will result in account loss.``|left|\nadd_smalltext|`e4.`` `rTrying to sell your account will result in ip-ban!``|left|\nadd_spacer|small|\n\nadd_textbox|`6Commands:``|left|\nadd_smalltext|`eAll commands are displayed in /help ``|left|\nadd_spacer|" + a + (credit_payment ? "\nadd_url_button|comment|Purchase with Credit Card/PayPal|noflags|" + web_url + "purchase/|Open premium wls shop?|0|0|" : "") + "\nadd_button|shopbuyrank_ghost|`0Purchase|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|shopbuyrank_") != string::npos) {
							if (pInfo(peer)->world .empty()) break;
							string role = cch.substr(60, cch.length() - 62).c_str();
							int price = 0;
							if (role == "growpass") price = role_price.growpass_price;
							else if (role == "vip") price = role_price.vip_price;
							else if (role == "vip_permament") price = role_price.vip_price_permament;
							else if (role == "glory") price = role_price.glory_price;
							else if (role == "allin")  price = role_price.allin_price;
							else if (role == "mod") price = role_price.mod_price;
							else if (role == "moderator") price = role_price.moderator_price;
							else if (role == "cheater") price = role_price.cheater_price + role_price.cheater_price2;
							else if (role == "cheater7days") price = role_price.cheater7days_price;
							else if (role == "dev" && domain == "https://gold-ps.com/") price = role_price.dev_price;
							else if (role == "ghost" && domain == "https://gold-ps.com/") price = role_price.ghost_price;
							gamepacket_t p;
							p.Insert("OnConsoleMessage");
							if (price != 0 && pInfo(peer)->gtwl >= price) {
								pInfo(peer)->gtwl -= price;
								packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
								if (role == "mod") {
									int give_count = 1;
									if (modify_inventory(peer, 9852, give_count) == 0) 	p.Insert("`o>> You purchased guardian! Consume the Guardian Role and type /mhelp");
									else {
										pInfo(peer)->gtwl += price;
										p.Insert("No inventory space.");
									}
								}
								else if (role == "cheater7days") {
									int give_count = 1;
									if (modify_inventory(peer, 9726, give_count) == 0) 	p.Insert("`o>> You purchased Cheater Role! Consume the Cheater Role!");
									else {
										pInfo(peer)->gtwl += price;
										p.Insert("No inventory space.");
									}
								}
								else if (role == "moderator") {
									pInfo(peer)->moderator = 1;
									pInfo(peer)->mod = 1;
									p.Insert("`o>> You purchased moderator! Type /mhelp");
								}
								else if (role == "dev" && domain == "https://gold-ps.com/") {
									pInfo(peer)->dev = 1;
									pInfo(peer)->mod = 1;
									p.Insert("`o>> You purchased developer!");
								}
								else if (role == "ghost" && domain == "https://gold-ps.com/") {
									pInfo(peer)->ghost_role = 1;
									p.Insert("`o>> You purchased /ghost");
								}
								else if (role == "vip") {
									int give_count = 1;
									if (modify_inventory(peer, 9854, give_count) == 0) p.Insert("`o>> You purchased vip! Consume the VIP Role and type /vhelp");
									else {
										pInfo(peer)->gtwl += price;
										p.Insert("No inventory space.");
									}
								}
								else if (role == "vip_permament") {
									pInfo(peer)->vip_p = 1;
									pInfo(peer)->vip = 1;
									p.Insert("`o>> You purchased VIP. /vhelp");
								}
								else if (role == "glory") {
									pInfo(peer)->glo = 1;
									OnSetGems(peer, 100000);
									p.Insert("`o>> You purchased Road to Glory! Wrench yourself and press on Road to Glory button!``");
								}
								else if (role == "growpass") {
									pInfo(peer)->gp = 1;
									p.Insert("`o>> You purchased Grow Pass! Wrench yourself to check the prizes``");
								}
								else if (role == "cheater") {
									pInfo(peer)->cheater_ = 1;
									role_price.cheater_price2 += 300;
									p.Insert("`o>> You purchased Cheater! Wrench yourself to check the 'Cheats' tab!``");
								}
								else if (role == "allin") {
									pInfo(peer)->all_in = 1;
									p.Insert("`o>> You purchased All-In Pass! Thank you.``");
									vector<int> list{ 10394 , 274 , 276 , 278 , 1488 };
									string name_ = pInfo(peer)->world;
									vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
									if (p != worlds.end()) {
										World* world_ = &worlds[p - worlds.begin()];
										world_->fresh_world = true;
										for (int i = 0; i < list.size(); i++) {
											int c_ = (list[i] == 10394 ? 100 : 5);
											if (modify_inventory(peer, list[i], c_) == 0) {

											}
											else {
												WorldDrop drop_block_{};
												drop_block_.id = list[i], drop_block_.count = (list[i] == 10394 ? 100 : 5), drop_block_.x = pInfo(peer)->x + rand() % 17, drop_block_.y = pInfo(peer)->y + rand() % 17;
												dropas_(world_, drop_block_);
											}
										}
									}
								}
								gamepacket_t pp2;
								pp2.Insert("OnParticleEffectV2"), pp2.Insert(199), pp2.Insert((float)pInfo(peer)->x + 16, (float)pInfo(peer)->y + 16);
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world) continue;
									pp2.CreatePacket(currentPeer);
								}
							}
							else {
								p.Insert("`o>> You don't have enough premium world locks! Type /deposit.``");
							}
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|shop_price_") != string::npos) {
							int item = atoi(cch.substr(59, cch.length() - 59).c_str());
							if (item <= 0 || item >= items.size() || items[item].pwl == 0) continue;
							string special = "`eThis item does not have any special effects.``";
							if (item == 6312 || item == 11118 || item == 9550) special = "`eGet `2Double Gems``";
							if (item == 9496) special = "`eGet `2Triple Gems``";
							if (item == 11118) special += ", `220% higher chance of getting Magic Ores``";
							if (item == 9548) special += ", `240% higher chance of getting Magic Ores``";
							if (item == 9774) special += ", 100% higher chance of getting Magic Ores``";
							if (item == 9546) special = "`eGet `2x4 Gems``";
							if (item == 9542) special = "`eGet `2x20 Gems``";
							if (item == 9548) special += "`eGet `250 Gems``";
							if (item == 9604) special += "`eGet `25000 Gems``";
							if (item == 9774) special += "`eGet `2x1000 Gems``";
							if (item == 9770) special = "`e1x hit all blocks & 3 hit far``";
							if (item == 9774) special += "`e1x hit all blocks & 3 hit far``";
							if (item == 9772) special = "`e1x hit all blocks & 3 hit far + double opc``";
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase " + items[item].name + "``|left|" + to_string(items[item].id) + "|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `3" + setGems(items[item].pwl) + "`` `9World Locks``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_textbox|`6Description:``|left|\nadd_smalltext|" + items[item].description + "|left|\nadd_textbox|`6Special Effects:``|left|\nadd_smalltext|" + special + "|left|\nadd_text_input|howmuch||1|3|\nadd_spacer|\nadd_button|" + to_string(item) + "|`0Purchase for `9" + setGems(items[item].pwl) + " Wls``|noflags|0|0||small|\nend_dialog|shop_item||Cancel|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|lock_price_") != string::npos) {
							int item = atoi(cch.substr(59, cch.length() - 59).c_str());
							if (item <= 0 || item >= items.size() || items[item].gtwl == 0) continue;
							pInfo(peer)->lockeitem = item;
							string care = "";
							get_wl_value(peer, care);

							int dls = items[item].gtwl / 100;
							int wls = items[item].gtwl - (dls * 100);
							string price = (dls != 0 ? to_string(dls) + " Diamond Lock" + (wls != 0 ? " " : "") : "") + (wls != 0 ? to_string(wls) + " World Lock" : "");
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`9Buy " + items[item].name + "?``|left|" + to_string(items[item].id) + "|\nadd_smalltext|`4" + items[item].description + "``|left|\nadd_smalltext|`1Price: " + (items[item].gtwl > 200 ? to_string(items[item].gtwl / 100) : to_string(items[item].gtwl)) + " " + (items[item].gtwl > 200 ? "Diamond Lock" : "World Locks") + "``|left|\nadd_spacer|\nadd_textbox|How many " + items[item].name + " do you want to buy, for "+ price +" each?|left|\nadd_text_input|howmuch||1|5|"+ care +"\nadd_button|lock_item_|`9Purchase``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|locke|No thanks|noflags|0|0|\nnend_dialog|gazette||OK|");
							p.CreatePacket(peer);
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|locm_price_") != string::npos) {
							int item = atoi(cch.substr(59, cch.length() - 59).c_str());
							if (item <= 0 || item >= items.size() || items[item].u_gtwl == 0) continue;
							pInfo(peer)->lockeitem = item;
							string care = "";
							get_wl_value(peer, care);

							int dls = items[item].u_gtwl / 100;
							int wls = items[item].u_gtwl - (dls * 100);
							string price = (dls != 0 ? to_string(dls) + " Diamond Lock" + (wls != 0 ? " " : "") : "") + (wls != 0 ? to_string(wls) + " World Lock" : "");
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`9Buy " + items[item].name + "?``|left|" + to_string(items[item].id) + "|\nadd_smalltext|`4" + items[item].description + "``|left|\nadd_smalltext|`1Price: " + setGems(items[item].u_gtwl) + " World Lock``|left|\nadd_spacer|\nadd_textbox|How many " + items[item].name + " do you want to buy, for "+price+" each?|left|\nadd_text_input|howmuch||1|5|"+ care +"\nadd_button|lock_item_|`9Purchase``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|lockm|No thanks|noflags|0|0|\nnend_dialog|gazette||OK|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|locke") != string::npos) {
							string care = "";
							get_wl_value(peer, care);
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert(a + "set_default_color|`o\n\nadd_label_with_icon|big|`9GTPS Salesman``|left|9828|\nadd_spacer|small|\nadd_smalltext|Ho there, friend! Locke's my name, and locks are my game. I Love 'em all, Diamond, Huge.. even Small! If you can part with some locks, I'll give you something special in return. Whaddya say?|left|\nadd_spacer|small|" + care + "\nadd_spacer|small|" + shop_list2 + "|\nadd_button|chc0|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|lockm") != string::npos) {
						string care = "";
						get_wl_value(peer, care);
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert(a + "set_default_color|`o\n\nadd_label_with_icon|big|`9Sales-Man Untradeable``|left|9528|\nadd_spacer|small|\nadd_smalltext|Damn.. I hate LOCKE!! This guy takes all my diamond locks, i'm hungry so feed me more. The Untradeable Sales-Man|left|\nadd_spacer|small|"+ care +"\nadd_spacer|small|" + shop_list2_2 + "|\nadd_button|chc0|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|lock_item_\n\nhowmuch|") != string::npos) {
							int count = atoi(cch.substr(68, cch.length() - 68).c_str()), count2 = atoi(cch.substr(68, cch.length() - 68).c_str());
							if (count <= 0 || count > 200) continue;
							int item = pInfo(peer)->lockeitem;
							if (item <= 0 || item >= items.size()) continue;
							if (items[item].gtwl == 0 and items[item].u_gtwl == 0) continue;
							int price = (items[item].gtwl == 0 ? items[item].u_gtwl : items[item].gtwl), priced = 0;
							price *= count;
							priced = price;
							int all_wls = get_wls(peer, true);
							gamepacket_t p;
							p.Insert("OnConsoleMessage");
							if (all_wls >= price) {
								int c_ = count;
								if (modify_inventory(peer, item, c_) == 0) {
									if (item == 1486) {
										if (pInfo(peer)->C_QuestActive && pInfo(peer)->C_QuestKind == 14 && pInfo(peer)->C_QuestProgress < pInfo(peer)->C_ProgressNeeded) {
											pInfo(peer)->C_QuestProgress += count;
											if (pInfo(peer)->C_QuestProgress >= pInfo(peer)->C_ProgressNeeded) {
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("`9Ring Quest task complete! Go tell the Ringmaster!");
												p.Insert(0), p.Insert(0);
												p.CreatePacket(peer);
											}
										}
									}
									get_wls(peer, true, true, price);
									p.Insert("`9[" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->tankIDName) + " bought " + to_string(count2) + " " + items[item].name + " for " + to_string(priced) + " World Lock]");
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(peer)->world == pInfo(currentPeer)->world) {
											p.CreatePacket(currentPeer);
											packet_(currentPeer, "action|play_sfx\nfile|audio/cash_register.wav\ndelayMS|0");
										}
									}
								}
								else {
									p.Insert("No inventory space.");
									p.CreatePacket(peer);
								}
							}
							else {
								p.Insert("`9You don't have enough World Locks!``");
								p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|shop_item") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() == 4) {
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase items``|left|9474|\n\nadd_spacer|small|\n\nadd_textbox|Choose which item do you want to purchase?|left|\nadd_smalltext|Note: We are only selling those items for premium wl, if you want any item to be added contact our support.|left|\nadd_spacer|small|\ntext_scaling_string|50,000OPC|" + shop_list + "||\nadd_button_with_icon||END_LIST|noflags|0||\nadd_button|shop|`0Back``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|chc0|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
								p.CreatePacket(peer);
							}
							else if (t_.size() == 5) {
								int item = atoi(explode("\n", t_[3])[0].c_str()), how_much = atoi(explode("\n", t_[4])[0].c_str());
								if (item <= 0 || item >= items.size() || items[item].pwl == 0 || how_much <= 0 || how_much > 200) continue;
								if ( pInfo(peer)->gtwl >= items[item].pwl * how_much) {
									int c_ = 1;
									if (how_much > 1) c_ = how_much;
									if (modify_inventory(peer, item, c_) == 0) {
										pInfo(peer)->gtwl -= items[item].pwl * how_much;
										packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("`o>> You purchased " + to_string(how_much) + " `2" + items[item].name + "``!");
										p.CreatePacket(peer);
										{
											gamepacket_t p;
											p.Insert("OnAddNotification"), p.Insert("interface/cash_icon_overlay.rttex"), p.Insert("You received " + to_string(how_much) + " `5" + items[item].name + "``!"), p.Insert("audio/piano_nice.wav.wav"), p.Insert(0), p.CreatePacket(peer);
										}
										{
											gamepacket_t pp;
											pp.Insert("OnParticleEffectV2"), pp.Insert(218), pp.Insert((float)pInfo(peer)->x + 16, (float)pInfo(peer)->y + 16);
											pp.CreatePacket(peer);
										}
									}
									else {
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("No inventory space.");
										p.CreatePacket(peer);
									}
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("You don't have enough world locks.");
									p.CreatePacket(peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|opop_price_") != string::npos) {
							int item = atoi(cch.substr(59, cch.length() - 59).c_str());
							if (item <= 0 || item >= items.size() || items[item].oprc == 0) continue;
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase " + items[item].name + "``|left|" + to_string(items[item].id) + "|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_spacer|small|\nadd_smalltext|Name: `3" + items[item].ori_name + "``|left|\nadd_smalltext|Price: `3" + setGems(items[item].oprc) + "`` `0OPC``|left|\nadd_smalltext|Rarity: `3" + setGems(items[item].rarity) + "``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_spacer|small|\nadd_smalltext|Preview:|left|\nadd_button_with_icon||||" + to_string(item) + "||\nadd_button_with_icon||END_LIST|noflags|0||\nadd_textbox|`6Other information:``|left|\nadd_smalltext|" + items[item].description + "|left|\nadd_spacer|\nadd_text_input|itemcount|Count:|1|3|\nadd_spacer|\nadd_button|opop_item_" + to_string(item) + "|`0Purchase `9" + setGems(items[item].oprc) + " OPC``|noflags|0|0|\nadd_spacer|small|\nadd_button|onlinepointhub|`wBack``|noflags|0|0|\nend_dialog|gazette||OK|\nadd_quick_exit|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|gazette\nbuttonClicked|opop_item_") != string::npos) {
						vector<string> t_ = explode("|", cch);
						if (t_.size() != 5) break;
						int count = atoi(explode("\n", t_[4])[0].c_str()), item = atoi(cch.substr(65, cch.length() - 65).c_str());
							if (count <= 0 || count > 200 || item <= 0 || item >= items.size() || items[item].oprc == 0) continue;
							gamepacket_t p;
							p.Insert("OnConsoleMessage");
							if (pInfo(peer)->opc >= (items[item].oprc * count)) {
								int c_ = 1;
								c_ = count;
								if (modify_inventory(peer, item, c_) == 0) {
									if (item == 1486 && pInfo(peer)->C_QuestActive && pInfo(peer)->C_QuestKind == 11 && pInfo(peer)->C_QuestProgress < pInfo(peer)->C_ProgressNeeded) {
										pInfo(peer)->C_QuestProgress+= count;
										if (pInfo(peer)->C_QuestProgress >= pInfo(peer)->C_ProgressNeeded) {
											pInfo(peer)->C_QuestProgress = pInfo(peer)->C_ProgressNeeded;
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("`9Ring Quest task complete! Go tell the Ringmaster!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
									}
									pInfo(peer)->opc -= items[item].oprc * count;
									packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
									p.Insert("`o>> You purchased "+to_string(count) + " `2" + items[item].name + "``!");
									PlayerMoving data_{};
									data_.packetType = 19, data_.punchX = item, data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16;
									int32_t to_netid = pInfo(peer)->netID;
									BYTE* raw = packPlayerMoving(&data_);
									raw[3] = 5;
									memcpy(raw + 8, &to_netid, 4);
									send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									data_.packetType = 17, data_.netID = 48, data_.YSpeed = 48, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
									raw = packPlayerMoving(&data_);
									send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									delete[]raw;
								}
								else p.Insert("No inventory space.");
							}
							else p.Insert("You don't have enough OPC!");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|shopxp_") != string::npos) {
							int xp = atoi(cch.substr(55, cch.length() - 55).c_str());
							if (xp > 0 && pInfo(peer)->gtwl >= (xp / 100 * 2)) {
								pInfo(peer)->gtwl -= (xp / 100 * 2);
								packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
								add_peer_xp(peer, xp);
								gamepacket_t p;
								p.Insert("OnConsoleMessage"), p.Insert("`o>> You purchased " + setGems(xp) + " XP!"), p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|shopgtoken_") != string::npos) {
							int gtokens = atoi(cch.substr(59, cch.length() - 59).c_str());
							if (gtokens > 0 && pInfo(peer)->gtwl >= gtokens * 100) {
								int give = gtokens;
								gamepacket_t p;
								p.Insert("OnConsoleMessage");
								if (modify_inventory(peer, 1486, give) == -1) p.Insert("No inventory space.");
								else {
									pInfo(peer)->gtwl -= gtokens * 100;
									if (pInfo(peer)->lwiz_step == 6) add_lwiz_points(peer, gtokens);
									packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
									p.Insert("`o>> You purchased " + to_string(gtokens) + " Growtoken!");
								}
								p.CreatePacket(peer);
							}
							break;
						}
						/*
						else if (cch.find("action|showblarneyprogress") != string::npos) {
						gamepacket_t p(550);
						p.Insert("OnDialogRequest");
						p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wBlarney Bonanza!``|left|528|\nadd_spacer|small|\nadd_textbox|Welcome to the Blarney Bonanza|left|\nadd_spacer|small|\nadd_textbox|As you, as a community, complete Blarneys and kiss the most magical stone, items will unlock for you to pick up in the store.|left|\nadd_spacer|small|\nadd_textbox|There are 4 items to unlock throughout the event.|left|\nadd_spacer|small|\nadd_textbox|Items will only remain unlocked for a short amount of time, so make sure you check back often! These items can be unlocked multiple times throughout the week.|left|\nadd_spacer|small|\nend_dialog|blarney_dialog||OK|\n");
						p.CreatePacket(peer);
						break;
						}*/
						/*
						else if (cch.find("action|showanniversarygoldenbox") != string::npos) {
						string items_ = "";
						for (int i = 0; i < current_iotm.size(); i++) {
							int item_id = current_iotm[i][0], left = current_iotm[i][1], total = current_iotm[i][2];
							items_ += "\nadd_label_with_icon|small| `2"+items[item_id].ori_name + "`` - "+to_string(left) + " / " + to_string(total) + " Found|left|"+to_string(item_id) + "|";
						}
						gamepacket_t p(550);
						p.Insert("OnDialogRequest");
						p.Insert("set_default_color|`o\nadd_label_with_icon|big|`w10 Year Anniversary Celebration``|left|7672|\nadd_spacer|small|\nadd_textbox|Returning for a 1 time only celebration of the last 10 years of Growtopia, we have classic IOTMS from coming back to party once more for a very limited time only.|left|\nadd_spacer|small|\nadd_textbox|During the event, IOTM items from the past will begin to return, in strictly limited quantities, through the `2Golden Party-In-A-Box``.|left|\nadd_spacer|small|\nadd_textbox|The current selection of available IOTMs is as follows:|left|\nadd_spacer|small|"+ items_ +"\nadd_spacer|small|\nadd_textbox|The amount available is updated every 3 hours. If an item isn't found in that time it won't be added to the total available.|left|\nadd_spacer|small|\nend_dialog|anniversary_golden_party||Back|");
						p.CreatePacket(peer);
						break;
						}*/
						else if (cch.find("action|dialog_return\ndialog_name|gazette\nbuttonClicked|shop\n\ncheckbox|0\n") != string::npos) SendCmd(peer, "/shop", true);
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|deposit\n\n") SendCmd(peer, "/deposit", true); // news
						else if (cch.find("action|dialog_return\ndialog_name|storageboxxtreme\nitemid|") != string::npos) {
							int item = atoi(cch.substr(57, cch.length() - 57).c_str());
							if (item <= 0 || item >= items.size()) break;
							gamepacket_t p;
							if (pInfo(peer)->lastwrenchb != 4516 and items[item].untradeable == 1 or item == 1424 or item == 5816) {
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You can't store Untradeable items there!"), p.Insert(0), p.Insert(0);
							}
							else if (pInfo(peer)->lastwrenchb == 4516 and items[item].untradeable == 0 or item == 18 || item == 32 || item == 6336 || item == 1424 || item == 5816 || item == 8430) {
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You can't store Tradeable items there!"), p.Insert(0), p.Insert(0);
							}
							else {
								int receive = inventory_contains(peer, item);
								if (receive == 0) break;
								pInfo(peer)->lastchoosenitem = item;
								p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_label_with_icon|big|`w" + items[pInfo(peer)->lastwrenchb].name + "``|left|" + to_string(pInfo(peer)->lastwrenchb) + "|\nadd_textbox|You have " + to_string(receive) + " " + items[item].name + ". How many to store?|left|\nadd_text_input|itemcount||" + to_string(receive) + "|3|\nadd_spacer|small|\nadd_button|do_add|Store Items|noflags|0|0|\nend_dialog|storageboxxtreme|Cancel||\n");
							}
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|storageboxxtreme\nbuttonClicked|itm") != string::npos) {
						int itemnr = atoi(cch.substr(67, cch.length() - 67).c_str()) - 1;
						string name_ = pInfo(peer)->world;
						vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
						if (p != worlds.end()) {
							World* world_ = &worlds[p - worlds.begin()];
							world_->fresh_world = true;
							if (world_->sbox1.size() >= itemnr) {
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
								if (block_access(peer, world_, block_, false, true)) {
									if (world_->sbox1[itemnr].x == pInfo(peer)->lastwrenchx and world_->sbox1[itemnr].y == pInfo(peer)->lastwrenchy) {
										pInfo(peer)->lastchoosennr = itemnr;
										gamepacket_t p;
										int have = inventory_contains(peer, world_->sbox1[itemnr].id), set_have = world_->sbox1[itemnr].count;
										if (have != 0) {
											if (world_->sbox1[itemnr].count >= 200 - have) set_have = 200 - have;
										}
										p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_label_with_icon|big|`w" + items[pInfo(peer)->lastwrenchb].name + "``|left|" + to_string(pInfo(peer)->lastwrenchb) + "|\nadd_textbox|You have `w" + to_string(world_->sbox1[itemnr].count) + " " + items[world_->sbox1[itemnr].id].name + "`` stored.|left|\nadd_textbox|Withdraw how many?|left|\nadd_text_input|itemcount||" + to_string(set_have) + "|3|\nadd_spacer|small|\nadd_button|do_take|Remove Items|noflags|0|0|\nadd_button|cancel|Back|noflags|0|0|\nend_dialog|storageboxxtreme|Exit||\n"), p.CreatePacket(peer);
									}
								}
							}
						}
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|donation_box_edit\nitemid|") != string::npos) {
							int item = atoi(cch.substr(58, cch.length() - 58).c_str()), got = inventory_contains(peer, item);
							if (got == 0) break;
							gamepacket_t p;
							if (items[item].untradeable == 1 || item == 1424 || item == 5816 || items[item].blockType == BlockTypes::FISH) {
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`7[```4You can't place that in the box, you need it!`7]``"), p.Insert(0), p.Insert(0);
							}
							else if (items[item].rarity == 1) {
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`7[```4This box only accepts items rarity 2+ or greater`7]``"), p.Insert(0), p.Insert(0);
							}
							else {
								pInfo(peer)->lastchoosenitem = item;
								p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_label_with_icon|big|" + items[item].name + "``|left|" + to_string(item) + "|\nadd_textbox|How many to put in the box as a gift? (Note: You will `4LOSE`` the items you give!)|left|\nadd_text_input|count|Count:|" + to_string(got) + "|5|\nadd_text_input|sign_text|Optional Note:||128|\nadd_spacer|small|\nadd_button|give|`4Give the item(s)``|noflags|0|0|\nadd_spacer|small|\nadd_button|cancel|`wCancel``|noflags|0|0|\nend_dialog|give_item|||\n");
							}
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|donation_box_edit\nbuttonClicked|takeall\n") != string::npos) {
							bool took = false, fullinv = false;
							gamepacket_t p3;
							p3.Insert("OnTalkBubble"), p3.Insert(pInfo(peer)->netID);
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								world_->fresh_world = true;
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
								if (not block_access(peer, world_, block_)) break;
								if (items[block_->fg].blockType != BlockTypes::DONATION) break;
								for (int i_ = 0; i_ < block_->donates.size(); i_++) {
									int receive = block_->donates[i_].count;
									if (modify_inventory(peer, block_->donates[i_].item, block_->donates[i_].count) == 0) {
										took = true;
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("`7[``" + pInfo(peer)->tankIDName + " receives `5" + to_string(receive) + "`` `w" + items[block_->donates[i_].item].name + "`` from `w" + block_->donates[i_].name + "``, how nice!`7]``");
										block_->donates.erase(block_->donates.begin() + i_);
										i_--;
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											p.CreatePacket(currentPeer);
										}
									}
									else fullinv = true;
								}
								if (block_->donates.size() == 0) {
									if (block_->flags & 0x00400000) block_->flags ^= 0x00400000;
									PlayerMoving data_{};
									data_.packetType = 5, data_.punchX = pInfo(peer)->lastwrenchx, data_.punchY = pInfo(peer)->lastwrenchy, data_.characterState = 0x8;
									BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, block_));
									BYTE* blc = raw + 56;
									form_visual(blc, *block_, *world_, peer, false);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world) continue;
										send_raw(currentPeer, 4, raw, 112 + alloc_(world_, block_), ENET_PACKET_FLAG_RELIABLE);
									}
									delete[] raw, blc;
									if (block_->locked) upd_lock(*block_, *world_, peer);
								}
							}
							if (fullinv) {
								p3.Insert("I don't have enough room in my backpack to get the item(s) from the box!");
								gamepacket_t p2;
								p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("`2(Couldn't get all of the gifts)``"), p2.CreatePacket(peer);
							}
							else if (took) p3.Insert("`2Box emptied.``");
							p3.Insert(0), p3.Insert(0);
							p3.CreatePacket(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|surge\n") {
							if (pInfo(peer)->lastwrenchb == 4296 || pInfo(peer)->lastwrenchb == 8558) {
								setstats(peer, rand() % 31, "", items[pInfo(peer)->lastwrenchb].name);
								pInfo(peer)->lastwrenchb = 0;
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|ss_storage") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								world_->fresh_world = true;
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
								if (block_->fg == 8878) {
									if (to_lower(world_->owner_name) == to_lower(pInfo(peer)->tankIDName) || pInfo(peer)->dev) {
										vector<string> t_ = explode("|", cch);
										if (t_.size() < 4) break;
										string button = explode("\n", t_[3])[0].c_str();
										if (button == "s_password") {
											string text = explode("\n", t_[4])[0].c_str(), text2 = explode("\n", t_[5])[0].c_str();
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											if (not check_password(text) || not check_password(text2)) p.Insert("`4Your input contains special characters. It should only contain alphanumeric characters!``");
											else if (text.empty()) p.Insert("`4You did not enter a new password!``");
											else if (text2.empty()) p.Insert("`4You did not enter a recovery answer!``");
											else if (text.length() > 12 || text2.length() > 12) p.Insert("`4The password is too long! You can only use a maximum of 12 characters!``");
											else {
												p.Insert("`2Your password has been updated!``");
												block_->door_destination = text;
												block_->door_id = text2;
											}
											p.Insert(0), p.Insert(1);
											p.CreatePacket(peer);
										}
										else if (button == "check_password") {
											string password = explode("\n", t_[4])[0].c_str();
											if (to_lower(password) == to_lower(block_->door_destination)) { 
												pInfo(peer)->temporary_vault = password;
												load_storagebox(peer, world_, block_); 
											}
											else {
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID), p.Insert("`4The password you entered did not match!``"), p.Insert(0), p.Insert(1);
												p.CreatePacket(peer);
											}
										}
										else if (button == "show_recoveryanswer") {
											gamepacket_t p;
											p.Insert("OnDialogRequest");
											p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSafe Vault``|left|8878|\nadd_textbox|Please enter recovery answer.|left|\nadd_text_input|storage_recovery_answer|||12|\nadd_button|check_recovery|Enter Recovery Answer|noflags|0|0|\nend_dialog|ss_storage|Exit||\nadd_quick_exit|");
											p.CreatePacket(peer);
										}
										else if (button == "check_recovery") {
											string password = explode("\n", t_[4])[0].c_str();
											if (to_lower(password) == to_lower(block_->door_id)) {
												block_->door_destination = "", block_->door_id = "";
												load_storagebox(peer, world_, block_);
											}
											else {
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID), p.Insert("`4The recovery answer you entered does not match!``"), p.Insert(0), p.Insert(1);
												p.CreatePacket(peer);
											}
										}
									}
								}
							}
							break;
							}
						else if (cch.find("action|dialog_return\ndialog_name|balloonomatic_dialog\namount|") != string::npos) {
						int count = atoi(cch.substr(61, cch.length() - 61).c_str()), got = 0;
						string name_ = pInfo(peer)->world;
						vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
						if (p != worlds.end()) {
							int x_ = pInfo(peer)->lastwrenchx, y_ = pInfo(peer)->lastwrenchy;
							World* world_ = &worlds[p - worlds.begin()];
							world_->fresh_world = true;
							WorldBlock* block_ = &world_->blocks[x_ + (y_ * 100)];
							modify_inventory(peer, pInfo(peer)->lastchoosenitem, got);
							if (got < count || got <= 0 || count <= 0 || count > items.size() || items[pInfo(peer)->lastchoosenitem].untradeable == 1 || pInfo(peer)->lastchoosenitem == 1424 || pInfo(peer)->lastchoosenitem == 5816 || items[pInfo(peer)->lastchoosenitem].rarity >= 200 || items[pInfo(peer)->lastchoosenitem].rarity < 1) break;
							int remove = count * -1;
							modify_inventory(peer, pInfo(peer)->lastchoosenitem, remove);
							vector<int> list{ 4844, 4844, 4844 , 4844, 4844, 4846 , 4846, 4848 };
							vector<pair<int, int>> receive_items;
							int rarity_got = (block_->shelf_1 + items[pInfo(peer)->lastchoosenitem].rarity * count) / 200, total_rarity = items[pInfo(peer)->lastchoosenitem].rarity * count;
							for (int i = 0; i < rarity_got; i++) {
								int item = list[rand() % list.size()];
								vector<pair<int, int>>::iterator p = find_if(receive_items.begin(), receive_items.end(), [&](const pair < int, int>& element) { return element.second == item; });
								if (p != receive_items.end()) {
									if (receive_items[p - receive_items.begin()].first < 200) receive_items[p - receive_items.begin()].first++;
								}
								else receive_items.push_back(make_pair(1, item));
							}
							if (receive_items.size() != 0) {
								sort(receive_items.begin(), receive_items.end());
								reverse(receive_items.begin(), receive_items.end());
								string received = "";
								for (int i = 0; i < receive_items.size(); i++) {
									int give_count = receive_items[i].first;
									if (modify_inventory(peer, receive_items[i].second, give_count) == 0) {

									}
									else {
										WorldDrop drop_block_{};
										drop_block_.id = receive_items[i].second, drop_block_.count = receive_items[i].first, drop_block_.x = pInfo(peer)->x + rand() % 17, drop_block_.y = pInfo(peer)->y + rand() % 17;
										dropas_(world_, drop_block_);
									}
									received += ", " + to_string(receive_items[i].first) + " " + (items[receive_items[i].second].ori_name);
								}
								if (not received.empty()) {
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("The `5Balloon-O-Matic ``whirs to life and creates" + received + ".");
									p.CreatePacket(peer);
								}
							}
							pInfo(peer)->balloon_donated += total_rarity;
							if (pInfo(peer)->balloon_donated >= 8000) {
								pInfo(peer)->balloon_donated -= 8000;
							}
							block_->shelf_1 += total_rarity;
							block_->shelf_1 += (total_rarity)-(rarity_got >= 1 ? rarity_got * 200 : 0);
							if (block_->shelf_1 > 250000 && rand() % 100 < 5) {
								block_->fg = 4832;
								block_->shelf_1 = 0;
								block_->shelf_1 = 0;
								gamepacket_t p;
								p.Insert("OnParticleEffectV2"), p.Insert(8), p.Insert((float)x_*32 + 16, (float)y_*32 + 16);
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									if (pInfo(currentPeer)->world == pInfo(peer)->world) {
										update_tile(currentPeer, x_, y_, 4832, false, false);
										p.CreatePacket(currentPeer);
									}
								}
								vector<int> list{ 4878,4920, 4922, 7528, 7526, 7524, 7520, 7522, 7506, 7508, 7510, 7512, }, rare_list{ 4842, 4856, 4884, 4858,4840, 4922, 4892, }, random_xy{ 1, 0, -1 };
								list.insert(list.end(), { 10428 + (pInfo(peer)->balloon_faction * 2), 4832 + (pInfo(peer)->balloon_faction * 2),4884 + (pInfo(peer)->balloon_faction * 2),4858 + (pInfo(peer)->balloon_faction * 2),4864 + (pInfo(peer)->balloon_faction * 2),4922 + (pInfo(peer)->balloon_faction * 2),4870 + (pInfo(peer)->balloon_faction * 2),4928 + (pInfo(peer)->balloon_faction * 2),4934 + (pInfo(peer)->balloon_faction * 2) });
								for (int i = 0; i < 3; i++) {
									int item = list[rand() % list.size()];
									WorldDrop drop_block_{};
									if (rand() % 2 < 1) item = rare_list[rand() % rare_list.size()];
									drop_block_.id = item, drop_block_.count = (item == 850 || item == 442 || item == 822 || item == 832 || item == 846 ? 10 : (item == 834 ? 5 : 1));
									int randomx = random_xy[rand() % random_xy.size()], randomy = random_xy[rand() % random_xy.size()];
									if (randomx + x_ > 0 && randomx + x_ < world_->max_x && randomy + y_ > 0 && randomy + y_ < world_->max_y) {
										if (world_->blocks[(x_ + randomx) + ((y_ + randomy) * 100)].fg != 0) drop_block_.x = (x_ * 32) + rand() % 17, drop_block_.y = (y_ * 32) + rand() % 17;
										else drop_block_.x = ((x_ + randomx) * 32) + rand() % 17, drop_block_.y = ((y_ + randomy) * 32) + rand() % 17;
									}
									else drop_block_.x = (x_ * 32) + rand() % 17, drop_block_.y = (y_ * 32) + rand() % 17;
									dropas_(world_, drop_block_);
								}
							}
						}
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|give_item\nbuttonClicked|give\n\ncount|") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 6 || pInfo(peer)->lastchoosenitem <= 0 || pInfo(peer)->lastchoosenitem > items.size()) break;
							int count = atoi(explode("\n", t_[4])[0].c_str()), got = 0;
							string text = explode("\n", t_[5])[0].c_str();
							replace_str(text, "\n", "");
							replace_str(text, "<CR>", "");
							modify_inventory(peer, pInfo(peer)->lastchoosenitem, got);
							if (text.size() > 128 || got <= 0 || count <= 0 || count > items.size()) break;
							if (count > got || items[pInfo(peer)->lastchoosenitem].untradeable == 1 || pInfo(peer)->lastchoosenitem == 1424 || pInfo(peer)->lastchoosenitem == 5816 || items[pInfo(peer)->lastchoosenitem].blockType == BlockTypes::FISH) {
								gamepacket_t p;
								p.Insert("OnTalkBubble");
								p.Insert(pInfo(peer)->netID);
								if (count > got) p.Insert("You don't have that to give!");
								else p.Insert("`7[```4You can't place that in the box, you need it!`7]``");
								p.Insert(0), p.Insert(0);
								p.CreatePacket(peer);
							}
							else {
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									world_->fresh_world = true;
									WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
									if (items[block_->fg].blockType != BlockTypes::DONATION) break;
									Donate donate_{};
									donate_.item = pInfo(peer)->lastchoosenitem, donate_.count = count, donate_.name = pInfo(peer)->tankIDName, donate_.text = text;
									block_->donates.push_back(donate_);
									{
										PlayerMoving data_{};
										data_.packetType = 5, data_.punchX = pInfo(peer)->lastwrenchx, data_.punchY = pInfo(peer)->lastwrenchy, data_.characterState = 0x8;
										BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, block_));
										BYTE* blc = raw + 56;
										block_->flags = (block_->flags & 0x00400000 ? block_->flags : block_->flags | 0x00400000);
										form_visual(blc, *block_, *world_, peer, false, true);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 112 + alloc_(world_, block_), ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw, blc;
										if (block_->locked) upd_lock(*block_, *world_, peer);
									}
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world) continue;
										gamepacket_t p, p2;
										p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`7[```5[```w" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->tankIDName) + "`` places `5" + to_string(count) + "`` `2" + items[pInfo(peer)->lastchoosenitem].name + "`` into the " + items[pInfo(peer)->lastwrenchb].name + "`5]```7]``"), p.Insert(0), p.Insert(0);
										p.CreatePacket(currentPeer);
										p2.Insert("OnConsoleMessage");
										p2.Insert("`7[```5[```w" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->tankIDName) + "`` places `5" + to_string(count) + "`` `2" + items[pInfo(peer)->lastchoosenitem].name + "`` into the " + items[pInfo(peer)->lastwrenchb].name + "`5]```7]``");
										p2.CreatePacket(currentPeer);
									}
									send_logs("player: `" + pInfo(peer)->tankIDName + "` lvl: " + to_string(pInfo(peer)->level) + " places " + to_string(count) + " `" + items[pInfo(peer)->lastchoosenitem].name + "` into the " + items[pInfo(peer)->lastwrenchb].name + " in: [" + pInfo(peer)->world + "]", "Donation Box");
									modify_inventory(peer, pInfo(peer)->lastchoosenitem, count *= -1);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|storageboxxtreme\nbuttonClicked|cancel") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								world_->fresh_world = true;
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
								if (to_lower(world_->owner_name) == to_lower(pInfo(peer)->tankIDName) || pInfo(peer)->dev)load_storagebox(peer, world_, block_);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|storageboxxtreme\nbuttonClicked|do_take\n\nitemcount|") != string::npos) {
							int itemnr = pInfo(peer)->lastchoosennr, countofremoval = atoi(cch.substr(83, cch.length() - 83).c_str()), removed = countofremoval, itemcount = 0;
							if (countofremoval <= 0) break;
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								world_->fresh_world = true;
								if (world_->sbox1.size() > itemnr) {
									WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
									if (block_access(peer, world_, block_, false, true)) {
										if (world_->sbox1[itemnr].x == pInfo(peer)->lastwrenchx and world_->sbox1[itemnr].y == pInfo(peer)->lastwrenchy) {
											if (world_->sbox1[itemnr].count <= 0) break;
											if (countofremoval <= world_->sbox1[itemnr].count) {
												if (modify_inventory(peer, world_->sbox1[itemnr].id, countofremoval) == 0) {
													world_->sbox1[itemnr].count -= removed;
													gamepacket_t p, p2;
													p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("Removed `w" + to_string(removed) + " " + items[world_->sbox1[itemnr].id].name + "`` in " + items[pInfo(peer)->lastwrenchb].name + "."), p.Insert(0), p.Insert(1), p.CreatePacket(peer);
													p2.Insert("OnConsoleMessage"), p2.Insert("Removed `w" + to_string(removed) + " " + items[world_->sbox1[itemnr].id].name + "`` in the " + items[pInfo(peer)->lastwrenchb].name + "."), p2.CreatePacket(peer);
													PlayerMoving data_{};
													data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16, data_.packetType = 19, data_.plantingTree = 500, data_.punchX = world_->sbox1[itemnr].id, data_.punchY = pInfo(peer)->netID;
													int32_t to_netid = pInfo(peer)->netID;
													BYTE* raw = packPlayerMoving(&data_);
													raw[3] = 5;
													memcpy(raw + 8, &to_netid, 4);
													send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
													delete[] raw;
													if (world_->sbox1[itemnr].count <= 0) world_->sbox1.erase(world_->sbox1.begin() + itemnr);
												}
											}
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|reward_") != string::npos) {
							reward_show(peer, cch.substr(66, cch.length() - 68).c_str());
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|backpack_menu\nbuttonClicked|backpack_upgrade_request\n\n") {
						gamepacket_t p;
						p.Insert("OnDialogRequest");
						p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wUpgrade Backpack to level " + to_string(pInfo(peer)->b_l + 1) + "?``|left|8430|\nadd_spacer|small|\nadd_button|backpack_upgrade|`2Upgrade for " + setGems((1000 * ((pInfo(peer)->b_l * pInfo(peer)->b_l) + 25)) * 2) + " gems``|noflags|0|0|\nend_dialog|backpack_menu|Exit||\nadd_quick_exit|");
						p.CreatePacket(peer);
						break;
						}
						else if (cch == "action|dialog_return\ndialog_name|backpack_menu\nbuttonClicked|backpack_upgrade\n\n") {
							if (pInfo(peer)->gems >= (1000 * ((pInfo(peer)->b_l * pInfo(peer)->b_l) + 25)) * 2) {
								if (pInfo(peer)->b_l * 10 > 200) {
									{
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("You have reached max slots!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
								}
								else {
									OnSetGems(peer, ((1000 * ((pInfo(peer)->b_l * pInfo(peer)->b_l) + 25)) * 2) * - 1);
									pInfo(peer)->b_l++;
									packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
									{
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("Congratulations! You have upgraded your Backpack!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
									backpack_show(peer);
								}
							}
							else {
								gamepacket_t p;
								p.Insert("OnTalkBubble");
								p.Insert(pInfo(peer)->netID);
								p.Insert("You don't have enough gems!");
								p.Insert(0), p.Insert(0);
								p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|backpack_menu\nbuttonClicked|") != string::npos) {
							int choosen_item = atoi(cch.substr(61, cch.length() - 61).c_str());
							if (choosen_item >= pInfo(peer)->bp.size() || choosen_item > 200 || choosen_item > pInfo(peer)->b_l * 10) break;
							for (int i_ = 0; i_ < pInfo(peer)->bp.size(); i_++) {
								if (choosen_item == i_) {
									if (pInfo(peer)->bp[choosen_item].first <= 0 || pInfo(peer)->bp[choosen_item].first >= items.size()) break;
									int pickedup = pInfo(peer)->bp[choosen_item].second;
									int count = pInfo(peer)->bp[choosen_item].second;
									if (modify_inventory(peer, pInfo(peer)->bp[choosen_item].first, count) == 0) {
										{
											gamepacket_t p, p2;
											p.Insert("OnConsoleMessage"), p.Insert("You picked up " + to_string(pickedup) + " " + items[pInfo(peer)->bp[choosen_item].first].name + "."), p.CreatePacket(peer);
											p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p.Insert("You picked up " + to_string(pickedup) + " " + items[pInfo(peer)->bp[choosen_item].first].name + "."), p2.Insert(0), p2.Insert(0), p2.CreatePacket(peer);
										}
										PlayerMoving data_{};
										data_.packetType = 19, data_.punchX = pInfo(peer)->bp[choosen_item].first, data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16;
										int32_t to_netid = pInfo(peer)->netID;
										BYTE* raw = packPlayerMoving(&data_);
										raw[3] = 5;
										memcpy(raw + 8, &to_netid, 4);
										send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										delete[]raw;
										pInfo(peer)->bp.erase(pInfo(peer)->bp.begin() + i_);
									}
									else {
										gamepacket_t p;
										p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You don't have enough inventory space!"), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|backpack_menu\nitemid|") != string::npos) {
							if (pInfo(peer)->bp.size() <= pInfo(peer)->b_l * 10) {
								int got = 0, item = atoi(cch.substr(54, cch.length() - 54).c_str());
								modify_inventory(peer, item, got);
								if (got <= 0) break;
								if (item == 18 || item == 32 || item == 1424 || item == 5816 || items[item].blockType == BlockTypes::FISH || item == 1796 || item == 7188 || item == 242) {
									gamepacket_t p;
									p.Insert("OnTalkBubble");
									p.Insert(pInfo(peer)->netID);
									p.Insert("You can't store this item!");
									p.Insert(0), p.Insert(0);
									p.CreatePacket(peer);
								}
								else {
									pInfo(peer)->bp.push_back(make_pair(item, got));
									modify_inventory(peer, item, got *= -1);
									PlayerMoving data_{};
									data_.packetType = 19, data_.punchX = item, data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16;
									int32_t to_netid = pInfo(peer)->netID;
									BYTE* raw = packPlayerMoving(&data_);
									raw[3] = 5;
									memcpy(raw + 8, &to_netid, 4);
									send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									delete[]raw;
									backpack_show(peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|t_claimreward") != string::npos) {
							int reward = atoi(cch.substr(72, cch.length() - 72).c_str()), lvl = 0, count = 1;
							vector<int> list{ 98, 228, 1746, 1778, 1830, 5078, 1966, 6948, 6946, 4956 };
							if (reward <= 0 || reward > list.size()) break;
							if (list[reward - 1] == 228 || list[reward - 1] == 1746 || list[reward - 1] == 1778) count = 200;
							if (find(pInfo(peer)->t_p.begin(), pInfo(peer)->t_p.end(), lvl = reward * 5) == pInfo(peer)->t_p.end()) {
								if (pInfo(peer)->t_lvl >= lvl) {
									if (modify_inventory(peer, list[reward - 1], count) == 0) {
										pInfo(peer)->t_p.push_back(lvl);
										packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										{
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("Congratulations! You have received your Farmer Reward!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
										PlayerMoving data_{};
										data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
										BYTE* raw = packPlayerMoving(&data_);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw;
										{
											PlayerMoving data_{};
											data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16, data_.packetType = 19, data_.plantingTree = 100, data_.punchX = list[reward - 1], data_.punchY = pInfo(peer)->netID;
											int32_t to_netid = pInfo(peer)->netID;
											BYTE* raw = packPlayerMoving(&data_);
											raw[3] = 5;
											memcpy(raw + 8, &to_netid, 4);
											send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											delete[] raw;
										}
										reward_show(peer, "farmer");
									}
									else {
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("You have full inventory space!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|view_inventory\nbuttonClicked|") != string::npos) {
							if (pInfo(peer)->give_role) {
								int item = atoi(cch.substr(62, cch.length() - 62).c_str());
								pInfo(peer)->choosenitem = item;
								if (item <= 0 || item > items.size()) break;
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
										int got = 0;
										modify_inventory(currentPeer, pInfo(peer)->choosenitem, got);
										gamepacket_t p;
										p.Insert("OnDialogRequest");
										p.Insert("set_default_color|`o\nadd_label_with_icon|big|`4Take`` `w" + items[pInfo(peer)->choosenitem].name + " from`` `#" + pInfo(currentPeer)->tankIDName + "``|left|" + to_string(pInfo(peer)->choosenitem) + "|\nadd_textbox|How many to `4take``? (player has " + to_string(got) + ")|left|\nadd_text_input|count||" + to_string(got) + "|5|\nend_dialog|take_item|Cancel|OK|");
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|take_item\ncount|") != string::npos) {
							if (pInfo(peer)->give_role) {
								int count = atoi(cch.substr(49, cch.length() - 49).c_str()), receive = atoi(cch.substr(49, cch.length() - 49).c_str());
								int remove = count * -1;
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
										if (count <= 0 || count > 200) break;
										if (modify_inventory(peer, pInfo(peer)->choosenitem, count) == 0) {
											int total = 0;
											modify_inventory(currentPeer, pInfo(peer)->choosenitem, total += remove);
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("Collected `w" + to_string(receive) + " " + items[pInfo(peer)->choosenitem].name + "``." + (items[pInfo(peer)->choosenitem].rarity > 363 ? "" : " Rarity: `w" + to_string(items[pInfo(peer)->choosenitem].rarity) + "``") + "");
											p.CreatePacket(peer);
											send_logs(pInfo(peer)->tankIDName + " took " + to_string(receive) + " " + items[pInfo(peer)->choosenitem].name + " from " + pInfo(currentPeer)->tankIDName, "Inventory Steal");
											//send_logs("player: `" + pInfo(peer)->tankIDName + "` level: " + to_string(pInfo(peer)->level) + " (dropped item) collected `" + to_string(receive) + "` Blue gem locks " " in: [" + pInfo(peer)->world + "]", "1046804223515443210/Wsx7OIq4LGLXpysJ7aP_Fsg0qp0ZxcTw0zwL0c7foFXN-tlqRdU5PdenG858mHnDKNuS");
										}

									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|p_claimreward") != string::npos) {
							int reward = atoi(cch.substr(72, cch.length() - 72).c_str()), lvl = 0, count = 1;
							vector<int> list{ 1008,1044,872,10450,870,5084,876,6950,6952,9166 };
							if (reward <= 0 || reward > list.size()) break;
							if (list[reward - 1] == 1008) count = 5;
							if (list[reward - 1] == 1044) count = 50;
							if (list[reward - 1] == 872) count = 200;
							if (list[reward - 1] == 10450) count = 3;
							if (find(pInfo(peer)->p_p.begin(), pInfo(peer)->p_p.end(), lvl = reward * 5) == pInfo(peer)->p_p.end()) {
								if (pInfo(peer)->p_lvl >= lvl) {
									if (modify_inventory(peer, list[reward - 1], count) == 0) {
										pInfo(peer)->p_p.push_back(lvl);
										packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										{
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("Congratulations! You have received your Provider Reward!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
										PlayerMoving data_{};
										data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
										BYTE* raw = packPlayerMoving(&data_);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw;
										{
											PlayerMoving data_{};
											data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16, data_.packetType = 19, data_.plantingTree = 100, data_.punchX = list[reward - 1], data_.punchY = pInfo(peer)->netID;
											int32_t to_netid = pInfo(peer)->netID;
											BYTE* raw = packPlayerMoving(&data_);
											raw[3] = 5;
											memcpy(raw + 8, &to_netid, 4);
											send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											delete[] raw;
										}
										reward_show(peer, "provider");
									}
									else {
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("You have full inventory space!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|g_claimreward") != string::npos) {
							int reward = atoi(cch.substr(72, cch.length() - 72).c_str()), lvl = 0, count = 1;
							vector<int> list{ 4654,262,826,828,9712,3146,2266,5072,5070,9716 };
							if (reward <= 0 || reward > list.size()) break;
							if (list[reward - 1] == 262 || list[reward - 1] == 826 || list[reward - 1] == 828) count = 50;
							if (list[reward - 1] == 3146) count = 10;
							if (find(pInfo(peer)->g_p.begin(), pInfo(peer)->g_p.end(), lvl = reward * 5) == pInfo(peer)->g_p.end()) {
								if (pInfo(peer)->g_lvl >= lvl) {
									if (modify_inventory(peer, list[reward - 1], count) == 0) {
										pInfo(peer)->g_p.push_back(lvl);
										packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										{
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("Congratulations! You have received your Geiger Hunting Reward!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
										PlayerMoving data_{};
										data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
										BYTE* raw = packPlayerMoving(&data_);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw;
										{
											PlayerMoving data_{};
											data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16, data_.packetType = 19, data_.plantingTree = 100, data_.punchX = list[reward - 1], data_.punchY = pInfo(peer)->netID;
											int32_t to_netid = pInfo(peer)->netID;
											BYTE* raw = packPlayerMoving(&data_);
											raw[3] = 5;
											memcpy(raw + 8, &to_netid, 4);
											send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											delete[] raw;
										}
										reward_show(peer, "geiger");
									}
									else {
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("You have full inventory space!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|f_claimreward") != string::npos) {
							int reward = atoi(cch.substr(72, cch.length() - 72).c_str()), lvl = 0, count = 1;
							vector<int> list{ 3010, 3018, 3020, 3044, 5740, 3042, 3098, 3100, 3040, 10262 };
							if (reward <= 0 || reward > list.size()) break;
							if (list[reward - 1] == 3018) count = 200;
							if (list[reward - 1] == 3020 || list[reward - 1] == 3098) count = 50;
							if (list[reward - 1] == 3044) count = 25;
							if (find(pInfo(peer)->ff_p.begin(), pInfo(peer)->ff_p.end(), lvl = reward * 5) == pInfo(peer)->ff_p.end()) {
								if (pInfo(peer)->ff_lvl >= lvl) {
									if (modify_inventory(peer, list[reward - 1], count) == 0) {
										pInfo(peer)->ff_p.push_back(lvl);
										packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										{
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("Congratulations! You have received your Fishing Reward!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
										PlayerMoving data_{};
										data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
										BYTE* raw = packPlayerMoving(&data_);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw;
										{
											PlayerMoving data_{};
											data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16, data_.packetType = 19, data_.plantingTree = 100, data_.punchX = list[reward - 1], data_.punchY = pInfo(peer)->netID;
											int32_t to_netid = pInfo(peer)->netID;
											BYTE* raw = packPlayerMoving(&data_);
											raw[3] = 5;
											memcpy(raw + 8, &to_netid, 4);
											send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											delete[] raw;
										}
										reward_show(peer, "fishing");
									}
									else {
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("You have full inventory space!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|roadtoglory\nbuttonClicked|claimreward") != string::npos) {
							int count = atoi(cch.substr(70, cch.length() - 70).c_str());
							if (count < 1 || count >10) break;
							if (std::find(pInfo(peer)->glo_p.begin(), pInfo(peer)->glo_p.end(), count) == pInfo(peer)->glo_p.end()) {
								if (pInfo(peer)->level >= count * 10) {
									pInfo(peer)->glo_p.push_back(count);
									packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
									OnSetGems(peer, count * 10000);
									{
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("Congratulations! You have received your Road to Glory Reward!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
									PlayerMoving data_{};
									data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
									BYTE* raw = packPlayerMoving(&data_);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
										send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									}
									delete[] raw;
									glory_show(peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|bulletin_edit\nbuttonClicked|clear\n") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								world_->fresh_world = true;
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
								if (items[block_->fg].blockType == BlockTypes::BULLETIN_BOARD || items[block_->fg].blockType == BlockTypes::MAILBOX) {
									for (int i_ = 0; i_ < world_->bulletin.size(); i_++) {
										if (world_->bulletin[i_].x == pInfo(peer)->lastwrenchx and world_->bulletin[i_].y == pInfo(peer)->lastwrenchy) {
											world_->bulletin.erase(world_->bulletin.begin() + i_);
											i_--;
										}
									}
									{
										gamepacket_t p;
										p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert(items[pInfo(peer)->lastwrenchb].blockType == BlockTypes::MAILBOX ? "`2Mailbox emptied.``" : "`2Text cleared.``"), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
									}
									if (block_->flags & 0x00400000) block_->flags ^= 0x00400000;
									PlayerMoving data_{};
									data_.packetType = 5, data_.punchX = pInfo(peer)->lastwrenchx, data_.punchY = pInfo(peer)->lastwrenchy, data_.characterState = 0x8;
									BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, block_));
									BYTE* blc = raw + 56;
									form_visual(blc, *block_, *world_, peer, false);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
										send_raw(currentPeer, 4, raw, 112 + alloc_(world_, block_), ENET_PACKET_FLAG_RELIABLE);
									}
									delete[] raw, blc;
									if (block_->locked) upd_lock(*block_, *world_, peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|remove_bulletin") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								world_->fresh_world = true;
								if (world_->bulletin.size() != 0 && pInfo(peer)->lastchoosennr <= world_->bulletin.size() && world_->bulletin[pInfo(peer)->lastchoosennr].x == pInfo(peer)->lastwrenchx and world_->bulletin[pInfo(peer)->lastchoosennr].y == pInfo(peer)->lastwrenchy) {
									world_->bulletin.erase(world_->bulletin.begin() + pInfo(peer)->lastchoosennr);
									gamepacket_t p;
									p.Insert("OnTalkBubble");
									p.Insert(pInfo(peer)->netID);
									p.Insert("`2Bulletin removed.``");
									p.Insert(0), p.Insert(0);
									p.CreatePacket(peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|bulletin_edit\nbuttonClicked|edit") != string::npos) {
							pInfo(peer)->lastchoosennr = atoi(cch.substr(65, cch.length() - 65).c_str());
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								if (pInfo(peer)->lastchoosennr > world_->bulletin.size() || pInfo(peer)->lastchoosennr < 0) break;
								world_->fresh_world = true;
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|small|Remove`` \"`w" + world_->bulletin[pInfo(peer)->lastchoosennr].text + "\"`` from your board?|left|" + to_string(pInfo(peer)->lastwrenchb) + "|\nend_dialog|remove_bulletin|Cancel|OK|");
								p.CreatePacket(peer);
							}
							break;
							}
						else if (cch.find("action|dialog_return\ndialog_name|storageboxxtreme\nbuttonClicked|change_password") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								world_->fresh_world = true;
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
								if (block_->fg == 8878) {
									if (block_access(peer, world_, block_, false, true)) {
										gamepacket_t p;
										p.Insert("OnDialogRequest");
										p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSafe Vault``|left|8878|\nadd_smalltext|The ingenious minds at GrowTech bring you the `2Safe Vault`` - a place to store your items safely with its integrated password option!|left|\nadd_smalltext|How the password works:|left|\nadd_smalltext|The Safe Vault requires both a `2password`` and a `2recovery answer`` to be entered to use a password.|left|\nadd_smalltext|Enter your `2password`` and `2recovery answer`` below - keep them safe and `4DO NOT`` share them with anyone you do not trust!|left|\nadd_smalltext|The password and recovery answer can be no longer than 12 characters in length - number and alphabet only please, no special characters are allowed!|left|\nadd_smalltext|If you forget your password, enter your recovery answer to access the Safe Vault - The Safe Vault will `4NOT be password protected now``. You will need to enter a new password.|left|\nadd_smalltext|You can change your password, however you will need to enter the old password before a new one can be used.|left|\nadd_smalltext|`4WARNING``: DO NOT forget your password and recovery answer or you will not be able to access the Safe Vault!|left|\nadd_textbox|`4There is no password currently set on this Safe Vault.``|left|\nadd_textbox|Enter a new password.|left|\nadd_text_input_password|storage_newpassword|||18|\nadd_textbox|Enter a recovery answer.|left|\nadd_text_input|storage_recoveryanswer|||12|\nadd_button|s_password|Update Password|noflags|0|0|\nend_dialog|ss_storage|Exit||\nadd_quick_exit|");
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|advbegins\nnameEnter|") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								world_->fresh_world = true;
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
								if (block_->fg == 4722) {
									if (block_access(peer, world_, block_)) {
										string text = cch.substr(53, cch.length() - 54).c_str();
										if (text.size() > 32) break;
										block_->heart_monitor = text;
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID), p.Insert("Updated adventure!"), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world or pInfo(currentPeer)->adventure_begins == false) continue;
											pInfo(currentPeer)->adventure_begins = false;
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|bulletin_edit\nbuttonClicked|send\n\nsign_text|") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 4) break;
							string text = explode("\n", t_[4])[0].c_str();
							replace_str(text, "\n", "");
							replace_str(text, "<CR>", "");
							if (text.length() <= 2 || text.length() >= 100) {
								gamepacket_t p;
								p.Insert("OnTalkBubble");
								p.Insert(pInfo(peer)->netID);
								p.Insert("That's not interesting enough to post.");
								p.Insert(0), p.Insert(0);
								p.CreatePacket(peer);
							}
							else {
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									{
										World* world_ = &worlds[p - worlds.begin()];
										world_->fresh_world = true;
										if (items[pInfo(peer)->lastwrenchb].blockType == BlockTypes::MAILBOX || items[pInfo(peer)->lastwrenchb].blockType == BlockTypes::BULLETIN_BOARD) {
											int letter = 0;
											for (int i_ = 0; i_ < world_->bulletin.size(); i_++) if (world_->bulletin[i_].x == pInfo(peer)->lastwrenchx and world_->bulletin[i_].y == pInfo(peer)->lastwrenchy) letter++;
											if (letter == 21) world_->bulletin.erase(world_->bulletin.begin() + 0);
											WorldBulletin bulletin_{};
											bulletin_.x = pInfo(peer)->lastwrenchx, bulletin_.y = pInfo(peer)->lastwrenchy;
											if (pInfo(peer)->name_color == "`p@" || pInfo(peer)->name_color == "`8@" || pInfo(peer)->name_color == "`e@" || pInfo(peer)->name_color == "`6@" || pInfo(peer)->name_color == "`#@" || pInfo(peer)->name_color == "`0") bulletin_.name = (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->name_color + pInfo(peer)->tankIDName) + "``";
											else bulletin_.name = "`0" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->tankIDName) + "``";
											bulletin_.text = text;
											world_->bulletin.push_back(bulletin_);
											{
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert(items[pInfo(peer)->lastwrenchb].blockType == BlockTypes::MAILBOX ? "`2You place your letter in the mailbox.``" : "`2Bulletin posted.``");
												p.Insert(0), p.Insert(0);
												p.CreatePacket(peer);
											}
											if (items[pInfo(peer)->lastwrenchb].blockType == BlockTypes::MAILBOX) {
												WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
												block_->flags = (block_->flags & 0x00400000 ? block_->flags : block_->flags | 0x00400000);
												PlayerMoving data_{};
												data_.packetType = 5, data_.punchX = pInfo(peer)->lastwrenchx, data_.punchY = pInfo(peer)->lastwrenchy, data_.characterState = 0x8;
												BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, block_));
												BYTE* blc = raw + 56;
												form_visual(blc, *block_, *world_, peer, false, true);
												for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
													if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world) continue;
													send_raw(currentPeer, 4, raw, 112 + alloc_(world_, block_), ENET_PACKET_FLAG_RELIABLE);
												}
												delete[] raw, blc;
												if (block_->locked) upd_lock(*block_, *world_, peer);
											}
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|storageboxxtreme\nbuttonClicked|do_add\n\nitemcount|") != string::npos) {
							int count = atoi(cch.substr(82, cch.length() - 82).c_str());
							if (pInfo(peer)->lastchoosenitem <= 0 || pInfo(peer)->lastchoosenitem >= items.size()) break;
							if (pInfo(peer)->lastwrenchb != 4516 and items[pInfo(peer)->lastchoosenitem].untradeable == 1 or pInfo(peer)->lastchoosenitem == 1424 or pInfo(peer)->lastchoosenitem == 5816 or items[pInfo(peer)->lastchoosenitem].blockType == BlockTypes::FISH) {
								gamepacket_t p;
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You can't store Untradeable items there!"), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
							}
							else if (pInfo(peer)->lastwrenchb == 4516 and items[pInfo(peer)->lastchoosenitem].untradeable == 0 or pInfo(peer)->lastchoosenitem == 1424 or pInfo(peer)->lastchoosenitem == 5816 || items[pInfo(peer)->lastchoosenitem].blockType == BlockTypes::FISH || pInfo(peer)->lastchoosenitem == 18 || pInfo(peer)->lastchoosenitem == 32 || pInfo(peer)->lastchoosenitem == 6336 || pInfo(peer)->lastchoosenitem == 8430) {
								gamepacket_t p;
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You can't store Tradeable items there!"), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
							}
							else {
								int got = 0, receive = 0;
								modify_inventory(peer, pInfo(peer)->lastchoosenitem, got);
								if (count <= 0 || count > got) {
									gamepacket_t p;
									p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You don't have that many!"), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
								}
								else {
									receive = count * -1;
									string name_ = pInfo(peer)->world;
									vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
									if (p != worlds.end()) {
										World* world_ = &worlds[p - worlds.begin()];
										world_->fresh_world = true;
										WorldBlock block_ = world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
										if (items[pInfo(peer)->lastchoosenitem].untradeable == 1 && block_.fg != 4516) break;
										if (items[block_.fg].blockType != BlockTypes::STORAGE) break;
										gamepacket_t p1, p2;
										p1.Insert("OnTalkBubble"), p1.Insert(pInfo(peer)->netID), p1.Insert("Stored `w" + to_string(count) + " " + items[pInfo(peer)->lastchoosenitem].name + "`` in " + items[pInfo(peer)->lastwrenchb].name + "."), p1.Insert(0), p1.Insert(0), p1.CreatePacket(peer);
										p2.Insert("OnConsoleMessage"), p2.Insert("Stored `w" + to_string(count) + " " + items[pInfo(peer)->lastchoosenitem].name + "`` in the " + items[pInfo(peer)->lastwrenchb].name + "."), p2.CreatePacket(peer);
										modify_inventory(peer, pInfo(peer)->lastchoosenitem, receive);
										bool dublicated = true;
										for (int i_ = 0; i_ < world_->sbox1.size(); i_++) {
											if (dublicated) {
												if (world_->sbox1[i_].x == pInfo(peer)->lastwrenchx and world_->sbox1[i_].y == pInfo(peer)->lastwrenchy and world_->sbox1[i_].id == pInfo(peer)->lastchoosenitem and world_->sbox1[i_].count + count <= 200) {
													world_->sbox1[i_].count += count;
													dublicated = false;
												}
											}
										}
										if (dublicated) {
											WorldSBOX1 sbox1_{};
											sbox1_.x = pInfo(peer)->lastwrenchx, sbox1_.y = pInfo(peer)->lastwrenchy;
											sbox1_.id = pInfo(peer)->lastchoosenitem, sbox1_.count = count;
											world_->sbox1.push_back(sbox1_);
										}
										PlayerMoving data_{};
										data_.packetType = 19, data_.netID = -1, data_.plantingTree = 0;
										data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16;
										data_.XSpeed = pInfo(peer)->x + 16, data_.YSpeed = pInfo(peer)->y + 16;
										data_.punchX = pInfo(peer)->lastchoosenitem;
										BYTE* raw = packPlayerMoving(&data_);
										raw[3] = 6;
										send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										delete[] raw;
										load_storagebox(peer, world_, &block_);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|ban_player") != string::npos) {
							if (pInfo(peer)->mod == 0) break;
						if (pInfo(peer)->superdev == 0) {
							if (has_playmod2(pInfo(peer), 120)) {
								int time_ = 0;
								for (PlayMods peer_playmod : pInfo(peer)->playmods) {
									if (peer_playmod.id == 120) {
										time_ = peer_playmod.time - time(nullptr);
										break;
									}
								}
								packet_(peer, "action|log\nmsg|>> (" + to_playmod_time(time_) + " before you can ban again)", "");
								break;
							}
							add_playmod(peer, 120);
						}
						for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
							if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
								long long int seconds = pInfo(peer)->ban_seconds;
								add_ban_or_mute(currentPeer, pInfo(peer)->ban_seconds, pInfo(peer)->ban_reason, pInfo(peer)->name_color + pInfo(peer)->tankIDName + "``", 76);
								add_modlogs(peer, pInfo(peer)->name_color + pInfo(peer)->tankIDName, "BANNED (" + pInfo(peer)->ban_reason + "): " + pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName + "``", "`#" + ((seconds / 86400 > 0) ? to_string(seconds / 86400) + " days" : (seconds / 3600 > 0) ? to_string(seconds / 3600) + " hours" : (seconds / 60 > 0) ? to_string(seconds / 60) + " minutes" : to_string(seconds) + " seconds"));
							}
						}
						break;
						}
						else if (cch.find("action|drop") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								world_->fresh_world = true;
								vector<string> t_ = explode("|", cch);
								if (t_.size() < 4) break;
								int id_ = atoi(explode("\n", t_[3])[0].c_str()), c_ = 0;
								if (id_ <= 0 or id_ >= items.size()) break;
								if (find(world_->active_jammers.begin(), world_->active_jammers.end(), 4758) != world_->active_jammers.end()) {
									if (to_lower(world_->owner_name) == to_lower(pInfo(peer)->tankIDName) or find(world_->admins.begin(), world_->admins.end(), to_lower(pInfo(peer)->tankIDName)) != world_->admins.end()) {

									}
									else {
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("The Mini-Mod says no dropping items in this world!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
										break;
									}
								}
								if (items[id_].untradeable or id_ == 1424 or id_ == 5816) {
									gamepacket_t p;
									p.Insert("OnTextOverlay");
									p.Insert("You can't drop that.");
									p.CreatePacket(peer);
									break;
								}
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									world_->fresh_world = true;
									int a_ = rand() % 12;
									int x = (pInfo(peer)->state == 16 ? pInfo(peer)->x - (a_ + 20) : (pInfo(peer)->x + 20) + a_);
									int y = pInfo(peer)->y + rand() % 16;
									//BlockTypes type_ = FOREGROUND;
									int where_ = (pInfo(peer)->state == 16 ? x / 32 : round((double)x / 32)) + (y / 32 * 100);
									if (where_ < 0 || x < 0 || y < 0 || where_ > world_->blocks.size()) {
										gamepacket_t p;
										p.Insert("OnTextOverlay");
										p.Insert("You can't drop that here, face somewhere with open space.");
										p.CreatePacket(peer);
										break;
									}
									WorldBlock* block_ = &world_->blocks[where_];
									if (items[block_->fg].collisionType == 1 || block_->fg == 6 || items[block_->fg].blockType == BlockTypes::GATEWAY || items[block_->fg].toggleable and is_false_state(world_->blocks[(pInfo(peer)->state == 16 ? x / 32 : round((double)x / 32)) + (y / 32 * 100)], 0x00400000)) {
										gamepacket_t p;
										p.Insert("OnTextOverlay");
										p.Insert(items[block_->fg].blockType == BlockTypes::MAIN_DOOR ? "You can't drop items on the white door." : "You can't drop that here, face somewhere with open space.");
										p.CreatePacket(peer);
										break;
									}
									/*if (static_(type_, items[world_->blocks[((pInfo(peer)->state == 16 ? pInfo(peer)->x - 18 : pInfo(peer)->x + 22) / 32] + ((y / 32) * 100)).fg)) or static_(type_, items[world_->blocks[((pInfo(peer)->state == 16 ? pInfo(peer)->x - 24 : pInfo(peer)->x + 42) / 32] + ((y / 32) * 100)).fg))) {
										if (items[block_->fg].collisionType == 1) {
											gamepacket_t p;
											p.Insert("OnTextOverlay");
											p.Insert(type_ == MAIN_DOOR ? "You can't drop items on the white door." : "You can't drop that here, face somewhere with open space.");
											p.CreatePacket(peer);
											break;
										}
									}*/
									int count_ = 0;
									for (int i_ = 0; i_ < world_->drop_new.size(); i_++) {
										if (abs(world_->drop_new[i_][4] - y) <= 16 and abs(world_->drop_new[i_][3] - x) <= 16) count_ += 1;
									}
									if (count_ > 20) {
										gamepacket_t p;
										p.Insert("OnTextOverlay");
										p.Insert("You can't drop that here, find an emptier spot!");
										p.CreatePacket(peer);
										break;
									}
								}
								modify_inventory(peer, id_, c_);
								if (pInfo(peer)->cheater_settings & Gtps3::SETTINGS_5 && pInfo(peer)->disable_cheater == 0) {
									string name_ = pInfo(peer)->world;
									vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
									if (p != worlds.end()) {
										World* world_ = &worlds[p - worlds.begin()];
										world_->fresh_world = true;
										WorldDrop drop_{};
										drop_.id = id_, drop_.count = c_;
										int a_ = rand() % 12;
										drop_.x = (pInfo(peer)->state == 16 ? pInfo(peer)->x - (a_ + 20) : (pInfo(peer)->x + 20) + a_), drop_.y = pInfo(peer)->y + rand() % 16;
										c_ *= -1;
										if (modify_inventory(peer, id_, c_) == 0) {
											add_cctv(peer, "dropped", to_string(abs(c_)) + " " + items[id_].name);
											dropas_(world_, drop_, pInfo(peer)->netID);
										}
									}
								}
								else {
									{
										gamepacket_t p;
										p.Insert("OnDialogRequest");
										p.Insert("set_default_color|`o\nadd_label_with_icon|big|`w" + items[id_].ori_name + "``|left|" + to_string(id_) + "|\nadd_textbox|How many to drop?|left|\nadd_text_input|count||" + (items[id_].blockType == BlockTypes::FISH ? "1" : to_string(c_)) + "|5|\nembed_data|itemID|" + to_string(id_) + "" + (to_lower(world_->owner_name) != to_lower(pInfo(peer)->tankIDName) and not pInfo(peer)->dev and (/*!guild_access(peer, world_->guild_id) and */ find(world_->admins.begin(), world_->admins.end(), to_lower(pInfo(peer)->tankIDName)) == world_->admins.end()) ? "\nadd_textbox|If you are trying to trade an item with another player, use your wrench on them instead to use our Trade System! `4Dropping items is not safe!``|left|" : "") + "\nend_dialog|drop_item|Cancel|OK|");
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|setRoleIcon") != string::npos || cch.find("action|setRoleSkin") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 3) break;
							string id_ = explode("\n", t_[2])[0];
							if (not isdigit(id_[0])) break;
							uint32_t role_t = atoi(id_.c_str());
							if (cch.find("action|setRoleIcon") != string::npos) {
								if (role_t == 6) pInfo(peer)->roleIcon = role_t;
								else if (role_t == 0 and pInfo(peer)->t_lvl >= 50) pInfo(peer)->roleIcon = role_t;
								else if (role_t == 1 and pInfo(peer)->bb_lvl >= 50) pInfo(peer)->roleIcon = role_t;
								else if (role_t == 2 and pInfo(peer)->s_lvl >= 50) pInfo(peer)->roleIcon = role_t;
								else if (role_t == 3 and pInfo(peer)->ff_lvl >= 50) pInfo(peer)->roleIcon = role_t;
							}
							else {
								if (role_t == 6) pInfo(peer)->roleSkin = role_t;
								else if (role_t == 0 and pInfo(peer)->t_lvl >= 50) pInfo(peer)->roleSkin = role_t;
								else if (role_t == 1 and pInfo(peer)->bb_lvl >= 50) pInfo(peer)->roleSkin = role_t;
								else if (role_t == 2 and pInfo(peer)->s_lvl >= 50) pInfo(peer)->roleSkin = role_t;
								else if (role_t == 3 and pInfo(peer)->ff_lvl >= 50) pInfo(peer)->roleSkin = role_t;
							}
							gamepacket_t p(0, pInfo(peer)->netID);
							p.Insert("OnSetRoleSkinsAndIcons"), p.Insert(pInfo(peer)->roleSkin), p.Insert(pInfo(peer)->roleIcon), p.Insert(0);
							for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != pInfo(peer)->world) continue;
								p.CreatePacket(currentPeer);
							}
							break;
						}
						else if (cch.find("action|setSkin") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 3) break;
							string id_ = explode("\n", t_[2])[0];
							if (not isdigit(id_[0])) break;
							char* endptr = NULL;
							unsigned int skin_ = strtoll(id_.c_str(), &endptr, 10);
							if (skin_ == 3531226367 || skin_ == 4023103999 || skin_ == 1345519520 || skin_ == 194314239) {
								if (pInfo(peer)->supp == 2 or pInfo(peer)->subscriber) pInfo(peer)->skin = skin_;
							}
							else if (skin_ == 3578898848 || skin_ == 3317842336) {
								if (pInfo(peer)->gp || pInfo(peer)->subscriber) pInfo(peer)->skin = skin_;
							}
							else pInfo(peer)->skin = skin_;
							update_clothes(peer, true, true);
							break;
						}
						else if (cch.find("action|trash") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 4) break;
							int id_ = atoi(explode("\n", t_[3])[0].c_str()), c_ = 0;
							if (id_ <= 0 or id_ >= items.size()) break;
							gamepacket_t p;
							if (id_ == 18 || id_ == 32 || id_ == 6336 || id_ == 8430) {
								packet_(peer, "action|play_sfx\nfile|audio/cant_place_tile.wav\ndelayMS|0");
								p.Insert("OnTextOverlay"), p.Insert("You'd be sorry if you lost that!"), p.CreatePacket(peer);
								break;
							}
							modify_inventory(peer, id_, c_); // gauna itemo kieki 
							if (pInfo(peer)->cheater_settings & Gtps3::SETTINGS_15 && pInfo(peer)->disable_cheater == 0) {
								int count = c_;
								c_ = c_ * -1;
								if (modify_inventory(peer, id_, c_) == 0) {
									packet_(peer, "action|play_sfx\nfile|audio/trash.wav\ndelayMS|0");
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									if (pInfo(peer)->supp != 0 && domain != "https://gold-ps.com/") {
										int item = id_, maxgems = 0, receivegems = 0;
										if (id_ % 2 != 0) item -= 1;
										maxgems = items[item].max_gems2;
										if (items[item].max_gems2 != 0) if (maxgems != 0) for (int i = 0; i < count; i++) receivegems += rand() % maxgems;
										if (items[item].max_gems3 != 0) receivegems = count * items[item].max_gems3;
										if (receivegems != 0) OnSetGems(peer, receivegems);
										p.Insert((items[id_].blockType == BlockTypes::FISH ? (to_string(abs(c_))) + "lb." : to_string(abs(c_))) + " `w" + items[id_].ori_name + "`` recycled, `0" + setGems(receivegems) + "`` gems earned.");
									}
									else p.Insert((items[id_].blockType == BlockTypes::FISH ? (to_string(abs(c_))) + "lb." : to_string(abs(c_))) + " `w" + items[id_].ori_name + "`` trashed.");
									p.CreatePacket(peer);
									break;
								}
							}
							p.Insert("OnDialogRequest");
							if (pInfo(peer)->supp == 0) p.Insert("set_default_color|`o\nadd_label_with_icon|big|`4Trash`` `w" + items[id_].ori_name + "``|left|" + to_string(id_) + "|\nadd_textbox|How many to `4destroy``? (you have "+(items[id_].blockType == BlockTypes::FISH ? "1" : to_string(c_)) + ")|left|\nadd_text_input|count||0|5|\nembed_data|itemID|" + to_string(id_) + "\nend_dialog|trash_item|Cancel|OK|");
							else {
								int item = id_, maxgems = 0, maximum_gems = 0;
								if (id_ % 2 != 0) item -= 1;
								maxgems = items[item].max_gems2;
								if (items[item].max_gems3 != 0) maximum_gems = items[item].max_gems3;
								string recycle_text = "0" + (maxgems == 0 ? "" : "-" + to_string(maxgems)) + "";
								if (maximum_gems != 0) recycle_text = to_string(maximum_gems);
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`4Recycle`` `w" + items[id_].ori_name + "``|left|" + to_string(id_) + "|\nadd_textbox|You will get " + recycle_text + " gems per item.|\nadd_textbox|How many to `4destroy``? (you have " + (items[id_].blockType == BlockTypes::FISH ? "1" : to_string(c_)) + ")|left|\nadd_text_input|count||0|5|\nembed_data|itemID|" + to_string(id_) + "\nend_dialog|trash_item|Cancel|OK|");
							}
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|info") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 4) break;
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							int id_ = atoi(explode("\n", t_[3])[0].c_str());
							if (id_ > items.size() || id_ <= 0) break;
							if (id_ % 2 != 0) p.Insert("set_default_color|`o\nadd_label_with_ele_icon|big|`wAbout " + items[id_].ori_name + "``|left|" + to_string(id_) + "|" + to_string(items[id_ - 1].chi) + "|\nadd_spacer|small|\nadd_textbox|Plant this seed to grow a `0" + items[id_ - 1].ori_name + " Tree.``|left|\nadd_spacer|small|\nadd_textbox|Rarity: `0" + to_string(items[id_].rarity) + "``|left|\nadd_spacer|small|\nend_dialog|continue||OK|\n");
							else {
								string extra_ = "\nadd_textbox|";
								if (id_ == 18) {
									extra_ += "You've punched `w" + to_string(pInfo(peer)->punch_count) + "`` times.";
								} if (items[id_].blockType == BlockTypes::LOCK) {
									extra_ += "A lock makes it so only you (and designated friends) can edit an area.";
								}
								if (items[id_].extra_gems != 0 ) extra_ += "<CR>This item gives " + to_string(items[id_].extra_gems) + "x gems.";
								if (items[id_].r_1 == 0 or items[id_].r_2 == 0) {
									if (items[id_].properties & Property_Untradable) {
									}
									else extra_ += "<CR>This item can't be spliced.";
								}
								else {
									extra_ += "Rarity: `w" + to_string(items[id_].rarity) + "``<CR><CR>To grow, plant a `w" + items[id_ + 1].name + "``.   (Or splice a `w" + items[items[id_].r_1].name + "`` with a `w" + items[items[id_].r_2].name + "``)<CR>";
								} if (items[id_].properties & Property_Dropless or items[id_].rarity == 999) {
									if (items[id_].properties & Property_Untradable) {

									}
									else {
										if (items[id_].r_1 != 0 or items[id_].r_2 != 0) {
										}
										else extra_ += "<CR>`1This item never drops any seeds.``";
									}
								} if (items[id_].properties & Property_Untradable) {
									extra_ += "<CR>`1This item cannot be dropped or traded.``";
								} if (items[id_].properties & Property_AutoPickup) {
									extra_ += "<CR>`1This item can't be destroyed - smashing it will return it to your backpack if you have room!``";
								}
								if (items[id_].properties & Property_MultiFacing && items[id_].properties & Property_Wrenchable) {
									extra_ += "<CR>`1This item can be placed in two directions, depending on the direction you're facing.``";
								}
								else {
									if (items[id_].properties & Property_Wrenchable) {
										extra_ += "<CR>`1This item has special properties you can adjust with the Wrench.``";
									}
									if (items[id_].properties & Property_MultiFacing) {
										extra_ += "<CR>`1This item can be placed in two directions, depending on the direction you're facing.``";
									}
								}
								if (items[id_].properties & Property_NoSelf) {
									extra_ += "<CR>`1This item has no use... by itself.``";
								}
								extra_ += "|left|";
								if (extra_ == "\nadd_textbox||left|") extra_ = "";
								else extra_ = replace_str(extra_, "add_textbox|<CR>", "add_textbox|");

								string extra_ore = "";
								if (id_ == 9386) extra_ore = rare_text;
								if (id_ == 5136) extra_ore = rainbow_text;
								p.Insert("set_default_color|`o\nadd_label_with_ele_icon|big|`wAbout " + items[id_].name + "``|left|" + to_string(id_) + "|" + to_string(items[id_].chi) + "|\nadd_spacer|small|" + ((id_ == 7188 && inventory_contains(peer, 7188) >= 100) || id_ == 8470 ? "\nadd_label_with_icon|small|Convert `$" + a + (id_ == 7188 ? "100" : "1") + "``x " + items[id_].ori_name + " into " + (id_ == 7188 ? "`$1``x " + items[8470].ori_name + "" : "`$100``x " + items[7188].ori_name + "") + "|left|" + to_string(id_) + "|\ntext_scaling_string|100,000OPC|\nadd_spacer|small|\nadd_button_with_icon|convert_" + to_string(id_) + "|`pConvert``|noflags|" + (id_ == 7188 ? "10350" : "9638") + "|\nadd_button_with_icon||END_LIST|noflags|0||" : "") + "\nadd_textbox|" + items[id_].description + "|left|" + (pInfo(peer)->subscriber ? "\nadd_spacer|small|\nadd_textbox|`1This item average price is " + (items[id_].price.size() == 0 ? "unknown" : to_string(item_average2(items[id_].price)) + " World Locks") + "!``|left|" : "") + "" + (extra_ore != "" ? "\nadd_spacer|small|\nadd_textbox|This item also drops:|left|" + extra_ore : "") + "" + (id_ == 8552 ? "\nadd_spacer|small|\nadd_textbox|Angelic Healings: " + to_string(pInfo(peer)->surgery_done) + "|left|" : "") + "\nadd_spacer|small|" + extra_ + "\nadd_spacer|small|\nend_dialog|continue||OK|\n");
							}
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|wrench") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 4) break;
							int netID = atoi(explode("\n", t_[3])[0].c_str());
							if (pInfo(peer)->netID == netID) {
								send_wrench_self(peer);
							}
							else {
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									if (pInfo(currentPeer)->netID == netID and pInfo(currentPeer)->world == pInfo(peer)->world) {
										pInfo(peer)->last_wrenched = pInfo(currentPeer)->tankIDName;
										if (pInfo(peer)->cheater_settings & Gtps3::SETTINGS_14 && pInfo(peer)->disable_cheater == 0) {

											SendCmd(peer, "/pull " + pInfo(peer)->last_wrenched, true);
											break;
										}
										bool already_friends = false, trade_blocked = false, muted = false;
										for (int c_ = 0; c_ < pInfo(peer)->friends.size(); c_++) {
											if (pInfo(peer)->friends[c_].name == pInfo(currentPeer)->tankIDName) {
												already_friends = true;
												if (pInfo(peer)->friends[c_].block_trade)
													trade_blocked = true;
												if (pInfo(peer)->friends[c_].mute)
													muted = true;
												break;
											}
										}
										string name_ = pInfo(peer)->world;
										vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
										if (p != worlds.end()) {
											World* world_ = &worlds[p - worlds.begin()];
											world_->fresh_world = true;
											int x_ = (pInfo(currentPeer)->state == 16 ? (int)pInfo(currentPeer)->x / 32 : round((double)pInfo(currentPeer)->x / 32)), y_ = (int)pInfo(currentPeer)->y / 32;
											if (x_ < 0 or x_ >= world_->max_x or y_ < 0 or y_ >= world_->max_y) {
											}
											else {
												if (world_->blocks[x_ + (y_ * 100)].fg == 1256) pInfo(currentPeer)->hospital_bed = true;
												else pInfo(currentPeer)->hospital_bed = false;
											}
											string msg2 = "";
											 for (int i = 0; i < to_string(pInfo(currentPeer)->level).length(); i++) msg2 += "?";
											string inv_guild = "";
											string extra = "";
											if (pInfo(currentPeer)->guild_id != 0) {
												uint32_t guild_id = pInfo(currentPeer)->guild_id;
												vector<Guild>::iterator find_guild = find_if(guilds.begin(), guilds.end(), [guild_id](const Guild& a) { return a.guild_id == guild_id; });
												if (find_guild != guilds.end()) {
													Guild* guild_information = &guilds[find_guild - guilds.begin()];
													for (GuildMember member_search : guild_information->guild_members) {
														if (to_lower(member_search.member_name) == to_lower(pInfo(currentPeer)->tankIDName)) {
															if (guild_information->guild_mascot[1] == 0 and guild_information->guild_mascot[0] == 0) {
																extra += "\nadd_label_with_icon|small|`9Guild: `2" + guild_information->guild_name + "``|left|5814|\nadd_textbox|`9Rank: `2" + (member_search.role_id == 0 ? "Member" : (member_search.role_id == 1 ? "Elder" : (member_search.role_id == 2 ? "Co-Leader" : "Leader"))) + "``|left|\nadd_spacer|small|";
															}
															else {
																extra += "\nadd_dual_layer_icon_label|small|`9Guild: `2" + guild_information->guild_name + "``|left|" + to_string(guild_information->guild_mascot[1]) + "|" + to_string(guild_information->guild_mascot[0]) + "|1.0|1|\nadd_smalltext|`9Rank: `2" + (member_search.role_id == 0 ? "Member" : (member_search.role_id == 1 ? "Elder" : (member_search.role_id == 2 ? "Co-Leader" : "Leader"))) + "``|left|\nadd_spacer|small|";
															}
															break;
														}
													}
												}
											}
											if (pInfo(peer)->guild_id != 0 and pInfo(currentPeer)->guild_id == 0) {
												uint32_t guild_id = pInfo(peer)->guild_id;
												vector<Guild>::iterator find_guild = find_if(guilds.begin(), guilds.end(), [guild_id](const Guild& a) { return a.guild_id == guild_id; });
												if (find_guild != guilds.end()) {
													Guild* guild_information = &guilds[find_guild - guilds.begin()];
													for (GuildMember member_search : guild_information->guild_members) {
														if (to_lower(member_search.member_name) == to_lower(pInfo(peer)->tankIDName)) {
															if (member_search.role_id >= 1) {
																inv_guild = "\nadd_button|invitetoguild|`2Invite to Guild``|noflags|0|0|";
															}
															break;
														}
													}
												}
											}
											string personalize = (pInfo(currentPeer)->display_age || pInfo(currentPeer)->display_home ? "\nadd_spacer|small|" : "");
											personalize += "\nadd_dual_layer_icon_label|small|`1Gender:`` "+pInfo(currentPeer)->gender + "|left|0|" + (pInfo(currentPeer)->gender == "man" ? "9834" : "9836") + "|1.0|1|";
											if (pInfo(currentPeer)->display_age) {
												time_t s__;
												s__ = time(NULL);
												int days_ = int(s__) / (60 * 60 * 24);
												personalize += "\nadd_label|small|`1Account Age:`` " + to_string(days_ - pInfo(currentPeer)->account_created) + " days|left\nadd_spacer|small|";
											}
											if (pInfo(currentPeer)->display_home) {
												if (pInfo(currentPeer)->home_world != "") {
													personalize += "\nadd_label|small|`1Home World:``|left\nadd_button|visit_home_world|`$Visit " + pInfo(currentPeer)->home_world + "``|noflags|0|0|";
													pInfo(peer)->last_home_world = pInfo(currentPeer)->home_world;
												}
											}
											string surgery = "\nadd_spacer|small|\nadd_button|start_surg|`$Perform Surgery``|noflags|0|0|\nadd_smalltext|Surgeon Skill: " + to_string(pInfo(peer)->surgery_skill) + "|left|";

											if (has_playmod2(pInfo(currentPeer), 87)) surgery = "\nadd_spacer|small|\nadd_textbox|Recovering from surgery...|left|";
											if (pInfo(currentPeer)->hospital_bed == false) surgery = "";
											gamepacket_t p;
											p.Insert("OnDialogRequest");
											p.Insert("embed_data|netID|" + to_string(pInfo(peer)->netID) + "\nset_default_color|`o\nadd_label_with_icon|big|" + get_player_nick(currentPeer) + "```` `0(```2" + (pInfo(currentPeer)->dev ? (pInfo(currentPeer)->tmod == 1 ? to_string(pInfo(currentPeer)->level) : msg2) : to_string(pInfo(currentPeer)->level)) + "```0)``|left|18|" + personalize + surgery + "\nembed_data|netID|" + to_string(netID) + "\nadd_spacer|small|" + extra + (trade_blocked ? "\nadd_button||`4Trade Blocked``|disabled|||" : "\nadd_button|trade|`wTrade``|noflags|0|0|") + "\nadd_textbox|(No Battle Leash equipped)|left|\nadd_textbox|Your opponent needs a valid license to battle!|left|" + (to_lower(world_->owner_name) == to_lower(pInfo(peer)->tankIDName) or (/*guild_access(peer, world_->guild_id) or */ find(world_->admins.begin(), world_->admins.end(), to_lower(pInfo(peer)->tankIDName)) != world_->admins.end()) or pInfo(peer)->dev + pInfo(peer)->mod > 0 ? "\nadd_button|kick|`4Kick``|noflags|0|0|\nadd_button|pull|`5Pull``|noflags|0|0|\nadd_button|worldban|`4World Ban``|noflags|0|0|" : "") + (pInfo(peer)->mod == 1 || pInfo(peer)->dev == 1 ? "\nadd_button|punish_view|`5Punish/View``|noflags|0|0|" : "") + inv_guild + (!already_friends ? "\nadd_button|friend_add|`wAdd as friend``|noflags|0|0|" : "") + (muted ? "\nadd_button|unmute_player|`wUnmute``|noflags|0|0|" : (already_friends ? "\nadd_button|mute_player|`wMute``|noflags|0|0|" : "")) + ""/*"\nadd_button|ignore_player|`wIgnore Player``|noflags|0|0|\nadd_button|report_player|`wReport Player``|noflags|0|0|"*/"\nadd_button|view_worn_clothes|`wView worn clothes|noflags|0|0|\nadd_spacer|small|\nend_dialog|popup||Continue|\nadd_quick_exit|");
											p.CreatePacket(peer);
										}
										break;
									}
								}
							}
							break;
						}
						/*
						else if (cch == "action|warp_player_into_halloween_world\n") {
							string world = "GROWGANOTH";
							join_world(peer, world);
							break;
						}
						else if (cch == "action|warp_salesman\n") {
						string world = "LOCKETOWN";
						join_world(peer, world);
						break;
						}
						else if (cch == "action|warp_player_into_halloween_world\n") {
						string world = "GROWGANOTH";
						join_world(peer, world);
						break;
						}
						else if (cch == "action|halloweenquestpopup\n") {
						gamepacket_t p(550);
						p.Insert("OnDialogRequest");
						p.Insert("add_label_with_icon|big|Trick or Treat Tasks|left|12826||\nadd_spacer|small|\nadd_label|small|Spend this Halloween season completing tasks all around Growtopia to get candy. Find me in the GROWGANOTH world and I'll sell you some sweet gifts in exchange for candy.|left\nadd_spacer|small|\nadd_label|small|Quick Treats:|left\nadd_spacer|small|\nadd_label|small|Complete these tasks to make some quick candy!|left\nadd_spacer|small|\nadd_label_with_icon|small|Sacrifice Dark King's Offering (" + to_string(pInfo(peer)->halloween_task_1) + "/1)|left|10328|state:enabled;|\nadd_custom_margin|x:60;y:10|\nadd_smalltext|Worth 5 Candy per sacrifice|left|\nadd_button|halloween_claim_task_reward_1|" + (pInfo(peer)->halloween_task_1 > 0 ? "Task completed|noflags" : "Task Incomplete|off") + "|0|0|\nadd_custom_margin|x:-60;y:-10|\nadd_custom_margin|x:60;y:0|\nadd_custom_margin|x:-60;y:0|\nadd_spacer|small|\nadd_label_with_icon|small|Purchase a Dark Ticket (" + to_string(pInfo(peer)->halloween_task_2) + "/1)|left|9018|state:enabled;|\nadd_custom_margin|x:60;y:10|\nadd_smalltext|Worth 1 Candy per pair of tickets purchased|left|\nadd_button|halloween_claim_task_reward_2|" + (pInfo(peer)->halloween_task_2 > 0 ? "Task completed|noflags" : "Task Incomplete|off") + "|0|0|\nadd_custom_margin|x:-60;y:-10|\nadd_custom_margin|x:60;y:0|\nadd_custom_margin|x:-60;y:0|\nadd_spacer|small|\nadd_label_with_icon|small|Purchase a Gift of Growganoth (" + to_string(pInfo(peer)->halloween_task_3) + "/1)|left|10386|state:enabled;|\nadd_custom_margin|x:60;y:10|\nadd_smalltext|Worth 2 Candy per purchase|left|\nadd_button|halloween_claim_task_reward_3|" + (pInfo(peer)->halloween_task_3 > 0 ? "Task completed|noflags" : "Task Incomplete|off") + "|0|0|\nadd_custom_margin|x:-60;y:-10|\nadd_custom_margin|x:60;y:0|\nadd_custom_margin|x:-60;y:0|\nadd_spacer|small|\nadd_label_with_icon|small|Purchase Weather Machine - Dark Mountains (" + to_string(pInfo(peer)->halloween_task_4) + "/1)|left|12408|state:enabled;|\nadd_custom_margin|x:60;y:10|\nadd_smalltext|Worth 10 Candy per purchase|left|\nadd_button|halloween_claim_task_reward_4|" + (pInfo(peer)->halloween_task_4 > 0 ? "Task completed|noflags" : "Task Incomplete|off") + "|0|0|\nadd_custom_margin|x:-60;y:-10|\nadd_custom_margin|x:60;y:0|\nadd_custom_margin|x:-60;y:0|\nadd_spacer|small|\nadd_spacer|small|\nadd_label|small|Trick Crazy Jim:|left\nadd_spacer|small|\nadd_label|small|Let's make a fool of Jim by dressing up for the season while doing his tasks! In case you forget, his phone number is 12345. These tasks are reset every day. `5You MUST dress up wearing the required item while completing these tasks otherwise you don't get any candy``. Also, it's way more fun to dress up in general. The amount of candy given is based on your awesomeness level.|left\nadd_spacer|small|\ndud_checkbox||Complete Daily Quest wearing a " + items[halloween_quest].ori_name + "|" + (pInfo(peer)->halloween_ptask_1 > 1 ? "1" : "0") + "|useLabel\nadd_custom_margin|x:65;y:-30|\nadd_custom_margin|x:-65;y:30|\nadd_custom_margin|x:60;y:-20|\nadd_smalltext|Worth 20 Candy|left|\nadd_button|halloween_claim_task_reward_5|" + (pInfo(peer)->halloween_ptask_1 == 1 ? "Task completed|noflags" : pInfo(peer)->halloween_ptask_1 > 1 ? "Task claimed|off" : "Task Incomplete|off") + "|0|0|\nadd_custom_margin|x:-60;y:20|\nadd_custom_margin|x:60;y:0|\nadd_custom_margin|x:-60;y:0|\nadd_spacer|small|\ndud_checkbox||Complete Tomb of Growganoth parkour wearing a " + items[halloween_quest].ori_name + "|" + (pInfo(peer)->halloween_ptask_2 > 1 ? "1" : "0") + "|useLabel\nadd_custom_margin|x:65;y:-30|\nadd_custom_margin|x:-65;y:30|\nadd_custom_margin|x:60;y:-20|\nadd_smalltext|Worth 1 Candy|left|\nadd_button|halloween_claim_task_reward_6|" + (pInfo(peer)->halloween_ptask_2 == 1 ? "Task completed|noflags" : pInfo(peer)->halloween_ptask_2 > 1 ? "Task claimed|off" : "Task Incomplete|off") + "|0|0|\nadd_custom_margin|x:-60;y:20|\nadd_custom_margin|x:60;y:0|\nadd_custom_margin|x:-60;y:0|\nadd_spacer|small|\ndud_checkbox||Complete Halloween Dark King's Offering quest wearing a " + items[halloween_quest].ori_name + "|" + (pInfo(peer)->halloween_ptask_3 > 1 ? "1" : "0") + "|useLabel\nadd_custom_margin|x:65;y:-30|\nadd_custom_margin|x:-65;y:30|\nadd_custom_margin|x:60;y:-20|\nadd_smalltext|Worth 1 Candy|left|\nadd_button|halloween_claim_task_reward_7|" + (pInfo(peer)->halloween_ptask_3 == 1 ? "Task completed|noflags" : pInfo(peer)->halloween_ptask_3 > 1 ? "Task claimed|off" : "Task Incomplete|off") + "|0|0|\nadd_custom_margin|x:-60;y:20|\nadd_custom_margin|x:60;y:0|\nadd_custom_margin|x:-60;y:0|\nadd_spacer|small|\ndud_checkbox||Sacrifice Dark King's Offering wearing a " + items[halloween_quest].ori_name + "|" + (pInfo(peer)->halloween_ptask_4 > 1 ? "1" : "0") + "|useLabel\nadd_custom_margin|x:65;y:-30|\nadd_custom_margin|x:-65;y:30|\nadd_custom_margin|x:60;y:-20|\nadd_smalltext|Worth 1 Candy|left|\nadd_button|halloween_claim_task_reward_8|" + (pInfo(peer)->halloween_ptask_4 == 1 ? "Task completed|noflags" : pInfo(peer)->halloween_ptask_4 > 1 ? "Task claimed|off" : "Task Incomplete|off") + "|0|0|\nadd_custom_margin|x:-60;y:20|\nadd_custom_margin|x:60;y:0|\nadd_custom_margin|x:-60;y:0|\nadd_spacer|small|\ndud_checkbox||Get Boney Block from Growganoth wearing a " + items[halloween_quest].ori_name + " (" + (pInfo(peer)->halloween_ptask_5 > 1 ? "1" : "0") + "/1)|" + (pInfo(peer)->halloween_ptask_5 > 1 ? "1" : "0") + "|useLabel\nadd_custom_margin|x:65;y:-30|\nadd_custom_margin|x:-65;y:30|\nadd_custom_margin|x:60;y:-20|\nadd_smalltext|Worth 20 Candy|left|\nadd_button|halloween_claim_task_reward_9|" + (pInfo(peer)->halloween_ptask_5 == 1 ? "Task completed|noflags" : pInfo(peer)->halloween_ptask_5 > 1 ? "Task claimed|off" : "Task Incomplete|off") + "|0|0|\nadd_custom_margin|x:-60;y:20|\nadd_custom_margin|x:60;y:0|\nadd_custom_margin|x:-60;y:0|\nadd_spacer|small|\nadd_spacer|small|\nadd_label_with_icon|big|`wSpooky Tips:``|left|1236|\nadd_spacer|small|\nadd_smalltext|Another way to get some spooky prizes is to go to the Maw of Growganoth by entering the world GROWGANOTH. Your goal is to jump to the Maw at the top and reach portal at the center of the world.|left|\nadd_image_button|warp_player_into_halloween_world|interface/large/gui_halloween_banner1.rttex|bannerlayout|||\nadd_spacer|small|\nadd_smalltext|You can also get spooky version of specific items by dropping them into the maw.|left|\nadd_spacer|small|\nadd_smalltext|Venture through the Tomb of Growganoth during the Halloween season by purchasing a dark ticket from the store!|left|\nadd_image_button|open_store|interface/large/gui_halloween_banner2.rttex|bannerlayout|||\nadd_spacer|small|\nadd_smalltext|Consuming the ticket will teleport you to one of the many worlds that are the Tomb of Growganoth. |left|\nadd_quick_exit|\nend_dialog|halloween_tasks_popup_handle|Close|");
						p.CreatePacket(peer);
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|halloween_tasks_popup_handle\n") != string::npos) {
						vector<string> t_ = explode("|", cch);
						if (t_.size() < 3) break;
						string button = explode("\n", t_[3])[0].c_str();
						if (button == "warp_player_into_halloween_world") {
							string world = "GROWGANOTH";
							join_world(peer, world);
						}
						else if (button == "open_store") shop_tab(peer, "tab1_dark_ticket");
						else {
							if (button.find("halloween_claim_task_reward_") != string::npos) {
								int buttons = atoi(button.substr(28, button.length() - 28).c_str());
								int give_ = 0;
								if (buttons == 1 && pInfo(peer)->halloween_task_1 >0) give_ = 5;
								else if (buttons == 2 && pInfo(peer)->halloween_task_2 >0) give_ = 1;
								else if (buttons == 3 && pInfo(peer)->halloween_task_3 >0) give_ = 2;
								else if (buttons == 4 && pInfo(peer)->halloween_task_4 >0) give_ = 10;
								else if (buttons == 5 && pInfo(peer)->halloween_ptask_1 == 1) give_ = 20;
								else if (buttons == 6 && pInfo(peer)->halloween_ptask_2 == 1) give_ = 1;
								else if (buttons == 7 && pInfo(peer)->halloween_ptask_3 == 1) give_ = 1;
								else if (buttons == 8 && pInfo(peer)->halloween_ptask_4 == 1) give_ = 1;
								else if (buttons == 9 && pInfo(peer)->halloween_ptask_5 == 1)give_ = 20;
								if (give_ != 0) {
									int give_count = give_;
									gamepacket_t p;
									p.Insert("OnTalkBubble");
									p.Insert(pInfo(peer)->netID);
									if (modify_inventory(peer, 12766, give_count) == 0) {
										if (buttons == 1) pInfo(peer)->halloween_task_1--;
										else if (buttons == 2)  pInfo(peer)->halloween_task_2--;
										else if (buttons == 3) pInfo(peer)->halloween_task_3--;
										else if (buttons == 4)  pInfo(peer)->halloween_task_4--;
										else if (buttons == 5) pInfo(peer)->halloween_ptask_1 = 2;
										else if (buttons == 6)  pInfo(peer)->halloween_ptask_2 = 2;
										else if (buttons == 7) pInfo(peer)->halloween_ptask_3 = 2;
										else if (buttons == 8) pInfo(peer)->halloween_ptask_4 = 2;
										else if (buttons == 9)  pInfo(peer)->halloween_ptask_5 = 2;
										packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										p.Insert("You claimed your reward and received `2" + to_string(give_) + "`` Halloween Candy.");
										PlayerMoving data_{};
										data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
										BYTE* raw = packPlayerMoving(&data_);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw;
									}
									else p.Insert("No inventory space.");
									p.CreatePacket(peer);
								}
							}
						}
						break;
						}*/
						else if (cch == "action|eventmenu\n") {
						string event_text = "", prize = "";
						if (top_basher.size() == 0 && can_event) {
							event_text = "The Event has ended! The next event will start soon.";
							if (pInfo(peer)->participated == event_item) {
								if (pInfo(peer)->guild_id != 0) {
									vector<pair<int, string>>::iterator p = find_if(top_guild_winners.begin(), top_guild_winners.end(), [&](const pair < int, string>& element) { return element.second == to_string(pInfo(peer)->guild_id); });
									if (p != top_guild_winners.end()) prize += "\nadd_smalltext|`2Guild Event: Thanks for participating, your Guild was top #" + to_string(top_guild_winners[p - top_guild_winners.begin()].first) + ":``|\nadd_button|claim_event_guild|`0Claim reward``|noflags|0|0|\nadd_spacer|";
								}
							}
								vector<pair<int, string>>::iterator p = find_if(top_basher_winners.begin(), top_basher_winners.end(), [&](const pair < int, string>& element) { return element.second == pInfo(peer)->tankIDName; });
								if (p != top_basher_winners.end()) prize += "\nadd_smalltext|`2Personal Event: Thanks for participating, you were top #" + to_string(top_basher_winners[p - top_basher_winners.begin()].first) + ":``|\nadd_button|claim_event|`0Claim reward``|noflags|0|0|\nadd_spacer|";
						}
						else {
							event_text = "The Event " + items[event_item].hand_scythe_text + " has started!";
							if (pInfo(peer)->last_personal_update + 300000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
								sort(top_basher.begin(), top_basher.end());
								reverse(top_basher.begin(), top_basher.end());
								pInfo(peer)->personal_event.clear();
								pInfo(peer)->guild_event.clear();
								pInfo(peer)->last_personal_update = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
								vector<pair<long long int, string>>::iterator pa = find_if(top_basher.begin(), top_basher.end(), [&](const pair < long long int, string>& element) { return element.second == pInfo(peer)->tankIDName; });
								if (pa != top_basher.end()) pInfo(peer)->personal_event = "\nadd_smalltext|Personal Event Rank: " + to_string(distance(top_basher.begin(), pa) + 1) + "    Contribution: " + setGems_(top_basher[pa - top_basher.begin()].first) + "|";

								if (pInfo(peer)->guild_id != 0) {
									sort(top_guild.begin(), top_guild.end());
									reverse(top_guild.begin(), top_guild.end());
									vector<pair<long long int, string>>::iterator pa2 = find_if(top_guild.begin(), top_guild.end(), [&](const pair < long long int, string>& element) { return element.second == to_string(pInfo(peer)->guild_id); });
									if (pa2 != top_guild.end()) pInfo(peer)->guild_event = "\nadd_smalltext|Guild Event Rank: " + to_string(distance(top_guild.begin(), pa2) + 1) + "    Contribution: " + setGems_(top_guild[pa2 - top_guild.begin()].first) + "|";

								}
							}
						}
						gamepacket_t p;
						p.Insert("OnDialogRequest");
						p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0"+(top_basher.size() == 0 && can_event ? "Grow Events" : items[event_item].hand_scythe_text) + "``|left|6012|\nadd_textbox|" + event_text + "|left|"+(top_basher.size() == 0 && can_event ? "" : "\nadd_smalltext|`5Rules:``|\nadd_smalltext|" + items[event_item].description + "|") + "\nadd_textbox|`8Remember:`` Any guild members added when an event is active will not contribute any Points!|left|\nadd_textbox|Make sure to claim ALL your Grow Event rewards.|left|\nadd_spacer|small|\nadd_spacer|small|\nadd_smalltext|" + (top_basher.size() == 0 && can_event ? "Next event starts in: `2" + to_playmod_time(next_event - time(nullptr)) + "``" : "Event Time remaining: `2" + to_playmod_time(current_event - time(nullptr)) + "``") + "``|\nadd_spacer|small|"+ prize + pInfo(peer)->personal_event + pInfo(peer)->guild_event +"\nadd_spacer|small|" + (top_basher.size() == 0 && can_event ? "\nadd_button|old_leaderboard_guild|`0Past Guild Event Leaderboard``|noflags|0|0|\nadd_button|old_leaderboard|`0Past Personal Event Leaderboard``|noflags|0|0|" : "\nadd_button|guild_event|`0Guild Event Leaderboard``|noflags|0|0|\nadd_button|personal_event|`0Personal Event Leaderboard``|noflags|0|0|") + "\nadd_button|event_rewards|`0Personal Event Rewards``|noflags|0|0|\nadd_button|event_rewards_guild|`0Guild Event Rewards``|noflags|0|0|\nend_dialog|zz|Close||\nadd_quick_exit|");
						p.CreatePacket(peer);
						break;
						}
						else if (cch == "action|dialog_return\ndialog_name|zz\nbuttonClicked|personal_event\n\n") {
						gamepacket_t p;
						p.Insert("OnDialogRequest");
						p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Top Players - " + items[event_item].hand_scythe_text + "``|left|6012|\nadd_spacer|\nadd_smalltext|" + items[event_item].description + "|\nadd_spacer|" + top_basher_list + "\nadd_spacer|\nadd_textbox|Event Time remaining: `2" + to_playmod_time(current_event - time(nullptr)) + "``|\nadd_spacer|" + pInfo(peer)->personal_event + "\nadd_spacer|\nadd_button|old_leaderboard|`0Past Event Leaderboard``|noflags|0|0|\nadd_button|event_rewards|`0Event Rewards``|noflags|0|0|\nend_dialog|zz|Close||\n");
						p.CreatePacket(peer);
						break;
						}
						else if (cch == "action|dialog_return\ndialog_name|zz\nbuttonClicked|event_rewards\n\n") {
						gamepacket_t p;
						p.Insert("OnDialogRequest");
						p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Personal Event Rewards``|left|6012|\nadd_spacer|small|\nadd_textbox|You can claim these prizes if you end up in `5TOP 1-10``:|left|\nadd_spacer|small|\nadd_textbox|TOP #1|left|\nadd_label_with_icon|small| Summer Event Player Medal: Gold|left|6094|\nadd_label_with_icon|small| 50 Growtoken|left|1486|\nadd_label_with_icon|small| 2,000 Premium World locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP #2|left|\nadd_label_with_icon|small| Summer Event Player Medal: Silver|left|6132|\nadd_label_with_icon|small| 25 Growtoken|left|1486|\nadd_label_with_icon|small| 1,500 Premium World locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP #3|left|\nadd_label_with_icon|small| Summer Event Player Medal: Bronze|left|6130|\nadd_label_with_icon|small| 15 Growtoken|left|1486|\nadd_label_with_icon|small| 1,000 Premium World locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP <#4-#10>|left|\nadd_label_with_icon|small| 50 Premium World locks|left|242|\nend_dialog|zz|Close||\n");
						p.CreatePacket(peer);
						break;
						}
						else if (cch == "action|dialog_return\ndialog_name|zz\nbuttonClicked|event_rewards_guild\n\n") {
						gamepacket_t p;
						p.Insert("OnDialogRequest");
						p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Guild Event Rewards``|left|6012|\nadd_spacer|small|\nadd_textbox|You can claim these prizes per guild member if your guild ends up in `5TOP 1-10``:|left|\nadd_spacer|small|\nadd_textbox|TOP #1|left|\nadd_label_with_icon|small| Gold Guild Chest|left|5948|\nadd_label_with_icon|small| 25 Growtoken|left|1486|\nadd_label_with_icon|small| 250 Premium World locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP #2|left|\nadd_label_with_icon|small| Silver Guild Chest|left|5950|\nadd_label_with_icon|small| 15 Growtoken|left|1486|\nadd_label_with_icon|small| 150 Premium World locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP #3|left|\nadd_label_with_icon|small| Bronze Guild Chest|left|5952|\nadd_label_with_icon|small| 10 Growtoken|left|1486|\nadd_label_with_icon|small| 100 Premium World locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP <#4-#10>|left|\nadd_label_with_icon|small| 50 Premium World locks|left|242|\nend_dialog|zz|Close||\n");
						p.CreatePacket(peer);
						break;
						}
						/*
						else if (cch == "action|dialog_return\ndialog_name|popup\nbuttonClicked|event_rewards\n\n") {
						gamepacket_t p;
						p.Insert("OnDialogRequest");
						p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Event Rewards``|left|6012|\nadd_spacer|small|\nadd_textbox|You can claim these prizes if you end up in `5TOP 1-10`` in our event:|left|\nadd_spacer|small|\nadd_textbox|TOP #1|left|\nadd_label_with_icon|small| "+items[7528 + (pInfo(peer)->balloon_faction * 6)].ori_name + "|left|" + to_string(7528 + (pInfo(peer)->balloon_faction * 6)) + "|\nadd_label_with_icon|small| 50 Growtoken|left|1486|\nadd_label_with_icon|small| 2,000 Premium World locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP #2|left|\nadd_label_with_icon|small| " + items[7530 + (pInfo(peer)->balloon_faction * 6)].ori_name + "|left|" + to_string(7530 + (pInfo(peer)->balloon_faction * 6)) + "|\nadd_label_with_icon|small| 25 Growtoken|left|1486|\nadd_label_with_icon|small| 1,500 Premium World locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP #3|left|\nadd_label_with_icon|small| " + items[7532 + (pInfo(peer)->balloon_faction * 6)].ori_name + "|left|" + to_string(7532 + (pInfo(peer)->balloon_faction * 6)) + "|\nadd_label_with_icon|small| 15 Growtoken|left|1486|\nadd_label_with_icon|small| 1,000 Premium World locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP <#4-#10>|left|\nadd_label_with_icon|small| 50 Premium World locks|left|242|\nend_dialog|zz|Close||\n");
						p.CreatePacket(peer);
						}*/
						/*
						else if (cch.find("action|dialog_return\ndialog_name|popup\nbuttonClicked|claim_event") != string::npos) {
						string find = pInfo(peer)->tankIDName;
						vector<pair<int, string>>::iterator pa = find_if(top_balloon_winners.begin(), top_balloon_winners.end(), [find](const pair < int, string>& element) { return element.second == find; });
						if (pa != top_balloon_winners.end()) {
							int place = top_balloon_winners[pa - top_balloon_winners.begin()].first;
							int give_premium = (place == 1 ? 2000 : (place == 2 ? 1500 : (place == 3 ? 1000 : 50))), give_token = (place == 1 ? 50 : (place == 2 ? 25 : (place == 3 ? 15 : 0)));
							pInfo(peer)->gtwl += give_premium;
							gamepacket_t p, p2;
							p.Insert("OnAddNotification"), p.Insert("interface/guide_arrow.rttex"), p.Insert("You claimed your event reward!"), p.Insert("audio/piano_nice.wav.wav"), p.Insert(0), p.CreatePacket(peer);
							p2.Insert("OnConsoleMessage"), p2.Insert("Thanks for participating in our event, you ended up being #" + to_string(place) + "!"), p2.CreatePacket(peer);
							if (give_token != 0) {
								modify_inventory(peer, 1486, give_token);
								if (pInfo(peer)->lwiz_step == 6) add_lwiz_points(peer, 50);
							}
							int give_trophy = 1;
							if (place == 1) {
								modify_inventory(peer, 7528 + (pInfo(peer)->balloon_faction * 6), give_trophy);
							}
							else if (place == 2) {
								modify_inventory(peer, 7530 + (pInfo(peer)->balloon_faction * 6), give_trophy);
							}
							else if (place == 3) {
								modify_inventory(peer, 7532 + (pInfo(peer)->balloon_faction * 6), give_trophy);
							}
							top_balloon_winners.erase(pa);
						}
						break;
						}*/
						else if (cch == "action|dialog_return\ndialog_name|zz\nbuttonClicked|old_leaderboard\n\n") {
						gamepacket_t p;
						p.Insert("OnDialogRequest");
						p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0" + items[old_event_item].hand_scythe_text + " Winners``|left|6012|\nadd_spacer|\nadd_smalltext|" + items[old_event_item].description + "|\nadd_spacer|" + ( top_old_winners) + "\nadd_spacer|\nadd_textbox|Get ready for the next event, next event will be `2" + items[get_next_event()].hand_scythe_text + "`` (`5starting soon``).|\nend_dialog|zz|Close||\n");
						p.CreatePacket(peer);
						break;
						}
						else if (cch == "action|dialog_return\ndialog_name|zz\nbuttonClicked|claim_event\n\n") {
						string find = pInfo(peer)->tankIDName;
						vector<pair<int, string>>::iterator pa = find_if(top_basher_winners.begin(), top_basher_winners.end(), [find](const pair < int, string>& element) { return element.second == find; });
						if (pa != top_basher_winners.end()) {
							int place = top_basher_winners[pa - top_basher_winners.begin()].first;
							int give_premium = (place == 1 ? 2000 : (place == 2 ? 1500 : (place == 3 ? 1000 : 50))), give_token = (place == 1 ? 50 : (place == 2 ? 25 : (place == 3 ? 15 : 0))), give_medal = (place == 1 ? 6094 : (place == 2 ? 6132 : (place == 3 ? 6130 : 0))), give_count = 1;
							pInfo(peer)->gtwl += give_premium;
							gamepacket_t p, p2;
							p.Insert("OnAddNotification"), p.Insert("interface/guide_arrow.rttex"), p.Insert("You claimed your Personal Event reward!"), p.Insert("audio/piano_nice.wav.wav"), p.Insert(0), p.CreatePacket(peer);
							p2.Insert("OnConsoleMessage"), p2.Insert("Thanks for participating in our event, you ended up being #" + to_string(place) + " (Your prize: `5" + setGems(give_premium) + " Premium GTPS World Locks``" + (give_medal != 0 ? ", `51 " + items[give_medal].ori_name + "``" : "") + "" + (give_token != 0 ? ", `5" + to_string(give_token) + " Growtoken``" : "") + ""), p2.CreatePacket(peer);
							if (give_token != 0) {
								if (pInfo(peer)->lwiz_step == 6) add_lwiz_points(peer, give_token);
								modify_inventory(peer, 1486, give_token);
							}
							if (give_medal != 0) modify_inventory(peer, give_medal, give_count);
							top_basher_winners.erase(pa);
						}
						break;
						}
						else if (cch == "action|dialog_return\ndialog_name|zz\nbuttonClicked|claim_event_guild\n\n") {
						if (pInfo(peer)->participated == event_item && pInfo(peer)->guild_id != 0) {
							vector<pair<int, string>>::iterator pa = find_if(top_guild_winners.begin(), top_guild_winners.end(), [&](const pair < int, string>& element) { return element.second == to_string(pInfo(peer)->guild_id); });
							if (pa != top_guild_winners.end()) {
								pInfo(peer)->participated = 0;
								int place = top_guild_winners[pa - top_guild_winners.begin()].first, give_premium = (place == 1 ? 250 : (place == 2 ? 150 : (place == 3 ? 100 : 50))), give_token = (place == 1 ? 25 : (place == 2 ? 15 : (place == 3 ? 10 : 0))), give_medal = (place == 1 ? 5948 : (place == 2 ? 5950 : (place == 3 ? 5952 : 0))), give_count = 1;
								pInfo(peer)->gtwl += give_premium;
								gamepacket_t p, p2;
								p.Insert("OnAddNotification"), p.Insert("interface/guide_arrow.rttex"), p.Insert("You claimed your Guild Event reward!"), p.Insert("audio/piano_nice.wav.wav"), p.Insert(0), p.CreatePacket(peer);
								p2.Insert("OnConsoleMessage"), p2.Insert("Thanks for participating in our Guild Event, your guild ended up being #" + to_string(place) + " (Your prize: `5" + setGems(give_premium) + " Premium GTPS World Locks``" + (give_medal != 0 ? ", `51 " + items[give_medal].ori_name + "``" : "") + "" + (give_token != 0 ? ", `5" + to_string(give_token) + " Growtoken``" : "") + ""), p2.CreatePacket(peer);
								if (give_token != 0) {
									if (pInfo(peer)->lwiz_step == 6) add_lwiz_points(peer, give_token);
									modify_inventory(peer, 1486, give_token);
								}
								if (give_medal != 0) modify_inventory(peer, give_medal, give_count);
							}
						}
						break;
						}
						/*
						else if (cch == "action|battlepasspopup\n") {
							gamepacket_t p(550);
							p.Insert("OnDialogRequest");
							int growpassid = 6124;
							if (today_day == pInfo(peer)->gd) growpassid = 6292;
							if (pInfo(peer)->gp == 1) p.Insert("set_default_color|`o\nadd_label_with_icon|big|Grow Pass Rewards|left|9222|\nadd_smalltext|`9You can claim your daily reward everyday here.``|left|\nadd_button_with_icon|claim_reward||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|claim_reward||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|claim_reward||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|claim_reward||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|claim_reward||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon||END_LIST|noflags|0||\nadd_spacer|small|\nend_dialog|worlds_list||Back|\nadd_quick_exit|\n");
							else p.Insert("set_default_color|`o\nadd_label_with_icon|big|Grow Pass Rewards|left|9222|\nadd_button|deposit|`2Purchase``|noflags|0|0|\nadd_smalltext|`4You must purchase the Grow Pass role to claim your prize!``|left|\nadd_button_with_icon|||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon||END_LIST|noflags|0||\nadd_spacer|small|\nend_dialog|||Back|\nadd_quick_exit|\n");
							p.CreatePacket(peer);
							break;
						}*/
						else if (cch == "action|top_wls\n") {
							string contribute = "";
							if (wls_event_time - time(nullptr) <= 0) {
								if (pInfo(peer)->received_recycle_prize == false) {
									int prize = 0;
									for (uint16_t i = 0; i < top_wls.size(); i++) {
										if (to_lower(top_wls[i].second) == to_lower(pInfo(peer)->tankIDName)) {
											prize = i + 1;
											break;
										}
									}
									contribute = "`2Received ";
									if (prize == 1) {
										contribute += "developer role";
										pInfo(peer)->received_recycle_prize = true;
										pInfo(peer)->dev = 1;
									}
									else if (prize == 2 || prize == 3 || prize == 4 || prize == 5) {
										int get_him = 1, prized = 0;
										if (prize == 2) prized = 10684;
										else if (prize == 3) prized = 10940;
										else if (prize == 4) prized = 9774;
										else if (prize == 5) prized = 9906;
										if (modify_inventory(peer, prized, get_him) == 0) {
											pInfo(peer)->received_recycle_prize = true;
											contribute += items[prized].ori_name;
										}
										else {
											gamepacket_t p;
											p.Insert("OnConsoleMessage"), p.Insert("No inventory space."), p.CreatePacket(peer);
											contribute = "";
										}
									}
									else {
										int get_wls = 0;
										if (prize == 6) get_wls = 9999;
										else if (prize == 7) get_wls = 8999;
										else if (prize == 8) get_wls = 7999;
										else if (prize == 9) get_wls = 6999;
										else if (prize == 10) get_wls = 5999;
										else if (prize == 11) get_wls = 4999;
										else if (prize == 12) get_wls = 3999;
										else if (prize == 13) get_wls = 2999;
										else if (prize == 14) get_wls = 1999;
										else if (prize == 15) get_wls = 999;
										else if (prize > 15 && prize <= 25) get_wls = 450;
										else if (prize > 25 && prize <= 50) get_wls = 200;
										else if (prize > 50 && prize <= 100) get_wls = 50;
										contribute += setGems(get_wls) + " Premium World Locks";
										pInfo(peer)->received_recycle_prize = true;
										pInfo(peer)->gtwl += get_wls;
									}
									if (not contribute.empty()) contribute += "!``";
								}
							}
							else {
								vector<pair<long long int, string>>::iterator pa2 = find_if(top_wls.begin(), top_wls.end(), [&](const pair < long long int, string>& element) { return to_lower(element.second) == to_lower(pInfo(peer)->tankIDName); });
								if (pa2 != top_wls.end()) contribute = "Contribution: " + setGems_(top_wls[pa2 - top_wls.begin()].first);
							}
							gamepacket_t p(500);
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0World Locks - Recycle Event``|left|242|\nadd_spacer|\nadd_smalltext|Hey, thanks for participating in our World Locks Recycle Event! By participating you will help the server fight with economy and inflation of World Locks! Of course we added `2extremely`` rare prizes for the top contributors!|\nadd_spacer|\nadd_smalltext|Total World Locks recycled `2" + setGems_(total_wls_recycled) + "``|\nadd_spacer|" + top_wls_list + "\nadd_spacer|\nadd_textbox|"+(wls_event_time - time(nullptr) <= 0 ?"The event is over!" :"Event Time remaining: `2" + to_playmod_time(wls_event_time - time(nullptr)) + "``") + "|\nadd_spacer|"+(not contribute.empty() ?"\nadd_smalltext|" + contribute + "|" : "") + "\nadd_spacer|" + (wls_event_time - time(nullptr) <= 0 ? "" : "\nadd_button|event_recycle|`0Recycle``|noflags|0|0|") + "\nadd_button|event_rewards_topwls|`0Event Rewards``|noflags|0|0|\nend_dialog|wls|Close||\nadd_quick_exit|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|wls\nbuttonClicked|event_recycle\n\n") {
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0World Locks - Recycle Event``|left|242|\nadd_smalltext|`4How many to recycle? (minimum 100)``|left|\nadd_textbox|You have " + to_string(get_wls(peer, true)) + " World Locks.|left|\nadd_text_input|howmuch|||7|\nadd_spacer|\nadd_button|recycle|`9Recycle``|noflags|0|0|\nend_dialog|wls|Close||\nadd_quick_exit|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|wls\nbuttonClicked|event_rewards_topwls\n\n") {
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0World Locks - Recycle Event``|left|242|\nadd_spacer|small|\nadd_textbox|You can claim these prizes once the event is done - `5TOP 1-100``:|left|\nadd_spacer|small|\nadd_textbox|TOP #1|left|\nadd_label_with_icon|small| Developer Role|left|7074|\nadd_spacer|small|\nadd_textbox|TOP #2|left|\nadd_label_with_icon|small| Golden Legendary Wings|left|10684|\nadd_spacer|small|\nadd_textbox|TOP #3|left|\nadd_label_with_icon|small|  Rosewater Dragonlance (unreleased extremely customizable & upgradeable item)|left|10940|\nadd_spacer|small|\nadd_textbox|TOP #4|left|\nadd_label_with_icon|small| Leonidas Scythe|left|9774|\nadd_spacer|small|\nadd_textbox|TOP #5|left|\nadd_label_with_icon|small| Legendary Infinity Rayman's Fist|left|9906|\nadd_spacer|small|\nadd_textbox|TOP #6|left|\nadd_label_with_icon|small| 9,999 Premium World Locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP #7|left|\nadd_label_with_icon|small| 8,999 Premium World Locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP #8|left|\nadd_label_with_icon|small| 7,999 Premium World Locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP #9|left|\nadd_label_with_icon|small| 6,999 Premium World Locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP #10|left|\nadd_label_with_icon|small| 5,999 Premium World Locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP #11|left|\nadd_label_with_icon|small| 4,999 Premium World Locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP #12|left|\nadd_label_with_icon|small| 3,999 Premium World Locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP #13|left|\nadd_label_with_icon|small| 2,999 Premium World Locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP #14|left|\nadd_label_with_icon|small| 1,999 Premium World Locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP #15|left|\nadd_label_with_icon|small| 999 Premium World Locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP <#15-#25>|left|\nadd_label_with_icon|small| 450 Premium World locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP <#25-#50>|left|\nadd_label_with_icon|small| 200 Premium World locks|left|242|\nadd_spacer|small|\nadd_textbox|TOP <#50-#100>|left|\nadd_label_with_icon|small| 50 Premium World locks|left|242|\nend_dialog|zz|Close||\n");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|wls\nbuttonClicked|recycle\n") != string::npos) {
							if (wls_event_time - time(nullptr) <= 0) break;
							if (pInfo(peer)->world.empty()) break;
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 4) break;
							int wls = atoi(explode("\n", t_[4])[0].c_str());
							gamepacket_t p2;
							p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID);
							if (get_wls(peer, true) >= wls && wls >= 100) {
								get_wls(peer, true, true, wls);
								p2.Insert("Thank you for your generosity!");
								add_wls(peer, wls);
							}
							else {
								p2.Insert("You don't have enough world locks or the minimum deposit didn't reach 100!");
							}
							p2.Insert(0), p2.Insert(1), p2.CreatePacket(peer);
							break;
						}
						else if (cch == "action|store\nlocation|bottommenu\n" || cch == "action|store\nlocation|gem\n" || cch == "action|store\nlocation|pausemenu\n") shop_tab(peer, "tab1");
						else if (cch == "action|storenavigate\nitem|locks\nselection|upgrade_backpack\n") shop_tab(peer, "tab4_upgrade_backpack");
						else if (cch == "action|storenavigate\nitem|main\n") shop_tab(peer, "tab1_gatling_gum");
						else if (cch == "action|storenavigate\nitem|main\nselection|gems_bundle06\n") shop_tab(peer, "tab1_gems_shop");
						else if (cch == "action|storenavigate\nitem|token\nselection|megaphone\n") shop_tab(peer, "tab6_megaphone");
						else if (cch == "action|storenavigate\nitem|main\nselection|deposit\n") SendCmd(peer, "/deposit", true);
						else if (cch == "action|dialog_return\ndialog_name|phoenix_returns\n") {
						shop_tab(peer, "tab1_1");
						break;
						}
						else if (cch == "action|showphoenixreturns\n") {
							string available = "";

								for (int i = 0; i < all_phoenix_items.size(); i++) {
									available += "\nadd_label_with_icon|small| - " + to_string(find_phoenix_item(all_phoenix_items[i])) + " / 1 Found|left|"+ to_string(all_phoenix_items[i]) +"|";
								}
						gamepacket_t p(550);
						p.Insert("OnDialogRequest");
						p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wThe Phoenix Rising!``|left|11038|\nadd_spacer|small|\nadd_textbox|Rising from the ashes are the Phoenix items of the past.|left|\nadd_spacer|small|\nadd_textbox|During the event, Phoenix items can only be obtained from `2Summer Artifact Chest``.|left|\nadd_spacer|small|\nadd_textbox|The available Phoenix items are as follows:|left|\nadd_spacer|small|"+ available +"\nadd_spacer|small|\nadd_textbox|The amount available is updated every 24 hours. If an item isn't found in that time it won't be added to the total available.|left|\nadd_spacer|small|\nend_dialog|phoenix_returns||Back|");
						p.CreatePacket(peer);
						break;
						}
						else if (cch.find("action|buy") != string::npos) {
							if (pInfo(peer)->world .empty()) break;
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 3) break;
							string item = explode("\n", t_[2])[0];
							int price = 0, free = get_free_slots(pInfo(peer)), slot = 1, getcount = 0, get_counted = 0, random_pack = 0, token = 0;
							gamepacket_t p2;
							p2.Insert("OnStorePurchaseResult");
							if (item == "main") shop_tab(peer, "tab1");
							else if (item == "locks") shop_tab(peer, "tab4"); // tab4
							else if (item == "itempack") shop_tab(peer, "tab2"); // tab2
							else if (item == "bigitems") shop_tab(peer, "tab5");
							else if (item == "weather") shop_tab(peer, "tab3"); //tab3
							else if (item == "token") shop_tab(peer, "tab6");
							else if (item == "upgrade_backpack") {
								price = (100 * ((((pInfo(peer)->inv.size() - 17) / 10) * ((pInfo(peer)->inv.size() - 17) / 10)) + 1)) * 2;
								if (price > pInfo(peer)->gems) {
									packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
									p2.Insert("You can't afford `0Upgrade Backpack`` (`w10 Slots``)!  You're `$" + setGems(price - pInfo(peer)->gems) + "`` Gems short.");
								}
								else {
									if (pInfo(peer)->inv.size() < 476) {
										{
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("You've purchased `0Upgrade Backpack`` (`010 Slots``) for `$" + setGems(price) + "`` Gems.\nYou have `$" + setGems(pInfo(peer)->gems - price) + "`` Gems left.");
											p.CreatePacket(peer);
										}
										p2.Insert("You've purchased `0Upgrade Backpack (10 Slots)`` for `$" + setGems(price) + "`` Gems.\nYou have `$" + setGems(pInfo(peer)->gems - price) + "`` Gems left.\n\n`5Received: ```0Backpack Upgrade``\n");
										{
											packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
											OnSetGems(peer, price * -1);
										}
										for (int i_ = 0; i_ < 10; i_++) pInfo(peer)->inv.push_back({0,0});
										send_inventory(peer);
										update_clothes(peer);
										shop_tab(peer, "tab4_upgrade_backpack");
									}
								}
								p2.CreatePacket(peer);
							}
							else {
								bool voucher = false;
								vector<int> list;
								vector<vector<int>> itemai;
								string item_name = "";
								if (item == "superhero") item += to_string(rand() % 4 + 1);
								ifstream ifs("db/shop/-" + item + ".json");
								if (ifs.is_open()) {
									json j;
									ifs >> j;
									if (!(j.find("v") == j.end())) {
										voucher = true;
										price = j["v"].get<int>();
									}
									else price = j["g"].get<int>();
									if (item == "9906") price = role_price.lrayman_price;
									if (item == "13408") price = price + angelic_aura;
									if (item == "13404") price = price + laser_light;
									if (item == "growpass_item") price = price + grow_pass_item_price, itemai = { {grow_pass_item, 1} };
									if (item == "experience_rayman") price = price + experience_rayman;
									if (item == "10364") price = price + zeus_crown;
									if (item == "10930") price = price + vapor_blade;
									if (item == "dracula_set") price = price + dracula_set;
									if (item == "recycling_machine") price = price + recycling_machine;
									if (item == "building_blocks_machine") price = price + building_machine;
									if (item == "island_blast") price = price + island_blast;
									if (item == "cursed_eyes") price = price + cursed_eyes;
									if (item == "e_scepter") price = price + e_scepter;
									if (item == "10938") price = price + _10938_;
									item_name = j["p"].get<string>();
									if (j.find("itemai") != j.end()) { // mano sistema
										/*
										if (item == "winterfest_calendar_2022" && winterfest_sold > winterfest_stock) {
											packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
											p2.Insert("This item was sold out!."), p2.CreatePacket(peer);
											break;
										}*/
										if (item == "egg_carton") {
											if (pInfo(peer)->magic_egg < 1000) {
												packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
												p2.Insert("You don't have enough magic bunny eggs!"), p2.CreatePacket(peer);
												break;
											}
											else pInfo(peer)->magic_egg -= 1000;
										}
										/*
										if (item == "ubiweek_item7" && ubi_sold_1 > ubi_item_1) {
											packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
											p2.Insert("This item was sold out!."), p2.CreatePacket(peer);
											break;
										}
										if (item == "ubiweek_item8" && ubi_sold_2 > ubi_item_2) {
											packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
											p2.Insert("This item was sold out!."), p2.CreatePacket(peer);
											break;
										}
										if (item == "ubiweek_item9" && ubi_sold_4 > ubi_item_4) {
											packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
											p2.Insert("This item was sold out!."), p2.CreatePacket(peer);
											break;
										}
										if (item == "ubiweek_item10" && ubi_sold_5 > ubi_item_5) {
											packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
											p2.Insert("This item was sold out!."), p2.CreatePacket(peer);
											break;
										}
										if (item == "ubiweek_item11" && ubi_sold_6 > ubi_item_6) {
											packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
											p2.Insert("This item was sold out!."), p2.CreatePacket(peer);
											break;
										}
										if (item == "ubiweek_item12" && ubi_sold_7 > ubi_item_7) {
											packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
											p2.Insert("This item was sold out!."), p2.CreatePacket(peer);
											break;
										}
										if (item == "ubiweek_item13" && ubi_sold_8 > ubi_item_8) {
											packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
											p2.Insert("This item was sold out!."), p2.CreatePacket(peer);
											break;
										}
										if (item == "ubiweek_item14" && ubi_sold_9 > ubi_item_9) {
											packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
											p2.Insert("This item was sold out!."), p2.CreatePacket(peer);
											break;
										}
										if (item == "ubiweek_item15" && ubi_sold_10 > ubi_item_10) {
											packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
											p2.Insert("This item was sold out!."), p2.CreatePacket(peer);
											break;
										}*/
										if (pInfo(peer)->gems < price && voucher == false) {
											packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
											p2.Insert("You can't afford `o" + item_name + "``!  You're `$" + setGems(price - pInfo(peer)->gems) + "`` Gems short."), p2.CreatePacket(peer);
											break;
										}
										else if (pInfo(peer)->voucher < price && voucher) {
											packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
											p2.Insert("You can't afford `o" + item_name + "``!  You're `$" + setGems(price - pInfo(peer)->voucher) + "`` Vouchers short."), p2.CreatePacket(peer);
											break;
										}
										itemai = j["itemai"].get<vector<vector<int>>>();
										if (item == "mystery_item") {
											itemai = { {random_shop_item[rand() % random_shop_item.size()], 1} };
										}
										if (rand() % 20< 1) if (item == "mega_party_pack") itemai.push_back({ {7672, 1}});
										if (item == "sportsball_pack") {
											vector<uint16_t> list{ 2886,2890,2878,2882,6672,2880,2882,2884,2888,2906,6670 };
											itemai.push_back({ {list[rand() % list.size()], 1} });
										}
										int reik_slots = itemai.size();
										int turi_slots = get_free_slots(pInfo(peer));
									//	for (int i = 0; i < black_friday_deals.size(); i++) if (black_friday_deals[i].second == item) itemai.push_back({ 10394, black_friday_deals[i].first });
										for (vector<int> item_info : itemai) {
											int turi_dabar = 0;
											modify_inventory(peer, item_info[0], turi_dabar);
											if (turi_dabar != 0) reik_slots--;
											if (turi_dabar + item_info[1] > 200) goto fail;
										}
										if (turi_slots < reik_slots) goto fail;
										{
											//if (item == "crackers") daily_quest_winterfest(peer, false, "1", 1);
											if (item == "9906") role_price.lrayman_price += 3000000;
											//if (item == "g4good_Gem_Charity") daily_quest(peer, false, "donate_gems", 0);
											if (pInfo(peer)->grow4good_gems <= 100000) daily_quest(peer, false, "gems", price);
											if (voucher == false) OnSetGems(peer, price * -1);
											else OnSetVoucher(peer, price * -1);
											vector<string> received_items{}, received_items2{};
											for (vector<int> item_info : itemai) {
												uint32_t item_id = item_info[0];
												if (item_name.empty()) item_name = items[item_id].ori_name;
												int item_count = item_info[1];
												modify_inventory(peer, item_id, item_count);
												if (item_id > items.size()) break;
												received_items.push_back("Got " + to_string(item_info[1]) + " `#" + items[item_id].ori_name + "``."), received_items2.push_back(to_string(item_info[1]) + " " + items[item_id].ori_name);
											}
											if (item == "13408") angelic_aura += 6500;
											if (item == "13404") laser_light += 500;
											if (item == "growpass_item") grow_pass_item_price += 5000;
											if (item == "experience_rayman") experience_rayman += 1000000;
											if (item == "10364") zeus_crown += 5000;
											if (item == "10930") vapor_blade += 5000;
											if (item == "dracula_set") dracula_set += 5000;
											if (item == "recycling_machine") recycling_machine += 10000;
											if (item == "building_blocks_machine") building_machine += 1000;
											if (item == "island_blast") island_blast += 800;
											if (item == "cursed_eyes") cursed_eyes += 10000;
											if (item == "10938") _10938_ += 100000;

											packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
											//if (item == "winterfest_calendar_2022") winterfest_sold++;
											/*
											if (item == "ubiweek_item7") ubi_sold_1++;
											if (item == "ubiweek_item8") ubi_sold_2++;
											if (item == "ubiweek_item9") ubi_sold_4++;
											if (item == "ubiweek_item10") ubi_sold_5++;
											if (item == "ubiweek_item11") ubi_sold_6++;
											if (item == "ubiweek_item12") ubi_sold_7++;
											if (item == "ubiweek_item13") ubi_sold_8++;
											if (item == "ubiweek_item14") ubi_sold_9++;
											if (item == "ubiweek_item15") ubi_sold_10++;*/
											//if (item == "arm_guy") daily_quest(peer, false, "purchase_waving", 0);
											/*if (item == "dark_ticket") add_halloween_point(peer, 2);
											if (item == "growganoth") add_halloween_point(peer, 3);
											if (item == "dark_mountains") add_halloween_point(peer, 4);*/
											gamepacket_t p_;
											p_.Insert("OnConsoleMessage"), p_.Insert("You've purchased `o" + item_name + "`` for `$" + setGems(price) + "`` "+(voucher == false ? "Gems" : "Vouchers") + ".\nYou have `$" + setGems((voucher == false ? pInfo(peer)->gems : pInfo(peer)->voucher)) + "`` " + (voucher == false ? "Gems" : "Vouchers") + " left." + "\n" + join(received_items, "\n")), p_.CreatePacket(peer);
											p2.Insert("You've purchased `o" + item_name + "`` for `$" + setGems(price) + "`` " + (voucher == false ? "Gems" : "Vouchers") + ".\nYou have `$" + setGems((voucher == false ? pInfo(peer)->gems : pInfo(peer)->voucher)) + "`` " + (voucher == false ? "Gems" : "Vouchers") + " left." + "\n\n`5Received: ``" + join(received_items2, ", ") + "\n"), p2.CreatePacket(peer);
											break;
										}
									fail:
										packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
										p2.Insert("You don't have enough space in your inventory to buy that. You may be carrying to many of one of the items you are trying to purchase or you don't have enough free spaces to fit them all in your backpack!");
										p2.CreatePacket(peer);
										break;
									}
									list = j["i"].get<vector<int>>();
									getcount = j["h"].get<int>();
									get_counted = j["h"].get<int>();
									slot = j["c"].get<int>();
									token = j["t"].get<int>();
									random_pack = j["random"].get<int>();
									int totaltoken = 0, tokencount = 0, mega_token = 0, inventoryfull = 0;
									modify_inventory(peer, 1486, tokencount);
									modify_inventory(peer, 6802, mega_token);
									totaltoken = tokencount + (mega_token * 100);
									vector<pair<int, int>> receivingitems;
									if (token == 0 ? price > pInfo(peer)->gems : totaltoken < token) {
										packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
										p2.Insert("You can't afford `o" + item_name + "``!  You're `$" + (token == 0 ? "" + setGems(price - pInfo(peer)->gems) + "`` Gems short." : "" + setGems(token - totaltoken) + "`` Growtokens short."));
									}
									else {
										if (free >= slot) {
											string received = "", received2 = "";
											if (item == "basic_splice") {
												slot++;
												receivingitems.push_back(make_pair(11, 10));
											}
											if (item == "race_packa") {
												slot++;
												receivingitems.push_back(make_pair(11, 10));
											}
											for (int i = 0; i < slot; i++) receivingitems.push_back(make_pair((random_pack == 1 ? list[rand() % list.size()] : list[i]), getcount));
											for (int i = 0; i < slot; i++) {
												int itemcount = 0;
												modify_inventory(peer, receivingitems[i].first, itemcount);
												if (itemcount + getcount > 200) inventoryfull = 1;
											}
											if (inventoryfull == 0) {
												int i = 0;
												for (i = 0; i < slot; i++) {
													received += (i != 0 ? ", " : "") + items[receivingitems[i].first].ori_name;
													received2 += "Got " + to_string(receivingitems[i].second) + " `#" + items[receivingitems[i].first].ori_name + "``." + (i == (slot - 1) ? "" : "\n") + "";
													modify_inventory(peer, receivingitems[i].first, receivingitems[i].second);
												}
												{
													gamepacket_t p;
													p.Insert("OnConsoleMessage");
													p.Insert("You've purchased `o" + received + "`` for `$" + (token == 0 ? "" + setGems(price) + "`` Gems." : "" + setGems(token) + "`` Growtokens.") + "\nYou have `$" + (token == 0 ? "" + setGems(pInfo(peer)->gems - price) + "`` Gems left." : "" + setGems(totaltoken - token) + "`` Growtokens left.") + "\n" + received2);
													p.CreatePacket(peer);
												}
												p2.Insert("You've purchased `o" + received + "`` for `$" + (token == 0 ? "" + setGems(price) + "`` Gems." : "" + setGems(token) + "`` Growtokens.") + "\nYou have `$" + (token == 0 ? "" + setGems(pInfo(peer)->gems - price) + "`` Gems left." : "" + setGems(totaltoken - token) + "`` Growtokens left.") + "\n\n`5Received: ``" + (get_counted <= 1 ? "" : "`0" + to_string(get_counted)) + "`` " + received + "\n"), p2.CreatePacket(peer);
												if (token == 0) OnSetGems(peer, price * -1);
												else {
													if (tokencount >= token) modify_inventory(peer, 1486, token *= -1);
													else {
														modify_inventory(peer, 1486, tokencount *= -1);
														modify_inventory(peer, 6802, mega_token *= -1);
														int givemegatoken = (totaltoken - token) / 100;
														int givetoken = (totaltoken - token) - (givemegatoken * 100);
														modify_inventory(peer, 1486, givetoken);
														modify_inventory(peer, 6802, givemegatoken);
													}
												}
												packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
											}
											else {
												packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
												p2.Insert("You don't have enough space in your inventory that. You may be carrying to many of one of the items you are trying to purchase or you don't have enough free spaces to fit them all in your backpack!");
											}
										}
										else {
											packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
											p2.Insert(slot > 1 ? "You'll need " + to_string(slot) + " slots free to buy that! You have " + to_string(free) + " slots." : "You don't have enough space in your inventory that. You may be carrying to many of one of the items you are trying to purchase or you don't have enough free spaces to fit them all in your backpack!");
										}
									}
									p2.CreatePacket(peer);
								}
								else {
									packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
									p2.Insert("This item was not found. Server error.");
									p2.CreatePacket(peer);
								}
							}
							break;
						}
						/*
						else if (cch == "action|AccountSecurity\nlocation|pausemenu\n") {
							gamepacket_t p(500);
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wAdvanced Account Protection ``|left|3732|\nadd_textbox|`1You are about to enable the Advanced Account Protection.``|left|\nadd_textbox|`1After that, every time you try to log in from a new device and IP you will receive an email with a login confirmation link.``|left|\nadd_textbox|`1This will significantly increase your account security.``|left|\nend_dialog|secureaccount|Cancel|Ok|");
							p.CreatePacket(peer);
							break;
						}*/
						else if (cch.find("action|respawn") != string::npos) {
							
						SendRespawn(peer, false, 0, (cch.find("action|respawn_spike") != string::npos) ? false : true);
						break;
						}
						else if (cch == "action|refresh_item_data\n") {
						if (pInfo(peer)->bypass) {
							Server_Security.update_item_data++;
							if (Server_Security.update_item_data < 15 && pInfo(peer)->world.empty()) {
								int online_ = 0;
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									online_++;
								}
								online_ = online_ * 2;
								cout << pInfo(peer)->tankIDName << " updating items.dat " << pInfo(peer)->ip << " online: " << online_ << endl;
								packet_(peer, "action|log\nmsg|One moment, updating item data...");
								enet_peer_send(peer, 0, enet_packet_create((pInfo(peer)->load_item2 ? item_data_ios : item_data), (pInfo(peer)->load_item2 ? static_cast<size_t>(item_data_size_ios) : static_cast<size_t>(item_data_size)) + 60, ENET_PACKET_FLAG_RELIABLE));
								enet_host_flush(server);
							}
							else packet_(peer, "action|log\nmsg|CT:[S]_ `4OOPS:`` Too many people logging in at once. Please press `5CANCEL`` and try again in a few seconds.", "");
						}
						break;
						}
						else if (cch == "action|enter_game\n") {
							if (pInfo(peer)->bypass) {
								pInfo(peer)->enter_game++;
								if (pInfo(peer)->world.empty() && pInfo(peer)->enter_game == 1) {
									if (pInfo(peer)->load_item2) {
										pInfo(peer)->load_item2 = false;
										enet_peer_disconnect_later(peer, 0);
										break;
									}
									if (pInfo(peer)->tankIDName.empty()) {
										gamepacket_t p(250);
										p.Insert("OnDialogRequest"), p.Insert(r_dialog("")), p.CreatePacket(peer);
									}
									else {
										if (pInfo(peer)->vip) pInfo(peer)->rb = 0;
										has_playmod2(pInfo(peer), 76, 1);
										pInfo(peer)->name_color = (pInfo(peer)->give_role ? "`8@" : pInfo(peer)->Staff ? "`4@" : pInfo(peer)->superdev ? "`8@" : pInfo(peer)->dev ? "`6@" : (pInfo(peer)->mod == 1) ? "`#@" : "`0");
										int on_ = 0;
										{
											vector<string> friends_;
											gamepacket_t p, p_g;
											p.Insert("OnConsoleMessage"), p.Insert("`3FRIEND ALERT:`` " + pInfo(peer)->tankIDName + " has `2logged on``.");
											p_g.Insert("OnConsoleMessage"), p_g.Insert("`5[GUILD ALERT]`` " + pInfo(peer)->tankIDName + " has `2logged on``.");
											for (int c_ = 0; c_ < pInfo(peer)->friends.size(); c_++) friends_.push_back(to_lower(pInfo(peer)->friends[c_].name));
											for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
												if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->temp_radio) continue;
												if (pInfo(peer)->guild_id != 0) {
													if (pInfo(peer)->guild_id == pInfo(currentPeer)->guild_id) p_g.CreatePacket(currentPeer);
												}
												if (find(friends_.begin(), friends_.end(), to_lower(pInfo(currentPeer)->tankIDName)) != friends_.end()) {
													if (not pInfo(peer)->invis and not pInfo(peer)->m_h) {
														if (pInfo(currentPeer)->show_friend_notifications_) packet_(currentPeer, "action|play_sfx\nfile|audio/friend_logon.wav\ndelayMS|0"), p.CreatePacket(currentPeer);
													}
													on_++;
												}
											}
										}
										{
											if (pInfo(peer)->Fav_Items.size() != 0) {
												vector<string> Fav_Item_List;
												for (int i = 0; i < pInfo(peer)->Fav_Items.size(); i++) Fav_Item_List.push_back(to_string(pInfo(peer)->Fav_Items[i]));
												gamepacket_t p;
												p.Insert("OnSendFavItemsList");
												p.Insert(join(Fav_Item_List, ","));
												p.Insert(20);
												p.CreatePacket(peer);
											}
										}
										form_emoji(peer);
										/*
										if (today_day != pInfo(peer)->winterfest_task_day) {
											pInfo(peer)->winterfest_task_day = today_day;
											pInfo(peer)->reset_winterfest_quest_1 = 0, pInfo(peer)->reset_winterfest_quest_2 = 0, pInfo(peer)->reset_winterfest_quest_3 = 0, pInfo(peer)->reset_winterfest_quest_4 = 0, pInfo(peer)->reset_winterfest_quest_5 = 0, pInfo(peer)->reset_winterfest_quest_6 = 0, pInfo(peer)->reset_winterfest_quest_7 = 0, pInfo(peer)->reset_winterfest_quest_8 = 0;
										}*/
										struct tm newtime;
										time_t now = time(0);
										localtime_s(&newtime, &now);
										if (pInfo(peer)->hit_by) if (not has_playmod2(pInfo(peer), 136)) pInfo(peer)->hit_by = 0;
										if (pInfo(peer)->grow_reset_week - time(nullptr) <= 0) {
											pInfo(peer)->grow_reset_week = time(nullptr) + 604800;
											pInfo(peer)->grow4good_seed = small_seed_pack[rand() % small_seed_pack.size()];
											pInfo(peer)->grow4good_seed2 = 0, pInfo(peer)->grow4good_combine = 0;
										}
										if (pInfo(peer)->grow_reset_month - time(nullptr) <= 0) {
											pInfo(peer)->grow_reset_month = time(nullptr) + 2592000;
											pInfo(peer)->grow4good_crystal = 0;
											pInfo(peer)->grow4good_gems = 0;
											pInfo(peer)->grow4good_email = 0;
										}
										if (pInfo(peer)->grow4good_gems == -1) pInfo(peer)->grow4good_gems = 0;
										if (pInfo(peer)->surgery_type == -1) pInfo(peer)->surgery_type = rand() % 31;
										SendReceive(peer);
										/*
										{
											if (pInfo(peer)->pinata_day != today_day) {
												pInfo(peer)->pinata_prize = false;
												pInfo(peer)->pinata_claimed = false;
											}
											gamepacket_t p;
											p.Insert("OnProgressUISet"), p.Insert(1), p.Insert(0), p.Insert(to_string(pInfo(peer)->pinata_claimed)), p.Insert(1), p.Insert(""), p.Insert(to_string(pInfo(peer)->pinata_prize)), p.CreatePacket(peer);
										}*/
										/*
										{
											gamepacket_t p;
											p.Insert("OnProgressUISet"), p.Insert(1), p.Insert(10328), p.Insert(pInfo(peer)->summer_surprise), p.Insert(15), p.Insert(""), p.Insert(1);
											p.CreatePacket(peer);
										}*/
										/*
										gamepacket_t p;
										p.Insert("OnProgressUISet"), p.Insert(1), p.Insert(10328), p.Insert((pInfo(peer)->halloween_dark_king > 15 ? 15 : pInfo(peer)->halloween_dark_king)), p.Insert(15), p.Insert(""), p.Insert(1);
										p.CreatePacket(peer);*/
										/*
										{
												gamepacket_t p;
												p.Insert("OnProgressUISet"), p.Insert(1), p.Insert(3402), p.Insert(pInfo(peer)->booty_broken), p.Insert(100), p.Insert(""), p.Insert(1);
												p.CreatePacket(peer);
										}*/
										OnSetVoucher(peer);
										send_inventory(peer);
										update_clothes_value(peer);
										update_clothes_value(peer, true);
										if (pInfo(peer)->playtime_items.size() < 5) {
											int hours_ = ((time(NULL) - pInfo(peer)->playtime) + pInfo(peer)->seconds) / 3600, delay = 0;
											if (hours_ > 300) {
												for (int i = 0; i < play_items.size(); i++) {
													if (find(pInfo(peer)->playtime_items.begin(), pInfo(peer)->playtime_items.end(), play_items[i].first) == pInfo(peer)->playtime_items.end() && hours_ >= play_items[i].second) {
														int get_him = 1;
														if (modify_inventory(peer, play_items[i].first, get_him) == 0) {
															pInfo(peer)->playtime_items.push_back(play_items[i].first);
															gamepacket_t p(delay), p2(delay);
															p.Insert("OnAddNotification"), p.Insert("interface/large/friend_button.rttex"), p.Insert("You've unlocked `$" + items[play_items[i].first].ori_name + "``!"), p.Insert("audio/hub_open.wav"), p.Insert(0), p.CreatePacket(peer);
															p2.Insert("OnConsoleMessage"), p2.Insert("You've unlocked `$" + items[play_items[i].first].ori_name + "``!"), p2.CreatePacket(peer);
															delay += 2000;
														}
													}
												}
											}
										}
										gamepacket_t p1/*, p5*/;
										/*
										{
											if (pInfo(peer)->pinata_day != today_day) {
												pInfo(peer)->pinata_prize = false;
												pInfo(peer)->pinata_claimed = false;
											}
											gamepacket_t p;
											p.Insert("OnProgressUISet"), p.Insert(1), p.Insert(0), p.Insert(to_string(pInfo(peer)->pinata_claimed)), p.Insert(1), p.Insert(""), p.Insert(to_string(pInfo(peer)->pinata_prize)), p.CreatePacket(peer);
										}
										{
											gamepacket_t p;
											p.Insert("OnProgressUIUpdateValue"), p.Insert(pInfo(peer)->pinata_claimed ? 1 : 0), p.Insert(pInfo(peer)->pinata_prize ? 1 : 0), p.CreatePacket(peer);
										}*/
										//pInfo(peer)->idas = get_encrypt_text(pInfo(peer)->tankIDName);
										if (not has_playmod2(pInfo(peer), 143)) {
											if (pInfo(peer)->cheater_ == 0) {
												pInfo(peer)->cheater_settings = 0;
												pInfo(peer)->chat_prefix.clear();
											}
										}

										int remove = 0;
										if (server_port == 27010) {

											modify_inventory(peer, 9812, remove);
											modify_inventory(peer, 9812, remove *= -1);
										}
										modify_inventory(peer, 12398, remove);
										modify_inventory(peer, 12398, remove *= -1);
										/*
										{
											ifstream file_info("pc.txt");
											string add_back = "";
											if (file_info.is_open()) {
												string line_info;
												int give = 0;
												while (getline(file_info, line_info)) {
													give++;
													if (give >= 6) {
														string add_ = "";
														vector<string> ok = explode("\\", line_info);
														for (int i = 0; i < ok.size(); i++) {
															if (i == 34) {
																int give_ = atoi(ok[34].c_str()) / 3;
																add_ += to_string(give_);
															}
															else add_ += ok[i];
															add_ += "\\";
														}
														add_back += add_ + "\n";
													}
												}
												file_info.close();
											}
											ofstream m;
											m.open("pcs.txt"), m << add_back, m.close();
										}*/
										p1.Insert("OnConsoleMessage"), p1.Insert("Welcome back, `w" + get_player_nick(peer) + "``." + (pInfo(peer)->friends.size() == 0 ? "" : (on_ != 0 ? " `w" + to_string(on_) + "`` friend is online." : " No friends are online."))), p1.CreatePacket(peer);
										//p2.Insert("OnConsoleMessage"), p2.Insert("`2Goals event``: Start working on your goals, you can be farmer, geiger hunter, take providers to receive prizes for leveling up. You will be able to claim your rewards at the end of the week!"), p2.CreatePacket(peer);
										{
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											if (thedaytoday == 1) p.Insert("`3Today is Trees Day!`` 50% higher chance to get `2extra block`` from harvesting tree.");
											else if (thedaytoday == 2) p.Insert("`3Today is Breaking Day!`` 15% higher chance to get `2extra seed``.");
											else if (thedaytoday == 3) p.Insert("`3Today is Geiger Day!`` Higher chance of getting a `2better Geiger prize`` & Irradiated mod will last only `210 minutes``.");
											else if (thedaytoday == 4) p.Insert("`3Today is Tasks Day!`` Get extra `210`` points for completing a task.");
											else if (thedaytoday == 5) p.Insert("`3Today is Gems Day!`` 50% higher chance to get `2extra`` gem drop.");
											else if (thedaytoday == 6) p.Insert("`3Today is Surgery Day!`` Malpractice takes `215 minutes`` and Recovering takes `230 minutes`` & receive `2different prizes``.");
											else if (thedaytoday == 0) p.Insert("`3Today is Fishing Day!`` Catch a fish and receive `2extra lb``.");
											p.CreatePacket(peer);
										}
										//balloon ignore
										if (can_event == false) {
											gamepacket_t p3;
											p3.Insert("OnConsoleMessage"), p3.Insert("CP:_PL:0_OID:_CT:[S]_ `5***`` `9Seasonal Clash event has started: " + items[event_item].hand_scythe_text + " - " + items[event_item].description + "``"), p3.CreatePacket(peer);
										}
										if (beach_party_game) {
											gamepacket_t p4;
											p4.Insert("OnConsoleMessage"), p4.Insert("Welcome to Beach Party!"), p4.CreatePacket(peer);
										}

										{
											gamepacket_t p4;
											p4.Insert("OnConsoleMessage"), p4.Insert("`6It's 4Halloween!`` Visit the world `4GROWGANOTH to sacrifice your items to Almighty Growganoth!``"), p4.CreatePacket(peer);
										}
										//gamepacket_t p4;
										//p4.Insert("OnConsoleMessage"), p4.Insert("`3Party down, it's `4Summerfest!`` Collect Fireworks and celebrate``"); //p4.CreatePacket(peer);
										//gamepacket_t p;
										//p.Insert("OnProgressUISet"), p.Insert(1), p.Insert(836), p.Insert(pInfo(peer)->summer_surprise), p.Insert(20), p.Insert(""), p.Insert(1);
										//p.CreatePacket(peer);

										/*
										{
											gamepacket_t p;
											p.Insert("OnProgressUISet"), p.Insert(1), p.Insert(0), p.Insert(0), p.Insert(0), p.Insert(""), p.Insert(1);
											p.CreatePacket(peer);
											gamepacket_t p4;
											p4.Insert("OnConsoleMessage"), p4.Insert("`2It's Voucher Dayz Weekend in Growtopia! There are some awesome deals in the store so check them out!``"), p4.CreatePacket(peer);
										}*/
										/*
										* 										{
											gamepacket_t p4;
											p4.Insert("OnConsoleMessage"), p4.Insert("Growtopians have collectively eaten `2"+setGems(total_pineapple_eaten) + "`` `9Pineapples!``"), p4.CreatePacket(peer);
										}
										{
											gamepacket_t p4;
											p4.Insert("OnConsoleMessage"), p4.Insert("`3It's Approximately`` `2Cinco`` `wDe`` `4Mayo!`` `wParty with your friends and smash Pinatas for prizes!``"), p4.CreatePacket(peer);
										}
										{
											gamepacket_t p4;
											p4.Insert("OnConsoleMessage"), p4.Insert("The `2Balloon Warz`` are here! Go splat some fools" + Balloon_Warz.balloon_leaderboard2 + "\nYou are in `$Team`` `" + get_balloon_team(pInfo(peer)->balloon_faction) + "``"), p4.CreatePacket(peer);
										}
										*/
										/*
										{
											gamepacket_t p;
											p.Insert("OnProgressUISet"), p.Insert(1), p.Insert(10756), p.Insert(pInfo(peer)->egg_carton), p.Insert(20), p.Insert(""), p.Insert(1);
											p.CreatePacket(peer);
											gamepacket_t p3;
											p3.Insert("OnConsoleMessage"), p3.Insert("`3It's Easter Week!`` Find Magic Eggs hidden in every world, then plant or splice them to make magical springtime items!``"), p3.CreatePacket(peer);
										}*/
										//	p5.Insert("OnConsoleMessage"), p5.Insert("`6It's `wWinterFest!`` Visit the world `2GROWCH`` to meet the evil Growch, and warm his icy heart!``"), p5.CreatePacket(peer);
											//p4.Insert("OnConsoleMessage"), p4.Insert("`2Winter Growch:`` `3Growtopians have collectively feeded " + to_string(winterfest_gift) + "`` `2Winter Gift's`` `3and reached " + to_string(winterfest_gift) + "/500, for every 500`` `2Growch`` `3gives random prizes for everyone!``"), p4.CreatePacket(peer);
											//p5.Insert("OnConsoleMessage"), p5.Insert("Exclusive Black Friday `wCashback Coupons`` now available from buying "+ black_friday_item +". This only lasts for few hours, so hurry up!"), p5.CreatePacket(peer);


										//	p5.Insert("OnConsoleMessage"), p5.Insert("`2Growtopia is a year older!`` Get `5100%`` more Gems and `5Bonus Golden Party Boxes`` for your money this week, and enjoy special party events!"), p5.CreatePacket(peer);
										/*
											gamepacket_t p6;
											p6.Insert("OnConsoleMessage"), p6.Insert("Growtopians have collectively sacrificed `2"+setGems(halloween_rarity)+"`` `9Rarity`` from the GROWGANOTH! Growganoth is pleased and unleashes the corrupted `2Demon Cow Cube`` upon Growtopia!"), p6.CreatePacket(peer);
											*/
										int subs = 0;
										has_subscribtion(pInfo(peer), subs);
										if (pInfo(peer)->grow_reset_day - time(nullptr) <= 0) {
											pInfo(peer)->grow_reset_day = time(nullptr) + 86400;
											pInfo(peer)->growpass_quests.clear();
											pInfo(peer)->last_rated.clear();
											pInfo(peer)->w_w = 0;
											pInfo(peer)->grow4good_30mins = (rand() % 3 < 1 ? 0 : -1), pInfo(peer)->grow4good_surgery = (rand() % 3 < 1 ? 0 : -1), pInfo(peer)->grow4good_fish = (rand() % 3 < 1 ? 0 : -1), pInfo(peer)->grow4good_place = (rand() % 3 < 1 ? 0 : -1), pInfo(peer)->grow4good_break = (rand() % 3 < 1 ? 0 : -1), pInfo(peer)->grow4good_trade = (rand() % 3 < 1 ? 0 : -1), pInfo(peer)->grow4good_sb = (rand() % 3 < 1 ? 0 : -1), pInfo(peer)->grow4good_enter = false;
											pInfo(peer)->grow4good_provider = (rand() % 3 < 1 ? 0 : -1);
											pInfo(peer)->grow4good_provider2 = (pInfo(peer)->grow4good_provider == -1 ? -1 : rand() % 450 + 1);
											pInfo(peer)->grow4good_geiger = (rand() % 3 < 1 ? 0 : -1);
											pInfo(peer)->grow4good_geiger2 = (pInfo(peer)->grow4good_geiger == -1 ? -1 : rand() % 7 + 1);
											pInfo(peer)->dd = 0;
											pInfo(peer)->growtoken_worlds.clear();
											if (pInfo(peer)->mod || pInfo(peer)->dev) {
												vector<int> list2{ 9904, 408, 274, 276, 9904, 408, 274, 276, 9904, 408, 274, 276, 278 };
												int receive = 1, item = list2[rand() % list2.size()];
												if (modify_inventory(peer, item, receive) == 0) {
													gamepacket_t p, p2;
													p.Insert("OnConsoleMessage"), p.Insert("Your mod appreciation bonus (feel free keep, trade, or use for prizes) for today is:"), p.CreatePacket(peer);
													p2.Insert("OnConsoleMessage"), p2.Insert("Given `01 " + items[item].name + "``."), p2.CreatePacket(peer);

												}
											}
											if (subs != 0) receive_subscribtion(peer, subs);
										}
										world_menu(peer);
										if (pInfo(peer)->gp) {
											gamepacket_t p;
											p.Insert("OnPaw2018SkinColor1Changed");
											p.Insert(1);
											p.CreatePacket(peer);
											{
												gamepacket_t p;
												p.Insert("OnPaw2018SkinColor2Changed");
												p.Insert(1);
												p.CreatePacket(peer);
											}
											complete_gpass_task(peer, "Claim 4,000 gems");
										}
										if (newtime.tm_hour >= 12 && newtime.tm_hour < 20) {
											if (not has_playmod2(pInfo(peer), 150)) add_playmod(peer, 150);
										}
										else has_playmod2(pInfo(peer), 150, 1);
										if ((not pInfo(peer)->temp_password.empty() && pInfo(peer)->temp_password == pInfo(peer)->tankIDPass) or pInfo(peer)->new_pass == false) {
											gamepacket_t p;
											p.Insert("OnDialogRequest");
											p.Insert("set_default_color|`o\nadd_label_with_icon|big|`0Account Security``|left|1424|\nadd_spacer|small|\nadd_textbox|`6Information``|left|\nadd_smalltext|A hacker may attempt to access your account more than once over a period of time.|left|\nadd_smalltext|Changing your password `2often reduces the risk that they will have frequent access``.|left|\nadd_spacer|small|\nadd_smalltext|Type your new `5password``|left|\nadd_text_input|change|||18|\nend_dialog|change_password||Continue|\n");
											p.CreatePacket(peer);
											pInfo(peer)->temp_password = "";
										}
										else {
											if (pInfo(peer)->gender.empty()) {
												gamepacket_t p;
												p.Insert("OnDialogRequest");
												p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wGender Selection``|left|7612|\nadd_spacer|small|\nadd_textbox|Select your gender:|left|\ntext_scaling_string|5,000ZB|\nadd_button_with_icon|man|Man|noflags|9834|\nadd_button_with_icon|woman|Woman|noflags|9836|\nadd_button_with_icon||END_LIST|noflags|0||\nend_dialog|gender|||");
												p.CreatePacket(peer);
											}
											else news(peer);
										}
									}
								}
								else enet_peer_disconnect_later(peer, 0);
							}
							break;
						}
						if (cch.find("action|itemfavourite") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 4) break;
							int id_ = atoi(explode("\n", t_[3])[0].c_str());
							int got = 0;
							modify_inventory(peer, id_, got);
							if (got == 0) break;
							bool found = false;
							for (int i = 0; i < pInfo(peer)->Fav_Items.size(); i++) {
								if (pInfo(peer)->Fav_Items[i] == id_) {
									pInfo(peer)->Fav_Items.erase(pInfo(peer)->Fav_Items.begin() + i);
									found = true;
									break;
								}
							}
							if (!found) {
								if (pInfo(peer)->Fav_Items.size() >= 20) {
									gamepacket_t p;
									p.Insert("OnTalkBubble"); p.Insert(pInfo(peer)->netID);
									p.Insert("You cannot favorite any more items. Remove some from your list and try again."); p.Insert(0), p.Insert(1);
									p.CreatePacket(peer);
									{
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("You cannot favorite any more items. Remove some from your list and try again.");
										p.CreatePacket(peer);
									}
									break;
								}
								else pInfo(peer)->Fav_Items.push_back(id_);
							}
							gamepacket_t p;
							p.Insert("OnFavItemUpdated");
							p.Insert(id_);
							p.Insert((found ? 0 : 1));
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|account_security\nchange|") != string::npos) {
							string change = cch.substr(57, cch.length() - 58).c_str();
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							if (change == "email") p.Insert("set_default_color|`o\nadd_label_with_icon|big|`0Account Security``|left|1424|\nadd_spacer|small|\nadd_textbox|`6Information``|left|\nadd_smalltext|Having an up-to-date email address attached to your account is a great step toward improved account security.|left|\nadd_smalltext|Email: `5" + pInfo(peer)->email + "``|left|\nadd_spacer|small|\nadd_smalltext|Type your new `5email address``|left|\nadd_text_input|change|||50|\nend_dialog|change_email|OK|Continue|\n");
							else if (change == "password") p.Insert("set_default_color|`o\nadd_label_with_icon|big|`0Account Security``|left|1424|\nadd_spacer|small|\nadd_textbox|`6Information``|left|\nadd_smalltext|A hacker may attempt to access your account more than once over a period of time.|left|\nadd_smalltext|Changing your password `2often reduces the risk that they will have frequent access``.|left|\nadd_spacer|small|\nadd_smalltext|Type your new `5password``|left|\nadd_text_input|change|||18|\nend_dialog|change_password|OK|Continue|\n");
							if (change == "email" or change == "password") p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|change_email\nchange|") != string::npos) {
							string change = cch.substr(53, cch.length() - 54).c_str();
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							if (change .empty()) p.Insert("set_default_color|`o\nadd_label_with_icon|big|`0Account Security``|left|1424|\nadd_spacer|small|\nadd_textbox|`6Information``|left|\nadd_smalltext|Having an up-to-date email address attached to your account is a great step toward improved account security.|left|\nadd_smalltext|Email: `5" + pInfo(peer)->email + "``|left|\nadd_spacer|small|\nadd_smalltext|Type your new `5email address``|left|\nadd_text_input|change|||50|\nend_dialog|change_email|OK|Continue|\n");
							else {
								pInfo(peer)->email = change;
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`0Account Security``|left|1424|\nadd_spacer|small|\nadd_textbox|`6Information``|left|\nadd_smalltext|Having an up-to-date email address attached to your account is a great step toward improved account security.|left|\nadd_smalltext|Your new Email: `5" + pInfo(peer)->email + "``|left|\nadd_spacer|small|\nend_dialog|changedemail|OK||\n");
							}
							p.CreatePacket(peer);

							if (pInfo(peer)->grow4good_email == 0) daily_quest(peer, false, "email", 1);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|change_password\nchange|") != string::npos) {
							string change = cch.substr(56, cch.length() - 57).c_str();
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							if (change .empty()) p.Insert("set_default_color|`o\nadd_label_with_icon|big|`0Account Security``|left|1424|\nadd_spacer|small|\nadd_textbox|`6Information``|left|\nadd_smalltext|A hacker may attempt to access your account more than once over a period of time.|left|\nadd_smalltext|Changing your password `2often reduces the risk that they will have frequent access``.|left|\nadd_spacer|small|\nadd_smalltext|Type your new `5password``|left|\nadd_text_input|change|||18|\nend_dialog|change_password||Continue|\n");
							else {
								{
									pInfo(peer)->temp_password = "";
									gamepacket_t p;
									p.Insert("SetHasGrowID"), p.Insert(1), p.Insert(pInfo(peer)->tankIDName), p.Insert(pInfo(peer)->tankIDPass = change);
									p.CreatePacket(peer);
								}
								pInfo(peer)->new_pass = true;
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`0Account Security``|left|1424|\nadd_spacer|small|\nadd_textbox|`6Information``|left|\nadd_smalltext|A hacker may attempt to access your account more than once over a period of time.|left|\nadd_smalltext|Changing your password `2often reduces the risk that they will have frequent access``.|left|\nadd_smalltext|Your new password: `5" + pInfo(peer)->tankIDPass + "``|left|\nadd_spacer|small|\nend_dialog|changedpassword|OK||\n");
							}
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|change_guild_name\nname|") != string::npos) {
							if (pInfo(peer)->guild_id == 0) break;
						string new_name = cch.substr(56, cch.length() - 56).c_str();
						replaceAll(new_name, "\n", "");
						string check_name = to_lower(new_name);
						bool error_dialog = false;
						for (int i = 0; i < swear_words.size(); i++) {
							if (check_name.find(to_lower(swear_words[i])) != string::npos) {
								error_dialog = true;
								new_name = "`4OOPS:`` `0The name includes bad words.``";
								break;
							}
						}
						for (Guild check_guild_name : guilds) {
							if (to_lower(check_guild_name.guild_name) == check_name) {
								error_dialog = true;
								new_name = "`4OOPS:`` `0The guild with this name already exists.``";
								break;
							}
						}
						if (check_name.size() < 3) {
							error_dialog = true;
							new_name = "`4OOPS:`` `0The guild name is too short.``";
						}
						if (check_name.size() > 15) {
							error_dialog = true;
							new_name = "`4OOPS:`` `0The guild name is too long.``";
						}
						if (special_char(check_name)) {
							error_dialog = true;
							new_name = "`4OOPS:`` `0Guild name can not have special characters.``";
						}
						change_guild_name(peer, new_name, error_dialog);
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|world_swap\nname_box|") != string::npos) {
						string world = cch.substr(53, cch.length() - 54).c_str(), currentworld = pInfo(peer)->world;
						transform(world.begin(), world.end(), world.begin(), ::toupper);
						if (not check_blast(world) || currentworld == world) {
							gamepacket_t p;
							p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSwap World Names``|left|2580|\nadd_textbox|`4World swap failed - you don't own both worlds!``|left|\nadd_smalltext|This will swap the name of the world you are standing in with another world `4permanently``.  You must own both worlds, with a World Lock in place.<CR>Your `wChange of Address`` will be consumed if you press `5Swap 'Em``.|left|\nadd_textbox|Enter the other world's name:|left|\nadd_text_input|name_box|||32|\nadd_spacer|small|\nend_dialog|world_swap|Cancel|Swap 'Em!|"), p.CreatePacket(peer);
						}
						else create_address_world(peer, world, currentworld);
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|name_change\nname_box|") != string::npos) {
						string name = cch.substr(54, cch.length() - 55).c_str();
						if (to_lower(name) != to_lower(pInfo(peer)->tankIDName)) {
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wChange your GrowID``|left|1280|\nadd_textbox|`4The name doesn't match your current name!``|left|\nadd_smalltext|This will change your GrowID `4permanently``.<CR>Your `wBirth Certificate`` will be consumed if you press `5Change It``.<CR>NOTE: The birth certificate only will change your name case (you can not change your whole GrowID)!``|left|\nadd_textbox|Enter your new name:|left|\nadd_text_input|name_box|||32|\nadd_spacer|small|\nend_dialog|name_change|Cancel|Change it!|");
							p.CreatePacket(peer);
						}
						else {
							int remove_ = -1;
							if (modify_inventory(peer, 1280, remove_) == 0) {
								pInfo(peer)->tankIDName = name;
								nick_update(peer, NULL);
							}
						}
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool") != string::npos) {
						if (pInfo(peer)->surgery_started) {
							int count = atoi(cch.substr(59, cch.length() - 59).c_str());
							if (count == 999) end_surgery(peer);
							else load_surgery(peer, count);
						}
						break;
						}
						/*
						else if (cch.find("action|dialog_return\ndialog_name|stuff4toys\namt|") != string::npos) {
						int count = atoi(cch.substr(48, cch.length() - 48).c_str()), item = pInfo(peer)->lastchoosenitem, got = 0;
						modify_inventory(peer, item, got);
						if (got < count || count <=0 || count > 200) break;
						if (items[item].block_possible_put == false || items[item].rarity <= 0 || items[item].rarity > 360) break;
						else {
							int rarity = items[pInfo(peer)->lastchoosenitem].rarity * count;
							pInfo(peer)->winterfest_stuff4toys += rarity;
							if (pInfo(peer)->winterfest_stuff4toys > 100000 && pInfo(peer)->got_4292 == 0) {
								int get_him = 1;
								if (modify_inventory(peer, 4292, get_him) == 0) {
									pInfo(peer)->got_4292 = 1;
									gamepacket_t p2;
									p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("You received `2" + items[4292].ori_name + "``"), p2.Insert(0), p2.Insert(1), p2.CreatePacket(peer);
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage"), p.Insert("No inventory space."), p.CreatePacket(peer);
								}
							}
							if (pInfo(peer)->winterfest_stuff4toys > 250000 && pInfo(peer)->got_4286 == 0) {
								int get_him = 1;
								if (modify_inventory(peer, 4286, get_him) == 0) {
									pInfo(peer)->got_4286 = 1;
									gamepacket_t p2;
									p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("You received `2" + items[4286].ori_name + "``"), p2.Insert(0), p2.Insert(1), p2.CreatePacket(peer);
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage"), p.Insert("No inventory space."), p.CreatePacket(peer);
								}
							}
							if (pInfo(peer)->winterfest_stuff4toys > 500000 && pInfo(peer)->got_4288 == 0) {
								int get_him = 1;
								if (modify_inventory(peer, 4288, get_him) == 0) {
									pInfo(peer)->got_4288 = 1;
									gamepacket_t p2;
									p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("You received `2" + items[4288].ori_name + "``"), p2.Insert(0), p2.Insert(1), p2.CreatePacket(peer);
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage"), p.Insert("No inventory space."), p.CreatePacket(peer);
								}
							}
							if (pInfo(peer)->winterfest_stuff4toys > 1000000 && pInfo(peer)->got_10500 == 0) {
								int get_him = 1;
								if (modify_inventory(peer, 10500, get_him) == 0) {
									pInfo(peer)->got_10500 = 1;
									gamepacket_t p2;
									p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("You received `2" + items[10500].ori_name + "``"), p2.Insert(0), p2.Insert(1), p2.CreatePacket(peer);
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage"), p.Insert("No inventory space."), p.CreatePacket(peer);
								}
							}
							winterfest_rarity += rarity;
							winterfest_rarity_250 += rarity;
							if (winterfest_rarity_250 > 250000) {
								bool give = false;
								int gave_ = 0;
								for (ENetPeer* currentPeer_event = server->peers; currentPeer_event < &server->peers[server->peerCount]; ++currentPeer_event) {
									if (currentPeer_event->state != ENET_PEER_STATE_CONNECTED or currentPeer_event->data == NULL) continue;
									if (gave_ == 0) {
										if (give == false) {
											int get_him = 0;
											modify_inventory(currentPeer_event, 4288, get_him);
											if (get_him == 0) {
												if (modify_inventory(currentPeer_event, 4288, get_him) == 0) give = true, gave_++;
												break;
											}
										}
									}
									else break;
								}
								{
									gamepacket_t p2;
									p2.Insert("OnConsoleMessage"), p2.Insert("`9Stuff-4-Toys has reached 250000/250000 rarity donations and gave a random player Present Goblin Home Edition!");
									for (ENetPeer* currentPeer_event = server->peers; currentPeer_event < &server->peers[server->peerCount]; ++currentPeer_event) {
										if (currentPeer_event->state != ENET_PEER_STATE_CONNECTED or currentPeer_event->data == NULL) continue;
										p2.CreatePacket(currentPeer_event);
									}
								}
								winterfest_total_toys++, winterfest_rarity_250 = 0;
							}
							daily_quest_winterfest(peer, false, "55", rarity);
							gamepacket_t p2;
							p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("Thank you for your generosity!"), p2.Insert(0), p2.Insert(1), p2.CreatePacket(peer);
							int remove = count * -1;
							modify_inventory(peer, item, remove);
						}
						break;
						}*/
						else if (cch.find("action|dialog_return\ndialog_name|compactor\ncount|") != string::npos) {
							int count = atoi(cch.substr(49, cch.length() - 49).c_str()), item = pInfo(peer)->lastchoosenitem, got = 0;
							modify_inventory(peer, item, got);
							if (got < count) break;
							if (items[item].r_1 == 2037 || items[item].r_2 == 2037 || items[item].r_1 == 2035 || items[item].r_2 == 2035 || items[item].r_1 + items[item].r_2 == 0 || items[item].blockType != BlockTypes::CLOTHING || items[item].untradeable || item == 1424 || item == 5816 || items[item].rarity > 200) break;
							else {
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									world_->fresh_world = true;
									string received = "";
									vector<pair<int, int>> receivingitems;
									vector<int> list = { items[item].r_1,  items[item].r_2,  items[item].r_1 - 1 ,  items[item].r_2 - 1 }, random_compactor_rare = { 3178, 2936, 5010, 2644, 2454, 2456, 2458, 2460, 6790, 9004, 11060 };
									for (int i = 0; i < count; i++) {
										if (rand() % items[item].newdropchance < 20) {
											bool dublicate = false;
											int given_item = list[rand() % list.size()];
											for (int i = 0; i < receivingitems.size(); i++) {
												if (receivingitems[i].first == given_item) {
													dublicate = true;
													receivingitems[i].second += 1;
												}
											}
											if (dublicate == false) receivingitems.push_back(make_pair(given_item, 1));
										}
										else if (rand() % 50 < 1) {
											bool dublicate = false;
											int given_item = 0;
											if (rand() % 100 < 1) given_item = random_compactor_rare[rand() % random_compactor_rare.size()];
											else given_item = 2462;
											for (int i = 0; i < receivingitems.size(); i++) {
												if (receivingitems[i].first == given_item) {
													dublicate = true;
													receivingitems[i].second += 1;
												}
											}
											if (dublicate == false) receivingitems.push_back(make_pair(given_item, 1));
										}
										else {
											bool dublicate = false;
											int given_item = 112, given_count = ((items[item].max_gems == 0 ? 5 : rand() % items[item].max_gems)) / 2 + 1;
											if (rand() % 3 < 1) given_item = 856, given_count = 1;
											for (int i = 0; i < receivingitems.size(); i++) {
												if (receivingitems[i].first == given_item) {
													dublicate = true;
													receivingitems[i].second += given_count;
												}
											}
											if (dublicate == false) receivingitems.push_back(make_pair(given_item, given_count));
										}
									}
									int remove = count * -1;
									modify_inventory(peer, item, remove);
									for (int i = 0; i < receivingitems.size(); i++) {
										if (receivingitems.size() == 1) received += to_string(receivingitems[i].second) + " " + (items[item].r_1 == receivingitems[i].first || items[item].r_2 == receivingitems[i].first || items[item].r_2 - 1 == receivingitems[i].first || items[item].r_1 - 1 == receivingitems[i].first ? "`2" + items[receivingitems[i].first].name + "``" : (receivingitems[i].first == 112) ? items[receivingitems[i].first].name : "`1" + items[receivingitems[i].first].name + "``");
										else {
											if (receivingitems.size() - 1 == i)received += "and " + to_string(receivingitems[i].second) + " " + (items[item].r_1 == receivingitems[i].first || items[item].r_2 == receivingitems[i].first || items[item].r_2 - 1 == receivingitems[i].first || items[item].r_1 - 1 == receivingitems[i].first ? "`2" + items[receivingitems[i].first].name + "``" : (receivingitems[i].first == 112) ? items[receivingitems[i].first].name : "`1" + items[receivingitems[i].first].name + "``");
											else if (i != receivingitems.size()) received += to_string(receivingitems[i].second) + " " + (items[item].r_1 == receivingitems[i].first || items[item].r_2 == receivingitems[i].first || items[item].r_2 - 1 == receivingitems[i].first || items[item].r_1 - 1 == receivingitems[i].first ? "`2" + items[receivingitems[i].first].name + "``" : (receivingitems[i].first == 112) ? items[receivingitems[i].first].name : "`1" + items[receivingitems[i].first].name + "``") + ", ";
										}
										int given_count = receivingitems[i].second;
										if (receivingitems[i].first != 112) {
											if (modify_inventory(peer, receivingitems[i].first, given_count) == 0) {
											}
											else {
												WorldDrop drop_block_{};
												drop_block_.id = receivingitems[i].first, drop_block_.count = given_count,  drop_block_.x = (pInfo(peer)->lastwrenchx * 32) + rand() % 17, drop_block_.y = (pInfo(peer)->lastwrenchy * 32) + rand() % 17;
												dropas_(world_, drop_block_);
											}
										}
										else OnSetGems(peer, given_count);
									}
									gamepacket_t p, p2;
									p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`7[``From crushing " + to_string(count) + " " + items[item].name + ", " + pInfo(peer)->tankIDName + " extracted " + received + ".`7]``"), p.Insert(0), p.Insert(0);
									p2.Insert("OnConsoleMessage"), p2.Insert("`7[``From crushing " + to_string(count) + " " + items[item].name + ", " + pInfo(peer)->tankIDName + " extracted " + received + ".`7]``");
									for (ENetPeer* currentPeer_event = server->peers; currentPeer_event < &server->peers[server->peerCount]; ++currentPeer_event) {
										if (currentPeer_event->state != ENET_PEER_STATE_CONNECTED or currentPeer_event->data == NULL or pInfo(currentPeer_event)->world != name_) continue;
										p.CreatePacket(currentPeer_event), p2.CreatePacket(currentPeer_event);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|statsblock\nbuttonClicked|floatingItems\n") != string::npos) {
							send_growscan_floating(peer, "start", "1");
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|statsblock\nbuttonClicked|search_") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 5 || pInfo(peer)->world.empty()) break;
							string type = explode("\n", t_[3])[0].c_str(), search = explode("\n", t_[4])[0].c_str();
							replaceAll(type, "search_", "");
							send_growscan_floating(peer, search, type);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|statsblock\nbuttonClicked|worldBlocks\n") != string::npos || cch.find("action|dialog_return\ndialog_name|statsblockworld\nbuttonClicked|worldBlocks\n") != string::npos) {
							send_growscan_worldblocks(peer, "start", "1");
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|statsblockworld\nbuttonClicked|search_") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 5 || pInfo(peer)->world.empty()) break;
							string type = explode("\n", t_[3])[0].c_str(), search = explode("\n", t_[4])[0].c_str();
							replaceAll(type, "search_", "");
							send_growscan_worldblocks(peer, search, type);
							break;
						}
						if (cch.find("action|dialog_return\ndialog_name|billboard_edit\nbillboard_toggle|") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 8) break;
							bool billboard_active = atoi(explode("\n", t_[3])[0].c_str()), billboard_buying = atoi(explode("\n", t_[4])[0].c_str());
							int billboard_price = atoi(explode("\n", t_[5])[0].c_str());
							bool peritem = atoi(explode("\n", t_[6])[0].c_str()), perlock = atoi(explode("\n", t_[7])[0].c_str());
							bool update_billboard = true;
							if (peritem == perlock or peritem == 0 and perlock == 0 or peritem == 1 and perlock == 1) {
								update_billboard = false;
								console_msg(peer, "You need to pick one pricing method - 'locks per item' or 'items per lock'");
								gamepacket_t p2;
								p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("You need to pick one pricing method - 'locks per item' or 'items per lock'"), p2.Insert(0), p2.Insert(1), p2.CreatePacket(peer);
							}
							else {
								if (peritem == 1) pInfo(peer)->b_w = 1;
								if (perlock == 1) pInfo(peer)->b_w = 0;
							}
							pInfo(peer)->b_bill = to_string(billboard_active) + "," + to_string(billboard_buying);
							if (billboard_price < 0 or billboard_price > 99999) {
								update_billboard = false;
								gamepacket_t  p2;
								console_msg(peer, "Price can't be negative. That's beyond science.");
								p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("Price can't be negative. That's beyond science."), p2.Insert(0), p2.Insert(1), p2.CreatePacket(peer);
							}
							else pInfo(peer)->b_p = billboard_price;
							if (update_billboard && pInfo(peer)->b_p != 0 && pInfo(peer)->b_i != 0) {
								gamepacket_t p(0, pInfo(peer)->netID);
								p.Insert("OnBillboardChange"), p.Insert(pInfo(peer)->netID), p.Insert(pInfo(peer)->b_i), p.Insert(pInfo(peer)->b_bill), p.Insert(pInfo(peer)->b_p), p.Insert(pInfo(peer)->b_w);
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != pInfo(peer)->world) continue;
									p.CreatePacket(currentPeer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|personalize_profile\nbuttonClicked|save") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 5 + (pInfo(peer)->home_world != "" ? 1 : 0)) break;
							pInfo(peer)->display_age = atoi(explode("\n", t_[4])[0].c_str());
							if (pInfo(peer)->home_world != "")pInfo(peer)->display_home = atoi(explode("\n", t_[5])[0].c_str());
							personalize_profile(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|personalize_profile\n") {
							personalize_profile(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|personalize_profile\nbuttonClicked|preview") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 5 + (pInfo(peer)->home_world != "" ? 1 : 0)) break;
							pInfo(peer)->display_age = atoi(explode("\n", t_[4])[0].c_str());
							if (pInfo(peer)->home_world != "") pInfo(peer)->display_home = atoi(explode("\n", t_[5])[0].c_str());
							string personalize = (pInfo(peer)->display_age || pInfo(peer)->display_home ? "\nadd_spacer|small|" : "");
							if (pInfo(peer)->display_age) {
								time_t s__;
								s__ = time(NULL);
								int days_ = int(s__) / (60 * 60 * 24);
								personalize += "\nadd_label|small|`1Account Age:`` " + to_string(days_ - pInfo(peer)->account_created) + " days|left\nadd_spacer|small|";
							}
							if (pInfo(peer)->display_home) {
								if (pInfo(peer)->home_world != "") personalize += "\nadd_label|small|`1Home World:``|left\nadd_button|visit_home_world_" + pInfo(peer)->home_world + "|`$Visit " + pInfo(peer)->home_world + "``|off|0|0|\nadd_spacer|small|";
							}
							string msg2 = "";
							for (int i = 0; i < to_string(pInfo(peer)->level).length(); i++) msg2 += "?";
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|" + (pInfo(peer)->mod == 1 || pInfo(peer)->dev == 1 ? pInfo(peer)->name_color : "`0") + "" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->tankIDName) + "`` `0(```2" + (pInfo(peer)->dev == 1 ? msg2 : to_string(pInfo(peer)->level)) + "```0)``|left|18|" + personalize + "\nadd_button|trade|`wTrade``|off|0|0|\nadd_textbox|(No Battle Leash equipped)|left|\nadd_button|friend_add|`wAdd as friend``|off|0|0|\nadd_button|ignore_player|`wIgnore Player``|off|0|0|\nadd_button|report_player|`wReport Player``|off|0|0|\nend_dialog|personalize_profile||Back|\nadd_quick_exit|");
							p.CreatePacket(peer);
							break;
						}
						if (cch.find("action|dialog_return\ndialog_name|xenonite_edit") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 17) break;
							bool force_double_jump = atoi(explode("\n", t_[3])[0].c_str()), block_double_jump = atoi(explode("\n", t_[4])[0].c_str()), force_high_jump = atoi(explode("\n", t_[5])[0].c_str()),  block_high_jump = atoi(explode("\n", t_[6])[0].c_str()), force_heat_resist = atoi(explode("\n", t_[7])[0].c_str()), block_heat_resist = atoi(explode("\n", t_[8])[0].c_str()),  force_strong_punch = atoi(explode("\n", t_[9])[0].c_str()),  block_strong_punch = atoi(explode("\n", t_[10])[0].c_str()), force_long_punch = atoi(explode("\n", t_[11])[0].c_str()),  block_long_punch = atoi(explode("\n", t_[12])[0].c_str()),  force_speedy = atoi(explode("\n", t_[13])[0].c_str()), block_speedy = atoi(explode("\n", t_[14])[0].c_str()), force_long_build = atoi(explode("\n", t_[15])[0].c_str()),  block_long_build = atoi(explode("\n", t_[16])[0].c_str());

							if (((force_double_jump != 0 && block_double_jump != 0) && force_double_jump == block_double_jump) || ((force_high_jump != 0 && block_high_jump != 0) && force_high_jump == block_high_jump) || ((force_heat_resist != 0 && block_heat_resist != 0) && force_heat_resist == block_heat_resist) || ((force_strong_punch != 0 && block_strong_punch != 0) && force_strong_punch == block_strong_punch) || ((force_long_punch != 0 && block_long_punch != 0) && force_long_punch == block_long_punch) || ((force_speedy != 0 && block_speedy != 0) && force_speedy == block_speedy) || ((force_long_build != 0 && block_long_build != 0) && force_long_build == block_long_build)) {
								console_msg(peer, "The Xenonite Crystal has shifted...");
							}
							else {
								string name_ = pInfo(peer)->world;
								vector<World>::iterator paa = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (paa != worlds.end()) {
									World* world_ = &worlds[paa - worlds.begin()];
									world_->fresh_world = true;
									WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
									if (block_->fg == 2072 && block_access(peer, world_, &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)])) {
										if (force_double_jump) world_->xenonite |= Gtps3::XENONITE_FORCE_DOUBLE_JUMP;
										else world_->xenonite &= ~Gtps3::XENONITE_FORCE_DOUBLE_JUMP;

										if (block_double_jump) world_->xenonite |= Gtps3::XENONITE_BLOCK_DOUBLE_JUMP;
										else world_->xenonite &= ~Gtps3::XENONITE_BLOCK_DOUBLE_JUMP;

										if (force_high_jump) world_->xenonite |= Gtps3::XENONITE_FORCE_HIGH_JUMP;
										else world_->xenonite &= ~Gtps3::XENONITE_FORCE_HIGH_JUMP;

										if (block_high_jump) world_->xenonite |= Gtps3::XENONITE_BLOCK_HIGH_JUMP;
										else world_->xenonite &= ~Gtps3::XENONITE_BLOCK_HIGH_JUMP;

										if (force_heat_resist) world_->xenonite |= Gtps3::XENONITE_FORCE_HEAT_RESIST;
										else world_->xenonite &= ~Gtps3::XENONITE_FORCE_HEAT_RESIST;

										if (block_heat_resist) world_->xenonite |= Gtps3::XENONITE_BLOCK_HEAT_RESIST;
										else world_->xenonite &= ~Gtps3::XENONITE_BLOCK_HEAT_RESIST;

										if (force_strong_punch) world_->xenonite |= Gtps3::XENONITE_FORCE_STRONG_PUNCH;
										else world_->xenonite &= ~Gtps3::XENONITE_FORCE_STRONG_PUNCH;

										if (block_strong_punch) world_->xenonite |= Gtps3::XENONITE_BLOCK_STRONG_PUNCH;
										else world_->xenonite &= ~Gtps3::XENONITE_BLOCK_STRONG_PUNCH;

										if (force_long_punch) world_->xenonite |= Gtps3::XENONITE_FORCE_LONG_PUNCH;
										else world_->xenonite &= ~Gtps3::XENONITE_FORCE_LONG_PUNCH;

										if (block_long_punch) world_->xenonite |= Gtps3::XENONITE_BLOCK_LONG_PUNCH;
										else world_->xenonite &= ~Gtps3::XENONITE_BLOCK_LONG_PUNCH;

										if (force_speedy) world_->xenonite |= Gtps3::XENONITE_FORCE_SPEEDY;
										else world_->xenonite &= ~Gtps3::XENONITE_FORCE_SPEEDY;

										if (block_speedy) world_->xenonite |= Gtps3::XENONITE_BLOCK_SPEEDY;
										else world_->xenonite &= ~Gtps3::XENONITE_BLOCK_SPEEDY;


										if (force_long_build) world_->xenonite |= Gtps3::XENONITE_FORCE_LONG_BUILD;
										else world_->xenonite &= ~Gtps3::XENONITE_FORCE_LONG_BUILD;

										if (block_long_build) world_->xenonite |= Gtps3::XENONITE_BLOCK_LONG_BUILD;
										else world_->xenonite &= ~Gtps3::XENONITE_BLOCK_LONG_BUILD;
										string text = xenonite_text(world_->xenonite);
										gamepacket_t p2;
										p2.Insert("OnConsoleMessage"), p2.Insert(text);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != pInfo(peer)->world) continue;
											pInfo(currentPeer)->xenonite = world_->xenonite;
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(currentPeer)->netID), p.Insert(text), p.Insert(0), p.Insert(1), p.CreatePacket(currentPeer),
											p2.CreatePacket(currentPeer);
											update_clothes(currentPeer, true);
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|dialog_scarf_of_seasons\nbuttonClicked") != string::npos) {
							if (pInfo(peer)->necklace == 11818) pInfo(peer)->i_11818_1 = 0, pInfo(peer)->i_11818_2 = 0;
							update_clothes(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|title_edit\nbuttonClicked|") != string::npos) {
							try {
								vector<string> t_ = explode("|", cch);
								if (t_.size() < 5) break;
								int total = 4; //never change
								if (pInfo(peer)->drtitle) {
									pInfo(peer)->drt = atoi(explode("\n", t_.at(total++)).at(0).c_str());
								}
								if (pInfo(peer)->level >= 125) pInfo(peer)->lvl125 = atoi(explode("\n", t_.at(total++)).at(0).c_str());
								if (pInfo(peer)->gp) {
									pInfo(peer)->donor = atoi(explode("\n", t_.at(total++)).at(0).c_str());
									pInfo(peer)->master = atoi(explode("\n", t_.at(total++)).at(0).c_str());
								}
								if (pInfo(peer)->drlegend) pInfo(peer)->is_legend = atoi(explode("\n", t_.at(total++)).at(0).c_str());
								if (pInfo(peer)->level >= 250) pInfo(peer)->black_color = atoi(explode("\n", t_.at(total++)).at(0).c_str());
								nick_update_2(peer, NULL);
								update_clothes_value(peer);
								update_clothes(peer);
							}
							catch (out_of_range) {
								cout << "ba" << endl;
								break;
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|dialog_scarf_of_seasons\ncheckbox") != string::npos) {
							if (pInfo(peer)->necklace == 11818) {
								vector<string> t_ = explode("|", cch);
								if (t_.size() < 11) break;
								for (int i = 3; i <= 10; i++) {
									if (i <= 6 && atoi(explode("\n", t_[i])[0].c_str()) == 1) pInfo(peer)->i_11818_1 = i - 3;
									else if (atoi(explode("\n", t_[i])[0].c_str()) == 1) pInfo(peer)->i_11818_2 = i - 7;
								}
								update_clothes(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|statsblock\nisStatsWorldBlockUsableByPublic") != string::npos or cch.find("action|dialog_return\ndialog_name|bulletin_edit\nsign_text|\ncheckbox_locked|") != string::npos) {
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [&](const World& a) { return a.name == pInfo(peer)->world; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								world_->fresh_world = true;
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
								if (block_access(peer, world_, block_) == false) break;
								int minimum = 6;
								if (block_->fg == 6016) minimum = 5;
								vector<string> t_ = explode("|", cch);
								if (t_.size() < minimum) break;
								block_->spin = atoi(explode("\n", t_[minimum - 2])[0].c_str());
								block_->invert = atoi(explode("\n", t_[minimum - 1])[0].c_str());
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|camera_edit\ncheckbox_showpick|") != string::npos) {
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [&](const World& a) { return a.name == pInfo(peer)->world; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								world_->fresh_world = true;
								vector<string> t_ = explode("|", cch);
								if (t_.size() < 11) break;
								bool show_item_taking = atoi(explode("\n", t_[3])[0].c_str()), show_item_dropping = atoi(explode("\n", t_[4])[0].c_str()), show_people_entering = atoi(explode("\n", t_[5])[0].c_str()), show_people_exiting = atoi(explode("\n", t_[6])[0].c_str()), dont_show_owner = atoi(explode("\n", t_[7])[0].c_str()), dont_show_admins = atoi(explode("\n", t_[8])[0].c_str()), dont_show_noaccess = atoi(explode("\n", t_[9])[0].c_str()), show_vend_logs = atoi(explode("\n", t_[10])[0].c_str()), changed = false;
								for (int i_ = 0; i_ < world_->cctv_settings.size(); i_++) {
									if (world_->cctv_settings[i_][0] == pInfo(peer)->lastwrenchx and world_->cctv_settings[i_][1] == pInfo(peer)->lastwrenchy) {
										changed = true;
										world_->cctv_settings[i_][2] = show_item_taking;
										world_->cctv_settings[i_][3] = show_item_dropping;
										world_->cctv_settings[i_][4] = show_people_entering;
										world_->cctv_settings[i_][5] = show_people_exiting;
										world_->cctv_settings[i_][6] = dont_show_owner;
										world_->cctv_settings[i_][7] = dont_show_admins;
										world_->cctv_settings[i_][8] = dont_show_noaccess;
										world_->cctv_settings[i_][9] = show_vend_logs;
									}
								}
								if (changed == false) world_->cctv_settings.push_back( { {pInfo(peer)->lastwrenchx}, {pInfo(peer)->lastwrenchy}, {show_item_taking}, {show_item_dropping}, {show_people_entering}, {show_people_exiting}, {dont_show_owner}, {dont_show_admins}, {dont_show_noaccess}, {show_vend_logs}});
							}
						}
						else if (cch.find("action|dialog_return\ndialog_name|camera_edit\nbuttonClicked|clear") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								world_->fresh_world = true;
								for (int i_ = 0; i_ < world_->cctv.size(); i_++)if (world_->cctv[i_].x == pInfo(peer)->lastwrenchx and world_->cctv[i_].y == pInfo(peer)->lastwrenchy) {
									if (i_ != 0) {
										world_->cctv.erase(world_->cctv.begin() + i_);
										i_--;
									}
								}
							}
							{
								send_logs(pInfo(peer)->tankIDName + " cleared cctv in World: [" + pInfo(peer)->world + "]", "CCTV Clear Logs");
								gamepacket_t p;
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`2Camera log cleared.``"), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|b_claimreward") != string::npos) {
							int reward = atoi(cch.substr(72, cch.length() - 72).c_str()), lvl = 0, count = 1;
							vector<int> list{ 6896, 9522, 6948, 1068, 1966, 1836, 5080, 10754, 1874, 6946 };
							if (reward <= 0 || reward > list.size()) break;
							if (list[reward - 1] == 9522) count = 200;
							if (list[reward - 1] == 1068) count = 10;
							if (find(pInfo(peer)->bb_p.begin(), pInfo(peer)->bb_p.end(), lvl = reward * 5) == pInfo(peer)->bb_p.end()) {
								if (pInfo(peer)->bb_lvl >= lvl) {
									if (modify_inventory(peer, list[reward - 1], count) == 0) {
										pInfo(peer)->bb_p.push_back(lvl);
										packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										{
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("Congratulations! You have received your Builder Reward!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
										PlayerMoving data_{};
										data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
										BYTE* raw = packPlayerMoving(&data_);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw;
										{
											PlayerMoving data_{};
											data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16, data_.packetType = 19, data_.plantingTree = 100, data_.punchX = list[reward - 1], data_.punchY = pInfo(peer)->netID;
											int32_t to_netid = pInfo(peer)->netID;
											BYTE* raw = packPlayerMoving(&data_);
											raw[3] = 5;
											memcpy(raw + 8, &to_netid, 4);
											send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											delete[] raw;
										}
										reward_show(peer, "builder");
									}
									else {
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("You have full inventory space!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|autoclave\nbuttonClicked|tool") != string::npos) {
							int itemtool = atoi(cch.substr(61, cch.length() - 61).c_str());
							if (itemtool == 1258 || itemtool == 1260 || itemtool == 1262 || itemtool == 1264 || itemtool == 1266 || itemtool == 1268 || itemtool == 1270 || itemtool == 4308 || itemtool == 4310 || itemtool == 4312 || itemtool == 4314 || itemtool == 4316 || itemtool == 4318) {
								int got = 0;
								modify_inventory(peer, itemtool, got);
								if (got >= 20) {
									pInfo(peer)->lastchoosenitem = itemtool;
									gamepacket_t p;
									p.Insert("OnDialogRequest");
									p.Insert("set_default_color|`o\nadd_label_with_icon|big|`9Autoclave``|left|4322|\nadd_spacer|small|\nadd_textbox|Are you sure you want to destroy 20 " + items[itemtool].ori_name + " in exchange for one of each of the other 12 surgical tools?|left|\nadd_button|verify|Yes!|noflags|0|0|\nend_dialog|autoclave|Cancel||");
									p.CreatePacket(peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|autoclave\nbuttonClicked|verify") != string::npos) {
							int removeitem = pInfo(peer)->lastchoosenitem, inventory_space = 12, slots = get_free_slots(pInfo(peer)), got = 0;
							modify_inventory(peer, removeitem, got);
							if (got >= 20) {
								vector<int> noobitems{ 1262, 1266,1260, 1264, 4314, 4312, 4318, 4308, 1268, 1258, 1270, 4310, 4316 };
								bool toobig = false;
								for (int i_ = 0, remove = 0; i_ < pInfo(peer)->inv.size(); i_++) for (int i = 0; i < noobitems.size(); i++) {
									if (pInfo(peer)->inv[i_].first == noobitems[i]) {
										if (pInfo(peer)->inv[i_].second == 200) toobig = true;
										else inventory_space -= 1;
									}
								}
								gamepacket_t p;
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID);
								if (toobig == false && slots >= inventory_space) {
									modify_inventory(peer, removeitem, got = -20);
									for (int i = 0; i < noobitems.size(); i++) {
										if (noobitems[i] == removeitem) continue;
										modify_inventory(peer, noobitems[i], got = 1);
									}
									gamepacket_t p2;
									p.Insert("[`3I swapped 20 " + items[removeitem].ori_name + " for 1 of every other instrument!``]");
									p2.Insert("OnTalkBubble"), p2.Insert("[`3I swapped 20 " + items[removeitem].name + " for 1 of every other instrument!``]"), p2.Insert(0), p2.Insert(0), p2.CreatePacket(peer);
								}
								else p.Insert("No inventory space!");
								p.Insert(0), p.Insert(1), p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|extractor\nbuttonClicked|extractOnceObj_") != string::npos or cch.find("action|dialog_return\ndialog_name|dynamo\nbuttonClicked|dynamoOnceObj_") != string::npos) {
						int got = 0, uid = 0;
							if (cch.find("action|dialog_return\ndialog_name|extractor\nbuttonClicked|extractOnceObj_") != string::npos) {
								modify_inventory(peer, 6140, got);
								uid = atoi(cch.substr(72, cch.length() - 72).c_str());
							}
							else {
								got = 1;
								uid = atoi(cch.substr(68, cch.length() - 68).c_str());
							}
							if (got >= 1) {
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									world_->fresh_world = true;
									if (block_access(peer, world_, &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)]) == false) break;
									for (int i_ = 0; i_ < world_->drop_new.size(); i_++) {
										if (items[world_->drop_new[i_][0]].untradeable == 0 && world_->drop_new[i_][0] != 0 && world_->drop_new[i_][3] > 0 && world_->drop_new[i_][4] > 0 && world_->drop_new[i_][2] == uid) {
											gamepacket_t p;
											p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID);
											int c_ = world_->drop_new[i_][1];
											if (modify_inventory(peer, world_->drop_new[i_][0], c_) == 0) {
												if (cch.find("action|dialog_return\ndialog_name|extractor\nbuttonClicked|extractOnceObj_") != string::npos) {
													modify_inventory(peer, 6140, got = -1);
												}
												p.Insert("You have extracted " + to_string(world_->drop_new[i_][1]) + " " + items[world_->drop_new[i_][0]].name + ".");
												int32_t to_netid = pInfo(peer)->netID;
												PlayerMoving data_{}, data2_{};
												data_.packetType = 14, data_.netID = 0, data_.plantingTree = world_->drop_new[i_][2];
												data2_.x = world_->drop_new[i_][3], data2_.y = world_->drop_new[i_][4], data2_.packetType = 19, data2_.plantingTree = 250, data2_.punchX = world_->drop_new[i_][0], data2_.punchY = pInfo(peer)->netID;
												BYTE* raw = packPlayerMoving(&data_);
												BYTE* raw2 = packPlayerMoving(&data2_);
												raw2[3] = 5;
												memcpy(raw2 + 8, &to_netid, 4);
												for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
													if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != name_) continue;
													send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
													send_raw(currentPeer, 4, raw2, 56, ENET_PACKET_FLAG_RELIABLE);
												}
												delete[]raw, raw2;
												world_->drop_new.erase(world_->drop_new.begin() + i_);
											}
											else p.Insert("No inventory space.");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
									}
								}
							}
							break;
						}
						if (cch == "action|dialog_return\ndialog_name|world_spray\n") {
							if (has_playmod2(pInfo(peer), 144)) {
								gamepacket_t p;
								p.Insert("OnConsoleMessage");
								p.Insert("`6>> That's sort of hard to do while having a cooldown.``");
								p.CreatePacket(peer);
							}
							else {
								int used_item = pInfo(peer)->lastwrenchb;
								if (used_item == 12600 || used_item == 13574) {
									string name = pInfo(peer)->world;
									vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name](const World& a) { return a.name == name; });
									if (p != worlds.end()) {
										World* world = &worlds[p - worlds.begin()];
										world->fresh_world = true;
										if (to_lower(world->owner_name) == to_lower(pInfo(peer)->tankIDName) || pInfo(peer)->superdev || find(world->admins.begin(), world->admins.end(), to_lower(pInfo(peer)->tankIDName)) != world->admins.end()) {
											int remove = -1;
											if (modify_inventory(peer, used_item, remove) == 0) {
												add_playmod(peer, 144);
												if (used_item == 12600) {
													for (int i_ = 0; i_ < world->blocks.size(); i_++) { if (items[world->blocks[i_].fg].blockType == SEED) world->blocks[i_].planted -= _int64(2.592e+6); }
												}
												else {
													for (int i_ = 0; i_ < world->blocks.size(); i_++) { if (items[world->blocks[i_].fg].blockType == SEED) world->blocks[i_].planted -= 86400; }
												}
												for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
													if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
													if (pInfo(currentPeer)->world == name) {
														pInfo(currentPeer)->spray_x = pInfo(currentPeer)->x;
														pInfo(currentPeer)->spray_y = pInfo(currentPeer)->y;
														exit_(currentPeer, true, true);
														pInfo(currentPeer)->x = pInfo(currentPeer)->spray_x;
														pInfo(currentPeer)->y = pInfo(currentPeer)->spray_y;
														join_world(currentPeer, name, pInfo(currentPeer)->spray_x / 32, pInfo(currentPeer)->spray_y / 32);
														pInfo(currentPeer)->x = pInfo(currentPeer)->spray_x;
														pInfo(currentPeer)->y = pInfo(currentPeer)->spray_y;
														pInfo(currentPeer)->spray_x = 0;
														pInfo(currentPeer)->spray_y = 0;
													}
												}
											}
										}
										else {
											gamepacket_t p;
											p.Insert("OnConsoleMessage"), p.Insert("`wYou must own the world!``"), p.CreatePacket(peer);
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|zombie_back\nbuttonClicked|zomb_price_") != string::npos) {
							int item = atoi(cch.substr(70, cch.length() - 70).c_str());
							if (item <= 0 || item >= items.size() || items[item].zombieprice == 0) continue;
							pInfo(peer)->lockeitem = item;
							int zombie_brain = 0, pile = 0, total = 0;
							modify_inventory(peer, 4450, zombie_brain);
							modify_inventory(peer, 4452, pile);
							total += zombie_brain + (pile * 100);
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							if (total >= items[item].zombieprice) p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSales-Man``|left|4358|\nadd_textbox|" + items[item].name + " costs " + setGems(items[item].zombieprice) + " Zombie Brains. Are you sure you want to buy it? You have " + setGems(total) + " Zombie Brains.|left|\nadd_button|zomb_item_|Yes, please|noflags|0|0|\nadd_button|back|No, thanks|noflags|0|0|\nend_dialog|zombie_purchase|Hang Up||\n");
							else p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSales-Man``|left|4358|\nadd_textbox|" + items[item].name + " costs " + setGems(items[item].zombieprice) + " Zombie Brains. You only have " + setGems(total) + " Zombie Brains so you can't afford it. Sorry!|left|\nadd_button|chc3_1|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|zurgery_back\nbuttonClicked|zurg_price_") != string::npos) {
						int item = atoi(cch.substr(71, cch.length() - 71).c_str());
						if (item <= 0 || item >= items.size() || items[item].surgeryprice == 0) continue;
						pInfo(peer)->lockeitem = item;
						int zombie_brain = 0, pile = 0, total = 0;
						modify_inventory(peer, 4298, zombie_brain);
						modify_inventory(peer, 4300, pile);
						total += zombie_brain + (pile * 100);
						gamepacket_t p;
						p.Insert("OnDialogRequest");
						if (total >= items[item].surgeryprice) p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSales-Man``|left|4358|\nadd_textbox|" + items[item].name + " costs " + setGems(items[item].surgeryprice) + " Caduceus. Are you sure you want to buy it? You have " + setGems(total) + " Caduceus.|left|\nadd_button|zurg_item_|Yes, please|noflags|0|0|\nadd_button|chc4_1|No, thanks|noflags|0|0|\nend_dialog|zurgery_purchase|Hang Up||\n");
						else p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSales-Man``|left|4358|\nadd_textbox|" + items[item].name + " costs " + setGems(items[item].surgeryprice) + " Caduceus. You only have " + setGems(total) + " Caduceus so you can't afford it. Sorry!|left|\nadd_button|chc4_1|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
						p.CreatePacket(peer);
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|wolf_back\nbuttonClicked|wolf_price_") != string::npos) {
						int item = atoi(cch.substr(68, cch.length() - 68).c_str());
						if (item <= 0 || item >= items.size() || items[item].wolfprice == 0) continue;
						pInfo(peer)->lockeitem = item;
						int zombie_brain = 0, pile = 0, total = 0;
						modify_inventory(peer, 4354, zombie_brain);
						modify_inventory(peer, 4356, pile);
						total += zombie_brain + (pile * 100);
						gamepacket_t p;
						p.Insert("OnDialogRequest");
						if (total >= items[item].wolfprice) p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSales-Man``|left|4358|\nadd_textbox|" + items[item].name + " costs " + setGems(items[item].wolfprice) + " Wolf Ticket. Are you sure you want to buy it? You have " + setGems(total) + " Wolf Ticket.|left|\nadd_button|wolf_item_|Yes, please|noflags|0|0|\nadd_button|chc5_1|No, thanks|noflags|0|0|\nend_dialog|wolf_purchase|Hang Up||\n");
						else p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSales-Man``|left|4358|\nadd_textbox|" + items[item].name + " costs " + setGems(items[item].wolfprice) + " Wolf Ticket. You only have " + setGems(total) + " Wolf Ticket so you can't afford it. Sorry!|left|\nadd_button|chc5_1|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
						p.CreatePacket(peer);
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|giantpotogold\namt|") != string::npos) {
							int count = atoi(cch.substr(51, cch.length() - 51).c_str()), got = 0;
							modify_inventory(peer, pInfo(peer)->lastchoosenitem, got);
							if (got <= 0 || count <= 0 || count > items.size()) break;
							int item = pInfo(peer)->lastchoosenitem;
							if (items[item].untradeable == 1 || item == 1424|| item == 5816 || items[item].rarity >= 363 || items[item].rarity == 0 || items[item].rarity < 1 || count > got) {
								gamepacket_t p;
								p.Insert("OnTalkBubble");
								p.Insert(pInfo(peer)->netID);
								if (count > got) p.Insert("You don't have that to give!");
								else p.Insert("I'm sorry, we can't accept items without rarity!");
								p.Insert(0), p.Insert(0);
								p.CreatePacket(peer);
							}
							else {
								pInfo(peer)->b_ra += count * items[item].rarity;
								modify_inventory(peer, pInfo(peer)->lastchoosenitem, count *= -1);
								if (pInfo(peer)->b_ra >= 20000) pInfo(peer)->b_lvl = 2;
								int chance = 29;
								if (pInfo(peer)->b_ra > 25000) chance += 7;
								if (pInfo(peer)->b_ra > 40000) chance += 25;
								if (rand() % 100 < chance && pInfo(peer)->b_ra >= 20000) {
									int give_count = 1, given_count = 1;
									vector<int> list{ 7978,5734, 7986,5724,7980,7990,5730,5726,5728,7988,7992 };
									if (pInfo(peer)->b_ra >= 40000 && rand() % 100 < 15) list = { 7978,5734, 7986,5724,7980,7990,5730,5726,5728,7988,7992, 7996,5718,5720,9418,5732,5722,8000,5740,8002,9414,11728,11730 };
									int given_item = list[rand() % list.size()];
									if (given_item == 7978 || given_item == 5734 || given_item == 7986 || given_item == 5724 || given_item == 7992 || given_item == 7980 || given_item == 7990) give_count = 5, given_count = 5;
									if (given_item == 5730 || given_item == 5726 || given_item == 5728 || given_item == 7988 || given_item == 7980 || given_item == 7990) give_count = 10, given_count = 10;
									if (modify_inventory(peer, given_item, given_count) == 0) {
										gamepacket_t p, p2;
										p.Insert("OnConsoleMessage"), p.Insert(a + "Thanks for your generosity! The pot overflows with `6" + (pInfo(peer)->b_ra < 40000 ? "20" : "40") + ",000 rarity``! Your `6Level 2 prize`` is a fabulous `2" + items[given_item].name + "!``"), p.CreatePacket(peer);
										p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert(a + "Thanks for your generosity! The pot overflows with `6" + (pInfo(peer)->b_ra < 40000 ? "20" : "40") + ",000 rarity``! Your `6Level 2 prize`` is a fabulous `2" + items[given_item].name + "!``"), p2.Insert(0), p2.Insert(0), p2.CreatePacket(peer);
										pInfo(peer)->b_lvl = 1, pInfo(peer)->b_ra = 0;
									}
									else {
										console_msg(peer, "No inventory space.");
									}
								}
								else {
									gamepacket_t p;
									p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("Thank you for your generosity!"), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|donation_box_edit\nbuttonClicked|clear_selected\n") != string::npos) {
							try {
								bool took = false, fullinv = false;
								gamepacket_t p3;
								p3.Insert("OnTalkBubble"), p3.Insert(pInfo(peer)->netID);
								string name_ = pInfo(peer)->world;
								vector<string> t_ = explode("|", cch);
								if (t_.size() < 4) break;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									world_->fresh_world = true;
									WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
									if (not block_access(peer, world_, block_) || items[block_->fg].blockType != BlockTypes::DONATION) break;
									for (int i_ = 0, remove_ = 0; i_ < block_->donates.size(); i_++, remove_++) {
										if (atoi(explode("\n", t_.at(4 + remove_)).at(0).c_str())) {
											int receive = block_->donates[i_].count;
											if (modify_inventory(peer, block_->donates[i_].item, block_->donates[i_].count) == 0) {
												took = true;
												gamepacket_t p;
												p.Insert("OnConsoleMessage"), p.Insert("`7[``" + pInfo(peer)->tankIDName + " receives `5" + to_string(receive) + "`` `w" + items[block_->donates[i_].item].name + "`` from `w" + block_->donates[i_].name + "``, how nice!`7]``");
												block_->donates.erase(block_->donates.begin() + i_), i_--;
												for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
													if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world) continue;
													p.CreatePacket(currentPeer);
												}
											}
											else fullinv = true;
										}
									}
									if (block_->donates.size() == 0) {
										if (block_->flags & 0x00400000) block_->flags ^= 0x00400000;
										PlayerMoving data_{};
										data_.packetType = 5, data_.punchX = pInfo(peer)->lastwrenchx, data_.punchY = pInfo(peer)->lastwrenchy, data_.characterState = 0x8;
										BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, block_));
										BYTE* blc = raw + 56;
										form_visual(blc, *block_, *world_, peer, false);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 112 + alloc_(world_, block_), ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw, blc;
										if (block_->locked) upd_lock(*block_, *world_, peer);
									}
								}
								if (fullinv) {
									p3.Insert("I don't have enough room in my backpack to get the item(s) from the box!");
									gamepacket_t p2;
									p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("`2(Couldn't get all of the gifts)``"), p2.CreatePacket(peer);
								}
								else if (took) p3.Insert("`2Box emptied.``");
								p3.Insert(0), p3.Insert(0);
								p3.CreatePacket(peer);
							}
							catch (out_of_range) {
								break;
							}
							break;
						}
						else if (cch == "action|handle_daily_quest\n") {
						daily_quest(peer, true, "tab_rewards", 0);
						break;
						}
						else if (cch == "action|handle_gems_shop\n") {
						shop_tab(peer, "tab7");
						break;
						}
						else if (cch == "action|warp_player_into_beach_world\n") {
							if (pInfo(peer)->world != "BEACHPARTYGAME") {
								string world = "BEACHPARTYGAME";
								join_world(peer, world);
							}
							break;
						}
						/*
						else if (cch == "action|showubisoftcrateinfopopup\n") {
						gamepacket_t p(500);
						p.Insert("OnDialogRequest");
						p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wUbiToken Hunt!``|left|13158|\nadd_image_button||interface/large/gui_event_banner_ubiweek.rttex||||\nadd_spacer|small|\nadd_textbox|As a community, find `6UbiTokens``, and unlock the `6UbiCrate`` in the store!|left|\nadd_spacer|small|\nadd_textbox|Once enough `6UbiTokens`` have been found by Growtopians, the UbiCrate will become available in the store for `23 hours``.|left|\nadd_spacer|small|\nadd_textbox|`6UbiCrate`` will only remain in the store for a short amount of time, so make sure you check back often! The `6UbiCrate`` can be unlocked multiple times throughout the event.|left|\nadd_spacer|small|\nend_dialog|OpenStore||OK|");
						p.CreatePacket(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|OpenStore\n") {
							shop_tab(peer, "tab1_1");
							break;
						}*/
						/*
						else if (cch == "action|winterrallymenu\n") {
						daily_quest_winterfest(peer);
						break;
						}
						else if (cch == "action|dialog_return\ndialog_name|winterrally_dialog\nbuttonClicked|10538\n\n") {
						gamepacket_t p;
						p.Insert("OnDialogRequest");
						p.Insert("set_default_color|`o\nadd_label_with_icon|big|Winter Wish|left|10538|\nadd_spacer|small|\nadd_textbox|Claim to receive `21 Winter Wish.|left|\nadd_spacer|small|\nadd_textbox|A stocking filled with all your winter wishes and maybe more!|left|\nadd_spacer|small|\nadd_textbox|May all your winter wishes come true...|left|\nadd_quick_exit|");
						p.CreatePacket(peer);
						break;
						}
						else if (cch == "action|dialog_return\ndialog_name|winterrally_dialog\nbuttonClicked|10536\n\n") {
						gamepacket_t p;
						p.Insert("OnDialogRequest");
						p.Insert("set_default_color|`o\nadd_label_with_icon|big|Special Winter Wish|left|10536|\nadd_spacer|small|\nadd_textbox|Claim to receive `21 Special Winter Wish.|left|\nadd_spacer|small|\nadd_textbox|A stocking filled with all your winter wishes and maybe even more!|left|\nadd_spacer|small|\nadd_textbox|Has a `52% chance`` of getting one of these `5Epic`` items:|left|\nadd_label_with_icon|small|Rift Wings|left|11478|\nadd_label_with_icon|small|Infinity Crown|left|12958|\nadd_label_with_icon|small|Rift Cape|left|10424|\nadd_label_with_icon|small|Diamond Dust|left|10412|\nadd_label_with_icon|small|Legendary Lock|left|10410|\nadd_label_with_icon|small|Candy Cane Scythe|left|10498|\nadd_spacer|small|\nadd_textbox|And many more cool and festive items.|left|\nadd_quick_exit|");
						p.CreatePacket(peer);
						break;
						}
						else if (cch == "action|dialog_return\ndialog_name|winterrally_dialog\nbuttonClicked|claim\n\n") {
							if (pInfo(peer)->winterfest_wishes >= 100) {
								if (get_free_slots(pInfo(peer)) >= 2) {
									int got = pInfo(peer)->winterfest_wishes;
									vector<pair<int, int>> prizes{ {10538, 1} };
									if (got >= 200)  prizes.push_back(make_pair(10538, 1));
									if (got >= 300) prizes.push_back(make_pair(10536, 1));
									bool toobig = false;
									for (int i = 0; i < prizes.size(); i++) {
										int have = 0;
										modify_inventory(peer, prizes[i].first, have);
										if (have + prizes[i].second > 200) toobig = true;
									}
									if (toobig) {
										gamepacket_t p;
										p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("Your inventory is full!"), p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
									if (toobig) break;
									vector<pair<int, int>> receivingitems;
									int free = get_free_slots(pInfo(peer)), slot = prizes.size(), getcount = 1;
									if (free >= slot) {
										for (int i = 0; i < slot; i++) {
											int randa = rand() % prizes.size(), itemid = prizes[randa].first, count = prizes[randa].second;
											vector<pair<int, int>>::iterator p_r = find_if(receivingitems.begin(), receivingitems.end(), [itemid](const pair < int, int>& element) { return element.first == itemid; });
											if (p_r != receivingitems.end()) receivingitems[p_r - receivingitems.begin()].second += count;
											else receivingitems.push_back(make_pair(itemid, count));
										}
										string received = "";
										for (int i = 0; i < receivingitems.size(); i++) {
											int itemcount = 0;
											modify_inventory(peer, receivingitems[i].first, itemcount);
											if (itemcount + receivingitems[i].second > 200) toobig = true;
											else received += "`5" + to_string(receivingitems[i].second) + " " + items[receivingitems[i].first].name + "``" + (receivingitems.size() - i == 1 ? "" : " ");
										}
										gamepacket_t p;
										p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID);
										if (toobig == false) {
											if (got >= 100) pInfo(peer)->winterfest_wishes -= 100;
											if (got >= 200) pInfo(peer)->winterfest_wishes -= 100;
											if (got >= 300) pInfo(peer)->winterfest_wishes -= 100;
											for (int i = 0; i < receivingitems.size(); i++) modify_inventory(peer, receivingitems[i].first, receivingitems[i].second);
											p.Insert("`0You got`` " + received + "`0.``");
										}
										else p.Insert("Your inventory is full!");
										p.Insert(0), p.Insert(1), p.CreatePacket(peer);
									}
								}
							}
							break;
						}*/
						/*
						else if (cch == "action|claimdailyreward\n") {
							if (pInfo(peer)->pinata_prize == false) {
								int c_ = 1;
								gamepacket_t p_c;
								p_c.Insert("OnConsoleMessage");
								if (modify_inventory(peer, 9616, c_) == 0) {
									pInfo(peer)->pinata_day = today_day;
									pInfo(peer)->pinata_prize = true;
									pInfo(peer)->pinata_claimed = false;
									gamepacket_t p, p2;
									p.Insert("OnProgressUIUpdateValue"), p.Insert(pInfo(peer)->pinata_claimed ? 1 : 0), p.Insert(pInfo(peer)->pinata_prize ? 1 : 0), p.CreatePacket(peer);
									p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("You got a Block De Mayo Block!"), p2.CreatePacket(peer);
									p_c.Insert("You got a Block De Mayo Block!");
								}
								else  p_c.Insert("You got a Block De Mayo Block!"),
									p_c.CreatePacket(peer);
							}
							break;
						}
						else if (cch == "action|dailyrewardmenu\n") {
						gamepacket_t p(500);
						p.Insert("OnDailyRewardRequest");
						if (pInfo(peer)->pinata_prize) {
							struct tm newtime;
							time_t now = time(0);
							localtime_s(&newtime, &now);
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wBlock De Mayo|left|9616|\nset_default_color|`o\nadd_image_button||interface/large/gui_shop_buybanner.rttex|bannerlayout|flag_frames:4,1,3,0|flag_surfsize:512,200|\nadd_smalltext|`7Get involved and get rewards!`` Smash an Ultra Pinata once a day during `5Cinco de Mayo Week`` and get a daily reward!|left|\nadd_spacer|small|\nadd_button|claimbutton|Come Back Later|noflags|0|0|\nadd_countdown|" + to_string(24 - newtime.tm_hour) + "H" + (60 - newtime.tm_min != 0 ? " " + to_string(60 - newtime.tm_min) + "M" : "") + "|center|disable|\nadd_quick_exit|");
						}
						else {
							if (pInfo(peer)->pinata_claimed) p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wBlock De Mayo|left|9616|\nset_default_color|`o\nadd_image_button||interface/large/gui_shop_buybanner.rttex|bannerlayout|flag_frames:4,1,3,0|flag_surfsize:512,200|\nadd_smalltext|`7Get involved and get rewards!`` Smash an Ultra Pinata once a day during `5Cinco de Mayo Week`` and get a daily reward!|left|\nadd_spacer|small|\nadd_button|claimbutton|CLAIM|noflags|0|0|\nadd_countdown||center|enable|\nadd_quick_exit|");
							else p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wBlock De Mayo|left|9616|\nset_default_color|`o\nadd_image_button||interface/large/gui_shop_buybanner.rttex|bannerlayout|flag_frames:4,1,3,0|flag_surfsize:512,200|\nadd_smalltext|`7Get involved and get rewards!`` Smash an Ultra Pinata once a day during `5Cinco de Mayo Week`` and get a daily reward!|left|\nadd_spacer|small|\nadd_button|claimbutton|Come Back Later|noflags|0|0|\nadd_countdown||center|disable|\nadd_quick_exit|");
						}
						p.CreatePacket(peer);
						break;
						}*/

						/*
						else if (cch == "action|claimgoldengarudachest\n" || cch == "action|showgarudachestprogress\n") {
						gamepacket_t p(500);
						p.Insert("OnDialogRequest");
						p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wGaruda's Glory!``|left|12566|\nadd_spacer|small|\nadd_textbox|During Garuda's Glory, consuming 15 Garuda Chest items will give you a Golden Garuda Chest. |left|\nadd_spacer|small|\nadd_textbox|Both golden and regular Garuda Chest's can be found by completing various activities in game. Wearing Garuda items will increase your chances of finding them!|left|\nadd_spacer|small|\nadd_textbox|Current Progress: " + to_string(pInfo(peer)->garuda) + "/15|left|\nadd_spacer|small|\nadd_textbox|Reward:|left|\nadd_label_with_icon|small|Golden Garuda Chest|left|12568|\nadd_spacer|small|\nadd_button|claimreward|Claim Reward|"+(pInfo(peer)->garuda >= 15 ? "on" : "off") + "|0|0|\nend_dialog|garudaevent_quest||CANCEL|");
						p.CreatePacket(peer);
						break;
						}
						else if (cch == "action|dialog_return\ndialog_name|garudaevent_quest\n") {
						shop_tab(peer, "tab1");
						break;
						}
						else if (cch == "action|dialog_return\ndialog_name|garudaevent_quest\nbuttonClicked|claimreward\n\n") {
							if (pInfo(peer)->garuda >= 15) {
								int c_ = 1;
								gamepacket_t p_c;
								p_c.Insert("OnConsoleMessage");
								if (modify_inventory(peer, 12568, c_) == 0) {
									pInfo(peer)->garuda = 0;
									gamepacket_t p2;
									p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("You received 1 Golden Garuda Chest."), p2.CreatePacket(peer);
									p_c.Insert("You received 1 Golden Garuda Chest.");
								}
								else {
									p_c.Insert("Your inventory is full!");
								}
								p_c.CreatePacket(peer);
							}
							break;
						}*/
						else if (cch == "action|showcincovolcaniccape\n" || cch == "action|showcincovolcanicwings\n" || cch == "action|showcincovolcanicpauldrons\n") {
							gamepacket_t p(500);
							p.Insert("OnDialogRequest");
							if (cch == "action|showcincovolcanicwings\n") p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wVolcanic Ventures : Volcanic Wings``|left|11870|\nadd_spacer|small|\nadd_textbox|Every `224 hours``, a limited amount of `2Volcanic Wings`` will be released into the game!|left|\nadd_spacer|small|\nadd_textbox|For your chance to find one of these `#Rare`` items, smash a `2Lava Pinata``. |left|\nadd_spacer|small|\nadd_textbox|There will only be 48 released every 24 hours so, be quick!|left|\nadd_spacer|small|\nadd_textbox|Did you know there are 48 active Volcanoes in Mexico?|left|\nend_dialog|volcanic_quest||OK|");
							else if (cch == "action|showcincovolcanicpauldrons\n")p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wVolcanic Ventures : Volcanic Pauldrons``|left|13428|\nadd_spacer|small|\nadd_textbox|Every `224 hours``, a limited amount of `2Volcanic Pauldrons`` will be released into the game!|left|\nadd_spacer|small|\nadd_textbox|For your chance to find one of these `#Rare`` items, smash a `2Lava Pinata``. |left|\nadd_spacer|small|\nadd_textbox|There will only be 48 released every 24 hours so, be quick!|left|\nadd_spacer|small|\nadd_textbox|Did you know there are 48 active Volcanoes in Mexico?|left|\nend_dialog|volcanic_quest||OK|");
							else p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wVolcanic Ventures : Volcanic Cape``|left|10806|\nadd_spacer|small|\nadd_textbox|Every `224 hours``, a limited amount of `2Volcanic Cape`` will be released into the game!|left|\nadd_spacer|small|\nadd_textbox|For your chance to find one of these `#Rare`` items, smash a `2Lava Pinata``. |left|\nadd_spacer|small|\nadd_textbox|There will only be 48 released every 24 hours so, be quick!|left|\nadd_spacer|small|\nadd_textbox|Did you know there are 48 active Volcanoes in Mexico?|left|\nend_dialog|volcanic_quest||OK|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch == "action|claimdailyreward\n") {
							if (pInfo(peer)->pinata_prize == false) {
								int c_ = 1;
								gamepacket_t p_c;
								p_c.Insert("OnConsoleMessage");
								if (modify_inventory(peer, 9616, c_) == 0) {
									pInfo(peer)->pinata_day = today_day;
									pInfo(peer)->pinata_prize = true;
									pInfo(peer)->pinata_claimed = false;
									gamepacket_t p, p2;
									p.Insert("OnProgressUIUpdateValue"), p.Insert(pInfo(peer)->pinata_claimed ? 1 : 0), p.Insert(pInfo(peer)->pinata_prize ? 1 : 0), p.CreatePacket(peer);
									p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("You got a Block De Mayo Block!"), p2.CreatePacket(peer);
									p_c.Insert("You got a Block De Mayo Block!");
								}
								else  p_c.Insert("You got a Block De Mayo Block!"),
									p_c.CreatePacket(peer);
							}
							break;
						}
						else if (cch == "action|dailyrewardmenu\n") {
							gamepacket_t p(500);
							p.Insert("OnDailyRewardRequest");
							if (pInfo(peer)->pinata_prize) {
								struct tm newtime;
								time_t now = time(0);
								localtime_s(&newtime, &now);
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wBlock De Mayo|left|9616|\nset_default_color|`o\nadd_image_button||interface/large/gui_shop_buybanner.rttex|bannerlayout|flag_frames:4,1,3,0|flag_surfsize:512,200|\nadd_smalltext|`7Get involved and get rewards!`` Smash an Ultra Pinata once a day during `5Cinco de Mayo Week`` and get a daily reward!|left|\nadd_spacer|small|\nadd_button|claimbutton|Come Back Later|noflags|0|0|\nadd_countdown|" + to_string(24 - newtime.tm_hour) + "H" + (60 - newtime.tm_min != 0 ? " " + to_string(60 - newtime.tm_min) + "M" : "") + "|center|disable|\nadd_quick_exit|");
							}
							else {
								if (pInfo(peer)->pinata_claimed) p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wBlock De Mayo|left|9616|\nset_default_color|`o\nadd_image_button||interface/large/gui_shop_buybanner.rttex|bannerlayout|flag_frames:4,1,3,0|flag_surfsize:512,200|\nadd_smalltext|`7Get involved and get rewards!`` Smash an Ultra Pinata once a day during `5Cinco de Mayo Week`` and get a daily reward!|left|\nadd_spacer|small|\nadd_button|claimbutton|CLAIM|noflags|0|0|\nadd_countdown||center|enable|\nadd_quick_exit|");
								else p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wBlock De Mayo|left|9616|\nset_default_color|`o\nadd_image_button||interface/large/gui_shop_buybanner.rttex|bannerlayout|flag_frames:4,1,3,0|flag_surfsize:512,200|\nadd_smalltext|`7Get involved and get rewards!`` Smash an Ultra Pinata once a day during `5Cinco de Mayo Week`` and get a daily reward!|left|\nadd_spacer|small|\nadd_button|claimbutton|Come Back Later|noflags|0|0|\nadd_countdown||center|disable|\nadd_quick_exit|");
							}
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|transmutated_device_edit\nbuttonClicked|permanttransmutation") != string::npos) {
						if (pInfo(peer)->transmute.size() >= 12) {
							gamepacket_t p;
							p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You have reached the limit of Transmutabooth."), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
							break;
						}
						gamepacket_t p;
						p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wTransmutabooth``|left|9170|\nadd_spacer|small|\nadd_textbox|Tired of how an item looks but want to keep its abilities? Here is where you can overwrite its appearance with that of another item from the SAME slot!|left|\nadd_textbox|Cost: `210,000`` Gems.|left|\nadd_spacer|small|\nadd_textbox|To begin, you'll need to pick the item you want to change (its mods will remain)...|left|\nadd_spacer|small|\nadd_item_picker|mainitemid|`wStart Transmuting!``|Choose the item you want to change!|\nadd_spacer|small|\nadd_button|beforeMainBackToModes|Back|noflags|0|0|\nend_dialog|transmutated_device_edit|Close||\nadd_quick_exit|"), p.CreatePacket(peer);
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|transmutated_device_edit\nmainitemid|") != string::npos) {
						int itemnr = atoi(cch.substr(69, cch.length() - 69).c_str()),  got = inventory_contains(peer, itemnr);
						if (itemnr <= 0 || itemnr > items.size() || got == 0) break;
						if (items[itemnr].blockType != BlockTypes::CLOTHING) {
							pInfo(peer)->transmute_item1 = 0, pInfo(peer)->transmute_item2 = 0;
							gamepacket_t p;
							p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`4" + items[itemnr].ori_name + "`` does not fit in the Transmutabooth."), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
							break;
						}
						vector<pair<int, int>>::iterator pf = find_if(pInfo(peer)->transmute.begin(), pInfo(peer)->transmute.end(), [&](const pair < int, int>& element) { return element.first == itemnr; });
						if (pf != pInfo(peer)->transmute.end()) {
							pInfo(peer)->transmute_item1 = 0, pInfo(peer)->transmute_item2 = 0;
							gamepacket_t p;
							p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`4You can't transmute the same item... that would be silly!``"), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
							break;
						}
						pInfo(peer)->transmute_item1 = itemnr;
						gamepacket_t p;
						p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wTransmutabooth``|left|9170|\nadd_spacer|small|\nadd_textbox|Okay, this is the item that will get NEW visuals - remember, you'll keep its mods, but it'll look different!|left|\nadd_label_with_icon|small|`w" + items[itemnr].ori_name + "``|left|" + to_string(itemnr) + "|\nadd_spacer|small|\nadd_textbox|Now you'll pick the NEW look for this item. Same slot only!|left|\nadd_item_picker|linkitemid|`wPick Transmute Target!``|Choose the item for your NEW look!|\nadd_spacer|small|\nend_dialog|transmutated_linkitem_edit|Close||\nadd_quick_exit|"), p.CreatePacket(peer);
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|transmutated_linkitem_edit\nlinkitemid|") != string::npos) {
						int itemnr = atoi(cch.substr(71, cch.length() - 71).c_str()), got = inventory_contains(peer, itemnr);
						if (itemnr <= 0 || itemnr > items.size() || got == 0) break;
						if (itemnr == pInfo(peer)->transmute_item1) {
							pInfo(peer)->transmute_item1 = 0, pInfo(peer)->transmute_item2 = 0;
							gamepacket_t p;
							p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`4You can't transmute the same item... that would be silly!``"), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
							break;
						}
						else if (items[pInfo(peer)->transmute_item1].clothType != items[itemnr].clothType) {
							pInfo(peer)->transmute_item1 = 0, pInfo(peer)->transmute_item2 = 0;
							gamepacket_t p;
							p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`4"+items[itemnr].ori_name + "`` does not fit in the Transmutabooth."), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
							break;
						}
						pInfo(peer)->transmute_item2 = itemnr;
						gamepacket_t p;
						p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wTransmutabooth``|left|9170|\nadd_spacer|small|\nadd_textbox|This item is about to get a NEW look! |left|\nadd_label_with_icon|small|" + items[pInfo(peer)->transmute_item1].ori_name + "``|left|" + to_string(pInfo(peer)->transmute_item1) + "|\nadd_spacer|small|\nadd_textbox|This is the item it's going to look like:|left|\nadd_label_with_icon|small|" + items[pInfo(peer)->transmute_item2].ori_name + "``|left|" + to_string(pInfo(peer)->transmute_item2) + "|\nadd_spacer|small|\nadd_textbox|`4Warning:`` There are no mods on your target - this transmute is kind of a waste! Are you SURE you want to proceed?``|left|\nadd_spacer|small|\nadd_textbox|Ready to go? Remember, your `4" + items[pInfo(peer)->transmute_item2].ori_name + "`` will be HELD in the Transmutabooth to power this change until you end the transmutation!|left|\nadd_spacer|small|\nadd_button|permanenttransmutation|I know - do it anyway!|noflags|0|0|\nadd_spacer|small|\nadd_button|fromBothItemBackToModes|Back|noflags|0|0|\nend_dialog|transmutated_final_edit|Cancel||\nadd_quick_exit|"), p.CreatePacket(peer);
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|transmutated_final_edit\nbuttonClicked|permanenttransmutation") != string::npos) {
						if (items[pInfo(peer)->transmute_item1].clothType != items[pInfo(peer)->transmute_item2].clothType or pInfo(peer)->transmute_item2 == pInfo(peer)->transmute_item1) {
							pInfo(peer)->transmute_item1 = 0, pInfo(peer)->transmute_item2 = 0;
							gamepacket_t p;
							p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`4You can't transmute the that item... that would be silly!``"), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
							break;
						}
						if (pInfo(peer)->gems >= 10000) {
							int got1 = 0, got2 = 0;
							modify_inventory(peer, pInfo(peer)->transmute_item1, got1);
							modify_inventory(peer, pInfo(peer)->transmute_item2, got2);
							if (got1 == 0 or got2 == 0) break;
							int remove = -1;
							modify_inventory(peer, pInfo(peer)->transmute_item2, remove);
							OnSetGems(peer, 10000 * -1);
							pInfo(peer)->transmute.push_back(make_pair(pInfo(peer)->transmute_item1, pInfo(peer)->transmute_item2));
							pInfo(peer)->transmute_item1 = 0, pInfo(peer)->transmute_item2 = 0;
							update_clothes_value(peer);
							update_clothes(peer);
						}
						else {
							talk_bubble(peer, pInfo(peer)->netID, "You don't have enough gems!", 0, false);
						}
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|transmutated_device_edit\nbuttonClicked|permanentlist") != string::npos) {
						string stuff = pInfo(peer)->transmuted;
						replaceAll(stuff, ":", ",");
						gamepacket_t p;
						p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wTransmutabooth``|left|9170|\nadd_spacer|small|\nadd_textbox|You have transmuted `2" + to_string(pInfo(peer)->transmute.size()) + "/12`` clothing items.|left|\nadd_label_with_icon_button_list|small|`w%s: Transmuted %s to %s|left|removetransmutation_|itemID_transID|" + stuff + "\nadd_spacer|small|\nadd_button|fromListBackToModes|Back|noflags|0|0|\nend_dialog|transmutated_device_edit|Close||\nadd_quick_exit|"), p.CreatePacket(peer);
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|transmutated_device_edit\nbuttonClicked|transmutationhelp") != string::npos) {
						gamepacket_t p;
						p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wHELP! Transmutabooth``|left|9170|\nadd_spacer|small|\nadd_textbox|Here you will find all the necessary details about the Transmutabooth.|left|\nadd_spacer|small|\nadd_smalltext|#1 The Transmutabooth lets you transfer the visuals of one clothing item onto another (of the same slot)! The transmuted item will keep its properties (and mods!).|left|\nadd_spacer|small|\nadd_smalltext|#2 You can transmute clothing items in two ways:|left|\nadd_label_with_icon|small|`5Permanent Transmutes``|left|9230|\nadd_smalltext|     - Lets you permanently change the visuals of clothing!|left|\nadd_smalltext|     - Cost is `210,000 Gems.``|left|\nadd_smalltext|     - The item will get NEW visuals - remember, it will keep its mods, but it'll look different!|left|\nadd_smalltext|     - You first pick the item you want to change, you then pick a Transmute Target to give the item a NEW look!|left|\nadd_smalltext|     - If you attempt to overwrite a permanently-transmuted item, example: Fairy Wings transmuted to look like the Backpack, but you then decide to transmute the Fairy Wings to Ripper Wings!|left|\nadd_smalltext|          - `4Alert!`` This item already has a permanent transmutation! Are you SURE you want to overwrite it?|left|\nadd_smalltext|     - Permanent Transmutes are designated with a 'purple circle' icon in the inventory.|left|\nadd_spacer|small|\nadd_button|fromHelpBackToModes|Back|noflags|0|0|\nend_dialog|transmutated_device_edit|Close||\nadd_quick_exit|"), p.CreatePacket(peer);
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|transmutated_device_edit\nbuttonClicked|removetransmutation_") != string::npos) {
						int item_removal = atoi(cch.substr(92, cch.length() - 92).c_str());
						vector<pair<int, int>>::iterator p = find_if(pInfo(peer)->transmute.begin(), pInfo(peer)->transmute.end(), [&](const pair < int, int>& element) { return element.first == item_removal; });
						if (p != pInfo(peer)->transmute.end()) {
							int item1 = pInfo(peer)->transmute[p - pInfo(peer)->transmute.begin()].first, item2 = pInfo(peer)->transmute[p - pInfo(peer)->transmute.begin()].second;

							pInfo(peer)->remove_transmute = item_removal;
							gamepacket_t p;
							p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wTransmutabooth``|left|9170|\nadd_spacer|small|\nadd_textbox|Done with this transmutation? Want your item back? Here's where you can break them apart!|left|\nadd_textbox|You have transmuted `2" + to_string(pInfo(peer)->transmute.size()) + "/12`` clothing items.|left|\nadd_spacer|small|\nadd_textbox|Here's the old item that got a new look:|left|\nadd_label_with_icon|small|" + items[item1].ori_name + "``|left|" + to_string(item1) + "|\nadd_spacer|small|\nadd_textbox|Here's the item that gave its look (and is held here):|left|\nadd_label_with_icon|small|" + items[item2].ori_name + "``|left|" + to_string(item2) + "|\nadd_spacer|small|\nadd_textbox|`4Warning:`` Are you sure you want to end this transmutation? Your `2" + items[item1].ori_name + "`` will go back to normal, and your `2" + items[item2].ori_name + "`` will be returned to your backpack!``|left|\nadd_spacer|small|\nadd_button|ConfirmRemoveTransmutation|END TRANSMUTATION!|noflags|0|0|\nend_dialog|remove_transmutated_dialog|Close|Back|\nadd_quick_exit|"), p.CreatePacket(peer);

						}
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|remove_transmutated_dialog\nbuttonClicked|ConfirmRemoveTransmutation") != string::npos) {
						int item_removal = pInfo(peer)->remove_transmute;
						vector<pair<int, int>>::iterator p = find_if(pInfo(peer)->transmute.begin(), pInfo(peer)->transmute.end(), [&](const pair < int, int>& element) { return element.first == item_removal; });
						if (p != pInfo(peer)->transmute.end()) {
							int got1 = 1, give_back = pInfo(peer)->transmute[p - pInfo(peer)->transmute.begin()].second;
							if (modify_inventory(peer, give_back, got1) == 0) {
								int i = p - pInfo(peer)->transmute.begin();
								pInfo(peer)->transmute.erase(pInfo(peer)->transmute.begin() + i);
								pInfo(peer)->temp_transmute = true;
								update_clothes_value(peer);
								update_clothes(peer);
								gamepacket_t p;
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You have removed the transmutation!"), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
							}
							else {
								console_msg(peer, "No inventory space.");
							}
						}
						break;
						}
						/*
						else if (cch.find("action|dialog_return\ndialog_name|valentines_quest\nbuttonClicked|claimreward") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								if (pInfo(peer)->booty_broken >= 100) {
									WorldDrop drop_block_{};
									pInfo(peer)->booty_broken -= 100;
									{
										gamepacket_t p;
										p.Insert("OnProgressUIUpdateValue"), p.Insert(pInfo(peer)->booty_broken), p.Insert(0);
										p.CreatePacket(peer);
									}
									int c_ = 1;
									if (modify_inventory(peer, 9350, c_) != 0) {
										drop_block_.id = 9350, drop_block_.count = 1, drop_block_.x = pInfo(peer)->x + rand() % 17, drop_block_.y = pInfo(peer)->y + rand() % 17;
										dropas_(world_, drop_block_);
									}
									gamepacket_t p, p2;
									p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You received " + items[9350].name + "!"), p.Insert(0), p.Insert(1), p.CreatePacket(peer);
									p2.Insert("OnConsoleMessage"), p2.Insert("You received " + items[9350].name + "!"), p2.CreatePacket(peer);
									PlayerMoving data_{};
									data_.packetType = 19, data_.plantingTree = 0, data_.netID = 0, data_.punchX = 9350, data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16;
									int32_t to_netid = pInfo(peer)->netID;
									BYTE* raw = packPlayerMoving(&data_);
									raw[3] = 5;
									memcpy(raw + 8, &to_netid, 4);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != world_->name) continue;
										send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									}
									delete[]raw;
								}
							}
							break;
						}*/
						else if (cch.find("action|dialog_return\ndialog_name|wishing_well\nbuttonClicked|wishing_well") != string::npos) {
						string name_ = pInfo(peer)->world;
						vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
						if (p != worlds.end()) {
							World* world_ = &worlds[p - worlds.begin()];
							world_->fresh_world = true;
							WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
							if (not block_access(peer, world_, block_) or block_->fg != 10656 or block_->shelf_1 >= 200) break;
							int got = 0;
							modify_inventory(peer, 3402, got);
							if (got >= 5) {
								got = 5;
								int remove_ = -1 * got;
								modify_inventory(peer, 3402, remove_);
								block_->shelf_1 += got;
								drop_valentine_box(peer, world_, block_, pInfo(peer)->lastwrenchx, pInfo(peer)->lastwrenchy, true, 5);
								if (block_->shelf_1 >= 200) {
									block_->planted = 0;
									PlayerMoving data_{};
									data_.packetType = 5, data_.punchX = pInfo(peer)->lastwrenchx, data_.punchY = pInfo(peer)->lastwrenchy, data_.characterState = 0x8;
									int alloc = alloc_(world_, block_);
									BYTE* raw = packPlayerMoving(&data_, 112 + alloc);
									BYTE* blc = raw + 56;
									form_visual(blc, *block_, *world_, peer, false);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(currentPeer)->world == world_->name) {
											send_raw(currentPeer, 4, raw, 112 + alloc, ENET_PACKET_FLAG_RELIABLE);
										}
									}
									delete[] raw, blc;
									if (block_->locked) upd_lock(*block_, *world_, peer);
								}
							}
						}
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|5958") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() == 7) {
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									world_->fresh_world = true;
									WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
									if (not block_access(peer, world_, block_) or block_->fg != 5958) break;
									int minutes = atoi(explode("\n", t_[6])[0].c_str());
									if (minutes < 1 || minutes > 60) {
										gamepacket_t p;
										p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("It is either too long or too low."), p.Insert(0), p.Insert(0), p.CreatePacket(peer);

									}
									else block_->shelf_4 = minutes;
									block_->shelf_1 = atoi(explode("\n", t_[3])[0].c_str());
									block_->shelf_2 = atoi(explode("\n", t_[4])[0].c_str());
									block_->shelf_3 = atoi(explode("\n", t_[5])[0].c_str());
									if (block_->shelf_1 != 0 || block_->shelf_2 != 0 || block_->shelf_3 != 0) {
										bool found_ = false;
										for (int i_ = 0; i_ < world_->machines.size(); i_++) {
											WorldMachines* machine_ = &world_->machines[i_];
											if (machine_->x == pInfo(peer)->lastwrenchx and machine_->y == pInfo(peer)->lastwrenchy) {
												machine_->enabled = block_->enabled;
												machine_->target_item = block_->shelf_4;
												block_->pr = 1;
												found_ = true;
												break;
											}
										}
										if (not found_) {
											WorldMachines new_machine;
											new_machine.enabled = block_->enabled;
											new_machine.x = pInfo(peer)->lastwrenchx, new_machine.y = pInfo(peer)->lastwrenchy;
											new_machine.id = block_->fg;
											new_machine.target_item = block_->shelf_4;
											block_->pr = 1;
											world_->machines.push_back(new_machine);
											if (find(World_Stuff.t_worlds.begin(), World_Stuff.t_worlds.end(), world_->name) == World_Stuff.t_worlds.end()) {
												World_Stuff.t_worlds.push_back(world_->name);
											}
										}
									}
								}
							}
							break;
						}
						else if (cch == "action|crypto\n") {
						send_crypto(peer, 500);
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|logs_search") != string::npos || cch.find("action|dialog_return\ndialog_name|logs\nbuttonClicked|") != string::npos) {
							if (pInfo(peer)->logger) {
								string button = "", search = "";
								vector<string> t_ = explode("|", cch);
								if (t_.size() > 3) {
									button = explode("\n", t_[3])[0].c_str();
									if (button == "Empty Logs" && to_lower(pInfo(peer)->tankIDName) == "tron") Server_Security.logs.clear();
								}
								if (t_.size() > 5) {
									search = explode("\n", t_[5])[0].c_str();
									if (search == "Next page") {
										pInfo(peer)->search_page += 20;
										search = "";
									}
									else if (search == "Previous page") {
										pInfo(peer)->search_page -= 20;
										if (pInfo(peer)->search_page < 20) pInfo(peer)->search_page = 20;
										search = "";
									}
								}
								send_logs_page(peer, button, search);
							}
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|zz\nbuttonClicked|guild_event\n\n") {
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Top Guilds - " + items[event_item].hand_scythe_text + "``|left|6012|\nadd_spacer|\nadd_smalltext|" + items[event_item].description + "|\nadd_spacer|" + top_guild_list + "\nadd_spacer|\nadd_textbox|Event Time remaining: `2" + to_playmod_time(current_event - time(nullptr)) + "``|\nadd_spacer|" + pInfo(peer)->guild_event + "\nadd_spacer|\nadd_button|old_leaderboard_guild|`0Past Event Leaderboard``|noflags|0|0|\nadd_button|event_rewards_guild|`0Event Rewards``|noflags|0|0|\nend_dialog|zz|Close||\nadd_quick_exit|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|zz\nbuttonClicked|old_leaderboard_guild\n\n") {
						gamepacket_t p;
						p.Insert("OnDialogRequest");
						p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0" + items[old_event_item].hand_scythe_text + " Guild Winners``|left|6012|\nadd_spacer|\nadd_smalltext|" + items[old_event_item].description + "|\nadd_spacer|" + top_old_guild_winners + "\nadd_spacer|\nadd_textbox|Get ready for the next event, next event will be `2" + items[get_next_event()].hand_scythe_text + "`` (`5starting soon``).|\nend_dialog|zz|Close||\nadd_quick_exit|");
						p.CreatePacket(peer);
						break;
						}
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|ancientdialog\n\n") {
						int b = b = pInfo(peer)->ances;
						gamepacket_t p;
						p.Insert("OnDialogRequest");
						p.Insert("set_default_color|`o\nadd_label_with_icon|big|`9Ancient Goddess|left|5086|\nadd_textbox|`9You are upgrading ances to " + items[ancesupgradeto(peer, b)].name + "\nadd_textbox|`$I will gift a part of my power to enhance your miraculous|\nadd_textbox|`$device, but this exchange, you must bring me the answers|\nadd_textbox|`$to my riddles:|\nadd_spacer|small|" + ancientdialog(peer, b));
						p.CreatePacket(peer);
						}
						else if (cch == "action|dialog_return\ndialog_name|tolol12\nbuttonClicked|ancientaltar\n\n") {
						int b = b = pInfo(peer)->ances;
						int c = 0, loler12 = 0, ex = 0, n = ancesupgradeto(peer, b), d = ancientprice(b);

						if (modify_inventory(peer, n, ex += 1) == -1) {
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`9Ancient Goddess|left|5086|\nadd_spacer|small|\nadd_textbox|`$Something wrong...|\nadd_spacer|small|\nend_dialog||Return|");
							p.CreatePacket(peer);
							packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
						}
						else {
							modify_inventory(peer, 1796, d);
							modify_inventory(peer, b, loler12 -= 1);
							equip_clothes(peer, n);
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`9Ancient Goddess|left|5086|\nadd_spacer|small|\nadd_textbox|`6You've pleased me, clever one.|\nadd_spacer|small|\nend_dialog||Return|");
							p.CreatePacket(peer);
							gamepacket_t p2(0, pInfo(peer)->netID);
							p2.Insert("OnPlayPositioned");
							p2.Insert("audio/change_clothes.wav");
							p2.CreatePacket(peer);
						}
						}
						else if (cch.find("action|dialog_return\ndialog_name|dnaproc") != string::npos) {
						string name_ = pInfo(peer)->world;
						vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
						if (p != worlds.end()) {
							World* world_ = &worlds[p - worlds.begin()];
							world_->fresh_world = true;
							WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
							if (not block_access(peer, world_, block_)) break;
							int DNAID;
							int remove = 0 - 1;
							int add = 1;
							if (cch.find("tilex|") != string::npos and cch.find("tiley|") != string::npos) {
								int x_ = atoi(explode("\n", explode("tilex|", cch)[1])[0].c_str()), y_ = atoi(explode("\n", explode("tiley|", cch)[1])[0].c_str());
								std::stringstream ss(cch);
								std::string to;
								try {
									while (std::getline(ss, to, '\n')) {
										vector<string> infoDat = explode("|", to);
										if (infoDat.at(0) == "choose") {
											DNAID = atoi(infoDat.at(1).c_str());
											if (items[DNAID].name.find("Dino DNA Strand") != string::npos || items[DNAID].name.find("Plant DNA Strand") != string::npos || items[DNAID].name.find("Raptor DNA Strand") != string::npos) {
												if (block_->shelf_4 == 0) {
													block_->shelf_1 = DNAID;
													block_->shelf_4 = 1;
													modify_inventory(peer, DNAID, remove);
													SendDNAProcessor(peer, x_, y_, false, true, false, 0, true, false);
												}
												else if (block_->shelf_4 == 1) {
													block_->shelf_2 = DNAID;
													block_->shelf_4 = 2;
													modify_inventory(peer, DNAID, remove);
													SendDNAProcessor(peer, x_, y_, false, true, false, 0, true, false);
												}
												else if (block_->shelf_4 == 2) {
													block_->shelf_3 = DNAID;
													block_->shelf_4 = 3;
													modify_inventory(peer, DNAID, remove);
													int DnaNumber1 = 0, DnaNumber2 = 0, DnaNumber3 = 0, What = 0;
													ifstream infile("db/DnaRecipe.txt");
													for (string line; getline(infile, line);) {
														if (line.length() > 3 && line.at(0) != '/' && line.at(1) != '/') {
															auto ex = explode("|", line);
															int id1 = atoi(ex.at(0).c_str());
															int id2 = atoi(ex.at(1).c_str());
															int id3 = atoi(ex.at(2).c_str());
															if (id1 == block_->shelf_1 && id2 == block_->shelf_2 && id3 == block_->shelf_3) {
																DnaNumber1 = atoi(ex.at(0).c_str());
																DnaNumber2 = atoi(ex.at(1).c_str());
																DnaNumber3 = atoi(ex.at(2).c_str());
																What = atoi(ex.at(3).c_str());
																break;
															}
														}
													}
													infile.close();
													if (block_->shelf_1 == DnaNumber1 && block_->shelf_2 == DnaNumber2 && block_->shelf_3 == DnaNumber3 && DnaNumber3 != 0 && DnaNumber2 != 0 && DnaNumber1 != 0 && What != 0) {
														SendDNAProcessor(peer, x_, y_, false, true, false, 0, true, false);
													}
													else {
														if (block_->shelf_4 >= 1) {
															SendDNAProcessor(peer, x_, y_, false, true, false, 0, true, true);
														}
														else {
															SendDNAProcessor(peer, x_, y_, false, false, false, 0, true, true);
														}
													}
												}
											}
											else {
												if (block_->shelf_4 >= 1) {
													SendDNAProcessor(peer, x_, y_, true, false, false, 0, true, false);
												}
												else {
													SendDNAProcessor(peer, x_, y_, true, false, false, 0, false, false);
												}
											}
										}
									}
								}
								catch (const std::out_of_range& e) {
									std::cout << e.what() << std::endl;
								}
								if (cch.find("buttonClicked|remove0") != string::npos) {
									if (block_->shelf_4 == 1) {
										int DNARemoved = block_->shelf_1;
										modify_inventory(peer, DNARemoved, add);
										block_->shelf_1 = 0;
										block_->shelf_4 = 0;
										SendDNAProcessor(peer, x_, y_, false, false, true, DNARemoved, false, false);
									}
									if (block_->shelf_4 == 2) {
										int DNARemoved = block_->shelf_1;
										modify_inventory(peer, DNARemoved, add);
										block_->shelf_1 = block_->shelf_2;
										block_->shelf_2 = 0;
										block_->shelf_4 = 1;
										SendDNAProcessor(peer, x_, y_, false, false, true, DNARemoved, true, false);
									}
									if (block_->shelf_4 == 3) {
										int DNARemoved = block_->shelf_1;
										modify_inventory(peer, DNARemoved, add);
										block_->shelf_1 = block_->shelf_2;
										block_->shelf_2 = block_->shelf_3;
										block_->shelf_3 = 0;
										block_->shelf_4 = 2;
										SendDNAProcessor(peer, x_, y_, false, false, true, DNARemoved, true, false);
									}
								}
								if (cch.find("buttonClicked|remove1") != string::npos) {
									if (block_->shelf_4 == 2) {
										int DNARemoved = block_->shelf_2;
										modify_inventory(peer, DNARemoved, add);
										block_->shelf_2 = 0;
										block_->shelf_4 = 1;
										SendDNAProcessor(peer, x_, y_, false, false, true, DNARemoved, true, false);
									}
									if (block_->shelf_4 == 3) {
										int DNARemoved = block_->shelf_2;
										modify_inventory(peer, DNARemoved, add);
										block_->shelf_2 = block_->shelf_3;
										block_->shelf_3 = 0;
										block_->shelf_4 = 2;
										SendDNAProcessor(peer, x_, y_, false, false, true, DNARemoved, true, false);
									}
								}
								if (cch.find("buttonClicked|remove2") != string::npos) {
									if (block_->shelf_4 == 3) {
										int DNARemoved = block_->shelf_3;
										modify_inventory(peer, DNARemoved, add);
										block_->shelf_3 = 0;
										block_->shelf_4 = 2;
										SendDNAProcessor(peer, x_, y_, false, false, true, DNARemoved, true, false);
									}
								}
								if (cch.find("buttonClicked|combine") != string::npos) {
									if (block_->shelf_4 == 3) {
										int DnaNumber1 = 0, DnaNumber2 = 0, DnaNumber3 = 0, What;
										ifstream infile("db/DnaRecipe.txt");
										for (string line; getline(infile, line);) {
											if (line.length() > 3 && line.at(0) != '/' && line.at(1) != '/') {
												auto ex = explode("|", line);
												int id1 = atoi(ex.at(0).c_str());
												int id2 = atoi(ex.at(1).c_str());
												int id3 = atoi(ex.at(2).c_str());
												if (id1 == block_->shelf_1 && id2 == block_->shelf_2 && id3 == block_->shelf_3) {
													DnaNumber1 = atoi(ex.at(0).c_str());
													DnaNumber2 = atoi(ex.at(1).c_str());
													DnaNumber3 = atoi(ex.at(2).c_str());
													What = atoi(ex.at(3).c_str());
													break;
												}
											}
										}
										infile.close();
										if (block_->shelf_1 == DnaNumber1 && block_->shelf_2 == DnaNumber2 && block_->shelf_3 == DnaNumber3 && DnaNumber3 != 0 && DnaNumber2 != 0 && DnaNumber1 != 0 && What != 0) {
											int count = 1;//items[What].blockType == BlockTypes::FOREGROUND ? 10 : 5;
											modify_inventory(peer, What, count);
											if (items[What].clothType == ClothTypes::FEET) pInfo(peer)->feet = What;
											else if (items[What].clothType == ClothTypes::HAND) pInfo(peer)->hand = What;
											block_->shelf_1 = 0; block_->shelf_2 = 0; block_->shelf_3 = 0; block_->shelf_4 = 0;
											if (pInfo(peer)->C_QuestActive && pInfo(peer)->C_QuestKind == 15 && pInfo(peer)->C_QuestProgress < pInfo(peer)->C_ProgressNeeded) {
												pInfo(peer)->C_QuestProgress++;
												if (pInfo(peer)->C_QuestProgress >= pInfo(peer)->C_ProgressNeeded) {
													pInfo(peer)->C_QuestProgress = pInfo(peer)->C_ProgressNeeded;
													gamepacket_t p;
													p.Insert("OnTalkBubble");
													p.Insert(pInfo(peer)->netID);
													p.Insert("`9Ring Quest task complete! Go tell the Ringmaster!");
													p.Insert(0), p.Insert(0);
													p.CreatePacket(peer);
												}
											}
											gamepacket_t p, p2;
											p.Insert("OnConsoleMessage"), p.Insert("DNA Processing complete! The DNA combined into a `2" + items[What].name + "``!"), p.CreatePacket(peer);
											p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("DNA Processing complete! The DNA combined into a `2" + items[What].name + "``!"), p2.Insert(0), p2.Insert(0), p2.CreatePacket(peer);
											update_clothes(peer);
											for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
												if (currentPeer->state != ENET_PEER_STATE_CONNECTED || currentPeer->data == NULL) continue;
												if (pInfo(currentPeer)->world == pInfo(peer)->world) {
													{
														gamepacket_t p;
														p.Insert("OnParticleEffect"); p.Insert(44); p.Insert((float)x_ * 32 + 16, (float)y_ * 32 + 16); p.CreatePacket(currentPeer);
													}
													{
														PlayerMoving data_{};
														data_.packetType = 19, data_.plantingTree = 150, data_.netID = pInfo(peer)->netID;
														data_.punchX = What, data_.punchY = What;
														int32_t to_netid = pInfo(peer)->netID;
														BYTE* raw = packPlayerMoving(&data_);
														raw[3] = 3;
														memcpy(raw + 8, &to_netid, 4);
														send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
														delete[]raw;
													}
												}
											}
										}
										else {
											SendDNAProcessor(peer, x_, y_, false, false, false, 0, true, true);
										}

									}
								}
							}
						}
						}
						else if (cch.find("action|dialog_return\ndialog_name|dnaget") != string::npos) {
							if (cch.find("tilex|") != string::npos and cch.find("tiley|") != string::npos and cch.find("item|") != string::npos) {
								int x_ = atoi(explode("\n", explode("tilex|", cch)[1])[0].c_str()), y_ = atoi(explode("\n", explode("tiley|", cch)[1])[0].c_str());
								int item = atoi(explode("\n", explode("item|", cch)[1])[0].c_str());
								int remove = -1;
								if (modify_inventory(peer, item, remove) == 0) {
									int Random = rand() % 100, reward = 0, count = 1;
									vector<int> list{ 4082, 4084, 4086, 4088, 4090, 4092, 4120, 4122, 5488 };
									gamepacket_t p, p2;
									p.Insert("OnTalkBubble"), p2.Insert("OnConsoleMessage"); p.Insert(pInfo(peer)->netID);
									if (Random >= 4 and Random <= 10) {
										reward = list[rand() % list.size()];
										p.Insert("You ground up a " + items[item].name + ", `9and found " + items[reward].name + " inside!``"), p2.Insert("You ground up a " + items[item].name + ", `9and found " + items[reward].name + " inside!``");
										modify_inventory(peer, reward, count);
									}
									else if (Random >= 1 and Random <= 3) {
										gamepacket_t a;
										a.Insert("OnConsoleMessage");
										a.Insert("Wow! You discovered the missing link between cave-rayman and the modern Growtopian.");
										reward = 5488;
										p.Insert("You ground up a " + items[item].name + ", `9and found " + items[reward].name + " inside!``"), p2.Insert("You ground up a " + items[item].name + ", `9and found " + items[reward].name + " inside!``");
										modify_inventory(peer, reward, count);
										a.CreatePacket(peer);
									}
									else {
										p.Insert("You ground up a " + items[item].name + ", `3but any DNA inside was destroyed in the process.``"), p2.Insert("You ground up a " + items[item].name + ", `3but any DNA inside was destroyed in the process.``");
									}
									p.Insert(0), p.Insert(0);
									p2.CreatePacket(peer), p.CreatePacket(peer);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED || currentPeer->data == NULL) continue;
										if (pInfo(currentPeer)->world == pInfo(peer)->world) {
											if (reward != 0) {
												packet_(currentPeer, "action|play_sfx\nfile|audio/bell.wav\ndelayMS|0");
												PlayerMoving data_{};
												data_.packetType = 19, data_.plantingTree = 150, data_.netID = pInfo(peer)->netID;
												data_.punchX = reward, data_.punchY = reward;
												int32_t to_netid = pInfo(peer)->netID;
												BYTE* raw = packPlayerMoving(&data_);
												raw[3] = 3;
												memcpy(raw + 8, &to_netid, 4);
												send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
												delete[]raw;
											}
											else {
												packet_(currentPeer, "action|play_sfx\nfile|audio/ch_start.wav\ndelayMS|0");
											}
										}
									}
								}
							}
						}
						else if (cch.find("action|dialog_return\ndialog_name|pianowings\nbuttonClicked|manual\n") != string::npos) {
						int volume = 50;
						string note = "C-C-D-C-E-F";
						gamepacket_t p;
						p.Insert("OnDialogRequest");
						p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wMusical Wings``|left|10182|\n"
							"add_spacer|small|\n"
							"add_textbox|This wing will play up to 16 music notes. Each note is triggered as you pass over a block.|left|\n"
							"add_textbox|In the `2Volume `obox, enter a volume level for these notes, from 1-100. 100 is the normal volume of music notes.|left|\n"
							"add_textbox|In the `2Notes `obox, enter up to 16 music notes to play. For each note, you enter 2 symbols:|left|\n"
							"add_smalltext|-The note to play, `2A to G`0, as in normal music notation. Lowercase for lower octave, uppercase for higher.|\n"
							"add_smalltext|Spaces are optional, but sure make it easier to read.|\n"
							"add_smalltext|- Last, a `2#`o for a sharp note, a - for a natural note, or a `2b `ofor a flat note.|\n"
							"add_smalltext|Spaces are optional, but sure make it easier to read.|\n"
							"add_text_input|volume|Volume|" + to_string(volume) + "|3|\n"
							"add_text_input|text|Notes|" + note + "|50|\n"
							"add_spacer|small|\nadd_button|resoterdefault|Restore to Default|noflags|0|0|\n"
							"end_dialog|pianowings|Cancel|Update| ");
						p.CreatePacket(peer);
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|pianowings\nbuttonClicked|resoterdefault\n") != string::npos) {
						int volume = 100;
						string note = "C-F-G#G-F-B#A#G-F-G#G-D#G-C-";
						if (note.find_first_not_of("ABCDEFG-#abcdefg") != string::npos) {
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wMusical Wings``|left|10182|\nadd_spacer|small|\nadd_textbox|`4Notes must be from A to G!``|left|\nadd_button|manual|Instructions|noflags|0|0|\nadd_spacer|small|\nadd_text_input|volume|Volume|" + to_string(volume) + "|3|\nadd_text_input|text|Notes|" + note + "|50|\nadd_spacer|small|\nadd_button|resoterdefault|Restore to Default|noflags|0|0|\nend_dialog|pianowings|Cancel|Update|");
							p.CreatePacket(peer);
							break;
						}
						else {
							pInfo(peer)->musical_volume = volume;
							pInfo(peer)->musical_note = note;
							update_clothes(peer);
							gamepacket_t p;
							p.Insert("OnTalkBubble");
							p.Insert(pInfo(peer)->netID);
							p.Insert("`wUpdated Musical Wings!");
							p.Insert(0);
							p.CreatePacket(peer);
						}
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|battlepass_tasks\nbuttonClicked|claim_") != string::npos) {
							if (pInfo(peer)->gp) {
								vector<string> t_ = explode("|", cch);
								if (t_.size() > 3) {
									string button = explode("\n", t_[3])[0].c_str();
									int remove_grow_points = 0;
									int grow_item = 0;
									int grow_item_count = 1;
									if (button == "claim_p2p_150") {
										grow_item = grow_pass_item;
										remove_grow_points = 150;
									}
									else if (button == "claim_p2p_300") {
										grow_item = 13556;
										remove_grow_points = 300;
									}
									else if (button == "claim_p2p_450") {
										grow_item = 10858;
										grow_item_count = 2;
										remove_grow_points = 450;
									}
									else if (button == "claim_p2p_600") {
										grow_item = 1486;
										remove_grow_points = 600;
									}
									else if (button == "claim_p2p_750") {
										grow_item = 2478;
										grow_item_count = 10;
										remove_grow_points = 750;
									}
									else if (button == "claim_p2p_900") {
										grow_item = 2480;
										remove_grow_points = 900;
									}
									else if (button == "claim_f2p_300") {
										grow_item = 10836;
										remove_grow_points = 300;
									}
									else if (button == "claim_f2p_600") {
										grow_item = 10838;
										remove_grow_points = 600;
									}
									else if (button == "claim_f2p_900") {
										grow_item = 1486;
										remove_grow_points = 900;
									}
									else if (button == "claim_p2p_1100") {
										grow_item = 8196;
										grow_item_count = 4;
										remove_grow_points = 1100;
									}
									else if (button == "claim_p2p_1300") {
										grow_item = 1486;
										grow_item_count = 2;
										remove_grow_points = 1300;
									}
									else if (button == "claim_f2p_1300") {
										grow_item = 10838;
										remove_grow_points = 1300;
									}
									else if (button == "claim_p2p_1500") {
										grow_item = 6140;
										remove_grow_points = 1500;
									}
									else if (button == "claim_p2p_1700") {
										grow_item = 2480;
										remove_grow_points = 1700;
									}
									else if (button == "claim_f2p_1700") {
										grow_item = 10836;
										remove_grow_points = 1700;
									}
									else if (button == "claim_p2p_1900") {
										grow_item = 2480;
										remove_grow_points = 1900;
									}
									else if (button == "claim_p2p_2100") {
										grow_item = grow_pass_item+2;
										remove_grow_points = 2100;
									}
									else if (button == "claim_f2p_2100") {
										grow_item = 2992;
										remove_grow_points = 2100;
									}
									if (remove_grow_points != 0) {
										if (pInfo(peer)->growpass_points >= remove_grow_points) {
											if (find(pInfo(peer)->growpass_prizes.begin(), pInfo(peer)->growpass_prizes.end(), button) == pInfo(peer)->growpass_prizes.end()) {
												if (grow_item == 10858) {
													pInfo(peer)->growpass_prizes.push_back(button);
													OnSetVoucher(peer, grow_item_count);
												}
												else {
													int give_now = grow_item_count;
													if (modify_inventory(peer, grow_item, give_now) == 0) {
														pInfo(peer)->growpass_prizes.push_back(button);
														string text = "`9Claimed " + to_string(grow_item_count) + " " + items[grow_item].ori_name + " from Grow Pass rewards!``";
														talk_bubble(peer, pInfo(peer)->netID, text, 0, 0);
														console_msg(peer, text);
													}
													else {
														console_msg(peer, "No inventory space.");
													}
												}
											}
											else {
												gamepacket_t p;
												p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wReward Details (claimed already)`|left|9222|\nadd_spacer|small|\nadd_spacer|small|\nadd_label_with_icon|big| " + to_string(grow_item_count) + " " + items[grow_item].ori_name + "|left|" + to_string(grow_item) + "|\nadd_textbox|" + items[grow_item].description + "|left|\nadd_spacer|small|\nadd_textbox|Earn `2" + to_string(remove_grow_points) + " Grow Pass Points`` and become a `5Royal Grow Pass Member``  to claim this reward.|left|\nadd_spacer|small|\nadd_textbox|`5This reward is only available for players with the `5Royal Grow Pass``.``|left|\nadd_spacer|small|\nadd_quick_exit|\nadd_button|back|OK|noflags|0|0|\nend_dialog|battlepass_itemPeek|||"), p.CreatePacket(peer);

											}
										}
										else {
											gamepacket_t p;
											p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wReward Details`|left|9222|\nadd_spacer|small|\nadd_spacer|small|\nadd_label_with_icon|big| " + to_string(grow_item_count) + " " + items[grow_item].ori_name + "|left|" + to_string(grow_item) + "|\nadd_textbox|" + items[grow_item].description + "|left|\nadd_spacer|small|\nadd_textbox|Earn `2" + to_string(remove_grow_points) + " Grow Pass Points`` and become a `5Royal Grow Pass Member``  to claim this reward.|left|\nadd_spacer|small|\nadd_textbox|`5This reward is only available for players with the `5Royal Grow Pass``.``|left|\nadd_spacer|small|\nadd_quick_exit|\nadd_button|back|OK|noflags|0|0|\nend_dialog|battlepass_itemPeek|||"), p.CreatePacket(peer);
										}
									}
								}

							}
							break;
						}
						else if (cch == "action|storenavigate\nitem|main\nselection|rt_grope_battlepass_bundle01\n") {
							shop_tab(peer, "tab1_growpass_item");
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|battlepass_tasks\nbuttonClicked|daily_quests\n\n") {
							daily_quest_info(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|battlepass_tasks\nbuttonClicked|life_goals\n\n") {
							daily_quest(peer, true, "tab_rewards", 0);
							break;
							}
						else if (cch == "action|dialog_return\ndialog_name|battlepass_tasks\nbuttonClicked|tab_perks\n\n") {
							growpass_perks(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|battlepass_tasks\nbuttonClicked|tab_rewards\n\n") {
							growpass_rewards(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|battlepass_tasks\nbuttonClicked|tab_tasks\n\n" || cch == "action|growpass\n" || cch == "action|battlepasspopup\n") {
							growpass_tasks(peer);
							break;
						}
						/*
						else if (cch == "action|handle_super_pinappleparty_info\n") {
							gamepacket_t p(500);
							p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wHow 2 Pineapple Party``|left|4752|\nadd_spacer|small|\nadd_smalltext|During the event, you can get Pineapples by splicing two seeds of combined rarity of 4 - 50 that don't normally splice. Splicing two seeds that don't normally splice with a combined rarity of 40 - 50 may have a chance to create a Super Pineapple.|left|\nadd_image_button||interface/large/gui_shop_pineapple1.rttex|bannerlayout|||\nadd_spacer|small|\nadd_smalltext|Other than using the Pineapples to slowly turn you into one, you can also feed other players pineapples as well. Can't seem to find another player to feed, then find yourself a Pineapple Guzzler whose is a being that is in their complete Pineapple Form. NOTE: Feeding the Guzzler too many Pineapples when it is too full will eventually lead to it exploding.|left|\nadd_spacer|small|\nadd_smalltext|You are probably wondering \"Why would I want to feed other people Pineapples ? !\" Well, it's so that you can receive a special prize!|left|\nadd_image_button||interface/large/gui_shop_pineapple2.rttex|bannerlayout|||\nadd_spacer|small|\nadd_smalltext|A magical pineapple will also float around you for 20 minutes before you can feed another pineapple again!|left|\nadd_image_button||interface/large/gui_shop_pineapple3.rttex|bannerlayout|||\nadd_spacer|small|\nadd_smalltext|You can also feed a Dangerous Pineapple to the Pineapple Guzzler or to another player although both you and the other player must be in Full Pineapple Form. Instead of a regular old pineapple, you receive Pineapple Tragic made evident by the spikes on the Pineapple orbiting around you.|left|\nadd_smalltext|But what if you don't want to wait 20 minutes and speed up the whole process instead? Well then you can use the Super Pineapple Time Warp Device, a magical device powered by a flux capacitor which allows it to bend the fabric of time so that you can feed pineapples again.|left|\nadd_image_button||interface/large/gui_shop_pineapple4.rttex|bannerlayout|||\nadd_spacer|small|\nadd_smalltext|And finally, we get to the party aspect of this event. Every Pineapple eaten contributes to a global count of pineapples eaten which can be found from the featured banner in the shop. The higher the count, the better rewards you can receive from the Super Pineapple Magic mod. So, get out there, eat those Pineapples, feed those Pineapples and party all week long!|left|\nadd_image_button||interface/large/gui_shop_pineapple5.rttex|bannerlayout|||\nadd_spacer|small|\nadd_button|open_store|Back|noflags|0|0|\nend_dialog|handle_super_pinappleparty_info||\nadd_quick_exit|"), p.CreatePacket(peer);
							break;
						}*/
						else if (cch.find("action|dialog_return\ndialog_name|washing_machine\nitemid|") != string::npos) {
							int item_ = atoi(cch.substr(56, cch.length() - 56).c_str()), got = inventory_contains(peer, item_);
							if (item_ <= 0 || item_ > items.size() || got == 0) break;
							if (items[item_].untradeable || items[item_].rarity <= 1 || items[item_].rarity == 999 || items[item_].block_possible_put == false) {
								gamepacket_t p;
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`4" + items[item_].ori_name + "`` does not fit in the Washing Machine."), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
								break;
							}
							pInfo(peer)->lastchoosenitem = item_;
							gamepacket_t p;
							p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wWashing Machine``|left|9946|\nadd_textbox|How many to wash?|left|\nadd_text_input|count||" + to_string(got) + "|3|\nadd_spacer|small|\nend_dialog|washing_machine|Cancel|OK|\nadd_quick_exit|"), p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|recycle_machine") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 3) break;
							string recycled_list = "";
							bool trashed_ = false;
							int total_receive = 0;
							for (int i_ = 2; i_ < t_.size(); i_++) {
								if (t_.size() - i_ <= 1) break;
								if (atoi(explode("\n", t_[i_ + 1])[0].c_str())) {
									int item_id = atoi(explode("\n", t_[i_])[1].c_str());
									if (item_id <= 0 && item_id > items.size() || items[item_id].untradeable) break;
									int removal = inventory_contains(peer, item_id) * -1;
									if (modify_inventory(peer, item_id, removal) == 0) {
										recycled_list += "\n" + to_string(abs(removal)) + " " + items[item_id].ori_name;
										total_receive += abs(removal);
										trashed_ = true;
									}
								}
							}
							if (trashed_) {
								OnSetGems(peer, abs(total_receive));
								gamepacket_t p;
								p.Insert("OnConsoleMessage"), p.Insert("Received gems: "+setGems(total_receive) + ".\nRecycled " + recycled_list + "");
								packet_(peer, "action|play_sfx\nfile|audio/trash.wav\ndelayMS|0");
								p.CreatePacket(peer);
							}
							break;
							}
						else if (cch.find("action|dialog_return\ndialog_name|continue\nbuttonClicked|convert_") != string::npos) {
							int item_ = atoi(cch.substr(64, cch.length() - 64).c_str()), got = inventory_contains(peer, item_);
							if ((item_ == 7188 && got < 100) or (item_ == 8470 && got < 1)) break;
							if (get_free_slots(pInfo(peer)) >= 1) {
								int compress_item_return = 0, compress_return_count = 0;
								if (item_ == 7188) compress_return_count = 100, compress_item_return = 8470;
								else compress_return_count = 1, compress_item_return = 7188;
								int countofused = 0, getdl = 1, getwl = 100, removewl = -100, removedl = -1, countwl = 0;
								modify_inventory(peer, compress_item_return, countwl);
								if (compress_return_count == 100 ? countwl <= 199 : countwl <= 100) {
									modify_inventory(peer, item_, countofused);
									if (compress_return_count == 100 ? countofused >= 100 : countofused >= 1) {
										modify_inventory(peer, item_, compress_return_count == 100 ? removewl : removedl);
										modify_inventory(peer,compress_item_return,compress_return_count == 100 ? getdl : getwl);
										gamepacket_t p, p2;
										p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert(compress_return_count == 100 ? "You compressed 100 `2" + items[item_].ori_name + "`` into a `2" + items[compress_item_return].ori_name + "``!" : "You shattered a `2" + items[item_].ori_name + "`` into 100 `2" + items[compress_item_return].ori_name + "``!"), p.Insert(0), p.Insert(1), p.CreatePacket(peer);
										p2.Insert("OnConsoleMessage"), p2.Insert(compress_return_count == 100 ? "You compressed 100 `2" + items[item_].ori_name + "`` into a `2" + items[compress_item_return].ori_name + "``!" : "You shattered a `2" + items[item_].ori_name + "`` into 100 `2" + items[compress_item_return].ori_name + "``!"), p2.CreatePacket(peer);
									}
								}
							}
							break;
							}
						else if (cch == "action|dialog_return\ndialog_name|wizard_quests\nbuttonClicked|give_up\n\n") {
						gamepacket_t p;
						p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_label_with_icon|big|`9Legendary Wizard``|left|1790|\nadd_spacer|small|\nadd_textbox|Are you sure you want to give up your Legendary Wizard quest?|left|\nadd_spacer|small|\nadd_button|bye|Give up|noflags|0|0|\nend_dialog|wizard_quests|Cancel||\nadd_quick_exit|"), p.CreatePacket(peer);
						break;
						}
						else if (cch == "action|dialog_return\ndialog_name|wizard_quests\nbuttonClicked|bye\n\n") {
						pInfo(peer)->legendary_quest.clear();
						pInfo(peer)->lwiz_quest = 0;
						pInfo(peer)->lwiz_notification = 0;
						pInfo(peer)->lwiz_step = 1;
						lwiz_quest(peer, "open");
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|checkoutcounter") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								world_->fresh_world = true;
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
								if (not block_access(peer, world_, block_)) break;
								vector<string> t_ = explode("|", cch);
								if (t_.size() > 4) {
									string button = explode("\n", t_[3])[0].c_str();
									if (button == "filterbytext") load_vendhub(peer, world_, block_, explode("\n", t_[4])[0].c_str());
									else if (button == "switchdirection") block_->spin = (block_->spin ? false : true);
									else {
										int i_ = atoi(explode("\n", t_[3])[0].c_str()), x_ = i_ % 100, y_ = i_ / 100;
										if (i_ > world_->blocks.size()) break;
										block_ = &world_->blocks[i_];
										if (items[world_->blocks[i_].fg].blockType != BlockTypes::VENDING) break;
										gamepacket_t p;
										p.Insert("OnDialogRequest");
										p.Insert(get_vending(peer, world_, block_, x_, y_));
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|wizard\nbuttonClicked|") != string::npos) {
						string item_ = cch.substr(54, cch.length() - 54).c_str();
						replaceAll(item_, "\n", "");
						lwiz_quest(peer, "open_" + item_);
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|wizard_start_") != string::npos) {
						string item_ = cch.substr(46, cch.length() - 46).c_str();
						replaceAll(item_, "\n", "");
						lwiz_quest(peer, "start_" + item_);
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|wizard_quests\nbuttonClicked|deliver") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								world_->fresh_world = true;
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
								if (not block_access(peer, world_, block_)) break;
								if (block_->fg == 1790) {
									if (pInfo(peer)->lwiz_quest != 0) {
										if (pInfo(peer)->legendary_quest[pInfo(peer)->lwiz_step - 1].size() == 3) {
											int item = pInfo(peer)->legendary_quest[pInfo(peer)->lwiz_step - 1][2], got = 0;
											modify_inventory(peer, item, got);
											if (got != 0) {
												int give_back = 0;
												if (pInfo(peer)->legendary_quest[pInfo(peer)->lwiz_step - 1][0] + got >= pInfo(peer)->legendary_quest[pInfo(peer)->lwiz_step - 1][1])give_back = pInfo(peer)->legendary_quest[pInfo(peer)->lwiz_step - 1][0] + got - pInfo(peer)->legendary_quest[pInfo(peer)->lwiz_step - 1][1];
												add_lwiz_points(peer, got);
												modify_inventory(peer, item, got *= -1);
												if (give_back != 0) modify_inventory(peer, item, give_back);
												lwiz_points(peer);
											}
											else if (pInfo(peer)->legendary_quest[pInfo(peer)->lwiz_step - 1][0] >= pInfo(peer)->legendary_quest[pInfo(peer)->lwiz_step - 1][1]) {
												lwiz_points(peer);
											}
										}
										else if (pInfo(peer)->lwiz_step == 10) {
											int need_more = pInfo(peer)->legendary_quest[pInfo(peer)->lwiz_step - 1][1] - pInfo(peer)->legendary_quest[pInfo(peer)->lwiz_step - 1][0];
											if (pInfo(peer)->gems != 0) {
												if (pInfo(peer)->gems >= need_more) {
													add_lwiz_points(peer, need_more);
													OnSetGems(peer, need_more * -1);
												}
												else {
													add_lwiz_points(peer, pInfo(peer)->gems);
													OnSetGems(peer, pInfo(peer)->gems * -1);
												}
												lwiz_points(peer);
											}
										}
										else if (pInfo(peer)->legendary_quest[pInfo(peer)->lwiz_step - 1][0] >= pInfo(peer)->legendary_quest[pInfo(peer)->lwiz_step - 1][1]) {
											lwiz_points(peer);
										}
									}
								}
							}
							break;
						}
						/*
						else if (cch.find("action|dialog_return\ndialog_name|statsblock\nbuttonClicked|findObject_") != string::npos) {
						int item_ = atoi(cch.substr(69, cch.length() - 69).c_str());
						if (item_ <= 0 || item_ > items.size()) break;
						cout << item_ << endl;
						BYTE* ex_raw = packEffectMoving37(37, item_);
						for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != pInfo(peer)->world) continue;
							send_raw(currentPeer, 4, ex_raw, 56, ENET_PACKET_FLAG_RELIABLE);
						}
						delete[] ex_raw;
						break;
						}*/
						else if (cch.find("action|dialog_return\ndialog_name|statsblockworld\nbuttonClicked|findTile_") != string::npos) {
							int item_ = atoi(cch.substr(72, cch.length() - 72).c_str());
							if (item_ <= 0 || item_ > items.size()) break;
							PlayerMoving data_{};
							data_.packetType = 37, data_.netID = item_;
							BYTE* raw = packPlayerMoving(&data_);
							for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != pInfo(peer)->world) continue;
								send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
							}
							delete[] raw;
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|gender\nbuttonClicked|") != string::npos) {
							string gender = cch.substr(54, cch.length() - 54).c_str();
							replaceAll(gender, "\n", "");
							pInfo(peer)->gender = gender;
							news(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|washing_machine\ncount|") != string::npos) {
						int item_ = pInfo(peer)->lastchoosenitem, got = 0, want_ = atoi(cch.substr(55, cch.length() - 55).c_str());
						if (item_ <= 0 || item_ > items.size()) break;
						modify_inventory(peer, item_, got);
						if (got < want_ or want_ <= 0 or want_ > 200) break;
						if (items[item_].untradeable || items[item_].rarity <= 1 || items[item_].rarity == 999 || items[item_].block_possible_put == false) {
							gamepacket_t p;
							p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`4" + items[item_].ori_name + "`` does not fit in the Washing Machine."), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
							break;
						}
						gamepacket_t p2;
						p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("Washing "+to_string(want_) + " "+items[item_].ori_name + ""), p2.CreatePacket(peer);
						string name_ = pInfo(peer)->world;
						vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
						if (p != worlds.end()) {
							World* world_ = &worlds[p - worlds.begin()];
							world_->fresh_world = true;
							WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
							if (block_->fg == 9946) {
								int i_ = pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100), x_ = (i_ % 100), y_ = (i_ / 100), remove_ = want_ * -1;
								modify_inventory(peer, item_, remove_);
								block_->shelf_1 = item_, block_->shelf_2 = want_;
								block_->fg = 9948;
								update_tile(peer, x_, y_, 9948, true);
								block_->planted = time(nullptr) -items[9948].growTime + rand() % 600 + 100;
								PlayerMoving data_{};
								data_.packetType = 5, data_.punchX = x_, data_.punchY = y_, data_.characterState = 0x8;
								int alloc = alloc_(world_, block_);
								BYTE* raw = packPlayerMoving(&data_, 112 + alloc);
								BYTE* blc = raw + 56;
								form_visual(blc, *block_, *world_, peer, false);
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									if (pInfo(currentPeer)->world == world_->name) {
										update_tile(currentPeer, x_, y_, 9948);
										send_raw(currentPeer, 4, raw, 112 + alloc, ENET_PACKET_FLAG_RELIABLE);
									}
								}
								delete[] raw, blc;
								if (block_->locked) upd_lock(*block_, *world_, peer);
							}
						}
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|bannerbandolier") != string::npos) {
							if (pInfo(peer)->necklace == 11748) {
								if (cch.find("buttonClicked|patternpicker") != string::npos) {
									string dialog = "";
									if (pInfo(peer)->Banner_Flag == 0) dialog += "set_default_color|`o\nadd_label_with_icon|big|`wBanner Bandolier``|left|5838|";
									else if (pInfo(peer)->Banner_Flag == 1) dialog += "set_default_color|`o\nadd_label_with_icon|big|`wBanner Bandolier``|left|5844|";
									else if (pInfo(peer)->Banner_Flag == 2) dialog += "set_default_color|`o\nadd_label_with_icon|big|`wBanner Bandolier``|left|5848|";
									else if (pInfo(peer)->Banner_Flag == 3) dialog += "set_default_color|`o\nadd_label_with_icon|big|`wBanner Bandolier``|left|5846|";
									else if (pInfo(peer)->Banner_Flag == 4) dialog += "set_default_color|`o\nadd_label_with_icon|big|`wBanner Bandolier``|left|5842|";
									dialog += "\nadd_spacer|small|\nadd_textbox|Pick a pattern for your banner.|left|\nadd_spacer|small|";
									dialog += "\nadd_label_with_icon_button|big|Harlequin|left|5838|pattern_1|\nadd_spacer|small|";
									dialog += "\nadd_label_with_icon_button|big|Slant|left|5844|pattern_2|\nadd_spacer|small|";
									dialog += "\nadd_label_with_icon_button|big|Stripe|left|5848|pattern_3|\nadd_spacer|small|";
									dialog += "\nadd_label_with_icon_button|big|Panel|left|5846|pattern_4|\nadd_spacer|small|";
									dialog += "\nadd_label_with_icon_button|big|Cross|left|5842|pattern_5|\nadd_spacer|small|";
									dialog += "\nend_dialog|bannerbandolier|Cancel||\nadd_quick_exit|";
									gamepacket_t p;
									p.Insert("OnDialogRequest"), p.Insert(dialog), p.CreatePacket(peer);
									break;
								}
								else if (cch.find("buttonClicked|pattern_") != string::npos) {
									int Pattern = atoi(cch.substr(49 + 22, cch.length() - 49 + 22).c_str());
									if (Pattern < 1 || Pattern > 5) break;
									pInfo(peer)->CBanner_Item = pInfo(peer)->Banner_Item;
									pInfo(peer)->CBanner_Flag = Pattern - 1;
									SendBannerBandolier2(peer);
									update_clothes(peer);
									break;
								}
								else if (cch.find("buttonClicked|reset") != string::npos) {
									pInfo(peer)->CBanner_Item = 0; pInfo(peer)->CBanner_Flag = 0; pInfo(peer)->Banner_Item = 0; pInfo(peer)->Banner_Flag = 0;
									SendBannerBandolier2(peer);
									update_clothes(peer);
									break;
								}
								else if (!(cch.find("buttonClicked|patternpicker") != string::npos || cch.find("buttonClicked|pattern_") != string::npos || cch.find("\nbanneritem|") != string::npos)) {
									if (pInfo(peer)->CBanner_Item != 0) pInfo(peer)->Banner_Item = pInfo(peer)->CBanner_Item;
									if (pInfo(peer)->CBanner_Flag != 0) pInfo(peer)->Banner_Flag = pInfo(peer)->CBanner_Flag;
									pInfo(peer)->CBanner_Item = 0; pInfo(peer)->CBanner_Flag = 0;
									update_clothes(peer);
									break;
								}
								else {
									if (cch.find("banneritem|") != string::npos) {
										int item_ = atoi(explode("\n", explode("banneritem|", cch)[1])[0].c_str());
										if (item_ <= 0 || item_ > items.size()) break;
										if (inventory_contains(peer, item_) == 0) break;
										pInfo(peer)->CBanner_Flag = pInfo(peer)->CBanner_Flag;
										pInfo(peer)->CBanner_Item = item_;
										update_clothes(peer);
										SendBannerBandolier2(peer);
									}
									break;
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|magic_compass") != string::npos) {
						if (cch.find("buttonClicked|clear_item") != string::npos) {
							pInfo(peer)->Magnet_Item = 0;
							update_clothes(peer);
							gamepacket_t p;
							p.Insert("OnMagicCompassTrackingItemIDChanged");
							p.Insert(pInfo(peer)->Magnet_Item);
							p.CreatePacket(peer);
							break;
						}
						else {
							if (cch.find("magic_compass_item|") != string::npos) {
								pInfo(peer)->Magnet_Item = atoi(explode("\n", explode("magic_compass_item|", cch)[1])[0].c_str());
								update_clothes(peer);
								gamepacket_t p;
								p.Insert("OnMagicCompassTrackingItemIDChanged");
								p.Insert(atoi(explode("\n", explode("magic_compass_item|", cch)[1])[0].c_str()));
								p.CreatePacket(peer);
							}
							break;
						}
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|dialog_cernuous_mask") != string::npos) {
						if (cch.find("buttonClicked|restore_default") != string::npos) {
							pInfo(peer)->Aura_Season = 2;
							pInfo(peer)->Trail_Season = 2;
							update_clothes(peer);
							break;
						}
						else {
							if (cch.find("checkbox_none0|") != string::npos and cch.find("checkbox_spring0|") != string::npos and cch.find("checkbox_summer0|") != string::npos and cch.find("checkbox_autumn0|") != string::npos and cch.find("checkbox_winter0|") != string::npos) 
								pInfo(peer)->Aura_Season = (atoi(explode("\n", explode("checkbox_none0|", cch)[1])[0].c_str()) == 1 ? 0 : (atoi(explode("\n", explode("checkbox_spring0|", cch)[1])[0].c_str()) == 1 ? 1 : (atoi(explode("\n", explode("checkbox_summer0|", cch)[1])[0].c_str()) == 1 ? 2 : (atoi(explode("\n", explode("checkbox_autumn0|", cch)[1])[0].c_str()) == 1 ? 3 : (atoi(explode("\n", explode("checkbox_winter0|", cch)[1])[0].c_str()) == 1 ? 4 : 0)))));
							if (cch.find("checkbox_none1|") != string::npos and cch.find("checkbox_spring1|") != string::npos and cch.find("checkbox_summer1|") != string::npos and cch.find("checkbox_autumn1|") != string::npos and cch.find("checkbox_winter1|") != string::npos)
								pInfo(peer)->Trail_Season = (atoi(explode("\n", explode("checkbox_none1|", cch)[1])[0].c_str()) == 1 ? 0 : (atoi(explode("\n", explode("checkbox_spring1|", cch)[1])[0].c_str()) == 1 ? 1 : (atoi(explode("\n", explode("checkbox_summer1|", cch)[1])[0].c_str()) == 1 ? 2 : (atoi(explode("\n", explode("checkbox_autumn1|", cch)[1])[0].c_str()) == 1 ? 3 : (atoi(explode("\n", explode("checkbox_winter1|", cch)[1])[0].c_str()) == 1 ? 4 : 0)))));
							update_clothes(peer);
							break;
						}
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|sessionlength_edit") != string::npos) {
							try {
								if (cch.find("checkbox_5|") != string::npos and cch.find("checkbox_10|") != string::npos and cch.find("checkbox_20|") != string::npos and cch.find("checkbox_30|") != string::npos and cch.find("checkbox_40|") != string::npos and cch.find("checkbox_50|") != string::npos and cch.find("checkbox_60|") != string::npos) {
									string name_ = pInfo(peer)->world;
									vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
									if (p != worlds.end()) {
										World* world_ = &worlds[p - worlds.begin()];
										world_->fresh_world = true;
										if (pInfo(peer)->tankIDName != world_->owner_name) break;
										world_->World_Time = (atoi(explode("\n", explode("checkbox_5|", cch)[1])[0].c_str()) == 1 ? 5 : (atoi(explode("\n", explode("checkbox_10|", cch)[1])[0].c_str()) == 1 ? 10 : (atoi(explode("\n", explode("checkbox_20|", cch)[1])[0].c_str()) == 1 ? 20 : (atoi(explode("\n", explode("checkbox_30|", cch)[1])[0].c_str()) == 1 ? 30 : (atoi(explode("\n", explode("checkbox_40|", cch)[1])[0].c_str()) == 1 ? 40 : (atoi(explode("\n", explode("checkbox_50|", cch)[1])[0].c_str()) == 1 ? 50 : (atoi(explode("\n", explode("checkbox_60|", cch)[1])[0].c_str()) == 1 ? 60 : 0)))))));
										gamepacket_t p;
										p.Insert("OnTalkBubble"); p.Insert(pInfo(peer)->netID);
										p.Insert((world_->World_Time == 0 ? "World Timer limit removed!" : "World Timer limit set to `2" + to_string(world_->World_Time) + " minutes``."));
										p.Insert(0); p.Insert(0); p.CreatePacket(peer);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(currentPeer)->world == world_->name) {
												if (pInfo(currentPeer)->tankIDName != world_->owner_name && world_->World_Time != 0) {
													pInfo(currentPeer)->World_Timed = time(nullptr) + (world_->World_Time * 60);
													pInfo(currentPeer)->WorldTimed = true;
												}
											}
										}
									}
								}
							}
							catch (out_of_range) {
								cout << "Server error invalid (out of range) on " + cch << endl;
								break;
							}
							break;
						}
						/*
						else if (cch.find("action|dialog_return\ndialog_name|dialog_infinity_crown") != string::npos) {
						if (cch.find("buttonClicked|button_manual") != string::npos) {
							SendDialogInfinityCrown(peer, true);
							break;
						}
						else if (cch.find("buttonClicked|restore_default") != string::npos) {
							// Infinity Crown
							pInfo(peer)->Crown_Time_Change = true;
							pInfo(peer)->Crown_Cycle_Time = 15;
							// Crown 1
							pInfo(peer)->Base_R_0 = 255, pInfo(peer)->Base_G_0 = 255, pInfo(peer)->Base_B_0 = 255;
							pInfo(peer)->Gem_R_0 = 255, pInfo(peer)->Gem_G_0 = 0, pInfo(peer)->Gem_B_0 = 255;
							pInfo(peer)->Crystal_R_0 = 0, pInfo(peer)->Crystal_G_0 = 205, pInfo(peer)->Crystal_B_0 = 249;
							pInfo(peer)->Crown_Floating_Effect_0 = false, pInfo(peer)->Crown_Laser_Beam_0 = true, pInfo(peer)->Crown_Crystals_0 = true, pInfo(peer)->Crown_Rays_0 = true;
							// Crown 2
							pInfo(peer)->Base_R_1 = 255, pInfo(peer)->Base_G_1 = 200, pInfo(peer)->Base_B_1 = 37;
							pInfo(peer)->Gem_R_1 = 255, pInfo(peer)->Gem_G_1 = 0, pInfo(peer)->Gem_B_1 = 64;
							pInfo(peer)->Crystal_R_1 = 26, pInfo(peer)->Crystal_G_1 = 45, pInfo(peer)->Crystal_B_1 = 140;
							pInfo(peer)->Crown_Floating_Effect_1 = false, pInfo(peer)->Crown_Laser_Beam_1 = true, pInfo(peer)->Crown_Crystals_1 = true, pInfo(peer)->Crown_Rays_1 = true;
							// Total
							pInfo(peer)->Crown_Value = 1768716607;
							pInfo(peer)->Crown_Value_0_0 = 4294967295, pInfo(peer)->Crown_Value_0_1 = 4278255615, pInfo(peer)->Crown_Value_0_2 = 4190961919;
							pInfo(peer)->Crown_Value_1_0 = 633929727, pInfo(peer)->Crown_Value_1_1 = 1073807359, pInfo(peer)->Crown_Value_1_2 = 2351766271;
							// End
							update_clothes(peer);
							break;
						}
						else {
							try {
								pInfo(peer)->Crown_Time_Change = atoi(explode("\n", explode("checkbox_time_cycle|", cch)[1])[0].c_str()) == 1 ? true : false;
								if (!is_number(explode("\n", explode("text_input_time_cycle|", cch)[1])[0])) break;
								pInfo(peer)->Crown_Cycle_Time = atoi(explode("\n", explode("text_input_time_cycle|", cch)[1])[0].c_str());
								{ // Crown 1
									pInfo(peer)->Crown_Floating_Effect_0 = atoi(explode("\n", explode("checkbox_floating0|", cch)[1])[0].c_str()) == 1 ? true : false;
									{
										auto Crown_Base_0 = explode(",", explode("\n", explode("text_input_base_color0|", cch)[1])[0].c_str());
										vector<string> t_ = explode(",", explode("\n", explode("text_input_base_color0|", cch)[1])[0].c_str());
										if (Crown_Base_0.size() != 3 || t_.size() < 2 || Crown_Base_0[0] == "" || Crown_Base_0[1] == "" || Crown_Base_0[2] == "" || Crown_Base_0[0].empty() || Crown_Base_0[1].empty() || Crown_Base_0[2].empty()) {
											SendDialogInfinityCrown(peer, false, "you need to enter an RGB (Red, Blue, Green) value");
											break;
										}
										if (!is_number(Crown_Base_0[0]) || !is_number(Crown_Base_0[1]) || !is_number(Crown_Base_0[2]) || atoi(Crown_Base_0[0].c_str()) > 255 || atoi(Crown_Base_0[1].c_str()) > 255 || atoi(Crown_Base_0[2].c_str()) > 255 || atoi(Crown_Base_0[0].c_str()) < 0 || atoi(Crown_Base_0[1].c_str()) < 0 || atoi(Crown_Base_0[2].c_str()) < 0) {
											SendDialogInfinityCrown(peer, false, "you need to enter values betwwen 0 and 255");
											break;
										}
										pInfo(peer)->Base_R_0 = atoi(Crown_Base_0[0].c_str());
										pInfo(peer)->Base_G_0 = atoi(Crown_Base_0[1].c_str());
										pInfo(peer)->Base_B_0 = atoi(Crown_Base_0[2].c_str());
										pInfo(peer)->Crown_Value_0_0 = (long long int)(255 + (256 * pInfo(peer)->Base_R_0) + pInfo(peer)->Base_G_0 * 65536 + (pInfo(peer)->Base_B_0 * (long long int)16777216));
									}
									{
										pInfo(peer)->Crown_Laser_Beam_0 = atoi(explode("\n", explode("checkbox_laser_beam0|", cch)[1])[0].c_str()) == 1 ? true : false;
										auto Crown_Gem_0 = explode(",", explode("\n", explode("text_input_gem_color0|", cch)[1])[0].c_str());
										vector<string> t_ = explode(",", explode("\n", explode("text_input_gem_color0|", cch)[1])[0].c_str());
										if (Crown_Gem_0.size() != 3 || t_.size() < 2 || Crown_Gem_0[0] == "" || Crown_Gem_0[1] == "" || Crown_Gem_0[2] == "" || Crown_Gem_0[0].empty() || Crown_Gem_0[1].empty() || Crown_Gem_0[2].empty()) {
											SendDialogInfinityCrown(peer, false, "you need to enter an RGB (Red, Blue, Green) value");
											break;
										}
										if (!is_number(Crown_Gem_0[0]) || !is_number(Crown_Gem_0[1]) || !is_number(Crown_Gem_0[2]) || atoi(Crown_Gem_0[0].c_str()) > 255 || atoi(Crown_Gem_0[1].c_str()) > 255 || atoi(Crown_Gem_0[2].c_str()) > 255 || atoi(Crown_Gem_0[0].c_str()) < 0 || atoi(Crown_Gem_0[1].c_str()) < 0 || atoi(Crown_Gem_0[2].c_str()) < 0) {
											SendDialogInfinityCrown(peer, false, "you need to enter values betwwen 0 and 255");
											break;
										}
										pInfo(peer)->Gem_R_0 = atoi(Crown_Gem_0[0].c_str());
										pInfo(peer)->Gem_G_0 = atoi(Crown_Gem_0[1].c_str());
										pInfo(peer)->Gem_B_0 = atoi(Crown_Gem_0[2].c_str());
										pInfo(peer)->Crown_Value_0_1 = 255 + (256 * atoi(Crown_Gem_0[0].c_str())) + atoi(Crown_Gem_0[1].c_str()) * 65536 + (atoi(Crown_Gem_0[2].c_str()) * (long long int)16777216);
									}
									{
										auto Crown_Crystal_0 = explode(",", explode("\n", explode("text_input_crystal_color0|", cch)[1])[0].c_str());
										vector<string> t_ = explode(",", explode("\n", explode("text_input_crystal_color0|", cch)[1])[0].c_str());
										if (Crown_Crystal_0.size() != 3 || t_.size() < 2 || Crown_Crystal_0[0] == "" || Crown_Crystal_0[1] == "" || Crown_Crystal_0[2] == "" || Crown_Crystal_0[0].empty() || Crown_Crystal_0[1].empty() || Crown_Crystal_0[2].empty()) {
											SendDialogInfinityCrown(peer, false, "you need to enter an RGB (Red, Blue, Green) value");
											break;
										};
										if (!is_number(Crown_Crystal_0[0]) || !is_number(Crown_Crystal_0[1]) || !is_number(Crown_Crystal_0[2]) || atoi(Crown_Crystal_0[0].c_str()) > 255 || atoi(Crown_Crystal_0[1].c_str()) > 255 || atoi(Crown_Crystal_0[2].c_str()) > 255 || atoi(Crown_Crystal_0[0].c_str()) < 0 || atoi(Crown_Crystal_0[1].c_str()) < 0 || atoi(Crown_Crystal_0[2].c_str()) < 0) {
											SendDialogInfinityCrown(peer, false, "you need to enter values betwwen 0 and 255");
											break;
										}
										pInfo(peer)->Crystal_R_0 = atoi(Crown_Crystal_0[0].c_str());
										pInfo(peer)->Crystal_G_0 = atoi(Crown_Crystal_0[1].c_str());
										pInfo(peer)->Crystal_B_0 = atoi(Crown_Crystal_0[2].c_str());
										pInfo(peer)->Crown_Value_0_2 = 255 + (256 * atoi(Crown_Crystal_0[0].c_str())) + atoi(Crown_Crystal_0[1].c_str()) * 65536 + (atoi(Crown_Crystal_0[2].c_str()) * (long long int)16777216);
									}
									pInfo(peer)->Crown_Crystals_0 = atoi(explode("\n", explode("checkbox_crystals0|", cch)[1])[0].c_str()) == 1 ? true : false;
									pInfo(peer)->Crown_Rays_0 = atoi(explode("\n", explode("checkbox_rays0|", cch)[1])[0].c_str()) == 1 ? true : false;
								}
								{ // Crown 2
									pInfo(peer)->Crown_Floating_Effect_1 = atoi(explode("\n", explode("checkbox_floating1|", cch)[1])[0].c_str()) == 1 ? true : false;
									{
										auto Crown_Base_1 = explode(",", explode("\n", explode("text_input_base_color1|", cch)[1])[0].c_str());
										vector<string> t_ = explode(",", explode("\n", explode("text_input_base_color1|", cch)[1])[0].c_str());
										if (Crown_Base_1.size() != 3 || t_.size() < 2 || Crown_Base_1[0] == "" || Crown_Base_1[1] == "" || Crown_Base_1[2] == "" || Crown_Base_1[0].empty() || Crown_Base_1[1].empty() || Crown_Base_1[2].empty()) {
											SendDialogInfinityCrown(peer, false, "you need to enter an RGB (Red, Blue, Green) value");
											break;
										}
										if (!is_number(Crown_Base_1[0]) || !is_number(Crown_Base_1[1]) || !is_number(Crown_Base_1[2]) || atoi(Crown_Base_1[0].c_str()) > 255 || atoi(Crown_Base_1[1].c_str()) > 255 || atoi(Crown_Base_1[2].c_str()) > 255 || atoi(Crown_Base_1[0].c_str()) < 0 || atoi(Crown_Base_1[1].c_str()) < 0 || atoi(Crown_Base_1[2].c_str()) < 0) {
											SendDialogInfinityCrown(peer, false, "you need to enter values betwwen 0 and 255");
											break;
										}
										pInfo(peer)->Base_R_1 = atoi(Crown_Base_1[0].c_str());
										pInfo(peer)->Base_G_1 = atoi(Crown_Base_1[1].c_str());
										pInfo(peer)->Base_B_1 = atoi(Crown_Base_1[2].c_str());
										pInfo(peer)->Crown_Value_1_1 = 255 + (256 * atoi(Crown_Base_1[0].c_str())) + atoi(Crown_Base_1[1].c_str()) * 65536 + (atoi(Crown_Base_1[2].c_str()) * (long long int)16777216);
									}
									{
										pInfo(peer)->Crown_Laser_Beam_1 = atoi(explode("\n", explode("checkbox_laser_beam1|", cch)[1])[0].c_str()) == 1 ? true : false;
										auto Crown_Gem_1 = explode(",", explode("\n", explode("text_input_gem_color1|", cch)[1])[0].c_str());
										vector<string> t_ = explode(",", explode("\n", explode("text_input_gem_color1|", cch)[1])[0].c_str());
										if (Crown_Gem_1.size() != 3 || t_.size() < 2 || Crown_Gem_1[0] == "" || Crown_Gem_1[1] == "" || Crown_Gem_1[2] == "" || Crown_Gem_1[0].empty() || Crown_Gem_1[1].empty() || Crown_Gem_1[2].empty()) {
											SendDialogInfinityCrown(peer, false, "you need to enter an RGB (Red, Blue, Green) value");
											break;
										}
										if (!is_number(Crown_Gem_1[0]) || !is_number(Crown_Gem_1[1]) || !is_number(Crown_Gem_1[2]) || atoi(Crown_Gem_1[0].c_str()) > 255 || atoi(Crown_Gem_1[1].c_str()) > 255 || atoi(Crown_Gem_1[2].c_str()) > 255 || atoi(Crown_Gem_1[0].c_str()) < 0 || atoi(Crown_Gem_1[1].c_str()) < 0 || atoi(Crown_Gem_1[2].c_str()) < 0) {
											SendDialogInfinityCrown(peer, false, "you need to enter values betwwen 0 and 255");
											break;
										}
										pInfo(peer)->Gem_R_1 = atoi(Crown_Gem_1[0].c_str());
										pInfo(peer)->Gem_G_1 = atoi(Crown_Gem_1[1].c_str());
										pInfo(peer)->Gem_B_1 = atoi(Crown_Gem_1[2].c_str());
										pInfo(peer)->Crown_Value_1_1 = 255 + (256 * atoi(Crown_Gem_1[0].c_str())) + atoi(Crown_Gem_1[1].c_str()) * 65536 + (atoi(Crown_Gem_1[2].c_str()) * (long long int)16777216);
									}
									{
										auto Crown_Crystal_1 = explode(",", explode("\n", explode("text_input_crystal_color1|", cch)[1])[0].c_str());
										vector<string> t_ = explode(",", explode("\n", explode("text_input_crystal_color1|", cch)[1])[0].c_str());
										if (Crown_Crystal_1.size() != 3 || t_.size() < 2 || Crown_Crystal_1[0] == "" || Crown_Crystal_1[1] == "" || Crown_Crystal_1[2] == "" || Crown_Crystal_1[0].empty() || Crown_Crystal_1[1].empty() || Crown_Crystal_1[2].empty()) {
											SendDialogInfinityCrown(peer, false, "you need to enter an RGB (Red, Blue, Green) value");
											break;
										}
										if (!is_number(Crown_Crystal_1[0]) || !is_number(Crown_Crystal_1[1]) || !is_number(Crown_Crystal_1[2]) || atoi(Crown_Crystal_1[0].c_str()) > 255 || atoi(Crown_Crystal_1[1].c_str()) > 255 || atoi(Crown_Crystal_1[2].c_str()) > 255 || atoi(Crown_Crystal_1[0].c_str()) < 0 || atoi(Crown_Crystal_1[1].c_str()) < 0 || atoi(Crown_Crystal_1[2].c_str()) < 0) {
											SendDialogInfinityCrown(peer, false, "you need to enter values betwwen 0 and 255");
											break;
										}
										pInfo(peer)->Crystal_R_1 = atoi(Crown_Crystal_1[0].c_str());
										pInfo(peer)->Crystal_G_1 = atoi(Crown_Crystal_1[1].c_str());
										pInfo(peer)->Crystal_B_1 = atoi(Crown_Crystal_1[2].c_str());
										pInfo(peer)->Crown_Value_1_2 = 255 + (256 * atoi(Crown_Crystal_1[0].c_str())) + atoi(Crown_Crystal_1[1].c_str()) * 65536 + (atoi(Crown_Crystal_1[2].c_str()) * (long long int)16777216);
									}
									pInfo(peer)->Crown_Crystals_1 = atoi(explode("\n", explode("checkbox_crystals1|", cch)[1])[0].c_str()) == 1 ? true : false;
									pInfo(peer)->Crown_Rays_1 = atoi(explode("\n", explode("checkbox_rays1|", cch)[1])[0].c_str()) == 1 ? true : false;
								}
							}
							catch (...) {
								break;
							}
							int Total_Value = 1768716288;
							if (pInfo(peer)->Crown_Time_Change) Total_Value += 256;
							if (pInfo(peer)->Crown_Floating_Effect_0) Total_Value += 64;
							if (pInfo(peer)->Crown_Laser_Beam_0) Total_Value += 1;
							if (pInfo(peer)->Crown_Crystals_0) Total_Value += 4;
							if (pInfo(peer)->Crown_Rays_0) Total_Value += 16;
							if (pInfo(peer)->Crown_Floating_Effect_1) Total_Value += 128;
							if (pInfo(peer)->Crown_Laser_Beam_1) Total_Value += 2;
							if (pInfo(peer)->Crown_Crystals_1) Total_Value += 8;
							if (pInfo(peer)->Crown_Rays_1) Total_Value += 32;
							pInfo(peer)->Crown_Value = Total_Value;
							update_clothes(peer);
						}
						break;
						}*/
						else if (cch.find("action|dialog_return\ndialog_name|dialog_rift_wings") != string::npos) {
						if (pInfo(peer)->back != 11478) break;
						vector<string> t_ = explode("|", cch), color;
						if (t_.size() == 29) {
							string color_convert = "";
							if (atoi(explode("\n", t_[3])[0].c_str()) > 0 && atoi(explode("\n", t_[3])[0].c_str()) < 86400) pInfo(peer)->_TimeDilation = atoi(explode("\n", t_[3])[0].c_str());
							bool time_dilation = atoi(explode("\n", t_[4])[0].c_str());
							if (time_dilation) pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_TIME_DILATION_ON;
							else pInfo(peer)->_flags &= ~Gtps3::RIFTWINGS_FLAGS_TIME_DILATION_ON;

							color_convert = explode("\n", t_[5])[0].c_str();
							color = explode(",", color_convert);
							if (color.size() == 3) {
								if (atoi(explode(",", color[0])[0].c_str()) >= 0 && atoi(explode(",", color[0])[0].c_str()) <= 255 && atoi(explode(",", color[1])[0].c_str()) >= 0 && atoi(explode(",", color[1])[0].c_str()) <= 255 && atoi(explode(",", color[2])[0].c_str()) >= 0 && atoi(explode(",", color[2])[0].c_str()) <= 255) {
									pInfo(peer)->_CapeStyleColor_1 = color_convert;
									uint8_t* cancer = (uint8_t*)(&(pInfo(peer)->wings_t));
									cancer[1] = atoi(explode(",", color[0])[0].c_str());
									cancer[2] = atoi(explode(",", color[1])[0].c_str());
									cancer[3] = atoi(explode(",", color[2])[0].c_str());
								}
							}

							color_convert = explode("\n", t_[6])[0].c_str();
							color = explode(",", color_convert);
							if (color.size() == 3) {
								if (atoi(explode(",", color[0])[0].c_str()) >= 0 && atoi(explode(",", color[0])[0].c_str()) <= 255 && atoi(explode(",", color[1])[0].c_str()) >= 0 && atoi(explode(",", color[1])[0].c_str()) <= 255 && atoi(explode(",", color[2])[0].c_str()) >= 0 && atoi(explode(",", color[2])[0].c_str()) <= 255) {
									pInfo(peer)->_CapeCollarColor_1 = color_convert;
									uint8_t* cancer = (uint8_t*)(&(pInfo(peer)->wings_c));
									cancer[1] = atoi(explode(",", color[0])[0].c_str());
									cancer[2] = atoi(explode(",", color[1])[0].c_str());
									cancer[3] = atoi(explode(",", color[2])[0].c_str());
								}
							}

							bool open_wings = atoi(explode("\n", t_[7])[0].c_str());
							if (open_wings) pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_1_OPEN_WINGS;
							else pInfo(peer)->_flags &= ~Gtps3::RIFTWINGS_FLAGS_STYLE_1_OPEN_WINGS;

							bool closed_wings = atoi(explode("\n", t_[8])[0].c_str());
							if (closed_wings) pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_1_CLOSE_WINGS;
							else pInfo(peer)->_flags &= ~Gtps3::RIFTWINGS_FLAGS_STYLE_1_CLOSE_WINGS;

							bool stamp_particle = atoi(explode("\n", t_[9])[0].c_str());
							if (stamp_particle) pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_1_STAMP_PARTICLE;
							else pInfo(peer)->_flags &= ~Gtps3::RIFTWINGS_FLAGS_STYLE_1_STAMP_PARTICLE;

							bool trail0 = atoi(explode("\n", t_[10])[0].c_str());
							if (trail0) pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_1_TRAIL_ON;
							else pInfo(peer)->_flags &= ~Gtps3::RIFTWINGS_FLAGS_STYLE_1_TRAIL_ON;


				

							pInfo(peer)->_flags &= ~Gtps3::RIFTWINGS_FLAGS_STYLE_1_PORTAL_AURA;
							pInfo(peer)->_flags &= ~Gtps3::RIFTWINGS_FLAGS_STYLE_2_PORTAL_AURA;
							pInfo(peer)->_flags &= ~Gtps3::RIFTWINGS_FLAGS_STYLE_1_STARFIELD_AURA;
							pInfo(peer)->_flags &= ~Gtps3::RIFTWINGS_FLAGS_STYLE_2_STARFIELD_AURA;

							bool p_aura = atoi(explode("\n", t_[11])[0].c_str()), starfield_Aura = atoi(explode("\n", t_[12])[0].c_str()), electrical_aura = atoi(explode("\n", t_[13])[0].c_str());
							if (electrical_aura)
								pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_1_ELECTRICAL_AURA;
							else if (p_aura)
								pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_1_PORTAL_AURA;
							else if (starfield_Aura)
								pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_1_STARFIELD_AURA;
							else
								pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_1_PORTAL_AURA;

							pInfo(peer)->_flags &= ~Gtps3::RIFTWINGS_FLAGS_STYLE_1_MATERIAL_FEATHERS;
							pInfo(peer)->_flags &= ~Gtps3::RIFTWINGS_FLAGS_STYLE_2_MATERIAL_FEATHERS;
							pInfo(peer)->_flags &= ~Gtps3::RIFTWINGS_FLAGS_STYLE_1_MATERIAL_BLADES;
							pInfo(peer)->_flags &= ~Gtps3::RIFTWINGS_FLAGS_STYLE_2_MATERIAL_BLADES;

							bool feathers = atoi(explode("\n", t_[14])[0].c_str()), blades = atoi(explode("\n", t_[15])[0].c_str()), scales = atoi(explode("\n", t_[16])[0].c_str());
							if (scales)
								pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_1_MATERIAL_SCALES;
							else if (feathers)
								pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_1_MATERIAL_FEATHERS;
							else if (blades)
								pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_1_MATERIAL_BLADES;
							else
								pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_1_MATERIAL_FEATHERS;

							color_convert = explode("\n", t_[17])[0].c_str();
							color = explode(",", color_convert);
							if (color.size() == 3) {
								if (atoi(explode(",", color[0])[0].c_str()) >= 0 && atoi(explode(",", color[0])[0].c_str()) <= 255 && atoi(explode(",", color[1])[0].c_str()) >= 0 && atoi(explode(",", color[1])[0].c_str()) <= 255 && atoi(explode(",", color[2])[0].c_str()) >= 0 && atoi(explode(",", color[2])[0].c_str()) <= 255) {
									pInfo(peer)->_CapeStyleColor_2 = color_convert;
									uint8_t* cancer = (uint8_t*)(&(pInfo(peer)->wings_t2));
									cancer[1] = atoi(explode(",", color[0])[0].c_str());
									cancer[2] = atoi(explode(",", color[1])[0].c_str());
									cancer[3] = atoi(explode(",", color[2])[0].c_str());
								}
							}

							color_convert = explode("\n", t_[18])[0].c_str();
							color = explode(",", color_convert);
							if (color.size() == 3) {
								if (atoi(explode(",", color[0])[0].c_str()) >= 0 && atoi(explode(",", color[0])[0].c_str()) <= 255 && atoi(explode(",", color[1])[0].c_str()) >= 0 && atoi(explode(",", color[1])[0].c_str()) <= 255 && atoi(explode(",", color[2])[0].c_str()) >= 0 && atoi(explode(",", color[2])[0].c_str()) <= 255) {
									pInfo(peer)->_CapeCollarColor_2 = color_convert;
									uint8_t* cancer = (uint8_t*)(&(pInfo(peer)->wings_c2));
									cancer[1] = atoi(explode(",", color[0])[0].c_str());
									cancer[2] = atoi(explode(",", color[1])[0].c_str());
									cancer[3] = atoi(explode(",", color[2])[0].c_str());
								}
							}

							bool open_wings2 = atoi(explode("\n", t_[19])[0].c_str());
							if (open_wings2) pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_2_OPEN_WINGS;
							else pInfo(peer)->_flags &= ~Gtps3::RIFTWINGS_FLAGS_STYLE_2_OPEN_WINGS;

							bool closed_wings2 = atoi(explode("\n", t_[20])[0].c_str());
							if (closed_wings2) pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_2_CLOSE_WINGS;
							else pInfo(peer)->_flags &= ~Gtps3::RIFTWINGS_FLAGS_STYLE_2_CLOSE_WINGS;

							bool stamp_particle2 = atoi(explode("\n", t_[21])[0].c_str());
							if (stamp_particle2) pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_2_STAMP_PARTICLE;
							else pInfo(peer)->_flags &= ~Gtps3::RIFTWINGS_FLAGS_STYLE_2_STAMP_PARTICLE;

							bool trail02 = atoi(explode("\n", t_[22])[0].c_str());
							if (trail02) pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_2_TRAIL_ON;
							else pInfo(peer)->_flags &= ~Gtps3::RIFTWINGS_FLAGS_STYLE_2_TRAIL_ON;


							bool p_aura2 = atoi(explode("\n", t_[23])[0].c_str()), starfield_Aura2 = atoi(explode("\n", t_[24])[0].c_str()), electrical_aura2 = atoi(explode("\n", t_[25])[0].c_str());
							if (electrical_aura2)
								pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_2_ELECTRICAL_AURA;
							else if (p_aura2)
								pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_2_PORTAL_AURA;
							else if (starfield_Aura2)
								pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_2_STARFIELD_AURA;
							else
								pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_2_PORTAL_AURA;

							bool feathers2 = atoi(explode("\n", t_[26])[0].c_str()), blades2 = atoi(explode("\n", t_[27])[0].c_str()), scales2 = atoi(explode("\n", t_[28])[0].c_str());
							if (scales2)
								pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_2_MATERIAL_SCALES;
							else if (feathers2)
								pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_2_MATERIAL_FEATHERS;
							else if (blades2)
								pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_2_MATERIAL_BLADES;
							else
								pInfo(peer)->_flags |= Gtps3::RIFTWINGS_FLAGS_STYLE_2_MATERIAL_FEATHERS;

						}
						else if (t_.size() == 30) {
							string button = explode("\n", t_[3])[0].c_str();
							if (button == "restore_default") {
								pInfo(peer)->wings_t = 3356909055, pInfo(peer)->wings_c = 4282965247, pInfo(peer)->wings_t2 = 723421695, pInfo(peer)->wings_c2 = 1059267327;
								pInfo(peer)->_flags = 104912, pInfo(peer)->_TimeDilation = 30;
								pInfo(peer)->_CapeStyleColor_1 = "93,22,200", pInfo(peer)->_CapeCollarColor_1 = "220,72,255", pInfo(peer)->_CapeStyleColor_2 = "137,30,43", pInfo(peer)->_CapeCollarColor_2 = "34,35,63";
							}
							else if (button == "button_manual") PSendRiftwingsDialog(peer, "\nadd_textbox|This Wing has several special functions!|left|\nadd_spacer|small|\nadd_textbox|To set the color for the wings and metal you need to enter an RGB(Red, Green, Blue) value. To separate the individual values, you need to use a comma.|left|\nadd_spacer|small|\nadd_textbox|Set the Time Dilation Cycle Time to define how often the Wings will change between the two Wing Styles. Cycle time is in seconds; maximum number of seconds allowed is: 86400 seconds (24 hours).|left|");
						}
						update_clothes(peer);
						break;
							}
						else if (cch.find("action|dialog_return\ndialog_name|dialog_rift_cape") != string::npos) {
						if (pInfo(peer)->back != 10424) break;
						vector<string> t_ = explode("|", cch), color;
						if (t_.size() == 23) {
							string color_convert = "";
							if (atoi(explode("\n", t_[3])[0].c_str()) > 0 && atoi(explode("\n", t_[3])[0].c_str()) < 86400) pInfo(peer)->TimeDilation = atoi(explode("\n", t_[3])[0].c_str());
							bool time_dilation = atoi(explode("\n", t_[4])[0].c_str());
							if (time_dilation) pInfo(peer)->flags |= Gtps3::RIFTCAPE_FLAGS_TIME_DILATION_ON;
							else pInfo(peer)->flags &= ~Gtps3::RIFTCAPE_FLAGS_TIME_DILATION_ON;

							color_convert = explode("\n", t_[5])[0].c_str();
							color = explode(",", color_convert);
							if (color.size() == 3) {
								if (atoi(explode(",", color[0])[0].c_str()) >= 0 && atoi(explode(",", color[0])[0].c_str()) <= 255 && atoi(explode(",", color[1])[0].c_str()) >= 0 && atoi(explode(",", color[1])[0].c_str()) <= 255 && atoi(explode(",", color[2])[0].c_str()) >= 0 && atoi(explode(",", color[2])[0].c_str()) <= 255) {
									pInfo(peer)->CapeStyleColor_1 = color_convert;
									uint8_t* cancer = (uint8_t*)(&(pInfo(peer)->cape_t));
									cancer[1] = atoi(explode(",", color[0])[0].c_str());
									cancer[2] = atoi(explode(",", color[1])[0].c_str());
									cancer[3] = atoi(explode(",", color[2])[0].c_str());
								}
							}

							bool cape_collar = atoi(explode("\n", t_[6])[0].c_str());
							if (cape_collar) pInfo(peer)->flags |= Gtps3::RIFTCAPE_FLAGS_STYLE_1_COLLAR_ON;
							else pInfo(peer)->flags &= ~Gtps3::RIFTCAPE_FLAGS_STYLE_1_COLLAR_ON;


							color_convert = explode("\n", t_[7])[0].c_str();
							color = explode(",", color_convert);
							if (color.size() == 3) {
								if (atoi(explode(",", color[0])[0].c_str()) >= 0 && atoi(explode(",", color[0])[0].c_str()) <= 255 && atoi(explode(",", color[1])[0].c_str()) >= 0 && atoi(explode(",", color[1])[0].c_str()) <= 255 && atoi(explode(",", color[2])[0].c_str()) >= 0 && atoi(explode(",", color[2])[0].c_str()) <= 255) {
									pInfo(peer)->CapeCollarColor_1 = color_convert;
									uint8_t* cancer = (uint8_t*)(&(pInfo(peer)->cape_c));
									cancer[1] = atoi(explode(",", color[0])[0].c_str());
									cancer[2] = atoi(explode(",", color[1])[0].c_str());
									cancer[3] = atoi(explode(",", color[2])[0].c_str());
								}
							}

							bool closed_cape = atoi(explode("\n", t_[8])[0].c_str());
							if (closed_cape) pInfo(peer)->flags |= Gtps3::RIFTCAPE_FLAGS_STYLE_1_CLOSED_CAPE;
							else pInfo(peer)->flags &= ~Gtps3::RIFTCAPE_FLAGS_STYLE_1_CLOSED_CAPE;


							bool open_cape = atoi(explode("\n", t_[9])[0].c_str());
							if (open_cape) pInfo(peer)->flags |= Gtps3::RIFTCAPE_FLAGS_STYLE_1_OPEN_CAPE_ON_MOVEMENT;
							else pInfo(peer)->flags &= ~Gtps3::RIFTCAPE_FLAGS_STYLE_1_OPEN_CAPE_ON_MOVEMENT;

							bool aura_on = atoi(explode("\n", t_[10])[0].c_str());
							if (aura_on) pInfo(peer)->flags |= Gtps3::RIFTCAPE_FLAGS_STYLE_1_AURA_ON;
							else pInfo(peer)->flags &= ~Gtps3::RIFTCAPE_FLAGS_STYLE_1_AURA_ON;

							bool p_aura = atoi(explode("\n", t_[11])[0].c_str()), starfield_Aura = atoi(explode("\n", t_[12])[0].c_str()), electrical_aura = atoi(explode("\n", t_[13])[0].c_str());

							pInfo(peer)->flags &= ~Gtps3::RIFTCAPE_FLAGS_STYLE_1_PORTAL_AURA;
							pInfo(peer)->flags &= ~Gtps3::RIFTCAPE_FLAGS_STYLE_2_PORTAL_AURA;
							pInfo(peer)->flags &= ~Gtps3::RIFTCAPE_FLAGS_STYLE_1_STARFIELD_AURA;
							pInfo(peer)->flags &= ~Gtps3::RIFTCAPE_FLAGS_STYLE_2_STARFIELD_AURA;

							if (electrical_aura)
								pInfo(peer)->flags |= Gtps3::RIFTCAPE_FLAGS_STYLE_1_ELECTRICAL_AURA;
							else if (p_aura)
								pInfo(peer)->flags |= Gtps3::RIFTCAPE_FLAGS_STYLE_1_PORTAL_AURA;
							else if (starfield_Aura)
								pInfo(peer)->flags |= Gtps3::RIFTCAPE_FLAGS_STYLE_1_STARFIELD_AURA;
							else
								pInfo(peer)->flags |= Gtps3::RIFTCAPE_FLAGS_STYLE_1_PORTAL_AURA;

							color_convert = explode("\n", t_[14])[0].c_str();
							color = explode(",", color_convert);
							if (color.size() == 3) {
								if (atoi(explode(",", color[0])[0].c_str()) >= 0 && atoi(explode(",", color[0])[0].c_str()) <= 255 && atoi(explode(",", color[1])[0].c_str()) >= 0 && atoi(explode(",", color[1])[0].c_str()) <= 255 && atoi(explode(",", color[2])[0].c_str()) >= 0 && atoi(explode(",", color[2])[0].c_str()) <= 255) {
									pInfo(peer)->CapeStyleColor_2 = color_convert;
									uint8_t* cancer = (uint8_t*)(&(pInfo(peer)->cape_t2));
									cancer[1] = atoi(explode(",", color[0])[0].c_str());
									cancer[2] = atoi(explode(",", color[1])[0].c_str());
									cancer[3] = atoi(explode(",", color[2])[0].c_str());
								}
							}


							bool collar_aura = atoi(explode("\n", t_[15])[0].c_str());
							if (collar_aura) pInfo(peer)->flags |= Gtps3::RIFTCAPE_FLAGS_STYLE_2_COLLAR_ON;
							else pInfo(peer)->flags &= ~Gtps3::RIFTCAPE_FLAGS_STYLE_2_COLLAR_ON;


							color_convert = explode("\n", t_[16])[0].c_str();
							color = explode(",", color_convert);
							if (color.size() == 3) {
								if (atoi(explode(",", color[0])[0].c_str()) >= 0 && atoi(explode(",", color[0])[0].c_str()) <= 255 && atoi(explode(",", color[1])[0].c_str()) >= 0 && atoi(explode(",", color[1])[0].c_str()) <= 255 && atoi(explode(",", color[2])[0].c_str()) >= 0 && atoi(explode(",", color[2])[0].c_str()) <= 255) {
									pInfo(peer)->CapeCollarColor_2 = color_convert;
									uint8_t* cancer = (uint8_t*)(&(pInfo(peer)->cape_c2));
									cancer[1] = atoi(explode(",", color[0])[0].c_str());
									cancer[2] = atoi(explode(",", color[1])[0].c_str());
									cancer[3] = atoi(explode(",", color[2])[0].c_str());
								}
							}



							bool closed_cape2 = atoi(explode("\n", t_[17])[0].c_str());
							if (closed_cape2) pInfo(peer)->flags |= Gtps3::RIFTCAPE_FLAGS_STYLE_2_CLOSED_CAPE;
							else pInfo(peer)->flags &= ~Gtps3::RIFTCAPE_FLAGS_STYLE_2_CLOSED_CAPE;

							bool cape_movement2 = atoi(explode("\n", t_[18])[0].c_str());
							if (cape_movement2) pInfo(peer)->flags |= Gtps3::RIFTCAPE_FLAGS_STYLE_2_OPEN_CAPE_ON_MOVEMENT;
							else pInfo(peer)->flags &= ~Gtps3::RIFTCAPE_FLAGS_STYLE_2_OPEN_CAPE_ON_MOVEMENT;

							bool aura_on_2 = atoi(explode("\n", t_[19])[0].c_str());
							if (aura_on_2) pInfo(peer)->flags |= Gtps3::RIFTCAPE_FLAGS_STYLE_2_AURA_ON;
							else pInfo(peer)->flags &= ~Gtps3::RIFTCAPE_FLAGS_STYLE_2_AURA_ON;

							bool p_aura_2 = atoi(explode("\n", t_[20])[0].c_str()), starfield_aura_2 = atoi(explode("\n", t_[21])[0].c_str()), electrical_2 = atoi(explode("\n", t_[22])[0].c_str());


							if (electrical_2)
								pInfo(peer)->flags |= Gtps3::RIFTCAPE_FLAGS_STYLE_2_ELECTRICAL_AURA;
							else if (p_aura_2)
								pInfo(peer)->flags |= Gtps3::RIFTCAPE_FLAGS_STYLE_2_PORTAL_AURA;
							else if (starfield_aura_2)
								pInfo(peer)->flags |= Gtps3::RIFTCAPE_FLAGS_STYLE_2_STARFIELD_AURA;
							else
								pInfo(peer)->flags |= Gtps3::RIFTCAPE_FLAGS_STYLE_2_PORTAL_AURA;

						}
						else if (t_.size() == 24) {
							string button = explode("\n", t_[3])[0].c_str();
							if (button == "restore_default") {
								pInfo(peer)->cape_t = 2402849791, pInfo(peer)->cape_c = 2402849791, pInfo(peer)->cape_t2 = 723421695, pInfo(peer)->cape_c2 = 1059267327;
								pInfo(peer)->flags = 19451, pInfo(peer)->TimeDilation = 30;
								pInfo(peer)->CapeStyleColor_1 = "147,56,143", pInfo(peer)->CapeCollarColor_1 = "147,56,143", pInfo(peer)->CapeStyleColor_2 = "137,30,43", pInfo(peer)->CapeCollarColor_2 = "34,35,36";
								update_clothes(peer);
							}
							else if (button == "button_manual") PSendRiftcapeDialog(peer, "\nadd_textbox|This cape has several special functions!|left|\nadd_spacer|small|\nadd_textbox|To set the color for the cape and collar you need to enter an RGB (Red, Blue, Green) value. To separate the individual values, you need to use a comma.|left|\nadd_spacer|small|\nadd_textbox|Set the Time Dilation Cycle Time to define how often the cape will change between the two Cape Styles. Cycle time is in seconds; maximum number of seconds allowed is: 86400 seconds (24 hours).|left|");
						}
						update_clothes(peer);
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|skin_color") != string::npos) {
						vector<string> t_ = explode("|", cch), color;
						string color_convert = explode("\n", t_[3])[0].c_str();
						if (color_convert == "restore_default") {
							pInfo(peer)->skin_c = "0,0,0";
							pInfo(peer)->skin = 0x8295C3FF;
						}
						else {
							color = explode(",", color_convert);
							if (color.size() == 3) {
								if (atoi(explode(",", color[0])[0].c_str()) >= 0 && atoi(explode(",", color[0])[0].c_str()) <= 255 && atoi(explode(",", color[1])[0].c_str()) >= 0 && atoi(explode(",", color[1])[0].c_str()) <= 255 && atoi(explode(",", color[2])[0].c_str()) >= 0 && atoi(explode(",", color[2])[0].c_str()) <= 255) {
									pInfo(peer)->skin_c = color_convert;
									uint8_t* cancer = (uint8_t*)(&(pInfo(peer)->skin));
									cancer[1] = atoi(explode(",", color[0])[0].c_str());
									cancer[2] = atoi(explode(",", color[1])[0].c_str());
									cancer[3] = atoi(explode(",", color[2])[0].c_str());
									//cout << pInfo(peer)->skin << endl;
								}
							}
						}
						update_clothes(peer);
						break;
						}
							/*
							* 						else if (cch == "action|pineapplepartycommunity\n") {
							gamepacket_t p(500);
							p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_image_button||interface/large/gui_shop_winter_rally.rttex||||\nadd_custom_textbox|`wEat as many pineapples as possible to get `2awesome rewards!``|size:medium|\nadd_spacer|small|\nadd_custom_textbox|Global Count:|size:medium|\nadd_custom_textbox|" + setGems(total_pineapple_eaten) + "|size:large|\nadd_spacer|small|\nadd_custom_textbox|Rewards|size:medium|\nadd_custom_textbox|Pineapple available from Super Pineapple Magic!|icon:2734;size:small;color:255,255,255,255|\nadd_spacer|small|\nadd_custom_textbox|50,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 50000 ? "255" : "80") + "|\nadd_custom_textbox|Pineapple Block available from Super Pineapple Magic!|icon:2732;size:small;color:255,255,255," + (total_pineapple_eaten > 50000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|250,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 250000 ? "255" : "80") + "|\nadd_custom_textbox|Pineapple Daiquiri available from Super Pineapple Magic!|icon:3622;size:small;color:255,255,255," + (total_pineapple_eaten > 250000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|500,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 500000 ? "255" : "80") + "|\nadd_custom_textbox|Sugarloaf Pineapple Block available from Super Pineapple Magic!|icon:2746;size:small;color:255,255,255," + (total_pineapple_eaten > 500000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|625,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 625000 ? "255" : "80") + "|\nadd_custom_textbox|Banana Onesie available from Super Pineapple Magic!|icon:13508;size:small;color:255,255,255," + (total_pineapple_eaten > 625000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|875,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 875000 ? "255" : "80") + "|\nadd_custom_textbox|Loop of Fruits available from Super Pineapple Magic!|icon:13504;size:small;color:255,255,255," + (total_pineapple_eaten > 875000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|975,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 975000 ? "255" : "80") + "|\nadd_custom_textbox|Maddening Mandarin available from Super Pineapple Magic!|icon:13506;size:small;color:255,255,255," + (total_pineapple_eaten > 975000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|1,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 1000000 ? "255" : "80") + "|\nadd_custom_textbox|Pineapple Pants available from Super Pineapple Magic!|icon:2742;size:small;color:255,255,255," + (total_pineapple_eaten > 1000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|5,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 5000000 ? "255" : "80") + "|add_custom_textbox|Frozen Pineapple Block available from Super Pineapple Magic!|icon:2750;size:small;color:255,255,255," + (total_pineapple_eaten > 5000000 ? "255" : "80") + "|add_spacer|small|\nadd_custom_textbox|10,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 10000000 ? "255" : "80") + "|\nadd_custom_textbox|Pineapple Hair available from Super Pineapple Magic!|icon:3624;size:small;color:255,255,255," + (total_pineapple_eaten > 10000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|12,500,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 12500000 ? "255" : "80") + "|\nadd_custom_textbox|Pineapple Sign available from Super Pineapple Magic!|icon:6102;size:small;color:255,255,255," + (total_pineapple_eaten > 12500000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|15,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 15000000 ? "255" : "80") + "|\nadd_custom_textbox|Hawaiian Shirt available from Super Pineapple Magic!|icon:2740;size:small;color:255,255,255," + (total_pineapple_eaten > 15000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|16,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 16000000 ? "255" : "80") + "|\nadd_custom_textbox|Super Pineapple House available from Super Pineapple Magic!|icon:12002;size:small;color:255,255,255," + (total_pineapple_eaten > 16000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|17,500,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 17500000 ? "255" : "80") + "|\nadd_custom_textbox|Pure Pineapple A+ Shirt available from Super Pineapple Magic!|icon:10898;size:small;color:255,255,255," + (total_pineapple_eaten > 17500000 ? "255" : "80") + "|\nadd_custom_textbox|Pure Pineapple A+ Scarf available from Super Pineapple Magic!|icon:10900;size:small;color:255,255,255," + (total_pineapple_eaten > 17500000 ? "255" : "80") + "|\nadd_custom_textbox|Pure Pineapple A+ Pants available from Super Pineapple Magic!|icon:10902;size:small;color:255,255,255," + (total_pineapple_eaten > 17500000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|20,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 20000000 ? "255" : "80") + "|\nadd_custom_textbox|Roasted Pineapple Block available from Super Pineapple Magic!|icon:2748;size:small;color:255,255,255," + (total_pineapple_eaten > 20000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|22,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 22000000 ? "255" : "80") + "|\nadd_custom_textbox|Pineapple Pennant available from Super Pineapple Magic!|icon:9664;size:small;color:255,255,255," + (total_pineapple_eaten > 22000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|25,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 25000000 ? "255" : "80") + "|\nadd_custom_textbox|Pineapple Earrings available from Super Pineapple Magic!|icon:2760;size:small;color:255,255,255," + (total_pineapple_eaten > 25000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|27,500,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 27500000 ? "255" : "80") + "|\nadd_custom_textbox|Pineapple Wallpaper available from Super Pineapple Magic!|icon:9668;size:small;color:255,255,255," + (total_pineapple_eaten > 27500000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|30,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 30000000 ? "255" : "80") + "|\nadd_custom_textbox|Adventure Item - Pineapple available from Super Pineapple Magic!|icon:4716;size:small;color:255,255,255," + (total_pineapple_eaten > 30000000 ? "255" : "80") + "|add_spacer|small|\nadd_custom_textbox|35,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 35000000 ? "255" : "80") + "|add_custom_textbox|Pineapple Glasses available from Super Pineapple Magic!|icon:4772;size:small;color:255,255,255," + (total_pineapple_eaten > 35000000 ? "255" : "80") + "|add_spacer|small|\nadd_custom_textbox|37,500,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 375000000 ? "255" : "80") + "|\nadd_custom_textbox|Pineapple Table available from Super Pineapple Magic!|icon:10908;size:small;color:255,255,255," + (total_pineapple_eaten > 375000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|40,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 40000000 ? "255" : "80") + "|\nadd_custom_textbox|Fruity Hat available from Super Pineapple Magic!|icon:2738;size:small;color:255,255,255," + (total_pineapple_eaten > 40000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|45,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 45000000 ? "255" : "80") + "|\nadd_custom_textbox|Pineapple Juice available from Super Pineapple Magic!|icon:2758;size:small;color:255,255,255," + (total_pineapple_eaten > 45000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|47,500,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 475000000 ? "255" : "80") + "|\nadd_custom_textbox|Pineapple Robes available from Super Pineapple Magic!|icon:9652;size:small;color:255,255,255," + (total_pineapple_eaten > 475000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|50,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 50000000 ? "255" : "80") + "|\nadd_custom_textbox|Bamboo Frame available from Super Pineapple Magic!|icon:6098;size:small;color:255,255,255," + (total_pineapple_eaten > 50000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|55,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 55000000 ? "255" : "80") + "|\nadd_custom_textbox|Pineapple Kite available from Super Pineapple Magic!|icon:2762;size:small;color:255,255,255," + (total_pineapple_eaten > 55000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|58,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 58000000 ? "255" : "80") + "|\nadd_custom_textbox|Super Pineapple Window available from Super Pineapple Magic!|icon:12004;size:small;color:255,255,255," + (total_pineapple_eaten > 58000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|60,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 60000000 ? "255" : "80") + "|\nadd_custom_textbox|Pineapple Tiki Door available from Super Pineapple Magic!|icon:6104;size:small;color:255,255,255," + (total_pineapple_eaten > 60000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|65,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 65000000 ? "255" : "80") + "|\nadd_custom_textbox|Pineapple Ring available from Super Pineapple Magic!|icon:3618;size:small;color:255,255,255," + (total_pineapple_eaten > 65000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|70,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 70000000 ? "255" : "80") + "|\nadd_custom_textbox|Pineapple Koa Lantern available from Super Pineapple Magic!|icon:6100;size:small;color:255,255,255," + (total_pineapple_eaten > 70000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|72,500,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 725000000 ? "255" : "80") + "|\nadd_custom_textbox|The Pine Throne available from Super Pineapple Magic!|icon:10896;size:small;color:255,255,255," + (total_pineapple_eaten > 725000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|75,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 75000000 ? "255" : "80") + "|\nadd_custom_textbox|Pineapple Air Freshener available from Super Pineapple Magic!|icon:2730;size:small;color:255,255,255," + (total_pineapple_eaten > 75000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|80,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 80000000 ? "255" : "80") + "|\nadd_custom_textbox|Pineapple Plating available from Super Pineapple Magic!|icon:6118;size:small;color:255,255,255," + (total_pineapple_eaten > 80000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|82,500,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 825000000 ? "255" : "80") + "|\nadd_custom_textbox|Growmoji Super Pineapple available from Super Pineapple Magic!|icon:6122;size:small;color:255,255,255," + (total_pineapple_eaten > 825000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|85,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 85000000 ? "255" : "80") + "|\nadd_custom_textbox|Pineapple Launcher available from Super Pineapple Magic!|icon:2752;size:small;color:255,255,255," + (total_pineapple_eaten > 85000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|90,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 90000000 ? "255" : "80") + "|\nadd_custom_textbox|Pineapple Finger Ring available from Super Pineapple Magic!|icon:4770;size:small;color:255,255,255," + (total_pineapple_eaten > 90000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|91,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 91000000 ? "255" : "80") + "|\nadd_custom_textbox|Pineapple Fountain available from Super Pineapple Magic!|icon:12012;size:small;color:255,255,255," + (total_pineapple_eaten > 91000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|92,500,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 92500000 ? "255" : "80") + "|\nadd_custom_textbox|Pineapple Coronet available from Super Pineapple Magic!|icon:9656;size:small;color:255,255,255," + (total_pineapple_eaten > 92500000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|95,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 95000000 ? "255" : "80") + "|\nadd_custom_textbox|Guardian Pineapple available from Super Pineapple Magic!|icon:3616;size:small;color:255,255,255," + (total_pineapple_eaten > 95000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|100,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 100000000 ? "255" : "80") + "|\nadd_custom_textbox|Weather Machine - Pineapples available from Super Pineapple Magic!|icon:2744;size:small;color:255,255,255," + (total_pineapple_eaten > 100000000 ? "255" : "80") + "|\nadd_spacer|small|\nadd_custom_textbox|105,000,000|size:medium;color:73,252,0," + (total_pineapple_eaten > 105000000 ? "255" : "80") + "|\nadd_custom_textbox|Pineapple Spear available from Super Pineapple Magic!|icon:6110;size:small;color:255,255,255," + (total_pineapple_eaten > 105000000 ? "255" : "80") + "|\nadd_spacer|small|\nend_dialog|pineapplepartycommunity|Close||\nadd_quick_exit|"), p.CreatePacket(peer);
							break;
						}
						else if (cch == "action|blackboxeventdialog\n") {
						gamepacket_t p(500);
						p.Insert("OnDialogRequest");
						p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wBlack Box X-Treme!``|left|11476|\nadd_spacer|small|\nadd_textbox|During the Black Friday weekend, consuming 16 Black Friday Black Box will give you a Black Friday Black Box Xtreme.|left|\nadd_spacer|small|\nadd_textbox|Black Boxes can be found by consuming Cashback Coupons or purchasing Gem Packs from the store.|left|\nadd_spacer|small|\nadd_textbox|Current Progress: " + to_string(pInfo(peer)->black_box_consumed) + "/16|left|\nadd_spacer|small|\nadd_textbox|Reward:|left|\nadd_label_with_icon|small|Black Friday Black Box Xtreme|left|11476|\nadd_spacer|small|\nadd_button|claimreward|Claim Reward|" + (pInfo(peer)->black_box_consumed >= 16 ? "on" : "off") + "|0|0|\nend_dialog|blackboxstoreevent||CANCEL|");
						p.CreatePacket(peer);
						break;
						}
						else if (cch == "action|dialog_return\ndialog_name|blackboxstoreevent\n" || cch == "action|dialog_return\ndialog_name|\nbuttonClicked|open_store\n\n") {
						shop_tab(peer, "tab1");
						break;
						}
						else if (cch == "action|dialog_return\ndialog_name|blackboxstoreevent\nbuttonClicked|claimreward\n\n") {
						if (pInfo(peer)->black_box_consumed >= 16) {
							int c_ = 1, give_ = 11476;
							gamepacket_t p_c;
							p_c.Insert("OnConsoleMessage");
							if (modify_inventory(peer, 11476, c_) == 0) {
								pInfo(peer)->black_box_consumed -= 16;
								gamepacket_t p2;
								p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("You received 1 "+items[give_].ori_name + "."), p2.CreatePacket(peer);
								p_c.Insert("You received 1 " + items[give_].ori_name + ".");
							}
							else p_c.Insert("Your inventory is full!");
							p_c.CreatePacket(peer);
						}
						break;
						}*/
						else if (cch.find("action|dialog_return\ndialog_name|handleHalloweenShopVerification\nbuttonClicked|back") != string::npos || cch.find("action|dialog_return\ndialog_name|wfcalendar_reward_list_dialog\nbuttonClicked|goto_maindialog") != string::npos or cch.find("action|dialog_return\ndialog_name|transmutated_device_edit\nbuttonClicked|fromHelpBackToModes") != string::npos or cch.find("action|dialog_return\ndialog_name|transmutated_device_edit\nbuttonClicked|beforeMainBackToModes") != string::npos or cch.find("action|dialog_return\ndialog_name|remove_transmutated_dialog") != string::npos or cch.find("action|dialog_return\ndialog_name|handleBeachPartyShopVerification\nbuttonClicked|back") != string::npos or cch.find("action|dialog_return\ndialog_name|handleRubblePartyShopVerification\nbuttonClicked|back") != string::npos or cch.find("action|dialog_return\ndialog_name|transmutated_device_edit\nbuttonClicked|fromListBackToModes") != string::npos) {
							edit_tile(peer, pInfo(peer)->lastwrenchx, pInfo(peer)->lastwrenchy, 32);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|statsblockworld\nbuttonClicked|back_to_gscan\n") != string::npos) {
						string name_ = pInfo(peer)->world;
						vector<World>::iterator pd = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
						if (pd != worlds.end()) {
							World* world_ = &worlds[pd - worlds.begin()];
							world_->fresh_world = true;
							WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
							growscan_load(peer, world_, block_);
						}
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|cheats") != string::npos) {
						if (pInfo(peer)->world == Hide_N_Seek.hidenseekworld) break;
						if (pInfo(peer)->cheater_ or has_playmod2(pInfo(peer), 143) or has_playmod2(pInfo(peer), 153)) {
							vector<string> t_ = explode("|", cch);
							bool choose_autofarm = false;
							if (t_.size() < 18) break;
							string btn = explode("\n", t_[3])[0].c_str();
							if (btn == "restore_default") {
								pInfo(peer)->cheater_settings = 0;
								pInfo(peer)->chat_prefix = "";
								break;
							}
							if (t_.size() > 18) {
								choose_autofarm = true;
								int item_ = atoi(explode("\n", t_[3])[0].c_str());
								if (item_ == 0) autofarm_status(peer);
								else if (item_ == 1) {
								}
								else {
									if (item_ > 0 && item_ < items.size()) {
										bool accept_ = false, not_farmable = false;
										if (item_ == 5640) item_ = 2;
										if (items[item_].untradeable || items[item_].rarity < 0 || items[item_].rarity == 999 || items[item_].block_possible_put == false) {
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("`6[``You can't autofarm this block!```6]``");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
										else {
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("`6[``Autofarm is enabled (choose location by placing a block)!```6]``");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
											pInfo(peer)->autofarm_x = -1;
											pInfo(peer)->autofarm_y = -1;
											pInfo(peer)->cheater_settings |= Gtps3::SETTINGS_0;
											pInfo(peer)->last_used_block = atoi(explode("\n", t_[3])[0].c_str());
										}
									}
								}
							}

							int autofarm_slot = atoi(explode("\n", t_[3 + (choose_autofarm ? 1 : 0)])[0].c_str());
							if (autofarm_slot >= 1 && autofarm_slot <= (pInfo(peer)->hand != 13700 ? 3 : 6)) {
								pInfo(peer)->autofarm_slot = autofarm_slot;
							}
							else {
								gamepacket_t p;
								p.Insert("OnTalkBubble");
								p.Insert(pInfo(peer)->netID);
								p.Insert("Autofarm slot is over limit!");
								p.Insert(0), p.Insert(0);
								p.CreatePacket(peer);
							}



							if (atoi(explode("\n", t_[4 + (choose_autofarm ? 1 : 0)])[0].c_str())) pInfo(peer)->cheater_settings |= Gtps3::SETTINGS_16;
							else pInfo(peer)->cheater_settings &= ~Gtps3::SETTINGS_16;

							if (atoi(explode("\n", t_[5 + (choose_autofarm ? 1 : 0)])[0].c_str())) pInfo(peer)->cheater_settings |= Gtps3::SETTINGS_3;
							else pInfo(peer)->cheater_settings &= ~Gtps3::SETTINGS_3;


							if (atoi(explode("\n", t_[6 + (choose_autofarm ? 1 : 0)])[0].c_str())) pInfo(peer)->cheater_settings |= Gtps3::SETTINGS_4;
							else pInfo(peer)->cheater_settings &= ~Gtps3::SETTINGS_4;
							if (atoi(explode("\n", t_[7 + (choose_autofarm ? 1 : 0)])[0].c_str())) pInfo(peer)->cheater_settings |= Gtps3::SETTINGS_2;
							else pInfo(peer)->cheater_settings &= ~Gtps3::SETTINGS_2;
							if (atoi(explode("\n", t_[8+ (choose_autofarm ? 1 : 0)])[0].c_str())) pInfo(peer)->cheater_settings |= Gtps3::SETTINGS_7;
							else pInfo(peer)->cheater_settings &= ~Gtps3::SETTINGS_7;
							if (atoi(explode("\n", t_[9 + (choose_autofarm ? 1 : 0)])[0].c_str())) pInfo(peer)->cheater_settings |= Gtps3::SETTINGS_8;
							else pInfo(peer)->cheater_settings &= ~Gtps3::SETTINGS_8;
							if (atoi(explode("\n", t_[10 + (choose_autofarm ? 1 : 0)])[0].c_str())) pInfo(peer)->cheater_settings |= Gtps3::SETTINGS_9;
							else pInfo(peer)->cheater_settings &= ~Gtps3::SETTINGS_9;
							if (atoi(explode("\n", t_[11 + (choose_autofarm ? 1 : 0)])[0].c_str())) pInfo(peer)->cheater_settings |= Gtps3::SETTINGS_10;
							else pInfo(peer)->cheater_settings &= ~Gtps3::SETTINGS_10;
							if (atoi(explode("\n", t_[12 + (choose_autofarm ? 1 : 0)])[0].c_str())) pInfo(peer)->cheater_settings |= Gtps3::SETTINGS_11;
							else pInfo(peer)->cheater_settings &= ~Gtps3::SETTINGS_11;
							if (atoi(explode("\n", t_[13 + (choose_autofarm ? 1 : 0)])[0].c_str())) pInfo(peer)->cheater_settings |= Gtps3::SETTINGS_6;
							else pInfo(peer)->cheater_settings &= ~Gtps3::SETTINGS_6;
							/*
							if (atoi(explode("\n", t_[13 + (choose_autofarm ? 1 : 0)])[0].c_str())) pInfo(peer)->cheater_settings |= Gtps3::SETTINGS_12;
							else pInfo(peer)->cheater_settings &= ~Gtps3::SETTINGS_12;
							if (atoi(explode("\n", t_[14 + (choose_autofarm ? 1 : 0)])[0].c_str())) pInfo(peer)->cheater_settings |= Gtps3::SETTINGS_13;
							else pInfo(peer)->cheater_settings &= ~Gtps3::SETTINGS_13;*/
							if (atoi(explode("\n", t_[14 + (choose_autofarm ? 1 : 0)])[0].c_str())) pInfo(peer)->cheater_settings |= Gtps3::SETTINGS_14;
							else pInfo(peer)->cheater_settings &= ~Gtps3::SETTINGS_14;
							if (atoi(explode("\n", t_[15 + (choose_autofarm ? 1 : 0)])[0].c_str())) pInfo(peer)->cheater_settings |= Gtps3::SETTINGS_5;
							else pInfo(peer)->cheater_settings &= ~Gtps3::SETTINGS_5;
							if (atoi(explode("\n", t_[16 + (choose_autofarm ? 1 : 0)])[0].c_str())) pInfo(peer)->cheater_settings |= Gtps3::SETTINGS_15;
							else pInfo(peer)->cheater_settings &= ~Gtps3::SETTINGS_15;

							pInfo(peer)->chat_prefix = explode("\n", t_[17 + (choose_autofarm ? 1 : 0)])[0].c_str();
							if (pInfo(peer)->chat_prefix.length() > 20) pInfo(peer)->chat_prefix.clear();
							console_msg(peer, "`o>> Applying cheats...");
							update_clothes_value(peer);
							update_clothes(peer);
							break;
						}
						}
						else if (cch.find("action|dialog_return\ndialog_name|crypto\nbuttonClicked|buy_") != string::npos) {
						string crypto_name = cch.substr(58, cch.length() - 58).c_str();
						replaceAll(crypto_name, "\n", "");
						vector<pair<string, int>>::iterator p = find_if(Crypto_Update.crypto.begin(), Crypto_Update.crypto.end(), [&](const pair < string, int>& element) { return element.first == crypto_name; });
						if (p != Crypto_Update.crypto.end()) {
							int crypto_price = Crypto_Update.crypto[p - Crypto_Update.crypto.begin()].second, can_buy = pInfo(peer)->gems / crypto_price;
							string inventory = "";
							for (int i = 0; i < Crypto_Update.crypto.size(); i++) {
								inventory += "\nadd_button_with_icon||" + Crypto_Update.crypto[i].first + "|staticBlueFrame|" + to_string(item_crypto(Crypto_Update.crypto[i].first)) + "|";
								vector<pair<string, int>>::iterator p = find_if(pInfo(peer)->crypto.begin(), pInfo(peer)->crypto.end(), [&](const pair < string, int>& element) { return element.first == Crypto_Update.crypto[i].first; });
								if (p != pInfo(peer)->crypto.end()) inventory += to_string(pInfo(peer)->crypto[p - pInfo(peer)->crypto.begin()].second) + "|";
								else inventory += "0|";
							}
							gamepacket_t p2;
							p2.Insert("OnDialogRequest"), p2.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`9Purchase " + crypto_name + "``|left|" + to_string(item_crypto(crypto_name)) + "|\nadd_spacer|small|\nadd_smalltext|1 Month Graph of price:|left|\nadd_image_button||interface/large/" + crypto_name + ".rttex||||\nadd_smalltext|You can purchase Crypto Currency by using Gems!|left|\nadd_spacer|small|\nadd_smalltext|How many do you want to buy? (can purchase `2" + setGems(can_buy) + "``)|left|\nadd_smalltext|" + to_upper(crypto_name) + " Price - 1/" + setGems(crypto_price) + " ė|left|\nadd_text_input|" + crypto_name + "||0|7|\nembed_data|crypto|" + crypto_name + "\nembed_data|method|buy\nadd_spacer|small|\nadd_smalltext|Inventory:|left|\ntext_scaling_string|Crypto Currency|" + inventory + "\nadd_button_with_icon||END_LIST|noflags|0||\nadd_spacer|small|\nadd_smalltext|Stats:|left|\nadd_smalltext|Crypto Sold (24hrs): " + setGems(Crypto_Update.crypto_sold) + "    Crypto Bought (24hrs): " + setGems(Crypto_Update.crypto_bought) + "    Crypto Tax 0.10%|left|\nend_dialog|crypto_exchange|Cancel|Purchase|\nadd_quick_exit|");
							p2.CreatePacket(peer);
						}
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|crypto\nbuttonClicked|tradehistory") != string::npos) {
						string trade_history = "";
						for (int i = 0; i < pInfo(peer)->crypto_history.size(); i++) trade_history += "\nadd_spacer|small|\nadd_smalltext|" + pInfo(peer)->crypto_history[i] + "|left|";
						gamepacket_t p;
						p.Insert("OnDialogRequest");
						p.Insert("set_default_color|`o\nadd_label_with_icon|small|" + pInfo(peer)->tankIDName + "'s Crypto Trade History|left|752|" + (pInfo(peer)->crypto_history.size() == 0 ? "\nadd_spacer|small|\nadd_smalltext|Nothing to show yet.|left|" : trade_history) + "\nadd_spacer|small|\nadd_button|crypto|Back|noflags|0|0|\nadd_button||Close|noflags|0|0|\nadd_quick_exit|\nend_dialog|crypto|||");
						p.CreatePacket(peer);
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|crypto\nbuttonClicked|crypto") != string::npos) {
						send_crypto(peer);
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|crypto\nbuttonClicked|sell_") != string::npos) {
						string crypto_name = cch.substr(59, cch.length() - 59).c_str();
						replaceAll(crypto_name, "\n", "");
						vector<pair<string, int>>::iterator p = find_if(Crypto_Update.crypto_sale.begin(), Crypto_Update.crypto_sale.end(), [&](const pair < string, int>& element) { return element.first == crypto_name; });
						if (p != Crypto_Update.crypto_sale.end()) {
							int crypto_price = Crypto_Update.crypto_sale[p - Crypto_Update.crypto_sale.begin()].second, have = 0;
							string inventory = "";
							for (int i = 0; i < Crypto_Update.crypto.size(); i++) {
								inventory += "\nadd_button_with_icon||" + Crypto_Update.crypto[i].first + "|staticBlueFrame|" + to_string(item_crypto(Crypto_Update.crypto[i].first)) + "|";
								vector<pair<string, int>>::iterator p = find_if(pInfo(peer)->crypto.begin(), pInfo(peer)->crypto.end(), [&](const pair < string, int>& element) { return element.first == Crypto_Update.crypto[i].first; });
								if (p != pInfo(peer)->crypto.end()) {
									if (pInfo(peer)->crypto[p - pInfo(peer)->crypto.begin()].first == crypto_name) have = pInfo(peer)->crypto[p - pInfo(peer)->crypto.begin()].second;
									inventory += to_string(pInfo(peer)->crypto[p - pInfo(peer)->crypto.begin()].second) + "|";
								}
								else inventory += "0|";
							}
							gamepacket_t p2;
							p2.Insert("OnDialogRequest"), p2.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`9Sell " + crypto_name + "``|left|" + to_string(item_crypto(crypto_name)) + "|\nadd_spacer|small|\nadd_smalltext|1 Month Graph of price:|left|\nadd_image_button||interface/large/" + crypto_name + ".rttex||||\nadd_smalltext|You can sell Crypto Currency by using Gems!|left|\nadd_spacer|small|\nadd_smalltext|How many do you want to sell? (you have `2"+setGems(have) + "``)|left|\nadd_smalltext|" + to_upper(crypto_name) + " Price - 1/" + setGems(crypto_price) + "  ė|left|\nadd_text_input|" + crypto_name + "||0|7|\nembed_data|crypto|" + crypto_name + "\nembed_data|method|sell\nadd_spacer|small|\nadd_smalltext|Inventory:|left|\ntext_scaling_string|Crypto Currency|" + inventory + "\nadd_button_with_icon||END_LIST|noflags|0||\nadd_spacer|small|\nadd_smalltext|Stats:|left|\nadd_smalltext|Crypto Sold (24hrs): " + setGems(Crypto_Update.crypto_sold) + "    Crypto Bought (24hrs): " + setGems(Crypto_Update.crypto_bought) + "    Crpyto Tax 0.1%|left|\nend_dialog|crypto_exchange|Cancel|Sell|\nadd_quick_exit|");
							p2.CreatePacket(peer);
						}
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|crypto_exchange") != string::npos) {
						vector<string> t_ = explode("|", cch), color;
						if (t_.size() == 8) {
							string crypto_name = explode("\n", t_[3])[0].c_str(), method = explode("\n", t_[5])[0].c_str();
							int count = atoi(explode("\n", t_[7])[0].c_str());
							gamepacket_t p, p2;
							p.Insert("OnConsoleMessage");
							p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID);
							if (count <= 0 or count > 10000) p.Insert("The amount is too big or too low!"), p2.Insert("The amount is too big or too low!");
							else {
								if (crypto_exchange(peer, crypto_name, count, (method == "buy" ? true : false))) {
									p.Insert("You have " + a + (method == "buy" ? "purchased" : "sold") + " " + setGems(count) + "x `2" + crypto_name + "``!"), p2.Insert("You have " + a + (method == "buy" ? "purchased" : "sold") + " " + setGems(count) + "x `2" + crypto_name + "``!");
								}
								else {
									p.Insert("You don't have enough "+a + (method == "buy" ? "gems" : "crypto") + "!"), p2.Insert("You don't have enough " + a+(method == "buy" ? "gems" : "crypto") + "!");
								}
							}
							p.CreatePacket(peer);
							p2.Insert(0), p2.Insert(0), p2.CreatePacket(peer);
						}
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|wfcalendar_dailyrewards\nbuttonClicked|calendarSystem_") != string::npos) {
							uint8_t id = atoi(cch.substr(86, cch.length() - 86).c_str());
							string epic = "", rare = "", uncommon = "", common = "";
							vector<double> ids = get_winterfest_calendar(id);
							vector <int> added_prizes;
							for (int i = 0; i < ids.size(); i++) {
								double rand_item = ids[i];
								int rarity_ = 0, count_ = 1, rand_item2 = (int)rand_item;
								for (int i_ = 0; i_ < ids.size(); i_++) if (rand_item == ids[i_]) rarity_++;
								if (to_string(rand_item).find(".") != string::npos) {
									string asd_ = explode(".", to_string(rand_item))[1];
									string s(1, asd_[0]);
									int c_ = atoi(s.c_str());
									if (c_ != 0) {
										if (asd_.size() == 2) {
											c_ /= 10;
										}
										count_ = c_;
									}
								}
								if (find(added_prizes.begin(), added_prizes.end(), rand_item2) == added_prizes.end()) {
									added_prizes.push_back(rand_item2);
									if (rarity_ == 1) epic += "\nadd_label_with_icon|small|`w" + items[rand_item2].ori_name + "" + (count_ > 1 ? " (x" + to_string(count_) + ")" : "") + "``|left|" + to_string(rand_item2) + "|";
									else if (rarity_ == 2) rare += "\nadd_label_with_icon|small|`w" + items[rand_item2].ori_name + "" + (count_ > 1 ? " (x" + to_string(count_) + ")" : "") + "``|left|" + to_string(rand_item2) + "|";
									else if (rarity_ == 3) uncommon += "\nadd_label_with_icon|small|`w" + items[rand_item2].ori_name + "" + (count_ > 1 ? " (x" + to_string(count_) + ")" : "") + "``|left|" + to_string(rand_item2) + "|";
									else if (rarity_ == 4) common += "\nadd_label_with_icon|small|`w" + items[rand_item2].ori_name + "" + (count_ > 1 ? " (x" + to_string(count_) + ")" : "") + "``|left|" + to_string(rand_item2) + "|";
								}
							}
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wDay "+to_string(id) + " Rewards``|left|12986|\nadd_spacer|small|\nadd_textbox|You have a chance of obtaining the following items:|left|\nadd_spacer|small|\nadd_textbox|`5Epic``|left|\n" + epic + "\nadd_spacer|small|\nadd_textbox|`1Rare``|left|\n" + rare + "\nadd_spacer|small|\nadd_textbox|`6Uncommon``|left|\n" + uncommon + "\nadd_spacer|small|\nadd_textbox|Common|left|\n" + common + "\nadd_spacer|small|\nadd_button|goto_maindialog|Thanks for the info!|0|0|\nend_dialog|wfcalendar_reward_list_dialog|||\nadd_quick_exit|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|handleBeachPartyShopPopup\nbuttonClicked|beachparty_store_item_open_purchase_") != string::npos) {
							uint8_t id = atoi(cch.substr(109, cch.length() - 109).c_str());
							if (id >= 0 && id <= 2) {
								int count = 60, itemid = 13598;
								if (id == 1) count = 300, itemid = 12262;
								else if (id == 2) count = 2000, itemid = 12264;
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								if (pInfo(peer)->pearl < count) p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wPearls Shop``|left|12260|\nadd_spacer|small|\nadd_textbox|" + items[itemid].ori_name + " costs " + to_string(count) + " pearls. You only have " + to_string(pInfo(peer)->pearl) + " so you can't afford it. Sorry!|left|\nadd_spacer|small|\nadd_button|back|Back|noflags|0|0|\nend_dialog|handleBeachPartyShopVerification||\nadd_quick_exit|");
								else {
									int give = 1;
									if (modify_inventory(peer, itemid, give) == 0) {
										add_pearl(peer, count * -1);
										p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wPearls Shop``|left|12260|\nadd_spacer|small|\nadd_textbox|You purchased " + items[itemid].ori_name + " which costed you " + to_string(count) + ".|left|\nadd_spacer|small|\nadd_button|back|Back|noflags|0|0|\nend_dialog|handleBeachPartyShopVerification||\nadd_quick_exit|");
									}
									else p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wPearls Shop``|left|12260|\nadd_spacer|small|\nadd_textbox|You have full inventory space!|left|\nadd_spacer|small|\nadd_button|back|Back|noflags|0|0|\nend_dialog|handleBeachPartyShopVerification||\nadd_quick_exit|");
								}
								p.CreatePacket(peer);
							}
							break;
							}
							
						else if (cch.find("action|dialog_return\ndialog_name|handleRubblePartyShopPopup\nbuttonClicked|beachparty_store_item_open_purchase_") != string::npos) {
							uint8_t id = atoi(cch.substr(110, cch.length() - 110).c_str());
							if (id >= 0 && id <= 1) {
								int count = 500, itemid = 0;
								if (id == 0) count = 500, itemid = 11402;
								else if (id == 1) count = 2500, itemid = 11404;
								else break;
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								if (pInfo(peer)->rubble < count) p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wRubbles Shop``|left|10992|\nadd_spacer|small|\nadd_textbox|" + items[itemid].ori_name + " costs " + to_string(count) + " rubbles. You only have " + to_string(pInfo(peer)->rubble) + " so you can't afford it. Sorry!|left|\nadd_spacer|small|\nadd_button|back|Back|noflags|0|0|\nend_dialog|handleRubblePartyShopVerification||\nadd_quick_exit|");
								else {
									int give = 1;
									if (modify_inventory(peer, itemid, give) == 0) {
										pInfo(peer)->rubble -= count;
										p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wRubbles Shop``|left|10992|\nadd_spacer|small|\nadd_textbox|You purchased " + items[itemid].ori_name + " which costed you " + to_string(count) + ".|left|\nadd_spacer|small|\nadd_button|back|Back|noflags|0|0|\nend_dialog|handleRubblePartyShopVerification||\nadd_quick_exit|");
									}
									else p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wRubbles Shop``|left|10992|\nadd_spacer|small|\nadd_textbox|You have full inventory space!|left|\nadd_spacer|small|\nadd_button|back|Back|noflags|0|0|\nend_dialog|handleRubblePartyShopVerification||\nadd_quick_exit|");
								}
								p.CreatePacket(peer);
							}
							break;
								}
						else if (cch == "action|claimprogressbar\n") {
								gamepacket_t p(500);
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wAbout Summerfest's Event``|left|10004|\nadd_spacer|small|\nadd_textbox|During Summerfest week, breaking Summer Surprises will give you bonus rewards. Every 20 Summer Surprises will give 1 Super Summer Surprise and at each milestone, you will receive guaranteed rewards.|left|\nadd_spacer|small|\nadd_textbox|Progress: " + to_string(pInfo(peer)->summer_surprise) + "/20|left|"+(pInfo(peer)->summer_surprise >= 20 ? add_small_font_summer("111") : "") + "\nadd_spacer|small|\nadd_label_with_icon|small|Super Summer Surprise|left|10004|\nadd_custom_margin|x:60;y:0|\nadd_smalltext|Chance of getting a Phoenix item and the only place to get a Neptune's Armor!|left|\nadd_custom_margin|x:-60;y:0|\nadd_spacer|small|\nadd_textbox|Milestone Progress: " + to_string(pInfo(peer)->summer_total) + "|left|\nadd_spacer|small|\nadd_label_with_icon|small|Open `21`` Summer Surprises|left|2946|state:" + (pInfo(peer)->summer_total >= 1 ? "enabled" : "disabled") + ";|\nadd_custom_margin|x:60;y:0|\nadd_custom_margin|x:0;y:5|\nadd_label_with_icon|small|25 Fireworks|left|834|state:" + (pInfo(peer)->summer_total >= 1 ? "enabled" : "disabled") + ";|" + (pInfo(peer)->summer_total >= 1 && find(pInfo(peer)->summer_milestone.begin(), pInfo(peer)->summer_milestone.end(), 1) == pInfo(peer)->summer_milestone.end() ? add_small_font_summer("1") : "") + "\nadd_custom_margin|x:-60;y:0|\nadd_spacer|small|\nadd_label_with_icon|small|Open `210`` Summer Surprises|left|2946|state:" + (pInfo(peer)->summer_total >= 10 ? "enabled" : "disabled") + ";|\nadd_custom_margin|x:60;y:0|\nadd_custom_margin|x:0;y:5|\nadd_label_with_icon|small|150 Fireworks|left|834|state:" + (pInfo(peer)->summer_total >= 10 ? "enabled" : "disabled") + ";|" + (pInfo(peer)->summer_total >= 10 && find(pInfo(peer)->summer_milestone.begin(), pInfo(peer)->summer_milestone.end(), 10) == pInfo(peer)->summer_milestone.end() ? add_small_font_summer("10") : "") + "\nadd_custom_margin|x:-60;y:0|\nadd_spacer|small|\nadd_label_with_icon|small|Open `230`` Summer Surprises|left|2946|state:" + (pInfo(peer)->summer_total >= 30 ? "enabled" : "disabled") + ";|\nadd_custom_margin|x:60;y:0|\nadd_custom_margin|x:0;y:5|\nadd_label_with_icon|small|1 Summer Surprise|left|836|state:" + (pInfo(peer)->summer_total >= 30 ? "enabled" : "disabled") + ";|" + (pInfo(peer)->summer_total >= 30 && find(pInfo(peer)->summer_milestone.begin(), pInfo(peer)->summer_milestone.end(), 30) == pInfo(peer)->summer_milestone.end() ? add_small_font_summer("30") : "") + "\nadd_custom_margin|x:-60;y:0|\nadd_spacer|small|\nadd_label_with_icon|small|Open `275`` Summer Surprises|left|2946|state:" + (pInfo(peer)->summer_total >= 75 ? "enabled" : "disabled") + ";|\nadd_custom_margin|x:60;y:0|\nadd_custom_margin|x:0;y:5|\nadd_label_with_icon|small|2 Summer Surprise|left|836|state:" + (pInfo(peer)->summer_total >= 75 ? "enabled" : "disabled") + ";|" + (pInfo(peer)->summer_total >= 75 && find(pInfo(peer)->summer_milestone.begin(), pInfo(peer)->summer_milestone.end(), 75) == pInfo(peer)->summer_milestone.end() ? add_small_font_summer("75") : "") + "\nadd_custom_margin|x:-60;y:0|\nadd_spacer|small|\nadd_label_with_icon|small|Open `2150`` Summer Surprises|left|2946|state:" + (pInfo(peer)->summer_total >= 150 ? "enabled" : "disabled") + ";|\nadd_custom_margin|x:60;y:0|\nadd_custom_margin|x:0;y:5|\nadd_label_with_icon|small|1 Summer Artifact Chest|left|11038|state:" + (pInfo(peer)->summer_total >= 150 ? "enabled" : "disabled") + ";|" + (pInfo(peer)->summer_total >= 150 && find(pInfo(peer)->summer_milestone.begin(), pInfo(peer)->summer_milestone.end(), 150) == pInfo(peer)->summer_milestone.end() ? add_small_font_summer("150") : "") + "\nadd_custom_margin|x:-60;y:0|\nadd_spacer|small|\nadd_label_with_icon|small|Open `2300`` Summer Surprises|left|2946|state:" + (pInfo(peer)->summer_total >= 300 ? "enabled" : "disabled") + ";|\nadd_custom_margin|x:60;y:0|\nadd_custom_margin|x:0;y:5|\nadd_label_with_icon|small|1 Super Fireworks|left|1680|state:" + (pInfo(peer)->summer_total >= 300 ? "enabled" : "disabled") + ";|" + (pInfo(peer)->summer_total >= 300 && find(pInfo(peer)->summer_milestone.begin(), pInfo(peer)->summer_milestone.end(), 300) == pInfo(peer)->summer_milestone.end() ? add_small_font_summer("300") : "") + "\nadd_custom_margin|x:-60;y:0|\nadd_spacer|small|\nadd_label_with_icon|small|Open `2400`` Summer Surprises|left|2946|state:" + (pInfo(peer)->summer_total >= 400 ? "enabled" : "disabled") + ";|\nadd_custom_margin|x:60;y:0|\nadd_custom_margin|x:0;y:5|\nadd_label_with_icon|small|1 Wind Surfer|left|13604|state:" + (pInfo(peer)->summer_total >= 400 ? "enabled" : "disabled") + ";|" + (pInfo(peer)->summer_total >= 400 && find(pInfo(peer)->summer_milestone.begin(), pInfo(peer)->summer_milestone.end(), 400) == pInfo(peer)->summer_milestone.end() ? add_small_font_summer("400") : "") + "\nadd_custom_margin|x:-60;y:0|\nadd_spacer|small|\nadd_label_with_icon|small|Open `2500`` Summer Surprises|left|2946|state:" + (pInfo(peer)->summer_total >= 500 ? "enabled" : "disabled") + ";|\nadd_custom_margin|x:60;y:0|\nadd_custom_margin|x:0;y:5|\nadd_label_with_icon|small|1 Super Fireworks|left|1680|state:" + (pInfo(peer)->summer_total >= 500 ? "enabled" : "disabled") + ";|" + (pInfo(peer)->summer_total >= 500 && find(pInfo(peer)->summer_milestone.begin(), pInfo(peer)->summer_milestone.end(), 500) == pInfo(peer)->summer_milestone.end() ? add_small_font_summer("500") : "") + "\nadd_custom_margin|x:-60;y:0|\nadd_spacer|small|\nadd_label_with_icon|small|Open `2600`` Summer Surprises|left|2946|state:" + (pInfo(peer)->summer_total >= 600 ? "enabled" : "disabled") + ";|\nadd_custom_margin|x:60;y:0|\nadd_custom_margin|x:0;y:5|\nadd_label_with_icon|small|1 Super Fireworks|left|1680|state:" + (pInfo(peer)->summer_total >= 600 ? "enabled" : "disabled") + ";|" + (pInfo(peer)->summer_total >= 600 && find(pInfo(peer)->summer_milestone.begin(), pInfo(peer)->summer_milestone.end(), 600) == pInfo(peer)->summer_milestone.end() ? add_small_font_summer("600") : "") + "\nadd_custom_margin|x:-60;y:0|\nadd_spacer|small|\nadd_label_with_icon|small|Open `2700`` Summer Surprises|left|2946|state:" + (pInfo(peer)->summer_total >= 700 ? "enabled" : "disabled") + ";|\nadd_custom_margin|x:60;y:0|\nadd_custom_margin|x:0;y:5|\nadd_label_with_icon|small|1 Sun Headband|left|13616|state:" + (pInfo(peer)->summer_total >= 700 ? "enabled" : "disabled") + ";|" + (pInfo(peer)->summer_total >= 700 && find(pInfo(peer)->summer_milestone.begin(), pInfo(peer)->summer_milestone.end(), 700) == pInfo(peer)->summer_milestone.end() ? add_small_font_summer("700") : "") + "\nadd_custom_margin|x:-60;y:0|\nadd_spacer|small|\nadd_label_with_icon|small|Open `2800`` Summer Surprises|left|2946|state:" + (pInfo(peer)->summer_total >= 800 ? "enabled" : "disabled") + ";|\nadd_custom_margin|x:60;y:0|\nadd_custom_margin|x:0;y:5|\nadd_label_with_icon|small|1 Summer Face Paint|left|13614|state:" + (pInfo(peer)->summer_total >= 800 ? "enabled" : "disabled") + ";|" + (pInfo(peer)->summer_total >= 800 && find(pInfo(peer)->summer_milestone.begin(), pInfo(peer)->summer_milestone.end(), 800) == pInfo(peer)->summer_milestone.end() ? add_small_font_summer("800") : "") + "\nadd_custom_margin|x:-60;y:0|\nadd_spacer|small|\nadd_label_with_icon|small|Open `2900`` Summer Surprises|left|2946|state:" + (pInfo(peer)->summer_total >= 900 ? "enabled" : "disabled") + ";|\nadd_custom_margin|x:60;y:0|\nadd_custom_margin|x:0;y:5|\nadd_label_with_icon|small|1 Leaf Wings|left|13608|state:" + (pInfo(peer)->summer_total >= 900 ? "enabled" : "disabled") + ";|" + (pInfo(peer)->summer_total >= 900 && find(pInfo(peer)->summer_milestone.begin(), pInfo(peer)->summer_milestone.end(), 900) == pInfo(peer)->summer_milestone.end() ? add_small_font_summer("900") : "") + "\nadd_custom_margin|x:-60;y:0|\nadd_spacer|small|\nadd_label_with_icon|small|Open `21000`` Summer Surprises|left|2946|state:" + (pInfo(peer)->summer_total >= 1000 ? "enabled" : "disabled") + ";|\nadd_custom_margin|x:60;y:0|\nadd_custom_margin|x:0;y:5|\nadd_label_with_icon|small|1 Ultraviolet Sword|left|13606|state:" + (pInfo(peer)->summer_total >= 1000 ? "enabled" : "disabled") + ";|" + (pInfo(peer)->summer_total >= 1000 && find(pInfo(peer)->summer_milestone.begin(), pInfo(peer)->summer_milestone.end(), 1000) == pInfo(peer)->summer_milestone.end() ? add_small_font_summer("1000") : "") + "\nadd_custom_margin|x:-60;y:0|\nadd_spacer|small|\nadd_quick_exit|\nend_dialog|summerfest_quest||OK|");
								//p.CreatePacket(peer);
							
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|summerfest_quest\nbuttonClicked|") != string::npos) {
							int id = atoi(cch.substr(64, cch.length() - 64).c_str());
							if ((id == 111 && pInfo(peer)->summer_surprise >= 20) or (id != 111 && pInfo(peer)->summer_total >= id && find(pInfo(peer)->summer_milestone.begin(), pInfo(peer)->summer_milestone.end(), id) == pInfo(peer)->summer_milestone.end())) {
								int give = 1, give_item = 836;

								if (id == 1) give = 25, give_item = 834;
								else if (id == 10) give = 150, give_item = 834;
								else if (id == 30) give = 1, give_item = 836;
								else if (id == 75) give = 2, give_item = 836;
								else if (id == 150) give = 1, give_item = 11038;
								else if (id == 300) give = 1, give_item = 1680;
								else if (id == 400) give = 1, give_item = 13604;
								else if (id == 500) give = 1, give_item = 1680;
								else if (id == 600) give = 1, give_item = 1680;
								else if (id == 700) give = 1, give_item = 13616;
								else if (id == 800) give = 1, give_item = 13614;
								else if (id == 900) give = 1, give_item = 13608;
								else if (id == 1000) give = 1, give_item = 13606;
								else if (id == 111) give = 1, give_item = 10004;
								int got = give;
								gamepacket_t p;
								p.Insert("OnTalkBubble");
								p.Insert(pInfo(peer)->netID);
								if (modify_inventory(peer, give_item, give) == 0) {
									if (id != 111) pInfo(peer)->summer_milestone.push_back(id);
									else {
										pInfo(peer)->summer_surprise -= 20;
										gamepacket_t p;
										p.Insert("OnProgressUIUpdateValue"), p.Insert(pInfo(peer)->summer_surprise), p.Insert(0);
										p.CreatePacket(peer);
									}
									packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
									p.Insert("Congratulations! You have claimed your reward `2"+to_string(got) + " " + items[give_item].ori_name + "``!");
									PlayerMoving data_{};
									data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
									BYTE* raw = packPlayerMoving(&data_);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world) continue;
										send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									}
									delete[] raw;
								}
								else p.Insert("You have full inventory space!");
								p.Insert(0), p.Insert(0), p.CreatePacket(peer);
							}
					}

								/*
						else if (cch.find("action|dialog_return\ndialog_name|handleHalloweenShopPopup\nbuttonClicked|halloween_store_item_open_purchase_") != string::npos) {
							uint8_t id = atoi(cch.substr(107, cch.length() - 107).c_str());
							if (id == 0 or id == 1) {
								int count = 20, itemid = 12830;
								if (id == 1) count = 100, itemid = 12832;a
								int candy_have = 0;
								modify_inventory(peer, 12766, candy_have);
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								if (candy_have < count) p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wShady Salesman``|left|12826|\nadd_spacer|small|\nadd_textbox|" + items[itemid].ori_name + " costs `5" + to_string(count) + " candies``. You have `4" + to_string(candy_have) + " candies`` so you can't afford it. Sorry!|left|\nadd_spacer|small|\nadd_button|back|Back|noflags|0|0|\nend_dialog|handleHalloweenShopVerification||\nadd_quick_exit|");
								else {
									int give = 1;
									if (modify_inventory(peer, itemid, give) == 0) {
										int remove_ = count * -1;
										modify_inventory(peer, 12766, remove_);
										p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wShady Salesman``|left|12826|\nadd_spacer|small|\nadd_textbox|You purchased " + items[itemid].ori_name + " which costed you " + to_string(count) + ".|left|\nadd_spacer|small|\nadd_button|back|Back|noflags|0|0|\nend_dialog|handleHalloweenShopVerification||\nadd_quick_exit|");
									}
									else p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wShady Salesman``|left|12826|\nadd_spacer|small|\nadd_textbox|You have full inventory space!|left|\nadd_spacer|small|\nadd_button|back|Back|noflags|0|0|\nend_dialog|handleHalloweenShopVerification||\nadd_quick_exit|");
								}
								p.CreatePacket(peer);
							}
							break;
						}
						else if (cch == "action|claimprogressbar\n" || cch == "action|dialog_return\ndialog_name|openggjui\nbuttonClicked|openggjui\n\n") {
						wildlife_rewards(peer);
						break;
						}
						else if (cch == "action|dialog_return\ndialog_name|openggjui\nbuttonClicked|open_store\n\n") {
						shop_tab(peer, "tab1");
						break;
						}
						else if (cch == "action|dialog_return\ndialog_name|greengamejam\nbuttonClicked|OPEN_GREEN_GAMEJAM_INFO_POPUP\n\n") {
						wildlife_info(peer, false);
						break;
						}
						else if (cch == "action|OPEN_GREEN_GAMEJAM_INFO_POPUP\n") {
						wildlife_info(peer, true);
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|greengamejam\nbuttonClicked|claimreward") != string::npos) {
						if (pInfo(peer)->planted >= 20) {
							int give = 1;
							gamepacket_t p;
							p.Insert("OnTalkBubble");
							p.Insert(pInfo(peer)->netID);
							if (modify_inventory(peer, 13319, give) == 0) {
								packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
								p.Insert("You have claimed a `2Golden Eco Seed``!");
								{
									gamepacket_t p;
									p.Insert("OnConsoleMessage"), p.Insert("You have claimed a `2Golden Eco Seed``!"), p.CreatePacket(peer);
								}
								OnSetEvent(peer, -20);
							}
							else p.Insert("You have full inventory space!");
							p.Insert(0), p.Insert(0), p.CreatePacket(peer);
						}
						break;
						}*/
					/*
						else if (cch == "action|claimprogressbar\n") {
						if (pInfo(peer)->summer_surprise >= 20) {
							int give = 1;
							gamepacket_t p;
							p.Insert("OnTalkBubble");
							p.Insert(pInfo(peer)->netID);
							if (modify_inventory(peer, 10004, give) == 0) {
								packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
								p.Insert("Congratulations! You have claimed your reward `2Super Summer Surprise``!");
								gamepacket_t p;
								p.Insert("OnProgressUIUpdateValue"), p.Insert(pInfo(peer)->summer_surprise = 0), p.Insert(0), p.CreatePacket(peer);
								PlayerMoving data_{};
								data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
								BYTE* raw = packPlayerMoving(&data_);
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world) continue;
									send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
								}
								delete[] raw;
							}
							else p.Insert("You have full inventory space!");
							p.Insert(0), p.Insert(0), p.CreatePacket(peer);
						}
						else {
							gamepacket_t p(500);
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wAbout Summerfest's Event``|left|10004|\nadd_spacer|small|\nadd_textbox|During Summerfest week breaking 20 Summer Surprises will give you a Super Summer Surprise!|left|\nadd_spacer|small|\nadd_textbox|Current Progress: " + to_string(pInfo(peer)->summer_surprise) + "/20|left|\nadd_spacer|small|\nadd_textbox|Reward:|left|\nadd_label_with_icon|small| Super Summer Surprise|left|10004|\nadd_smalltext|Chance of getting a Phoenix item and the only place to get a Neptune's Crown!|left|\nend_dialog|summerss||OK|\n");
							p.CreatePacket(peer);
						}
							break;
						}
						else if (cch == "action|claimprogressbar\n") {
							gamepacket_t p(500);
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wBonus Reward - Dark King's Offering``|left|10328|\nadd_spacer|small|\nadd_textbox|During the Halloween season, sacrificing 15 of the Dark King's Offering at the Maw of Growganoth will please him. As a token of thanks, he will grant you an extra reward.|left|\nadd_spacer|small|\nadd_textbox|Current Sacrifices: " + to_string(pInfo(peer)->halloween_dark_king) + "/15|left|" + (pInfo(peer)->halloween_dark_king >= 15 ? "\nadd_button|claim|Claim reward|0|0|" : "") + "\nadd_spacer|small|\nadd_quick_exit|\nend_dialog|halloween_quest||OK|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|halloween_quest\nbuttonClicked|claim") != string::npos) {
							if (pInfo(peer)->halloween_dark_king >= 15) {
								int give = 1;
								gamepacket_t p;
								p.Insert("OnTalkBubble");
								p.Insert(pInfo(peer)->netID);
								if (modify_inventory(peer, 10386, give) == 0) {
									pInfo(peer)->halloween_dark_king -= 15;
									packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
									p.Insert("Congratulations! You have claimed your reward `2Halloween Surprise``!");
									gamepacket_t p;
									p.Insert("OnProgressUIUpdateValue"), p.Insert((pInfo(peer)->halloween_dark_king > 15 ? 15 : pInfo(peer)->halloween_dark_king)), p.Insert(0), p.CreatePacket(peer);
									PlayerMoving data_{};
									data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
									BYTE* raw = packPlayerMoving(&data_);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world) continue;
										send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									}
									delete[] raw;
								}
								else p.Insert("You have full inventory space!");
								p.Insert(0), p.Insert(0), p.CreatePacket(peer);
							}
							break;
						}*//*
												else if (cch == "action|salesman_popup\n") {
						string world_name = "TRADE";
						join_world(peer, world_name);
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|egseeventinfo\nbuttonClicked|egse_claim_reward") != string::npos) {
						if (pInfo(peer)->egg_carton >= 20) {
							vector<int> shard{ 10766, 10768,10766, 10768,10766, 10768,10766, 10768,10766, 10768,10766, 10768,10766, 10768,10766, 10768,10766, 10768,10766, 10764 };
							vector<int> egg{ 10762, 2554, 13372, 8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550, 8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550, 8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550, 8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550,8064, 8066, 8068, 2552, 9460, 2548, 2546, 2544, 2550 };
							if (pInfo(peer)->egg_chance == 1) {
								egg.insert(egg.end(), { 10762, 2554, 13372 });
							}
							else if (pInfo(peer)->egg_chance == 2) {
								egg.insert(egg.end(), { 10762, 2554, 13372, 10762, 2554, 13372 });
							}
							else if (pInfo(peer)->egg_chance == 3) {
								egg.insert(egg.end(), { 10762, 2554, 13372, 10762, 2554, 13372, 10762, 2554, 13372, 10762, 2554, 13372 });
							}
							else if (pInfo(peer)->egg_chance == 4) {
								egg.insert(egg.end(), { 10762, 2554, 13372, 10762, 2554, 13372, 10762, 2554, 13372, 10762, 2554, 13372, 10762, 2554, 13372 });
							}
							else {
								egg.insert(egg.end(), { 10762, 2554, 13372, 10762, 2554, 13372, 10762, 2554, 13372, 10762, 2554, 13372, 10762, 2554, 13372, 10762, 2554, 13372 });
							}
							bool toobig = false;
							for (int i = 0; i < shard.size(); i++) {
								int have = 0;
								modify_inventory(peer, shard[i], have);
								if (have >= 199) toobig = true;
							}
							for (int i = 0; i < egg.size(); i++) {
								int have = 0;
								modify_inventory(peer, egg[i], have);
								if (have >= 199) toobig = true;
							}
							if (toobig) break;
							vector<pair<int, int>> receivingitems;
							int free = get_free_slots(pInfo(peer)), slot = 2, getcount = 1, inventoryfull = 0;
							if (free >= slot) {
								for (int i = 0; i < slot; i++) {
									int itemid = i == 0 ? shard[rand() % shard.size()] : egg[rand() % egg.size()];
									receivingitems.push_back(make_pair(itemid, getcount = 1));
								}
								for (int i = 0; i < slot; i++) {
									int itemcount = 0;
									modify_inventory(peer, receivingitems[i].first, itemcount);
									if (itemcount + receivingitems[i].second > 200) inventoryfull = 1;
								}
								if (inventoryfull == 0) {
									pInfo(peer)->egg_chance++;
									if (pInfo(peer)->egg_chance > 5) pInfo(peer)->egg_chance = 5;
									string received = "";
									for (int i = 0; i < slot; i++) {
										if (receivingitems[i].first == 13372 || receivingitems[i].first == 10762 || receivingitems[i].first == 2554) {
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("Easter Gold Streak rewards "+ get_player_nick(peer) + " with a `5Rare " + items[receivingitems[i].first].ori_name + ".");
											for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
												if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->temp_radio) continue;
												p.CreatePacket(currentPeer);
											}
										}
										int receiveds = receivingitems[i].second;
										modify_inventory(peer, receivingitems[i].first, receivingitems[i].second);
										if (i == 0)received += "`2" + to_string(receiveds) + " " + items[receivingitems[i].first].ori_name + "`` and ";
										else received += "`2" + to_string(receiveds) + " " + items[receivingitems[i].first].ori_name + "``";
									}
									OnSetEvent(peer, -20);
									talk_bubble(peer, pInfo(peer)->netID, "You received " + received + " from a Golden Egg Carton.", 0, 1);
									console_msg(peer, "You received " + received + " from a Golden Egg Carton.");
								}
							}
						}
						break;
						}*/
						else if (cch.find("action|dialog_return\ndialog_name|mooncake_altar_dialog\nbuttonClicked|slot_btn_") != string::npos) {
							int slot = atoi(cch.substr(78, cch.length() - 78).c_str());
							string inventory = "", clear_slot = "";
							for (int i_ = 0; i_ < pInfo(peer)->inv.size(); i_++) if (items[pInfo(peer)->inv[i_].first].mooncake) inventory += "\nadd_button_with_icon|item_btn_" + to_string(pInfo(peer)->inv[i_].first) + "||frame|" + to_string(pInfo(peer)->inv[i_].first) + "|" + to_string(pInfo(peer)->inv[i_].second) + "|\nadd_custom_margin|x:-40;y:0|\nadd_textbox|" + items[pInfo(peer)->inv[i_].first].name + "|left||\nadd_button_with_icon||END_LIST|noflags|0||\nadd_custom_margin|x:0;y:-80|";
							string name_ = pInfo(peer)->world;
							vector<World>::iterator pd = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (pd != worlds.end()) {
								World* world_ = &worlds[pd - worlds.begin()];
								world_->fresh_world = true;
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
								for (int i_ = 0; i_ < block_->donates.size(); i_++) {
									if (i_ == slot) clear_slot = "\nadd_button|clear_slot|Clear slot|0|0|";
								}
							}
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							if (clear_slot .empty() && inventory .empty())p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wNot Enough Mooncakes``|left|1432|\nadd_textbox|You don't have any Mooncakes!|left|\nadd_spacer|small|\nadd_button|goto_maindialog|OK|0|0|\nadd_spacer|small|\nend_dialog|altar_warning_dialog|||");
							else p.Insert("set_default_color|`w\nadd_label_with_icon|big|`wSelect a Mooncake``|left|12598|\nadd_spacer|small|\nadd_textbox|Select a Mooncake to place on the offering table.|left|" + (inventory.empty() ? "" : inventory) + "\nadd_spacer|small|" + clear_slot + "\nadd_button|goto_maindialog|Not right now|0|0|\nembed_data|slot|" + to_string(slot) + "\nend_dialog|mooncake_choose_dialog|||");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|mooncake_choose_dialog\n") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 4) break;
							string button = explode("\n", t_[5])[0].c_str();
							if (button == "goto_maindialog") offering_table(peer);
							else {
								int slot = atoi(explode("\n", t_[3])[0].c_str());
								if (button.find("item_btn_") != string::npos) {
									int item = atoi(button.substr(9, button.length() - 9).c_str()), have = 0;
									if (items[item].mooncake) {
										modify_inventory(peer, item, have);
										gamepacket_t p;
										p.Insert("OnDialogRequest");
										p.Insert("set_default_color|`o\nadd_label_with_icon|big|`w" + items[item].name + "``|left|" + to_string(item) + "|\nadd_textbox|`2How many do you want to offer?``|left|\nadd_text_input|count||" + to_string(have) + "|5|\nembed_data|itemID|" + to_string(item) + "\nembed_data|slot|" + to_string(slot) + "\nadd_spacer|small|\nadd_button|goto_choosedoalog|Cancel|0|0|\nadd_button|ok|OK|0|0|\nadd_spacer|small|\nend_dialog|mooncake_count_dialog|||");
										p.CreatePacket(peer);
									}
								}
								else if (button == "clear_slot") {
									string name_ = pInfo(peer)->world;
									vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
									if (p != worlds.end()) {
										World* world_ = &worlds[p - worlds.begin()];
										world_->fresh_world = true;
										WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
										for (int i_ = 0; i_ < block_->donates.size(); i_++) {
											if (i_ == slot) {
												if (modify_inventory(peer, block_->donates[i_].item, block_->donates[i_].count) == 0) {
													block_->donates.erase(block_->donates.begin() + i_);
												}
												else {
													console_msg(peer, "No inventory space.");
												}
											}
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|mooncake_count_dialog\n") != string::npos) {
						vector<string> t_ = explode("|", cch);
						if (t_.size() < 4) break;
						string button = explode("\n", t_[7])[0].c_str();
						if (button == "goto_choosedoalog") offering_table(peer);
						else if (button == "ok") {
							int item = atoi(explode("\n", t_[3])[0].c_str()), slot = atoi(explode("\n", t_[5])[0].c_str()), count = atoi(explode("\n", t_[8])[0].c_str()), have = 0;
							if (items[item].mooncake) {
								modify_inventory(peer, item, have);
								if (have >= count && count > 0)offering_table(peer, 0, "", item, count, slot);
							}
						}
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|guide_book\n") != string::npos) {
						vector<string> t_ = explode("|", cch);
						if (t_.size() < 3) break;
						string button = explode("\n", t_[3])[0].c_str();
						if (button == "news") news(peer);
						else if (button == "rules") SendCmd(peer, "/rules", false);
						else {
							pInfo(peer)->page_number = 26, pInfo(peer)->page_item = "", splicing_recipe(peer, (button == "splicing" ? splicing : button == "combining" ? combining : button == "combusting" ? combusting : crystals));
						}
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|guide_book_s\n") != string::npos || cch.find("action|dialog_return\ndialog_name|guide_book_c\n") != string::npos || cch.find("action|dialog_return\ndialog_name|guide_book_f\n") != string::npos || cch.find("action|dialog_return\ndialog_name|guide_book_r\n") != string::npos) {
						vector<string> t_ = explode("|", cch);
						if (t_.size() < 4) break;
						string button = explode("\n", t_[3])[0].c_str();
						if (button == "search") {
							pInfo(peer)->page_number = 26;
							pInfo(peer)->page_item = explode("\n", t_[4])[0].c_str();
						}
						else if (button == "next_pg") pInfo(peer)->page_number += 26;
						else if (button == "last_pg") {
							pInfo(peer)->page_number -= 26;
							if (pInfo(peer)->page_number < 0) pInfo(peer)->page_number = 26;
						}
						if (t_.size() > 4) {
							if (cch.find("action|dialog_return\ndialog_name|guide_book_s\n") != string::npos) splicing_recipe(peer, splicing);
							else if (cch.find("action|dialog_return\ndialog_name|guide_book_c\n") != string::npos) splicing_recipe(peer, combining);
							else if (cch.find("action|dialog_return\ndialog_name|guide_book_f\n") != string::npos) splicing_recipe(peer, combusting);
							else if (cch.find("action|dialog_return\ndialog_name|guide_book_r\n") != string::npos) splicing_recipe(peer, crystals);
						}
						else guide_book(peer);
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|2fa\nverificationcode|") != string::npos) {
							int code = atoi(cch.substr(54, cch.length() - 54).c_str());
							if (code >= 1000 && code < 9999) {
								if (pInfo(peer)->fa2 == 0) pInfo(peer)->fa2 = code, onsupermain(peer);
								else {
									if (pInfo(peer)->fa2 == code) {
										pInfo(peer)->bypass2 = true;
										onsupermain(peer);
										pInfo(peer)->fa_ip = pInfo(peer)->ip;
									}
									else {
										onsupermain(peer, "You entered wrong 2FA Code.");
									}
								}
							}
							else onsupermain(peer, "There was an error in your 2FA Code.");
							break;
						}
						/*
						else if (cch.find("action|dialog_return\ndialog_name|puzzle_captcha_submit\n") != string::npos) {
							try {
								vector<string> t_ = explode("|", cch);
								if (t_.size() < 5) break;
								int captcha_id = atoi(explode("\n", t_[5])[0].c_str());
								string button = explode("\n", t_[3])[0].c_str();
								replaceAll(button, "0.", "");
								int number = atoi(button.substr(0, 2).c_str()), answer = 0;
								if (captcha_id == 1 || captcha_id == 20) answer = 79;
								else if (captcha_id == 2) answer = 53;
								else if (captcha_id == 3) answer = 54;
								else if (captcha_id == 4 || captcha_id == 11) answer = 9;
								else if (captcha_id == 5) answer = 26;
								else if (captcha_id == 6) answer = 48;
								else if (captcha_id == 7) answer = 16;
								else if (captcha_id == 8) answer = 20;
								else if (captcha_id == 9) answer = 73;
								else if (captcha_id == 10 || captcha_id == 19) answer = 27;
								else if (captcha_id == 12) answer = 60;
								else if (captcha_id == 13) answer = 71;
								else if (captcha_id == 14) answer = 20;
								else if (captcha_id == 15) answer = 36;
								else if (captcha_id == 16) answer = 80;
								else if (captcha_id == 17) answer = 62;
								else if (captcha_id == 18) answer = 65;
								else failed_login(peer, "`4Wrong answer for the captcha!");
								if (abs(answer - number) > 2) failed_login(peer, "`4Wrong answer for the captcha!");
								else {
									if (pInfo(peer)->update_player == false) {
										pInfo(peer)->update_player = true;
										onsupermain(peer);
									}
								}
							}
							catch (out_of_range) {
								failed_login(peer, "`4Wrong answer for the captcha!");
								break;
							}
							break;
						}*/
						else if (cch == "action|dialog_return\ndialog_name|mooncake_reward_dialog\nbuttonClicked|take_reward\n\n") {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								world_->fresh_world = true;
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
								if (block_->shelf_1 != 0) {
									block_->shelf_1 = 0;
									int give = 1;
									if (modify_inventory(peer, block_->shelf_1, give) == 0) {
										block_->shelf_1 = 0;
									}
									else {
										console_msg(peer, "No inventory space.");
									}
								}
							}
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|mooncake_altar_dialog\nbuttonClicked|reward_list_btn\n\n") {
							offering_table(peer, 0, "reward");
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|mooncake_reward_list_dialog\nbuttonClicked|goto_maindialog\n\n" || cch == "action|dialog_return\ndialog_name|altar_warning_dialog\nbuttonClicked|goto_maindialog\n\n") {
							offering_table(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|mooncake_altar_dialog\nbuttonClicked|offer_btn\n\n") {
							offering_table(peer, 0, "offer");
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|mooncake_reward_dialog\nbuttonClicked|reroll\n\n") {
							offering_table(peer, 0, "reroll");
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|grow4goodtasks_dialog\nbuttonClicked|tab_tasks\n\n") {
							daily_quest(peer, true, "tab_tasks", 0);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|grow4goodtasks_dialog\nbuttonClicked|tab_rewards\n\n") {
							daily_quest(peer, true, "tab_rewards", 0);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|grow4goodtasks_dialog\nbuttonClicked|claimrewardsg4g\n\n") {
							if (pInfo(peer)->grow4good_points >= 400) {
								vector<uint16_t> list{ 10838, 10394, 10394,10394,10394, 5138, 5140, 5142, 7962, 9526 };
								bool inv_full = false;
								for (int i = 0; i < list.size(); i++) {
									int addup = (list[i] == 7962 || list[i] == 9526 ? 50 : 1), current = 0;
									modify_inventory(peer, list[i], current);
									if (addup + current >= 200) inv_full = true;
								}
								gamepacket_t p;
								p.Insert("OnTalkBubble");
								p.Insert(pInfo(peer)->netID);
								if (inv_full) p.Insert("You have full inventory space!");
								else {
									uint16_t item = list[rand() % list.size()];
									int give = 1;
									if (item == 7962 || item == 9526) give = 50;
									if (modify_inventory(peer, item, give) == 0) {
										pInfo(peer)->grow4good_points -= 400;
										packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										p.Insert("Congratulations! You have claimed your reward [`2" + items[item].name + "``]!");
										PlayerMoving data_{};
										data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
										BYTE* raw = packPlayerMoving(&data_);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw;
									}
									else p.Insert("You have full inventory space!");
								}
								p.Insert(0), p.Insert(0), p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|ticket_booth\ncount|") != string::npos) {
							int count = atoi(cch.substr(52, cch.length() - 52).c_str()), item = pInfo(peer)->lastchoosenitem, got = 0;
							if (count <= 0 || count > 200)break;
							modify_inventory(peer, item, got);
							if (count > got or got <= 0) break;
							int total_points_ticket = (item == 242 ? 3000 * count : items[item].rarity * count) + pInfo(peer)->carnival_credit;
							int give_points_for_ticket = total_points_ticket / 100;
							pInfo(peer)->carnival_credit = 0;
							if (total_points_ticket - (give_points_for_ticket * 100) > 0) pInfo(peer)->carnival_credit += total_points_ticket - (give_points_for_ticket * 100);

							int remove_t = give_points_for_ticket;
							if (give_points_for_ticket >= 1) {
								if (modify_inventory(peer, 1898, give_points_for_ticket) == 0) {
									modify_inventory(peer, item, count *= -1);
									gamepacket_t p;
									p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`9You got " + to_string(remove_t) + " Golden Tickets.``"), p.CreatePacket(peer);
									console_msg(peer, "`9You got " + to_string(remove_t) + " Golden Tickets.``");
								}
								else {
									console_msg(peer, "No inventory space.");
								}
							}
							else {
								pInfo(peer)->carnival_credit += total_points_ticket - (give_points_for_ticket * 100);
								console_msg(peer, "`9You have a credit of " + to_string(pInfo(peer)->carnival_credit) + " Rarity.``");
							}
						}
						else if (cch.find("action|dialog_return\ndialog_name|password_reply\npassword|") != string::npos) {
							string password = cch.substr(57, cch.length() - 58).c_str();
							string name_ = pInfo(peer)->world;
							vector<World>::iterator pa = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (pa != worlds.end()) {
								World* world_ = &worlds[pa - worlds.begin()];
								world_->fresh_world = true;
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
								if (block_->fg == 762 && block_->door_id != "") {
									gamepacket_t p;
									p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID);
									transform(password.begin(), password.end(), password.begin(), ::toupper);
									if (block_->door_id != password) p.Insert("`4Wrong password!``");
									else {
										p.Insert(a + "`2The door opens!" + (block_->door_destination .empty() ? " But nothing is behind it." : "") + "``");
										if (block_->door_destination != "") {
											gamepacket_t p3(0, pInfo(peer)->netID);
											p3.Insert("OnPlayPositioned"), p3.Insert("audio/door_open.wav"), p3.CreatePacket(peer);
											string door_target = block_->door_destination, door_id = "";
											World target_world = worlds[pa - worlds.begin()];
											int spawn_x = 0, spawn_y = 0;
											if (door_target.find(":") != string::npos) {
												vector<string> detales = explode(":", door_target);
												door_target = detales[0], door_id = detales[1];
											}
											bool found_ = true;
											if (not door_id.empty()) {
												vector<WorldBlock>::iterator p = find_if(target_world.blocks.begin(), target_world.blocks.end(), [&](const WorldBlock& a) { return (items[a.fg].path_marker || items[a.fg].blockType == BlockTypes::DOOR || items[a.fg].blockType == BlockTypes::PORTAL) && a.door_id == door_id; });
												if (p != target_world.blocks.end()) {
													int i_ = p - target_world.blocks.begin();
													spawn_x = i_ % 100, spawn_y = i_ / 100;
													found_ = false;
												}
											}
											else found_ = true;
											join_world(peer, target_world.name, spawn_x, spawn_y, 250, false, true, found_);

										}
									}
									p.Insert(0), p.Insert(0);
									p.CreatePacket(peer);
								}
							}
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|2646\nbuttonClicked|off\n\n") {
						string name_ = pInfo(peer)->world;
						vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
						if (p != worlds.end()) {
							World* world_ = &worlds[p - worlds.begin()];
							world_->fresh_world = true;
							WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
							for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world or block_->heart_monitor != pInfo(currentPeer)->tankIDName) continue;
								pInfo(currentPeer)->spotlight = false;
								form_state(pInfo(currentPeer));
								update_clothes(currentPeer, true);
								console_msg(peer, "Back to anonymity. (`$In the Spotlight`` mod removed)");
							}
							gamepacket_t p;
							p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("Lights out!"), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
							block_->heart_monitor = "";
						}
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|2646\nID|") != string::npos) {
							int netID = atoi(cch.substr(41, cch.length() - 41).c_str());
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								string new_spotlight = "";
								World* world_ = &worlds[p - worlds.begin()];
								world_->fresh_world = true;
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world) continue;
									if (block_->heart_monitor == pInfo(currentPeer)->tankIDName || pInfo(currentPeer)->netID == netID) {
										if (pInfo(currentPeer)->netID == netID) {
											new_spotlight = pInfo(currentPeer)->tankIDName, pInfo(currentPeer)->spotlight = true;
											gamepacket_t p;
											p.Insert("OnConsoleMessage"), p.Insert("All eyes are on you! (`$In the Spotlight`` mod added)"), p.CreatePacket(currentPeer);
										}
										else {
											gamepacket_t p;
											p.Insert("OnConsoleMessage"), p.Insert("Back to anonymity. (`$In the Spotlight`` mod removed)"), p.CreatePacket(currentPeer);
											pInfo(currentPeer)->spotlight = false;
										}
										if (new_spotlight != "") {
											vector<WorldBlock>::iterator p = find_if(world_->blocks.begin(), world_->blocks.end(), [&](const WorldBlock& a) { return a.heart_monitor == new_spotlight; });
											if (p != world_->blocks.end()) {
												WorldBlock* block__ = &world_->blocks[p - world_->blocks.begin()];
												block__->heart_monitor = "";
											}
										}
										gamepacket_t p;
										p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You shine the light on " + (new_spotlight == pInfo(peer)->tankIDName ? "yourself" : new_spotlight) + "!"), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
										form_state(pInfo(currentPeer));
										update_clothes(currentPeer, true);
									}
								}
								block_->heart_monitor = new_spotlight;
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|grinder\ncount|") != string::npos) {
							int count = atoi(cch.substr(47, cch.length() - 47).c_str()), item = pInfo(peer)->lastchoosenitem, got = inventory_contains(peer, item);
							if (items[item].grindable_count == 0 || got == 0 || count <= 0 || count * items[item].grindable_count > got) break;
							int remove = (count * items[item].grindable_count) * -1;
							if (modify_inventory(peer, item, remove) == 0) {
								gamepacket_t p, p2;
								p.Insert("OnConsoleMessage"), p.Insert("Ground up " + to_string(count * items[item].grindable_count) + " " + items[item].name + " into " + to_string(count) + " " + items[items[item].grindable_prize].name + "!"), p.CreatePacket(peer);
								p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("Ground up " + to_string(count * items[item].grindable_count) + " " + items[item].name + " into " + to_string(count) + " " + items[items[item].grindable_prize].name + "!"), p2.Insert(0), p2.Insert(1), p2.CreatePacket(peer);
								{
									string name_ = pInfo(peer)->world;
									vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
									if (p != worlds.end()) {
										World* world_ = &worlds[p - worlds.begin()];
										world_->fresh_world = true;
										PlayerMoving data_{};
										data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16, data_.packetType = 19, data_.plantingTree = 500, data_.punchX = items[item].grindable_prize, data_.punchY = pInfo(peer)->netID;
										int32_t to_netid = pInfo(peer)->netID;
										BYTE* raw = packPlayerMoving(&data_);
										raw[3] = 5;
										memcpy(raw + 8, &to_netid, 4);
										send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										delete[] raw;
										int c_ = count;
										if (modify_inventory(peer, items[item].grindable_prize, c_) != 0) {
											WorldDrop drop_block_{};
											drop_block_.id = items[item].grindable_prize, drop_block_.count = count, drop_block_.x = pInfo(peer)->x + rand() % 17, drop_block_.y = pInfo(peer)->y + rand() % 17;
											dropas_(world_, drop_block_);
										}
										{
											PlayerMoving data_{};
											data_.packetType = 17, data_.netID = 221, data_.YSpeed = 221, data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16, data_.XSpeed = item;
											BYTE* raw = packPlayerMoving(&data_);
											send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											delete[] raw;
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|s_claimreward") != string::npos) {
							int reward = atoi(cch.substr(72, cch.length() - 72).c_str()), lvl = 0, count = 1;
							vector<int> list{ 6900, 6982, 6212, 3172, 9068, 6912, 10836, 5142, 3130, 8284 };
							if (reward <= 0 || reward > list.size()) break;
							if (list[reward - 1] == 10836) count = 100;
							if (list[reward - 1] == 6212) count = 50;
							if (list[reward - 1] == 3172 || list[reward - 1] == 6912) count = 25;
							if (list[reward - 1] == 5142) count = 5;
							if (find(pInfo(peer)->surg_p.begin(), pInfo(peer)->surg_p.end(), lvl = reward * 5) == pInfo(peer)->surg_p.end()) {
								if (pInfo(peer)->s_lvl >= lvl) {
									if (modify_inventory(peer, list[reward - 1], count) == 0) {
										pInfo(peer)->surg_p.push_back(lvl);
										packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										{
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("Congratulations! You have received your Surgeon Reward!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
										PlayerMoving data_{};
										data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
										BYTE* raw = packPlayerMoving(&data_);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw;
										{
											PlayerMoving data_{};
											data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16, data_.packetType = 19, data_.plantingTree = 100, data_.punchX = list[reward - 1], data_.punchY = pInfo(peer)->netID;
											int32_t to_netid = pInfo(peer)->netID;
											BYTE* raw = packPlayerMoving(&data_);
											raw[3] = 5;
											memcpy(raw + 8, &to_netid, 4);
											send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											delete[] raw;
										}
										reward_show(peer, "surgery");
									}
									else {
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("You have full inventory space!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|zombie_purchase\nbuttonClicked|zomb_item_") != string::npos) {
							int item = pInfo(peer)->lockeitem;
							if (item <= 0 || item >= items.size() || items[item].zombieprice == 0) continue;
							int allwl = 0, wl = 0, dl = 0, price = items[item].zombieprice;
							modify_inventory(peer, 4450, wl);
							modify_inventory(peer, 4452, dl);
							allwl = wl + (dl * 100);
							if (allwl >= price) {
								int c_ = 1;
								if (modify_inventory(peer, item, c_) == 0) {
									if (wl >= price) modify_inventory(peer, 4450, price *= -1);
									else {
										modify_inventory(peer, 4450, wl *= -1);
										modify_inventory(peer, 4452, dl *= -1);
										int givedl = (allwl - price) / 100;
										int givewl = (allwl - price) - (givedl * 100);
										modify_inventory(peer, 4450, givewl);
										modify_inventory(peer, 4452, givedl);
									}
									if (item == 1486) if (pInfo(peer)->lwiz_step == 6) add_lwiz_points(peer, 1);
									if (item == 1486 && pInfo(peer)->C_QuestActive && pInfo(peer)->C_QuestKind == 11 && pInfo(peer)->C_QuestProgress < pInfo(peer)->C_ProgressNeeded) {
										pInfo(peer)->C_QuestProgress += 1;
										if (pInfo(peer)->C_QuestProgress >= pInfo(peer)->C_ProgressNeeded) {
											pInfo(peer)->C_QuestProgress = pInfo(peer)->C_ProgressNeeded;
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("`9Ring Quest task complete! Go tell the Ringmaster!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
									}
									PlayerMoving data_{};
									data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16, data_.packetType = 19, data_.plantingTree = 500, data_.punchX = item, data_.punchY = pInfo(peer)->netID;
									int32_t to_netid = pInfo(peer)->netID;
									BYTE* raw = packPlayerMoving(&data_);
									raw[3] = 5;
									memcpy(raw + 8, &to_netid, 4);
									send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									delete[] raw;
									gamepacket_t p;
									p.Insert("OnConsoleMessage"), p.Insert("`3You bought " + items[item].name + " for " + setGems(items[item].zombieprice) + " Zombie Brains."), p.CreatePacket(peer);
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage"), p.Insert("No inventory space."), p.CreatePacket(peer);
								}
							}
							else {
								gamepacket_t p;
								p.Insert("OnConsoleMessage"), p.Insert("`9You don't have enough Zombie Brains!``"), p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|zurgery_purchase\nbuttonClicked|zurg_item_") != string::npos) {
							int item = pInfo(peer)->lockeitem;
							if (item <= 0 || item >= items.size() || items[item].surgeryprice == 0) continue;
							int allwl = 0, wl = 0, dl = 0, price = items[item].surgeryprice;
							modify_inventory(peer, 4298, wl);
							modify_inventory(peer, 4300, dl);
							allwl = wl + (dl * 100);
							if (allwl >= price) {
								int c_ = 1;
								if (modify_inventory(peer, item, c_) == 0) {
									if (wl >= price) modify_inventory(peer, 4298, price *= -1);
									else {
										modify_inventory(peer, 4298, wl *= -1);
										modify_inventory(peer, 4300, dl *= -1);
										int givedl = (allwl - price) / 100;
										int givewl = (allwl - price) - (givedl * 100);
										modify_inventory(peer, 4298, givewl);
										modify_inventory(peer, 4300, givedl);
									}
									if (item == 1486) {
										if (pInfo(peer)->lwiz_step == 6) add_lwiz_points(peer, 1);
										if (item == 1486 && pInfo(peer)->C_QuestActive && pInfo(peer)->C_QuestKind == 11 && pInfo(peer)->C_QuestProgress < pInfo(peer)->C_ProgressNeeded) {
											pInfo(peer)->C_QuestProgress++;
											if (pInfo(peer)->C_QuestProgress >= pInfo(peer)->C_ProgressNeeded) {
												pInfo(peer)->C_QuestProgress = pInfo(peer)->C_ProgressNeeded;
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("`9Ring Quest task complete! Go tell the Ringmaster!");
												p.Insert(0), p.Insert(0);
												p.CreatePacket(peer);
											}
										}
									}
									PlayerMoving data_{};
									data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16, data_.packetType = 19, data_.plantingTree = 500, data_.punchX = item, data_.punchY = pInfo(peer)->netID;
									int32_t to_netid = pInfo(peer)->netID;
									BYTE* raw = packPlayerMoving(&data_);
									raw[3] = 5;
									memcpy(raw + 8, &to_netid, 4);
									send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									delete[] raw;
									gamepacket_t p;
									p.Insert("OnConsoleMessage"), p.Insert("`3You bought " + items[item].name + " for " + setGems(items[item].surgeryprice) + " Caduceus."), p.CreatePacket(peer);
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage"), p.Insert("No inventory space."), p.CreatePacket(peer);
								}
							}
							else {
								gamepacket_t p;
								p.Insert("OnConsoleMessage"), p.Insert("`9You don't have enough Caduceus!``"), p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|wolf_purchase\nbuttonClicked|wolf_item_") != string::npos) {
						int item = pInfo(peer)->lockeitem;
						if (item <= 0 || item >= items.size() || items[item].wolfprice == 0) continue;
						int allwl = 0, wl = 0, dl = 0, price = items[item].wolfprice;
						modify_inventory(peer, 4354, wl);
						modify_inventory(peer, 4356, dl);
						allwl = wl + (dl * 100);
						if (allwl >= price) {
							int c_ = 1;
							if (modify_inventory(peer, item, c_) == 0) {
								if (wl >= price) modify_inventory(peer, 4354, price *= -1);
								else {
									modify_inventory(peer, 4354, wl *= -1);
									modify_inventory(peer, 4356, dl *= -1);
									int givedl = (allwl - price) / 100;
									int givewl = (allwl - price) - (givedl * 100);
									modify_inventory(peer, 4354, givewl);
									modify_inventory(peer, 4356, givedl);
								}
								if (item == 1486) if (pInfo(peer)->lwiz_step == 6) add_lwiz_points(peer, 1);
								if (item == 1486 && pInfo(peer)->C_QuestActive && pInfo(peer)->C_QuestKind == 11 && pInfo(peer)->C_QuestProgress < pInfo(peer)->C_ProgressNeeded) {
									pInfo(peer)->C_QuestProgress += 1;
									if (pInfo(peer)->C_QuestProgress >= pInfo(peer)->C_ProgressNeeded) {
										pInfo(peer)->C_QuestProgress = pInfo(peer)->C_ProgressNeeded;
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("`9Ring Quest task complete! Go tell the Ringmaster!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
								}
								PlayerMoving data_{};
								data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16, data_.packetType = 19, data_.plantingTree = 500, data_.punchX = item, data_.punchY = pInfo(peer)->netID;
								int32_t to_netid = pInfo(peer)->netID;
								BYTE* raw = packPlayerMoving(&data_);
								raw[3] = 5;
								memcpy(raw + 8, &to_netid, 4);
								send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
								delete[] raw;
								gamepacket_t p;
								p.Insert("OnConsoleMessage"), p.Insert("`3You bought " + items[item].name + " for " + setGems(items[item].wolfprice) + " Wolf Ticket."), p.CreatePacket(peer);
							}
							else {
								gamepacket_t p;
								p.Insert("OnConsoleMessage"), p.Insert("No inventory space."), p.CreatePacket(peer);
							}
						}
						else {
							gamepacket_t p;
							p.Insert("OnConsoleMessage"), p.Insert("`9You don't have enough Wolf Ticket!``"), p.CreatePacket(peer);
						}
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|worldreport\nreport_reason|") != string::npos) {
							if (pInfo(peer)->tankIDName .empty()) break;
							string report = cch.substr(59, cch.length() - 60).c_str();
							add_modlogs(peer, pInfo(peer)->name_color + pInfo(peer)->tankIDName, "`4reports:`` " + report, " (/gor ?)");
							World_Stuff.report_world = pInfo(peer)->world;
							gamepacket_t p2;
							p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("Thank you for your report. Now leave this world so you don't get punished along with the scammers!"), p2.Insert(0), p2.Insert(0), p2.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|dialog_minokawa") != string::npos) {
						if (pInfo(peer)->back == 12640) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 5) break;
							bool minokawa_wings = atoi(explode("\n", t_[3])[0].c_str()), minokawa_pet = atoi(explode("\n", t_[4])[0].c_str());
							if (minokawa_wings && minokawa_pet) pInfo(peer)->minokawa_wings = 2;
							else if (minokawa_wings == false && minokawa_pet == false) {
								gamepacket_t p2;
								p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("You must choose one or two of the options!"), p2.Insert(0), p2.Insert(0), p2.CreatePacket(peer);
							}
							else {
								if (minokawa_wings) pInfo(peer)->minokawa_wings = 0;
								if (minokawa_pet) pInfo(peer)->minokawa_wings = 1;
							}
							update_clothes(peer);
						}
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|dialog_eqaura\nchange_item|") != string::npos) {
						if (pInfo(peer)->ances == 12634) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() >= 3) {
								int item = atoi(explode("\n", t_[3])[0].c_str());
								if (item < 0 || item > items.size()) break;
								if (items[item].musical_block) pInfo(peer)->eq_aura = item, update_clothes(peer);
								else {
									gamepacket_t p2;
									p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("Please choose a musical block!"), p2.Insert(0), p2.Insert(0), p2.CreatePacket(peer);
								}
							}
							else pInfo(peer)->eq_aura = 0;
						}
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|dialog_eqaura\nbuttonClicked") != string::npos) {
						if (pInfo(peer)->ances == 12634) pInfo(peer)->eq_aura = 0, update_clothes(peer);
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|billboard_edit\nbillboard_item|") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 4) break;
							int billboard_item = atoi(explode("\n", t_[3])[0].c_str());
							if (billboard_item > 0 && billboard_item < items.size()) {
								int got = inventory_contains(peer, billboard_item);
								if (got == 0) break;
								if (items[billboard_item].untradeable == 1 or billboard_item == 1424 or billboard_item == 5816 or items[billboard_item].blockType == BlockTypes::LOCK or items[billboard_item].blockType == BlockTypes::FISH) {
									gamepacket_t p, p2;
									p.Insert("OnConsoleMessage"), p.Insert("Item can not be untradeable."), p.CreatePacket(peer);
									p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("Item can not be untradeable."), p2.Insert(0), p2.Insert(1), p2.CreatePacket(peer);
								}
								else {
									pInfo(peer)->b_i = billboard_item;
									if (pInfo(peer)->b_p != 0 && pInfo(peer)->b_i != 0) {
										gamepacket_t p(0, pInfo(peer)->netID);
										p.Insert("OnBillboardChange"), p.Insert(pInfo(peer)->netID), p.Insert(pInfo(peer)->b_i), p.Insert(pInfo(peer)->b_bill), p.Insert(pInfo(peer)->b_p), p.Insert(pInfo(peer)->b_w);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != pInfo(peer)->world) continue;
											p.CreatePacket(currentPeer);
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return") != string::npos) {
							call_dialog(peer, cch);
							break;
						}
					}
					else if (cch.find("action|dialog_return") != string::npos) {
						call_dialog(peer, cch);
						break;
					}
					break;
				}
				case 3: // world/enter
				{
					cchs3 = pInfo(peer)->tankIDName + "("+ pInfo(peer)->world +"): " + cch;
					if (pInfo(peer)->tankIDName.empty()) break;
					if (not Server_Security.log_player.empty() && Server_Security.log_player == pInfo(peer)->tankIDName) {
						cout << "LOGGING: " << pInfo(peer)->tankIDName << " | " << "ENTER/LEFT" << "|" << cch << endl;
					}
					if (cch == "action|quit") enet_peer_disconnect_later(peer, 0);
					else if (cch == "action|quit_to_exit" && pInfo(peer)->world != "") exit_(peer);
					else if (cch == "action|gohomeworld\n" && pInfo(peer)->world .empty()) {
						if (pInfo(peer)->home_world .empty()) {
							gamepacket_t p, p2;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`9No Home World Set ``|left|1432|\nadd_spacer|small|\nadd_textbox|Use /sethome to assign the current world as your home world.|left|\nadd_spacer|small|\nend_dialog||OK||");
							p.CreatePacket(peer);
							p2.Insert("OnFailedToEnterWorld"), p2.CreatePacket(peer);
						}
						else {
							packet_(peer, "action|log\nmsg|Magically warping to home world `5" + pInfo(peer)->home_world + "``...");
							string world_name = pInfo(peer)->home_world;
							join_world(peer, world_name);
						}
					}
					else {
						if (pInfo(peer)->world .empty()) {
							if (cch.find("action|join_request") != string::npos) {
								if (pInfo(peer)->last_world_enter + 500 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
									pInfo(peer)->last_world_enter = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
									vector<string> t_ = explode("|", cch);
									if (t_.size() < 3) break;
									string world_name = explode("\n", t_[2])[0];
									/*
									if (world_name == "Random World") {
										if (World_Stuff.top_active_worlds.size() != 0)world_name = World_Stuff.top_active_worlds[rand() % World_Stuff.top_active_worlds.size()].second;
										else world_name = "LOCKETOWN";
									}*/
									transform(world_name.begin(), world_name.end(), world_name.begin(), ::toupper);
									join_world(peer, world_name);
								}
							}
							else if (cch.find("action|world_button") != string::npos) {
								vector<string> t_ = explode("|", cch);
								if (t_.size() < 3) break;
								string dialog = explode("\n", t_[2])[0];
								if (dialog == "w1_") world_menu(peer, false);
								else {
									string c_active_worlds = "", world_category_list = "";
									if (dialog == "_catselect_") {
										world_category_list = "\nadd_button|Default|w1_|0.7|3529161471|\nadd_button|Top Worlds|w2_|0.7|3529161471|\nadd_button|Random|w1_|0.7|3529161471|\nadd_button|Your Worlds|w3_|0.7|3529161471|";
										for (int i = 1; i < world_rate_types.size(); i++) world_category_list += "\nadd_button|" + (world_category(i)) + "|w" + to_string(i + 3) + "_|0.7|3529161471|";
									}
									else {
										if (dialog == "w2_") c_active_worlds = a + "\nadd_heading|" + (World_Stuff.top_list_world_menu.empty() ? "The list should update in few minutes" : "Top Worlds") + "|", c_active_worlds += World_Stuff.top_list_world_menu;
										else if (dialog == "w3_") {
											c_active_worlds = pInfo(peer)->worlds_owned.size() != 0 ? "\nadd_heading|Your Worlds ("+to_string(pInfo(peer)->worlds_owned.size()) + ")|" : "\nadd_heading|You don't have any worlds.<CR>|";
											for (int w_ = 0; w_ < (pInfo(peer)->worlds_owned.size() >= 32 ? 32 : pInfo(peer)->worlds_owned.size()); w_++) c_active_worlds += "\nadd_floater|" + pInfo(peer)->worlds_owned[w_] + "|0|0.5|2147418367";
										}
										else {
											int world_rateds = atoi(dialog.substr(1, dialog.length() - 1).c_str()) - 3;
											if (world_rateds > world_rate_types.size()) break;
											c_active_worlds = "\nadd_heading|" + (world_category(world_rateds)) + " Worlds|\nset_max_rows|4|";
											bool added_ = false;
											for (int i = 0; i < (world_rate_types[world_rateds].size() > 5 ? 6 : world_rate_types[world_rateds].size()); i++) {
												string world = world_rate_types[world_rateds][i].substr(0, world_rate_types[world_rateds][i].find("|"));
												c_active_worlds += "\nadd_floater|" + world + "|" + to_string((i + 1) * -1) + "|0.5|3417414143";
												added_ = true;
											}
											if (added_ == false) c_active_worlds += "\nadd_heading|There isn't any active worlds yet.<CR>|";
										}
										if (dialog != "w3_") {
											string recently_visited = "\nset_max_rows|-1|";
											for (auto it = pInfo(peer)->last_visited_worlds.rbegin(); it != pInfo(peer)->last_visited_worlds.rend(); ++it) recently_visited += "\nadd_floater|" + *it + "|0|0.5|3417414143";
											c_active_worlds += "\nadd_heading|Recently Visited Worlds<CR>|" + recently_visited;
										}
									}
									gamepacket_t p;
									p.Insert("OnRequestWorldSelectMenu"), p.Insert((dialog == "_catselect_" ?world_category_list : "add_filter|" + c_active_worlds)), p.CreatePacket(peer);
								}
							}
						}
					}
					break;
				}
				case 4:
				{
					if (pInfo(peer)->tankIDName.empty()) break;
					//auto start = chrono::steady_clock::now();
					if (pInfo(peer)->world.empty()) break;
					BYTE* data_ = get_struct(event.packet);
					if (data_ == nullptr) break;
					PlayerMoving* p_ = unpackPlayerMoving(data_);
					switch (p_->packetType) {
					case 0: /*Kai zaidejas pajuda*/
					{
						bool ignore_ = false;
						int currentX = pInfo(peer)->x / 32, currentY = pInfo(peer)->y / 32;
						int targetX = p_->x / 32;
						int targetY = p_->y / 32;

						if (!pInfo(peer)->whitelist_role) {
							if (p_->characterState == 34 or p_->characterState == 50) {

								variants::CrashTheGameClient(peer);
								enet_peer_disconnect_later(peer, 0);
							}
						}
						
						if (pInfo(peer)->level < 2 && pInfo(peer)->mod == 0) {
							if (abs(targetX - currentX) >= 7 || abs(targetY - currentY) >= 7) {
								bool skip = false;
								if (pInfo(peer)->anticheat_cooldown + 3500 > (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count() || pInfo(peer)->anticheat_cooldown2 + 1100 > (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) skip = true;
								if (currentX != 0 && currentY != 0) {
									if (skip == false) enet_peer_disconnect_later(peer, 0);
								}
							}

							string name_ = pInfo(peer)->world;
							if (name_ != "") {
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds.at(p - worlds.begin());
									if (pInfo(peer)->x != -1 and pInfo(peer)->y != -1) {
										//Game_Check_Auto_Join(world_, peer);
										int x_ = (pInfo(peer)->state == 16 ? (int)p_->x / 32 : round((double)p_->x / 32)), y_ = (int)p_->y / 32;
										if (x_ < 0 or x_ >= world_->max_x * 32 or y_ < 0 or y_ >= world_->max_y * 32) continue;
										WorldBlock* block_ = &world_->blocks.at(x_ + (y_ * 100));
										if (pInfo(peer)->c_x * 32 != (int)p_->x and pInfo(peer)->c_y * 32 != (int)p_->y and not pInfo(peer)->ghost) {
											bool impossible = ar_turi_noclipa(world_, pInfo(peer)->x, pInfo(peer)->y, x_ + (y_ * 100), peer);
											if (impossible) {
												if (pInfo(peer)->pickup_time + 2500 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
													pInfo(peer)->pickup_limits = 0;
													pInfo(peer)->pickup_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
												}
												else {
													if (pInfo(peer)->pickup_limits >= 14) enet_peer_disconnect_later(peer, 0);
													else pInfo(peer)->pickup_limits++;
												}
												OnSetPos(peer, pInfo(peer)->x, pInfo(peer)->y);
												ignore_ = true;
												break;
											}
										}
									}
								}
							}
						}
						if (ignore_) break;
						if (pInfo(peer)->update) {
							if (not pInfo(peer)->world.empty() && p_->characterState == 4) {
								if (pInfo(peer)->x > 0 && pInfo(peer)->y > 0) pInfo(peer)->update = false;
								update_clothes(peer);
							}
						}
						/*
						if (pInfo(peer)->temp_x != 0 && pInfo(peer)->temp_y != 0) {
							pInfo(peer)->x = pInfo(peer)->temp_x;
							pInfo(peer)->y = pInfo(peer)->temp_y;
							p_->x = pInfo(peer)->temp_x;
							p_->y = pInfo(peer)->temp_y;
							pInfo(peer)->temp_x = 0;
							pInfo(peer)->temp_y = 0;
						}*/
						if (not pInfo(peer)->spectate_person.empty()) {
							bool found_ = false;
							for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->growid == false) continue;
								if (pInfo(peer)->spectate_person == pInfo(currentPeer)->tankIDName) OnSetPos(currentPeer, p_->x, p_->y), found_ = true;
							}
							if (found_ == false) pInfo(peer)->spectate_person = "";
						}
						//cout << float(p_->characterState) << endl;
						if (p_->characterState == 268435472 || float(p_->characterState) == 80 || float(p_->characterState) == 6.71091e+07 || float(p_->characterState) == 6.71113e+07 || p_->characterState == 268435488 || p_->characterState == 262208 || p_->characterState == 327680 || p_->characterState == 673712008 || p_->characterState == 67371216 || p_->characterState == 268435504 || p_->characterState == 268435616 || p_->characterState == 268435632 || p_->characterState == 268435456 || p_->characterState == 224 || p_->characterState == 112 || p_->characterState == 80 || p_->characterState == 96 || p_->characterState == 224 || p_->characterState == 65584 || p_->characterState == 65712 || p_->characterState == 65696 || p_->characterState == 65536 || p_->characterState == 65552 || p_->characterState == 65568 || p_->characterState == 65680 || p_->characterState == 192 || p_->characterState == 65664 || p_->characterState == 65600 || p_->characterState == 67860 || p_->characterState == 64) {
							if (pInfo(peer)->lava_time + 5000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
								pInfo(peer)->lavaeffect = 0;
								pInfo(peer)->lava_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
							}
							else {
								if (pInfo(peer)->xenonite & Gtps3::XENONITE_FORCE_HEAT_RESIST || pInfo(peer)->xenonite & Gtps3::XENONITE_BLOCK_HEAT_RESIST || (pInfo(peer)->cheater_settings & Gtps3::SETTINGS_8 && pInfo(peer)->disable_cheater == 0)) {
									if (pInfo(peer)->lavaeffect >= (pInfo(peer)->xenonite & Gtps3::XENONITE_FORCE_HEAT_RESIST or (pInfo(peer)->cheater_settings & Gtps3::SETTINGS_8 && pInfo(peer)->disable_cheater == 0) ? 7 : 3)) {
										pInfo(peer)->lavaeffect = 0;
										SendRespawn(peer, false, 0, true);
									}
									else pInfo(peer)->lavaeffect++;
								}
								else {
									if (pInfo(peer)->lavaeffect >= (pInfo(peer)->feet == 250 || pInfo(peer)->necklace == 5426 || (pInfo(peer)->mask == 5712 && pInfo(peer)->wild == 6) ? 7 : 3)) {
										pInfo(peer)->lavaeffect = 0;
										SendRespawn(peer, false, 0, true);
									}
									else pInfo(peer)->lavaeffect++;
								}
							}
						}
						if (pInfo(peer)->fishing_used != 0) {
							if (pInfo(peer)->f_xy != pInfo(peer)->x + pInfo(peer)->y) pInfo(peer)->move_warning++;
							if (pInfo(peer)->move_warning > 1) stop_fishing(peer, true, "Sit still if you wanna fish!");
							if (p_->punchX > 0 && p_->punchY > 0) {
								pInfo(peer)->punch_warning++;
								if (pInfo(peer)->punch_warning >= 2) stop_fishing(peer, false, "");
							}
						}
						if (pInfo(peer)->hand == 2286 or  pInfo(peer)->hand == 2560) {
							if (rand() % 100 < 4) {
								pInfo(peer)->geiger_++;
								int give_Back = items[pInfo(peer)->hand].geiger_give_back;
								if (pInfo(peer)->geiger_ >= 100) {
									int c_ = -1, c_2 = 1;
									modify_inventory(peer, pInfo(peer)->hand, c_);
									modify_inventory(peer, give_Back, c_2);
									pInfo(peer)->hand = give_Back;
									pInfo(peer)->geiger_ = 0;
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("You are detecting radiation... (`$"+items[pInfo(peer)->hand].ori_name + "`` mod added)");
									p.CreatePacket(peer);
									packet_(peer, "action|play_sfx\nfile|audio/dialog_confirm.wav\ndelayMS|0");
									update_clothes(peer);
								}
							}
						}
						if (pInfo(peer)->back == 240) {
							if (pInfo(peer)->gems > 0) {
								if (pInfo(peer)->x != (int)p_->x) {
									if (pInfo(peer)->i240 + 750 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
										pInfo(peer)->i240 = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
										pInfo(peer)->gems -= 1;
										WorldDrop item_{};
										string name_ = pInfo(peer)->world;
										vector<World>::iterator pb = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
										if (pb != worlds.end()) {
											World* world_ = &worlds[pb - worlds.begin()];
											world_->fresh_world = true;
											item_.id = 112, item_.count = 1, item_.x = (int)p_->x + rand() % 17, item_.y = (int)p_->y + rand() % 17;
											dropas_(world_, item_);
										}
										OnSetGems(peer);
									}
								}
							}
						}
						move_(peer, p_);
						pInfo(peer)->x = (int)p_->x, pInfo(peer)->y = (int)p_->y, pInfo(peer)->state = p_->characterState & 0x10;
						pInfo(peer)->last_state = p_->characterState;
						
						break;
					}
					case 3: /*Kai zaidejas papunchina/wrenchina bloka*/
					{
						if (pInfo(peer)->trading_with != -1) cancel_trade(peer, false);
						if (pInfo(peer)->punch_aura) {
							gamepacket_t p;
							p.Insert("OnParticleEffectV2"), p.Insert(pInfo(peer)->punch_aura_id), p.Insert((float)(p_->punchX * 32) + 16, (float)(p_->punchY * 32) + 16);
							for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
								if (pInfo(currentPeer)->world == pInfo(peer)->world) p.CreatePacket(currentPeer);
							}
						}
						player_punch(peer, p_->plantingTree, p_->punchX, p_->punchY, p_->x, p_->y);
						break;
					}
					case 7: /*Kai zaidejas ieina pro duris arba portal*/ /*2/16/2022 update: cia dar gali buti STEAM USE*/
					{
						if (p_->punchX < 0 || p_->punchY < 0) break;
						string name_ = pInfo(peer)->world;
						vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
						if (p != worlds.end()) {
							World* world_ = &worlds[p - worlds.begin()];
							if (p_->punchX >= world_->max_x || p_->punchY >= world_->max_y) break;
							int x = p_->punchX, y = p_->punchY;

							world_->fresh_world = true;
							//try {
							WorldBlock* block_ = &world_->blocks[p_->punchX + (p_->punchY * 100)];
							if (items[items[block_->fg ? block_->fg : block_->bg].id].blockType == BlockTypes::CHECKPOINT) {
								if (ar_turi_noclipa(world_, pInfo(peer)->x, pInfo(peer)->y, p_->punchX + (p_->punchY * 100), peer) == false) {
									if (block_->fg == 9858 && world_->name == "GROWCH") {
										pInfo(peer)->c_x = 0, pInfo(peer)->c_y = 0;
										SendRespawn(peer, true, 0, 1);
										int c_ = inventory_contains(peer, 3210);
										if (c_ == 0) {
											//daily_quest_winterfest(peer, false, "33", 1);
											c_ = 1;
											if (modify_inventory(peer, 3210, c_)) {
												gamepacket_t p;
												p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`2You have claimed Icy Heart Of Winter!``"), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
											}
										}
									}
									if (block_->fg == 9858) wipe_darkticket(peer, true);
									else if (block_->fg == 2994) wipe_whistle(peer);
									if (block_->fg == 4882) if (not has_playmod2(pInfo(peer), 138) && has_playmod2(pInfo(peer), 136, 1)) {
										add_playmod(peer, 138);
									}
									pInfo(peer)->c_x = p_->punchX, pInfo(peer)->c_y = p_->punchY;
									gamepacket_t p(0, pInfo(peer)->netID);
									p.Insert("SetRespawnPos");
									p.Insert(pInfo(peer)->c_x + (pInfo(peer)->c_y * 100));
									p.CreatePacket(peer);

								}
							}
							else if (items[block_->fg ? block_->fg : block_->bg].id == 6) exit_(peer);
							else if (block_->fg == 4722 && pInfo(peer)->adventure_begins == false) {
								pInfo(peer)->adventure_begins = true;
								gamepacket_t p;
								p.Insert("OnAddNotification"), p.Insert("interface/large/adventure.rttex"), p.Insert(block_->heart_monitor), p.Insert("audio/gong.wav"), p.Insert(0), p.CreatePacket(peer);
							}
							else if (block_->fg == 428 || block_->fg == 430) {
								if ((block_->fg == 428 && pInfo(peer)->race_flag == 0) or block_->fg == 430) {
									int time = 0;
									gamepacket_t p(0, pInfo(peer)->netID), p3(0, pInfo(peer)->netID);
									p.Insert("OnRace" + a + (block_->fg == 428 ? "Start" : "End"));
									if (block_->fg == 430) {
										time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count() - pInfo(peer)->race_flag;
										p.Insert(time);
									}
									else pInfo(peer)->race_flag = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
									p.CreatePacket(peer);
									p3.Insert("OnPlayPositioned"), p3.Insert("audio/"+a + (block_->fg == 428 ? "race_start" : "race_end") + ".wav"), p3.CreatePacket(peer);
									if (block_->fg == 430) {
										gamepacket_t p_c;
										p_c.Insert("OnConsoleMessage"), p_c.Insert(get_player_nick(peer) + " `0finished in`` `2" +detailMSTime(time) + "```o!``");
										pInfo(peer)->race_flag = 0;
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != world_->name) continue;
											p_c.CreatePacket(currentPeer);
										}
									}
								}
							}
							/*
							else if (block_->fg == 1508 and not world_->name.empty()) {
								//blarney prize
								char blarney_world = world_->name.back();
								if (isdigit(blarney_world)) {
									long long current_time = time(nullptr);
									vector<vector<long long>> av_blarneys = pInfo(peer)->completed_blarneys;
									for (int i_ = 0; i_ < av_blarneys.size(); i_++) {
										int t_blarney_world = av_blarneys[i_][0];
										if ((int)blarney_world - 48 == t_blarney_world) {
											long long blarney_time = av_blarneys[i_][1];
											if (blarney_time - current_time <= 0) {
												av_blarneys[i_][1] = current_time + 86400;
												vector<vector<int>> blarney_prizes{
													{11712, 1},{11742, 1},{11710, 1},{11722, 1}, {528, 1},{540, 1},{1514, 5},{1544, 1},{260, 1},{1546, 1},{2400, 1},{2404, 1},{2406, 1},{2414, 1},{2416, 1},{2464, 1},{3428, 1},{3426, 1},{4532, 1},{4528, 1},{4526, 5},{4520, 1},{5740, 1},{5734, 1},{7982, 1},{7992, 1},{7994, 1},{7980, 1},{7998, 1},{7984, 3},{7988, 1},{9416, 1},{9424, 1},{10704, 1},{10680, 1},{10670, 1},{10676, 1}
												};
												vector<int> prize_ = blarney_prizes[rand() % blarney_prizes.size()];
												uint32_t give_id = prize_[0];
												uint32_t give_count = prize_[1];
												int c_ = give_count;
												if (modify_inventory(peer, give_id, c_) != 0) {
													WorldDrop drop_block_{};
													drop_block_.id = give_id, drop_block_.count = give_count, drop_block_.x = pInfo(peer)->x + rand() % 17, drop_block_.y = pInfo(peer)->y + rand() % 17;
													dropas_(world_, drop_block_);
												}
												int c_2 = 1;
												if (modify_inventory(peer, 1510, c_2) != 0) {
													WorldDrop drop_block_{};
													drop_block_.id = 1510, drop_block_.count = c_2, drop_block_.x = pInfo(peer)->x + rand() % 17, drop_block_.y = pInfo(peer)->y + rand() % 17;
													dropas_(world_, drop_block_);
												}
												pInfo(peer)->remind_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("You kissed the " + items[block_->fg].name + " and got a `2" + items[1510].name + "`` and `2" + items[give_id].name + "``");
												p.Insert(1);
												p.CreatePacket(peer);
												{
													gamepacket_t p;
													p.Insert("OnConsoleMessage");
													p.Insert("You kissed the " + items[block_->fg].name + " and got a `2" + items[1510].name + "`` and `2" + items[give_id].name + "``");
													p.CreatePacket(peer);
												}

												bonanza_sold++;
												if (bonanza_sold == bonanza_item) {
													bonanza_item_current = bonanza_items[rand() % bonanza_items.size()];
													bonanza_time = time(nullptr) + 10800;
												}
											}
											else {
												if (pInfo(peer)->remind_time + 8000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
													pInfo(peer)->remind_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
													gamepacket_t p;
													p.Insert("OnTalkBubble");
													p.Insert(pInfo(peer)->netID);
													p.Insert("You will be able to kiss the stone again in " + to_playmod_time(blarney_time - current_time) + "");
													p.Insert(0);
													p.CreatePacket(peer);
												}
											}
											break;
										}
									}
									pInfo(peer)->completed_blarneys = av_blarneys;
								}
							}*/
							else if (block_->fg == 1792) {
								int c_ = inventory_contains(peer, 1794);
								if (c_ == 0) {
									c_ = 1;
									if (modify_inventory(peer, 1794, c_)) {
										gamepacket_t p;
										p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`9You have claimed a Legendary Orb!``"), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
										{
											gamepacket_t p;
											p.Insert("OnParticleEffect"), p.Insert(46), p.Insert((float)pInfo(peer)->x + 16, (float)pInfo(peer)->y + 16);
											p.CreatePacket(peer);
										}
									}
								}
							}
							else if (items[block_->fg].blockType == BlockTypes::DOOR or items[block_->fg].blockType == BlockTypes::PORTAL) {
								string door_target = block_->door_destination, door_id = "";
								World target_world = worlds[p - worlds.begin()];
								bool locked = true, found_ = false;
								if (block_access(peer, world_, block_) or block_->open) locked = false;
								int spawn_x = 0, spawn_y = 0;
								if (not locked && block_->fg != 762) {
									if (door_target.find(":") != string::npos) {
										vector<string> detales = explode(":", door_target);
										door_target = detales[0], door_id = detales[1];
									} if (not door_target.empty() and door_target != world_->name) {
										if (not check_name(door_target)) {
											gamepacket_t p(250, pInfo(peer)->netID);
											p.Insert("OnSetFreezeState");
											p.Insert(1);
											p.CreatePacket(peer);
											{
												gamepacket_t p(250);
												p.Insert("OnConsoleMessage");
												p.Insert(door_target);
												p.CreatePacket(peer);
											}
											{
												gamepacket_t p(250);
												p.Insert("OnZoomCamera");
												p.Insert((float)10000.000000);
												p.Insert(1000);
												p.CreatePacket(peer);
											}
											{
												gamepacket_t p(250, pInfo(peer)->netID);
												p.Insert("OnSetFreezeState");
												p.Insert(0);
												p.CreatePacket(peer);
											}
											break;
										}
										target_world = get_world(door_target);
									}
									if (locked == false) {
										if (not door_id.empty()) {
											vector<WorldBlock>::iterator p = find_if(target_world.blocks.begin(), target_world.blocks.end(), [&](const WorldBlock& a) { return (items[a.fg].path_marker || items[a.fg].blockType == BlockTypes::DOOR || items[a.fg].blockType == BlockTypes::PORTAL) && a.door_id == door_id; });
											if (p != target_world.blocks.end()) {
												int i_ = p - target_world.blocks.begin();
												spawn_x = i_ % 100, spawn_y = i_ / 100;
											}
											else found_ = true;
										}
									}
									if (locked || door_id.empty()) found_ = true;
									
								}
								if (block_->fg == 762) {
									pInfo(peer)->lastwrenchx = p_->punchX, pInfo(peer)->lastwrenchy = p_->punchY;
									gamepacket_t p2;
									if (block_->door_id.empty()) p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("No password has been set yet!"), p2.Insert(0), p2.Insert(1);
									else p2.Insert("OnDialogRequest"), p2.Insert("set_default_color|`o\nadd_label_with_icon|big|`wPassword Door``|left|762|\nadd_textbox|The door requires a password.|left|\nadd_text_input|password|Password||24|\nend_dialog|password_reply|Cancel|OK|");
									p2.CreatePacket(peer);
									gamepacket_t p(250, pInfo(peer)->netID), p3(250), p4(250, pInfo(peer)->netID);
									p.Insert("OnSetFreezeState"), p.Insert(1), p.CreatePacket(peer);
									p3.Insert("OnZoomCamera"), p3.Insert((float)10000.000000), p3.Insert(1000), p3.CreatePacket(peer);
									p4.Insert("OnSetFreezeState"), p4.Insert(0), p4.CreatePacket(peer);
								}
								bool block_a = false;
								if ((block_->fg == 10358 && block_access(peer, world_, block_) == false) && block_->open && block_->shelf_1 != 0) {
									int my_wls = get_wls(peer, true);
									if (my_wls >= block_->shelf_1) {
										get_wls(peer, true, true, block_->shelf_1);
										block_->wl += block_->shelf_1;
										block_a = false;
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("`2Entry!``");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
									else {
										block_a = true;
										pInfo(peer)->lastwrenchx = p_->punchX, pInfo(peer)->lastwrenchy = p_->punchY;
										gamepacket_t p2;
										p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("You don't have enough world locks!"), p2.Insert(0), p2.Insert(1);
										p2.CreatePacket(peer);
										gamepacket_t p(250, pInfo(peer)->netID), p3(250), p4(250, pInfo(peer)->netID);
										p.Insert("OnSetFreezeState"), p.Insert(1), p.CreatePacket(peer);
										p3.Insert("OnZoomCamera"), p3.Insert((float)10000.000000), p3.Insert(1000), p3.CreatePacket(peer);
										p4.Insert("OnSetFreezeState"), p4.Insert(0), p4.CreatePacket(peer);
									}
								}
								if (block_->fg != 762 && block_a == false) {
									join_world(peer, target_world.name, spawn_x, spawn_y, 250, locked, true, found_);
								}
							}
							else {
								switch (block_->fg) {
								case 3270: case 3496:
								{
									Position2D steam_connector = track_steam(world_, block_, p_->punchX, p_->punchY);
									if (steam_connector.x >= 0 and steam_connector.y >= 0) {
										WorldBlock* block_s = &world_->blocks[steam_connector.x + (steam_connector.y * 100)];
										switch (block_s->fg) {
										case 3286: //steam door
										{
											block_s->flags = (block_s->flags & 0x00400000 ? block_s->flags ^ 0x00400000 : block_s->flags | 0x00400000);
											PlayerMoving data_{};
											data_.packetType = 5, data_.punchX = steam_connector.x, data_.punchY = steam_connector.y, data_.characterState = 0x8;
											BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, block_s));
											BYTE* blc = raw + 56;
											form_visual(blc, *block_s, *world_, peer, false);
											for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
												if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
												if (pInfo(currentPeer)->world == world_->name) {
													variants::OnPlayPositioned(currentPeer, pInfo(peer)->netID, "audio/hiss3.wav", 100);
													send_raw(currentPeer, 4, raw, 112 + alloc_(world_, block_s), ENET_PACKET_FLAG_RELIABLE);
												}
											}
											delete[] raw, blc;
											break;
										}
										case 3724: // spirit storage unit
										{
											uint32_t scenario = 20;
											{
												// check for ghost jars
												for (int i = 0; i < world_->drop_new.size(); i++) {
													Position2D dropped_at{ world_->drop_new[i][3] / 32, world_->drop_new[i][4] / 32};
													if (dropped_at.x == steam_connector.x and dropped_at.y == steam_connector.y) {
														if (world_->drop_new[i][0] == 3722) {
															uint32_t explo_chance = world_->drop_new[i][1];
															// remove drop
															{
																PlayerMoving data_{};
																data_.packetType = 14, data_.netID = -2, data_.plantingTree = world_->drop_new[i][2];
																BYTE* raw = packPlayerMoving(&data_);
																int32_t item = -1;
																memcpy(raw + 8, &item, 4);
																for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
																	if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
																	if (pInfo(currentPeer)->world == name_) {
																		send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
																	}
																}
																world_->drop_new[i][0] = 0, world_->drop_new[i][3] = -32, world_->drop_new[i][4] = -32;
																world_->drop_new.erase(world_->drop_new.begin() + i);
																delete[] raw;
															}
															block_s->c_ += explo_chance;
															// explode or not
															{
																if (block_s->c_ * 5 >= 105) {
																	float explosion_chance = (float)((block_s->c_ * 5) - 100) * 0.5;
																	if (explosion_chance > rand() % 100) {
																		//bam bam
																		block_s->fg = 3726;
																		// drop the prize
																		{
																			vector<int> all_p{ 13020, 12464, 3734, 3732, 3748, 3712, 3706, 3708, 3718, 11136, 3728, 10056, 3730, 3788, 3750, 3738, 6060, 3738, 6840, 3736, 7784, 9596, 9598, 12288 };
																			uint32_t prize = 0;
																			if (block_s->c_ * 5 <= 115) prize = 3734;
																			else if (block_s->c_ * 5 <= 130) prize = 3732;
																			else if (block_s->c_ * 5 <= 140) prize = 3748;
																			else if (block_s->c_ * 5 <= 150) prize = 12464;
																			else if (block_s->c_ * 5 <= 170) {
																				vector<int> p_drops = {
																					3712, 3706, 3708, 3718, 11136
																				};
																				prize = p_drops[rand() % p_drops.size()];
																			}
																			else if (block_s->c_ * 5 <= 190)  prize = 3728;
																			else if (block_s->c_ * 5 <= 205)  prize = 10056;
																			else if (block_s->c_ * 5 <= 220)  prize = 3730;
																			else if (block_s->c_ * 5 == 225)  prize = 3788;
																			else if (block_s->c_ * 5 <= 240)  prize = 3750;
																			else if (block_s->c_ * 5 == 245)  prize = 3738;
																			else if (block_s->c_ * 5 <= 255)  prize = 6060;
																			else if (block_s->c_ * 5 <= 265 or explo_chance * 5 >= 265) {
																				if (explo_chance * 5 >= 265) prize = all_p[rand() % all_p.size()];
																				else prize = 3738;
																			}
																			else {
																				vector<int> p_drops = {
																					6840
																				};
																				if (block_s->c_ * 5 >= 270) p_drops.push_back(3736);
																				if (block_s->c_ * 5 >= 295) p_drops.push_back(7784);
																				if (block_s->c_ * 5 >= 369) p_drops.push_back(9598);
																				if (block_s->c_ * 5 >= 500) p_drops.push_back(9596);
																				if (block_s->c_ * 5 >= 850) p_drops.push_back(12288);
																				prize = p_drops[rand() % p_drops.size()];
																			} if (prize != 0) {
																				WorldDrop drop_block_{};
																				drop_block_.x = steam_connector.x * 32 + rand() % 17;
																				drop_block_.y = steam_connector.y * 32 + rand() % 17;
																				drop_block_.id = prize, drop_block_.count = 1;
																				dropas_(world_, drop_block_);
																				{
																					PlayerMoving data_{};
																					data_.packetType = 0x11, data_.x = steam_connector.x * 32 + 16, data_.y = steam_connector.y * 32 + 16;
																					data_.YSpeed = 97, data_.XSpeed = 3724;
																					BYTE* raw = packPlayerMoving(&data_);
																					PlayerMoving data_2{};
																					data_2.packetType = 0x11, data_2.x = steam_connector.x * 32 + 16, data_2.y = steam_connector.y * 32 + 16;
																					data_2.YSpeed = 108;
																					BYTE* raw2 = packPlayerMoving(&data_2);
																					gamepacket_t p;
																					p.Insert("OnConsoleMessage");
																					p.Insert("`#[A `9Spirit Storage Unit`` exploded, bringing forth an `9" + items[prize].name + "`` from The Other Side!]``");
																					for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
																						if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
																						if (pInfo(currentPeer)->world == world_->name) {
																							p.CreatePacket(currentPeer);
																							send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
																							send_raw(currentPeer, 4, raw2, 56, ENET_PACKET_FLAG_RELIABLE);
																						}
																					}
																					delete[] raw, raw2;
																				}
																				scenario = 22;
																			}
																		}
																		block_s->c_ = 0;
																	}
																}
															}
															// update visuals
															{
																PlayerMoving data_{};
																data_.packetType = 5, data_.punchX = steam_connector.x, data_.punchY = steam_connector.y, data_.characterState = 0x8;
																BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, block_s));
																BYTE* blc = raw + 56;
																form_visual(blc, *block_s, *world_, peer, false);
																for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
																	if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
																	if (pInfo(currentPeer)->world == world_->name) {
																		send_raw(currentPeer, 4, raw, 112 + alloc_(world_, block_s), ENET_PACKET_FLAG_RELIABLE);
																	}
																}
																delete[] raw, blc;
															}
															break;
														}
													}
												}
											}
											PlayerMoving data_{};
											data_.packetType = 32; // steam update paketas
											data_.punchX = steam_connector.x;
											data_.punchY = steam_connector.y;
											BYTE* raw = packPlayerMoving(&data_);
											raw[3] = scenario;
											for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
												if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
												if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
												send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											}
											delete[] raw;
											break;
										}
										default:
											break;
										}
									}
									PlayerMoving data_{};
									data_.packetType = 32; // steam update paketas
									data_.punchX = p_->punchX;
									data_.punchY = p_->punchY;
									BYTE* raw = packPlayerMoving(&data_);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
										send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									}
									delete[] raw;
									break;
								}
								default:
									break;
								}
							}
						}
						//catch (out_of_range& klaida) {
							//cout << "case 7 klaida -> " << klaida.what() << endl;
						//}
					//}
						break;
					}
					case 10: /*Kai zaidejas paspaudzia du kartus ant inventory itemo*/
					{
						if (pInfo(peer)->trading_with != -1) {
							cancel_trade(peer, false);
							break;
						}
						if (p_->plantingTree <= 0 or p_->plantingTree >= items.size()) break;
						int c_ = 0;
						modify_inventory(peer, p_->plantingTree, c_);
						if (c_ == 0) break;
						if (items[p_->plantingTree].blockType != BlockTypes::CLOTHING) {
							if (get_free_slots(pInfo(peer)) >= 1) {
								if (items[p_->plantingTree].compress_return_count != 0) {
									int countofused = 0, getdl = 1, getwl = 100, removewl = -100, removedl = -1, countwl = 0;
									modify_inventory(peer, items[p_->plantingTree].compress_item_return, countwl);
									if (items[p_->plantingTree].compress_return_count == 100 ? countwl <= 199 : countwl <= 100) {
										modify_inventory(peer, p_->plantingTree, countofused);
										if (items[p_->plantingTree].compress_return_count == 100 ? countofused >= 100 : countofused >= 1) {
											modify_inventory(peer, p_->plantingTree, items[p_->plantingTree].compress_return_count == 100 ? removewl : removedl);
											modify_inventory(peer, items[p_->plantingTree].compress_item_return, items[p_->plantingTree].compress_return_count == 100 ? getdl : getwl);
											gamepacket_t p, p2;
											p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert(items[p_->plantingTree].compress_return_count == 100 ? "You compressed 100 `2" + items[p_->plantingTree].ori_name + "`` into a `2" + items[items[p_->plantingTree].compress_item_return].ori_name + "``!" : "You shattered a `2" + items[p_->plantingTree].ori_name + "`` into 100 `2" + items[items[p_->plantingTree].compress_item_return].ori_name + "``!"), p.Insert(0), p.Insert(1), p.CreatePacket(peer);
											p2.Insert("OnConsoleMessage"), p2.Insert(items[p_->plantingTree].compress_return_count == 100 ? "You compressed 100 `2" + items[p_->plantingTree].ori_name + "`` into a `2" + items[items[p_->plantingTree].compress_item_return].ori_name + "``!" : "You shattered a `2" + items[p_->plantingTree].ori_name + "`` into 100 `2" + items[items[p_->plantingTree].compress_item_return].ori_name + "``!"), p2.CreatePacket(peer);
										}
									}
								}
							}
						}
						else {
							equip_clothes(peer, p_->plantingTree);
						}
						break;
					}
					case 11: /*Kai zaidejas paema isdropinta itema*/
					{
						if (p_->x < 0 || p_->y < 0) break;
						if (pInfo(peer)->ghost or pInfo(peer)->invis) break;
						if (pInfo(peer)->level < 2 && pInfo(peer)->mod == 0) {
							int currentX = pInfo(peer)->x / 32, currentY = pInfo(peer)->y / 32, targetX = p_->x / 32, targetY = p_->y / 32;
							if (abs(targetX - pInfo(peer)->x / 32) >= 9 || abs(targetY - pInfo(peer)->y / 32) >= 9)
							{
								if (currentX != 0 && currentY != 0)
								{
									if (pInfo(peer)->pickup_time + 2500 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
										pInfo(peer)->pickup_limits = 0;
										pInfo(peer)->pickup_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
									}
									else {
										if (pInfo(peer)->pickup_limits >= 14) enet_peer_disconnect_later(peer, 0);
										else pInfo(peer)->pickup_limits++;
									}
									break;
								}
							}
						}
						
						bool displaybox = true;
						string name_ = pInfo(peer)->world;
						vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
						if (p != worlds.end()) {
							World* world_ = &worlds[p - worlds.begin()];
							if (p_->y / 32 >= world_->max_y || p_->x / 32 >= world_->max_x) break;
							world_->fresh_world = true;
							for (int i_ = 0; i_ < world_->drop_new.size(); i_++) {
								if (world_->drop_new[i_][2] == p_->plantingTree) {
									int x = pInfo(peer)->x / 32, y = pInfo(peer)->y / 32;
									int take_x = p_->x / 32;
									int take_y = p_->y / 32;
									if (x > take_x) x--;
									if (x < take_x) x++;
									bool block = false;
									if (items[world_->drop_new[i_][0]].blockType == BlockTypes::FISH) {
										for (int a_ = 0; a_ < pInfo(peer)->inv.size(); a_++) {
											if (pInfo(peer)->inv[a_].first == world_->drop_new[i_][0]) {
												block = true;
												break;
											}

										}
									}
									if (block) continue;
									bool noclipas = false;
									if (pInfo(peer)->level < 3) noclipas = ar_turi_noclipa(world_, x * 32, y * 32, take_x + (take_y * 100), peer);
									if (noclipas == false) {
										WorldBlock* block_ = &world_->blocks[world_->drop_new[i_][3] / 32 + (world_->drop_new[i_][4] / 32 * 100)];
										if (world_->world_settings & Gtps3::SETTINGS_2 && block_access(peer, world_, block_) == false) continue;
										if (block_->fg == 1422 || block_->fg == 2488) displaybox = block_access(peer, world_, block_);
										if (abs((int)p_->x / 32 - world_->drop_new[i_][3] / 32) > 1 || abs((int)p_->x - world_->drop_new[i_][3]) >= 32 or abs((int)p_->y - world_->drop_new[i_][4]) >= 32) displaybox = false;
										bool noclip = false;
										if (pInfo(peer)->superdev || pInfo(peer)->dev || to_lower(world_->owner_name) == to_lower(pInfo(peer)->tankIDName) || find(world_->admins.begin(), world_->admins.end(), to_lower(pInfo(peer)->tankIDName)) != world_->admins.end()) {
											if (pInfo(peer)->superdev || to_lower(world_->owner_name) == to_lower(pInfo(peer)->tankIDName) || find(world_->admins.begin(), world_->admins.end(), to_lower(pInfo(peer)->tankIDName)) != world_->admins.end()) displaybox = true;
										}
										if (displaybox) {
											int c_ = world_->drop_new[i_][1];
											if (world_->special_event && find(world_->world_event_items.begin(), world_->world_event_items.end(), world_->drop_new[i_][0]) != world_->world_event_items.end()) {
												world_->special_event_item_taken++;
												if (items[world_->special_event_item].event_total == world_->special_event_item_taken) {
													gamepacket_t p, p3;
													p.Insert("OnAddNotification"), p.Insert("interface/large/special_event.rttex"), p.Insert("`2" + items[world_->special_event_item].event_name + ":`` `oSuccess! " + (items[world_->special_event_item].event_total == 1 ? "`2" + pInfo(peer)->tankIDName + "`` found it!``" : "All items found!``") + ""), p.Insert("audio/cumbia_horns.wav"), p.Insert(0);
													p3.Insert("OnConsoleMessage"), p3.Insert("`2" + items[world_->special_event_item].event_name + ":`` `oSuccess!`` " + (items[world_->special_event_item].event_total == 1 ? "`2" + pInfo(peer)->tankIDName + "`` `ofound it!``" : "All items found!``") + "");
													for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
														if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != name_) continue;
														if (items[world_->special_event_item].event_total != 1) {
															gamepacket_t p2;
															p2.Insert("OnConsoleMessage"), p2.Insert("`2" + items[world_->special_event_item].event_name + ":`` `0" + pInfo(peer)->tankIDName + "`` found a " + items[world_->drop_new[i_][0]].ori_name + "! (" + to_string(world_->special_event_item_taken) + "/" + to_string(items[world_->special_event_item].event_total) + ")``"), p2.CreatePacket(currentPeer);
														}
														p.CreatePacket(currentPeer);
														p3.CreatePacket(currentPeer);
													}
													world_->world_event_items.clear();
													world_->last_special_event = 0, world_->special_event_item = 0, world_->special_event_item_taken = 0, world_->special_event = false;
												}
												else {
													gamepacket_t p2;
													p2.Insert("OnConsoleMessage"), p2.Insert("`2" + items[world_->special_event_item].event_name + ":`` `0" + pInfo(peer)->tankIDName + "`` found a " + items[world_->drop_new[i_][0]].ori_name + "! (" + to_string(world_->special_event_item_taken) + "/" + to_string(items[world_->special_event_item].event_total) + ")``");
													for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
														if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != name_) continue;
														p2.CreatePacket(currentPeer);
													}
												}
											}

											/*
											if (items[world_->drop_new[i_][0]].untradeable) {
												PlayerMoving data_{};
												data_.packetType = 14, data_.netID = -2, data_.plantingTree = world_->drop_new[i_][2];
												BYTE* raw = packPlayerMoving(&data_);
												for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
													if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != name_) continue;
													send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
												}
												delete[]raw;
												world_->drop_new.erase(world_->drop_new.begin() + i_);
												break;
											}*/
											
											else if ((world_->drop_new[i_][0] != 4490 && modify_inventory(peer, world_->drop_new[i_][0], c_, false, true) == 0) or world_->drop_new[i_][0] == 112 or world_->drop_new[i_][0] == 4490) {
												PlayerMoving data_{};
												 data_.packetType = 14, data_.netID = (world_->drop_new[i_][0] == 4490 ? -2 : pInfo(peer)->netID), data_.plantingTree = world_->drop_new[i_][2];
												BYTE* raw = packPlayerMoving(&data_);
												if (world_->drop_new[i_][0] == 112 || world_->drop_new[i_][0] == 4490) {
													if (world_->drop_new[i_][0] == 4490) {
														OnSetGems(peer, (1000 * world_->drop_new[i_][1]));
														PlayerMoving data2_{};
														data2_.x = world_->drop_new[i_][3] + 16, data2_.y = world_->drop_new[i_][4] + 16, data2_.packetType = 19, data2_.punchX = world_->drop_new[i_][0], data2_.punchY = pInfo(peer)->netID;
														int32_t to_netid = pInfo(peer)->netID;
														BYTE* raw2 = packPlayerMoving(&data2_);
														raw2[3] = 5;
														memcpy(raw2 + 8, &to_netid, 4);
														send_raw(peer, 4, raw2, 56, ENET_PACKET_FLAG_RELIABLE);
														delete[] raw2;
													}
													else pInfo(peer)->gems += c_;
												}
												else {
													add_cctv(peer, "took", to_string(world_->drop_new[i_][1]) + " " + items[world_->drop_new[i_][0]].name);
													gamepacket_t p;
													p.Insert("OnConsoleMessage"), p.Insert("Collected `w" + to_string(world_->drop_new[i_][1]) + "" + (items[world_->drop_new[i_][0]].blockType == BlockTypes::FISH ? "lb." : "") + " " + items[world_->drop_new[i_][0]].ori_name + "``." + (items[world_->drop_new[i_][0]].rarity > 363 ? "" : " Rarity: `w" + to_string(items[world_->drop_new[i_][0]].rarity) + "``") + ""), p.CreatePacket(peer);
												}
												for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
													if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != name_) continue;
													send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
												}
												delete[]raw;
												world_->drop_new.erase(world_->drop_new.begin() + i_);
											}
											else {
												if (c_ < 200 and world_->drop_new[i_][1] >(200 - c_)) {
													int b_ = 200 - c_;
													world_->drop_new[i_][1] -= b_;
													if (modify_inventory(peer, world_->drop_new[i_][0], b_, false) == 0) {
														add_cctv(peer, "took", to_string(world_->drop_new[i_][1]) + " " + items[world_->drop_new[i_][0]].name);
														world_->drop_new.push_back({ {world_->drop_new[i_][0]}, {world_->drop_new[i_][1]} , {world_->total_drop_uid += 1} , {world_->drop_new[i_][3]} , {world_->drop_new[i_][4]} });
														gamepacket_t p;
														p.Insert("OnConsoleMessage");
														p.Insert("Collected `w" + to_string(200 - c_) + " " + items[world_->drop_new[i_][0]].ori_name + "``." + (items[world_->drop_new[i_][0]].rarity > 363 ? "" : " Rarity: `w" + to_string(items[world_->drop_new[i_][0]].rarity) + "``") + "");
														PlayerMoving data_{};
														data_.packetType = 14, data_.netID = -1, data_.plantingTree = world_->drop_new[i_][0], data_.x = world_->drop_new[i_][3], data_.y = world_->drop_new[i_][4];
														int32_t item = -1;
														float val = world_->drop_new[i_][1];
														BYTE* raw = packPlayerMoving(&data_);
														data_.plantingTree = world_->drop_new[i_][0];
														memcpy(raw + 8, &item, 4);
														memcpy(raw + 16, &val, 4);
														val = 0;
														data_.netID = pInfo(peer)->netID;
														data_.plantingTree = world_->drop_new[i_][2];
														data_.x = 0, data_.y = 0;
														BYTE* raw2 = packPlayerMoving(&data_);
														BYTE val2 = 0;
														memcpy(raw2 + 8, &item, 4);
														memcpy(raw2 + 16, &val, 4);
														memcpy(raw2 + 1, &val2, 1);
														world_->drop_new.erase(world_->drop_new.begin() + i_);
														for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
															if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != name_) continue;
															send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
															if (pInfo(currentPeer)->netID == pInfo(peer)->netID)
																p.CreatePacket(currentPeer);
															send_raw(currentPeer, 4, raw2, 56, ENET_PACKET_FLAG_RELIABLE);
														}
														delete[]raw, raw2;
													}
												}
											}
										}
									}
									else {
										if (pInfo(peer)->pickup_time + 2500 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
											pInfo(peer)->pickup_limits = 0;
											pInfo(peer)->pickup_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
										}
										else {
											if (pInfo(peer)->pickup_limits >= 14) enet_peer_disconnect_later(peer, 0);
											else pInfo(peer)->pickup_limits++;
										}
									}
								}
							}
						}
						break;
					}
					case 18: { //chat bubble kai raso
						move_(peer, p_);
						break;
					}
					case 21: { 
						/*
						pInfo(peer)->ping_replied = true;
						pInfo(peer)->ping_reply = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();*/
						break;
					}
					case 25:
					{
						// message_(event.packet) << "|" << pInfo(peer)->tankIDName << "|Detected cheat engine" << endl;
						/*
						gamepacket_t p, p2;
						p.Insert("OnAddNotification"), p.Insert("interface/atomic_button.rttex"), p.Insert("Warning from `4Admin:`` Cheat Engine not allowed to be running while you play!"), p.Insert("audio/hub_open.wav"), p.Insert(0), p.CreatePacket(peer);
						p2.Insert("OnConsoleMessage"), p2.Insert("`4OOPS:`` Warning from `4System``: Cheat Engine not allowed to be running while you play!"), p2.CreatePacket(peer);
						enet_peer_disconnect_later(peer, 0);*/
						break;
					}
					/*
			 case 23: //Kai zaidejas papunchina kita
			 {
				 if (pInfo(peer)->last_inf + 5000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
					 pInfo(peer)->last_inf = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
					 string name_ = pInfo(peer)->world;
					 vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
					 if (p != worlds.end()) {
						 World* world_ = &worlds[p - worlds.begin()];
						 bool can_cancel = true;
						 if (find(world_->active_jammers.begin(), world_->active_jammers.end(), 1276) != world_->active_jammers.end()) can_cancel = false;
						 if (can_cancel) {
							 if (pInfo(peer)->trading_with != -1 and p_->packetType != 0 and p_->packetType != 18) {
								 cancel_trade(peer, false, true);
								 break;
							 }
						 }
					 }
				 }
				 if (pInfo(peer)->last_inf + 5000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
					 pInfo(peer)->last_inf = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
					 bool inf = false;
					 for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
						 if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world or pInfo(peer)->netID == pInfo(currentPeer)->netID) continue;
						 if (abs(pInfo(currentPeer)->last_infected - p_->plantingTree) <= 3) {
							 if (has_playmod(pInfo(currentPeer), "Infected!") && not has_playmod(pInfo(peer), "Infected!") && inf == false) {
								 if (has_playmod(pInfo(peer), "Antidote!")) {
									 for (ENetPeer* currentPeer2 = server->peers; currentPeer2 < &server->peers[server->peerCount]; ++currentPeer2) {
										 if (currentPeer2->state != ENET_PEER_STATE_CONNECTED or currentPeer2->data == NULL or pInfo(peer)->world != pInfo(currentPeer2)->world) continue;
										 PlayerMoving data_{};
										 data_.packetType = 19, data_.punchX = 782, data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16;
										 int32_t to_netid = pInfo(peer)->netID;
										 BYTE* raw = packPlayerMoving(&data_);
										 raw[3] = 5;
										 memcpy(raw + 8, &to_netid, 4);
										 send_raw(currentPeer2, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										 delete[]raw;
									 }
								 }
								 else {
									 pInfo(currentPeer)->last_infected = 0;
									 inf = true;
									 gamepacket_t p, p2;
									 p.Insert("OnAddNotification"), p.Insert("interface/large/infected.rttex"), p.Insert("`4You were infected by " + pInfo(currentPeer)->tankIDName + "!"), p.CreatePacket(peer);
									 p2.Insert("OnConsoleMessage"), p2.Insert("You've been infected by the g-Virus. Punch others to infect them, too! Braiiiins... (`$Infected!`` mod added, `$1 mins`` left)"), p2.CreatePacket(peer);
									 PlayMods give_playmod{};
									 give_playmod.id = 28, give_playmod.time = time(nullptr) + 60;
									 pInfo(peer)->playmods.push_back(give_playmod);
									 update_clothes(peer);
								 }
							 }
							 if (has_playmod(pInfo(peer), "Infected!") && not has_playmod(pInfo(currentPeer), "Infected!") && inf == false) {
								 inf = true;
								 SendRespawn(peer, 0, true);
								 for (int i_ = 0; i_ < pInfo(peer)->playmods.size(); i_++) {
									 if (pInfo(peer)->playmods[i_].id == 28) {
										 pInfo(peer)->playmods[i_].time = 0;
										 break;
									 }
								 }
								 string name_ = pInfo(currentPeer)->world;
								 vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								 if (p != worlds.end()) {
									 World* world_ = &worlds[p - worlds.begin()];
									 WorldDrop drop_block_{};
									 drop_block_.id = rand() % 100 < 50 ? 4450 : 12370, drop_block_.count = pInfo(currentPeer)->hand == 9500 ? 2 : 1, drop_block_.uid = uint16_t(world_->drop_new.size()) + 1, drop_block_.x = pInfo(peer)->x, drop_block_.y = pInfo(peer)->y;
									 dropas_(world_, drop_block_);
								 }
							 }
						 }
					 }
				 }
				 break;
			 }*/
					default:
					{
						// message_(event.packet) << endl;
						break;
					}
					}
					free(p_);
					break;
				}
				default:
					break;
				}
				enet_packet_destroy(event.packet);
				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT:
			{
				if (f_saving_) break;
				if (peer->data != NULL) {
					if (not pInfo(peer)->spectate_person.empty()) pInfo(peer)->spectate_person = "";
					if (not Server_Security.log_player.empty() && Server_Security.log_player == pInfo(peer)->tankIDName) {
						cout << "LOGGING: " << pInfo(peer)->tankIDName << " | " << "disconnected" << "|" << endl;
					}
					if (pInfo(peer)->trading_with != -1) cancel_trade(peer, false);
					if (not pInfo(peer)->world.empty()) exit_(peer, true);
					if (pInfo(peer)->hider or pInfo(peer)->seeker) wipe_hidenseek(peer);
					pInfo(peer)->savedd = true;
					save_player(pInfo(peer), true);
					peer->data = NULL;
					delete peer->data;
				}
				break;
			}
			default:
				cout << pInfo(peer)->tankIDName << " weird event type... " << event.type << " ??" << endl;
				break;
			}
		}
	}
	/*
	enet_host_destroy(server);
	enet_deinitialize();*/
	return 0;
}
