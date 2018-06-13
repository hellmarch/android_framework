
// binder_wrapper.h

#ifndef _BINDER_WRAPPHER_H_
#define _BINDER_WRAPPHER_H_

#include "binder.h"
#define INTERFACE_TRANSACTION	B_PACK_CHARS('_', 'N', 'T', 'F')

static
int add_service(struct binder_state *bs, const char *name, void *ptr) {
	unsigned status;
	unsigned iodata[512 / 4];
	struct binder_io msg, reply;

	bio_init(&msg, iodata, sizeof(iodata), 4);
	bio_put_uint32(&msg, 0);
	bio_put_string16_x(&msg, SVC_MGR_NAME);
	bio_put_string16_x(&msg, name);
	bio_put_obj(&msg, ptr);

	if (binder_call(bs, &msg, &reply, BINDER_SERVICE_MANAGER,
				SVC_MGR_ADD_SERVICE))
		return -1;

	status = bio_get_uint32(&reply);
	binder_done(bs, &msg, &reply);

	return status;
}

static
uint32_t get_service(struct binder_state *bs, const char *name) {
	unsigned iodata[512 / 4];
	struct binder_io msg, reply;

	bio_init(&msg, iodata, sizeof(iodata), 4);
	bio_put_uint32(&msg, 0);
	bio_put_string16_x(&msg, SVC_MGR_NAME);
	bio_put_string16_x(&msg, name);

	if (binder_call(bs, &msg, &reply, BINDER_SERVICE_MANAGER,
				SVC_MGR_GET_SERVICE))
		return 0;

	uint32_t handle = bio_get_ref(&reply);
	if (handle)
		binder_acquire(bs, handle);
	binder_done(bs, &msg, &reply);

	return handle;
}

#endif


