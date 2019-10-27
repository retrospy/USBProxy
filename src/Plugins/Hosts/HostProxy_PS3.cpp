/*
 * This file is part of USBProxy.
 */

#include "HostProxy_PS3.h"

#include <cstring>
#include <iostream>

#include <unistd.h>
#include <poll.h>
#include <math.h>

#include "GadgetFS_helpers.h"
#include "errno.h"
#include "TRACE.h"
#include "HexString.h"

#include "DeviceQualifier.h"
#include "Configuration.h"
#include "Interface.h"
#include "Endpoint.h"


HostProxy_PS3::HostProxy_PS3(ConfigParser *cfg)
	: HostProxy_GadgetFS(cfg)
{

}

HostProxy_PS3::~HostProxy_PS3() {
}

static uint8_t fake_ds3_descriptor[104] = {
	0x00, 0x00, 0x00, 0x00,

	0x09, 0x02, 0x29, 0x00, 0x01, 0x01, 0x00, 0x80,
	0xFA, 0x09, 0x04, 0x00, 0x00, 0x02, 0x03, 0x00,
	0x00, 0x00, 0x09, 0x21, 0x11, 0x01, 0x00, 0x01,
	0x22, 0x94, 0x00, 0x07, 0x05, 0x81, 0x03, 0x40,
	0x00, 0x01, 0x07, 0x05, 0x02, 0x03, 0x40, 0x00,
	0x01,

	// Duplicate of 1.0 descriptor
	0x09, 0x02, 0x29, 0x00, 0x01, 0x01, 0x00, 0x80,
	0xFA, 0x09, 0x04, 0x00, 0x00, 0x02, 0x03, 0x00,
	0x00, 0x00, 0x09, 0x21, 0x11, 0x01, 0x00, 0x01,
	0x22, 0x94, 0x00, 0x07, 0x05, 0x81, 0x03, 0x40,
	0x00, 0x01, 0x07, 0x05, 0x02, 0x03, 0x40, 0x00,
	0x01,

	// Real 2.0 descriptopr
	/*0x09, 0x02, 0x29, 0x00, 0x01, 0x01, 0x00, 0x80,
	0xfa, 0x09, 0x04, 0x00, 0x00, 0x02, 0x03, 0x00,
	0x00, 0x00, 0x09, 0x21, 0x11, 0x01, 0x00, 0x01,
	0x22, 0x94, 0x00, 0x07, 0x05, 0x02, 0x03, 0x40,
	0x00, 0x04, 0x07, 0x05, 0x81, 0x03, 0x40, 0x00,
	0x04,*/

	0x12, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00,
	0x40, 0x4C, 0x05, 0x68, 0x02, 0x00, 0x01, 0x01,
	0x02, 0x00, 0x01
};


void HostProxy_PS3::handle_USB_REQ_SET_CONFIGURATION()
{
	return;
}

int HostProxy_GadgetFS::send_descriptor(int p_device_file, char* descriptor, int descriptorLength)
{
	return write(p_device_file, fake_ds3_descriptor, 104);
}

static HostProxy_PS3 *proxy;

extern "C" {
	HostProxy * get_hostproxy_plugin(ConfigParser *cfg) {
		proxy = new HostProxy_PS3(cfg);
		return (HostProxy *) proxy;
	}
	
	void destroy_plugin() {
		delete proxy;
	}
}
