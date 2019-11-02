/*
 * This file is part of USBProxy.
 */

#ifndef USBPROXY_DEVICEPROXY_DOT11_H
#define USBPROXY_DEVICEPROXY_DOT11_H

#include "DeviceProxy.h"
#include "Dot11_Interface.h"

extern "C" {

class Configuration;

class DeviceProxy_dot11 : public DeviceProxy {
private:
	bool p_is_connected;
	struct usb_device_descriptor dot11_device_descriptor;
	struct usb_config_descriptor dot11_config_descriptor;
	struct usb_interface_descriptor dot11_interface_descriptor;
	struct usb_endpoint_descriptor dot11_eps[2];
	struct usb_string;
	std::string interface;
	/* Are we in monitor mode? */
	bool monitor;

public:
	DeviceProxy_dot11(ConfigParser *cfg);
	~DeviceProxy_dot11();

	int connect(int timeout=250);
	void disconnect();
	void reset();
	bool is_connected();

	bool is_highspeed();

	//return -1 to stall
	int control_request(const usb_ctrlrequest *setup_packet, int *nbytes, __u8* dataptr,int timeout=500);
	int vendor_request(const usb_ctrlrequest *setup_packet, int *nbytes, __u8* dataptr,int timeout=500);
	
	void send_data(__u8 endpoint,__u8 attributes,__u16 maxPacketSize,__u8* dataptr,int length);
	bool send_wait_complete(__u8 endpoint,int timeout=500) {return true;}
	void receive_data(__u8 endpoint,__u8 attributes,__u16 maxPacketSize,__u8** dataptr, int* length,int timeout=500);
	void setConfig(Configuration* fs_cfg,Configuration* hs_cfg,bool hs);
	char* toString() {return (char *) "Lookback device";}

	void claim_interface(__u8 interface);
	void release_interface(__u8 interface);

	void set_identity(const char* manufacturer, const char* product, const char* serialNumber);
	bool skip_action(const char* action);

	__u8 get_address();
};
} /* extern C */
#endif /* USBPROXY_DEVICEPROXY_DOT11_H */
