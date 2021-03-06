/*
 * This file is part of USBProxy.
 */

#include "PacketFilter_PSClassic.h"

PacketFilter_PSClassic::PacketFilter_PSClassic(ConfigParser *cfg) {
	file  = (FILE *) cfg->get_pointer("PacketFilter_PSClassic::file");
}

void PacketFilter_PSClassic::filter_packet(Packet* packet) {
	if (packet->wLength >= 2) {
		printf("%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d\n",
			(packet->data[0] & 0b10000000) != 0,
			(packet->data[0] & 0b01000000) != 0,
			(packet->data[0] & 0b00100000) != 0,
			(packet->data[0] & 0b00010000) != 0,
			(packet->data[0] & 0b00001000) != 0,
			(packet->data[0] & 0b00000100) != 0,
			(packet->data[0] & 0b00000010) != 0,
			(packet->data[0] & 0b00000001) != 0,
			0,
			0,
			(packet->data[1] & 0b00100000) != 0,
			(packet->data[1] & 0b00010000) == 0 && (packet->data[1] & 0b00100000) == 0,
			(packet->data[1] & 0b00001000) != 0,
			(packet->data[1] & 0b00000100) == 0 && (packet->data[1] & 0b00001000) == 0,
			(packet->data[1] & 0b00000010) != 0,
			(packet->data[1] & 0b00000001) != 0);
	}
}
void PacketFilter_PSClassic::filter_setup_packet(SetupPacket* packet,bool direction) {
/*	if (packet->ctrl_req.wLength && packet->data) {
		char* hex_setup=hex_string(&(packet->ctrl_req),sizeof(packet->ctrl_req));
		char* hex_data=hex_string((void*)(packet->data),packet->ctrl_req.wLength);
		fprintf(file,"[%s]: %s\n",hex_setup,hex_data);
		free(hex_data);
		free(hex_setup);
	} else {
		char* hex_setup=hex_string(&(packet->ctrl_req),sizeof(packet->ctrl_req));
		fprintf(file,"[%s]\n",hex_setup);
		free(hex_setup);
	}*/
}

static PacketFilter_PSClassic *proxy;

extern "C" {
	int plugin_type = PLUGIN_FILTER;
	
	PacketFilter * get_plugin(ConfigParser *cfg) {
		proxy = new PacketFilter_PSClassic(cfg);
		return (PacketFilter *) proxy;
	}
	
	void destroy_plugin() {
		delete proxy;
	}
}
