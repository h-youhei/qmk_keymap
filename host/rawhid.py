#!/usr/bin/env python3

import sys
import hid
import logging

vendor_id = 0x3297
product_id = 0x4974

usage_page = 0xFF60
usage = 0x61

def get_raw_hid_interface():
	device_interfaces = hid.enumerate(vendor_id, product_id)
	raw_hid_interfaces = [i for i in device_interfaces if i['usage_page'] == usage_page and i['usage'] == usage]

	if len(raw_hid_interfaces) == 0:
		return None

	interface = hid.device()
	interface.open_path(raw_hid_interfaces[0]['path'])

	logging.info("Manufacturer: %s" % interface.get_manufacturer_string())
	logging.info("Product: %s" % interface.get_product_string())

	return interface

def send_raw_packet(data):
	interface = get_raw_hid_interface()

	if interface is None:
		Print("No Device found")
		sys.exit(1)

	request_packet = [0x00] * 33 # First byte is Report ID
	request_packet[1:len(data) + 1] = data

	logging.info("Request:")
	logging.info(request_packet)

	try:
		interface.write(request_packet)
	finally:
		interface.close()

id_im_off = 0x01
id_im_on = 0x02

if __name__ == '__main__':
	for i, arg in enumerate(sys.argv):
		if arg == 'im_off':
			send_raw_packet([id_im_off])
		elif arg == 'im_on':
			send_raw_packet([id_im_on])
