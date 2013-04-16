/*
 * The Shadow Simulator
 *
 * Copyright (c) 2010-2011 Rob Jansen <jansen@cs.umn.edu>
 * Copyright (c) 2011-2013
 * To the extent that a federal employee is an author of a portion
 * of this software or a derivative work thereof, no copyright is
 * claimed by the United States Government, as represented by the
 * Secretary of the Navy ("GOVERNMENT") under Title 17, U.S. Code.
 * All Other Rights Reserved.
 *
 * Permission to use, copy, and modify this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * GOVERNMENT ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS" CONDITION
 * AND DISCLAIMS ANY LIABILITY OF ANY KIND FOR ANY DAMAGES WHATSOEVER
 * RESULTING FROM THE USE OF THIS SOFTWARE.
 */

#include "shadow.h"

struct _PacketArrivedEvent {
	Event super;
	Packet* packet;
	MAGIC_DECLARE;
};

EventFunctionTable packetarrived_functions = {
	(EventRunFunc) packetarrived_run,
	(EventFreeFunc) packetarrived_free,
	MAGIC_VALUE
};

PacketArrivedEvent* packetarrived_new(Packet* packet) {
	PacketArrivedEvent* event = g_new0(PacketArrivedEvent, 1);
	MAGIC_INIT(event);

	shadowevent_init(&(event->super), &packetarrived_functions);

	packet_ref(packet);
	event->packet = packet;

	return event;
}

void packetarrived_run(PacketArrivedEvent* event, Node* node) {
	MAGIC_ASSERT(event);

	debug("event started");

	in_addr_t ip = packet_getDestinationIP(event->packet);
	NetworkInterface* interface = node_lookupInterface(node, ip);
	networkinterface_packetArrived(interface, event->packet);

	debug("event finished");
}

void packetarrived_free(PacketArrivedEvent* event) {
	MAGIC_ASSERT(event);

	packet_unref(event->packet);

	MAGIC_CLEAR(event);
	g_free(event);
}
