/*
 * This file is part of USBProxy.
 */

#ifndef USBPROXY_HOSTPROXY_PS3_H
#define USBPROXY_HOSTPROXY_PS3_H

extern "C" {
#include <linux/usb/gadgetfs.h>
}
#include "HostProxy.h"
#include <pthread.h>
#include <unistd.h>
#include "TRACE.h"
#include "errno.h"
#include "aio.h"
#include <linux/usb/ch9.h>

class HostProxy_PS3: public HostProxy {
private:
	bool p_is_connected;
	int p_device_file;
	const char* device_filename;
	struct aiocb* p_epin_async[16];
	struct aiocb* p_epout_async[16];
	bool p_epin_active[16];

	char* descriptor;
	int descriptorLength;

	int reconnect();
	int generate_descriptor(Device* device);

	usb_ctrlrequest lastControl;

protected:

	virtual void handle_USB_REQ_SET_CONFIGURATION();

public:
	HostProxy_PS3(ConfigParser *cfg);
	virtual ~HostProxy_PS3();

	int connect(Device* device,int timeout=250);
	void disconnect();
	void reset();
	bool is_connected();

	//return 0 in usb_ctrlrequest->brequest if there is no request
	int control_request(usb_ctrlrequest *setup_packet, int *nbytes, __u8** dataptr,int timeout=500);
	void send_data(__u8 endpoint,__u8 attributes,__u16 maxPacketSize,__u8* dataptr,int length);
	bool send_wait_complete(__u8 endpoint,int timeout=500);
	void receive_data(__u8 endpoint,__u8 attributes,__u16 maxPacketSize,__u8** dataptr, int* length,int timeout=500);
	void control_ack();
	void stall_ep(__u8 endpoint);
	void setConfig(Configuration* fs_cfg,Configuration* hs_cfg,bool hs);
};

#endif /* USBPROXY_HOSTPROXY_PS3_H */