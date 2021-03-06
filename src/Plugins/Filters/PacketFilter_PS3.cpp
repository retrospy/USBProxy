/*
 * This file is part of USBProxy.
 */

#include "HexString.h"
#include "PacketFilter_PS3.h"

PacketFilter_PS3::PacketFilter_PS3(ConfigParser *cfg) {
	file  = (FILE *) cfg->get_pointer("PacketFilter_PS3::file");
}

void PacketFilter_PS3::filter_packet(Packet* packet) {
	if (packet->wLength == 49 && packet->bEndpoint == 0x81) {
		
		// digital buttons
		for (int j = 2; j < 5; ++j)
			for (int i = 0; i < 8; ++i)
				fprintf(file, "%d", (packet->data[j] & (1 << i)) != 0);

		// sticks
		for (int j = 6; j < 10; ++j)
			for (int i = 0; i < 8; ++i)
				fprintf(file, "%d", (packet->data[j] & (1 << i)) != 0);
 
		// analog buttons
		for (int j = 14; j < 26; ++j)
			for (int i = 0; i < 8; ++i)
				fprintf(file, "%d", (packet->data[j] & (1 << i)) != 0);

		fprintf(file, "\n");
	}
	else if (packet->wLength == 64 && packet->bEndpoint == 0x84)
	{
		// digital buttons
		
		// share/select
		fprintf(file, "%d", (packet->data[6] & 0b00010000) == 0 ? 0 : 1);
		// lstick
		fprintf(file, "%d", (packet->data[6] & 0b01000000) == 0 ? 0 : 1);
		// rstick
		fprintf(file, "%d", (packet->data[6] & 0b10000000) == 0 ? 0 : 1);
		// options/start
		fprintf(file, "%d", (packet->data[6] & 0b00100000) == 0 ? 0 : 1);

		//up
		fprintf(file, "%d", ((packet->data[5] & 0b00001111) == 0b0000 
					|| (packet->data[5] & 0b00001111) == 0b0001 
					|| (packet->data[5] & 0b00001111) == 0b0111) ? 1 : 0);
		//right
		fprintf(file, "%d", ((packet->data[5] & 0b00001111) == 0b0001 
					|| (packet->data[5] & 0b00001111) == 0b0010 
					|| (packet->data[5] & 0b00001111) == 0b0011) ? 1 : 0);
		//down
		fprintf(file, "%d", ((packet->data[5] & 0b00001111) == 0b0011 
					|| (packet->data[5] & 0b00001111) == 0b0100 
					|| (packet->data[5] & 0b00001111) == 0b0101) ? 1 : 0);
		//left
		fprintf(file, "%d", ((packet->data[5] & 0b00001111) == 0b0110 
					|| (packet->data[5] & 0b00001111) == 0b0111 
					|| (packet->data[5] & 0b00001111) == 0b0101) ? 1 : 0);

		// l2
		fprintf(file, "%d", (packet->data[6] & 0b00000100) == 0 ? 0 : 1);
		// r2
		fprintf(file, "%d", (packet->data[6] & 0b00001000) == 0 ? 0 : 1);
		// l1
		fprintf(file, "%d", (packet->data[6] & 0b00000001) == 0 ? 0 : 1);
		// r1
		fprintf(file, "%d", (packet->data[6] & 0b00000010) == 0 ? 0 : 1);

		// triangle
		fprintf(file, "%d", (packet->data[5] & 0b10000000) == 0 ? 0 : 1);
		// circle
		fprintf(file, "%d", (packet->data[5] & 0b01000000) == 0 ? 0 : 1);
		// x
		fprintf(file, "%d", (packet->data[5] & 0b00100000) == 0 ? 0 : 1);
		// square
		fprintf(file, "%d", (packet->data[5] & 0b00010000) == 0 ? 0 : 1);

		// ps
		fprintf(file, "%d", (packet->data[7] & 0b00000001) == 0 ? 0 : 1);

		// padding
		for(int i = 0; i < 7; ++i)
			fprintf(file, "0");
		
		// sticks
		for (int j = 1; j < 5; ++j)
			for (int i = 0; i < 8; ++i)
				fprintf(file, "%d", (packet->data[j] & (1 << i)) != 0);
		
		// fake 4 analog buttons
		for (int j = 0; j < 4; ++j)
			for (int i = 0; i < 8; ++i)
				fprintf(file, "0");
 
		// triggers
		for (int j = 8; j < 10; ++j)
			for (int i = 0; i < 8; ++i)
				fprintf(file, "%d", (packet->data[j] & (1 << i)) != 0);
		
		// fake 6 analog buttons
		for (int j = 0; j < 6; ++j)
			for (int i = 0; i < 8; ++i)
				fprintf(file, "0");

		printf("\n");

	}
}
void PacketFilter_PS3::filter_setup_packet(SetupPacket* packet,bool direction) {
	//if (packet->ctrl_req.wLength && packet->data) {
	//	char* hex_setup=hex_string(&(packet->ctrl_req),sizeof(packet->ctrl_req));
	//	char* hex_data=hex_string((void*)(packet->data),packet->ctrl_req.wLength);
	//	fprintf(file,"[%s]: %s\n",hex_setup,hex_data);
	//	free(hex_data);
	//	free(hex_setup);
	//} else {
	//	char* hex_setup=hex_string(&(packet->ctrl_req),sizeof(packet->ctrl_req));
	//	fprintf(file,"[%s]\n",hex_setup);
	//	free(hex_setup);
	//}
}

static PacketFilter_PS3 *proxy;

extern "C" {
	int plugin_type = PLUGIN_FILTER;
	
	PacketFilter * get_plugin(ConfigParser *cfg) {
		proxy = new PacketFilter_PS3(cfg);
		return (PacketFilter *) proxy;
	}
	
	void destroy_plugin() {
		delete proxy;
	}
}
