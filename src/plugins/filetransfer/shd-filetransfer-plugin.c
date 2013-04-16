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

#include <string.h>

#include "shd-filetransfer.h"

/* create a new node using this plug-in */
static void filetransferplugin_new(int argc, char* argv[]) {
	filetransfer_new(argc, argv);
}

static void filetransferplugin_free() {
	filetransfer_free();
}

static void filetransferplugin_activate() {
	filetransfer_activate();
}

/* my global structure to hold all variable, node-specific application state */
FileTransfer filetransferplugin_globalData;

/* shadow calls this function for a one-time initialization
 *
 * !WARNING! dont malloc() (or g_new()) anything until filetransferplugin_new
 * unless that memory region is registered with shadow by giving a pointer to it.
 * its better to register as little as possible because everything that is
 * registered is copied on every shadow-to-plugin context switch.
 */
void __shadow_plugin_init__(ShadowFunctionTable* shadowlibFuncs) {
	/* clear our memory before initializing */
	memset(&filetransferplugin_globalData, 0, sizeof(FileTransfer));

	/* save the shadow functions we will use since it will be the same for all nodes */
	filetransferplugin_globalData.shadowlib = shadowlibFuncs;

	/* give the filetransfer code a pointer to this global struct. this allows
	 * access to our FileTransfer struct without needing to 'extern' it.
	 */
	filetransfer_init(&filetransferplugin_globalData);

	/*
	 * tell shadow which of our functions it can use to notify our plugin,
	 * and allow it to track our state for each instance of this plugin
	 */
	gboolean success = shadowlibFuncs->registerPlugin(&filetransferplugin_new, &filetransferplugin_free, &filetransferplugin_activate);
	if(success) {
		shadowlibFuncs->log(G_LOG_LEVEL_MESSAGE, __FUNCTION__, "successfully registered filetransfer plug-in state");
	} else {
		shadowlibFuncs->log(G_LOG_LEVEL_INFO, __FUNCTION__, "error registering filetransfer plug-in state");
	}
}
