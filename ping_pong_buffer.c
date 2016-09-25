/**
 * @brief simple ping pong buffer implementation
 */

#include <string.h>
#include "ping_pong_buffer.h"


int ppbuf_insert_active(ppbuf_t *p, void *data, int size){
	int ret = 0;
	unsigned char *ptr;

	if(p == NULL || data == NULL || size == 0) {
		/* check your parameters */
		ret = -1;
	} else {
		if(size > (p->buffer_size - p->put_index)) {
			/* not enough room for new samples */
			ret = -1;
		} else {
			/* take the current position */
			int mem_position = ((p->ping) * p->buffer_size) + p->put_index;
			ptr = (unsigned char *)p->buffer_data;

			/* copy the contents */
			memcpy(&ptr[mem_position], data, size);

			/* update put index */
			p->put_index += size;
			p->full_signal = (p->put_index >= p->buffer_size?true:false);

			/* swap will only generated when ppbuf_get_full_signal is called */
			ret = 0;
		}
	}
	return(ret);
}

int ppbuf_remove_inactive(ppbuf_t *p, void *data, int size){
	int ret = 0;
	unsigned char *ptr;

	if(p == NULL || data == NULL || size == 0) {
		/* check your parameters */
		ret = -1;
	} else {
		if(size > (p->buffer_size - p->get_index)) {
			/* not enough data in sample buffer */
			ret = -1;
		} else {
			/* take the current position */
			int mem_position = ((p->pong) * p->buffer_size) + p->get_index;
			ptr = (unsigned char *)p->buffer_data;

			/* copy the contents */
			memcpy(data,&ptr[mem_position], size);

			/* update put index */
			p->get_index += size;

			/* when buffer is empty we are not able to extract anymore data */
			ret = 0;
		}
	}
	return(ret);


}

unsigned char *ppbuf_dma_get_active_addr(ppbuf_t* p, int *size){
	if(p == NULL || size == NULL) {
		/* no valid parameters return a invalid pointer */
		return(NULL);
	} else {
		/* insertion buffer is always the pong */
		return((unsigned char *)&p->buffer_data[p->pong * p->buffer_size]);
	}
}


unsigned char *ppbuf_dma_get_inactive_addr(ppbuf_t* p, int *size){
	if(p == NULL || size == NULL) {
		/* no valid parameters return a invalid pointer */
		return(NULL);
	} else {
		/* insertion buffer is always the pong */
		return((unsigned char *)&p->buffer_data[p->ping * p->buffer_size]);
	}
}

int ppbuf_dma_force_swap(ppbuf_t* p) {
	int ret = 0;
	/* this function is asynchronous, so it must be used with
	 * caution or a buffer corrpution will occur
	 */


	if(p == NULL) {
		ret = -1;
	} else {
		/* for safety swaps ocurrs only with a presence of a previous full signal */
		if(p->full_signal != false) {

			p->full_signal = false;

			/* swap the buffer switches */
			p->ping = p->ping ^ p->pong;
			p->pong = p->pong ^ p->ping;
			p->ping = p->ping ^ p->pong;

			p->get_index = 0;
			p->put_index = 0;
		}

	}
	return(ret);
}

bool ppbuf_get_full_signal(ppbuf_t *p, bool consume) {
	/* take the last signaled full occurrence */
	bool ret = (p != NULL ? p->full_signal : false);

	if((consume != false) && (p != NULL) && (ret != false)) {
		p->full_signal = false;

		/* swap the buffer switches */
		p->ping = p->ping ^ p->pong;
		p->pong = p->pong ^ p->ping;
		p->ping = p->ping ^ p->pong;

		/* resets the buffer position */
		p->get_index = 0;
		p->put_index = 0;
	}

	return(ret);
}
