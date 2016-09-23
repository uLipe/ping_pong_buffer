/**
 * @brief simple ping-pong buffer implementation
 */

#ifndef PING_PONG_BUFFER_H_
#define PING_PONG_BUFFER_H_

#include <stdbool.h>

/* ping pong buffer control structure */
typedef struct {
	unsigned char *buffer_data;
	unsigned char ping;
	unsigned char pong;
	int buffer_size;
	int put_index;
	int get_index;
	bool full_signal;
}ppbuf_t;

/**
 * @brief insert on active buffer
 */
int ppbuf_insert_active(ppbuf_t *p, void *data, int size);

/**
 * @brief remove from inactive buffer
 */
int ppbuf_remove_inactive(ppbuf_t *p, void *data, int size);

/**
 * @brief USE WITH DMA ONLY! get the current active buffer address
 */
unsigned char *ppbuf_dma_get_active_addr(ppbuf_t* p, int *size);

/**
 * @brief USE WITH DMA ONLY! get the current inactive buffer address
 */
unsigned char *ppbuf_dma_get_inactive_addr(ppbuf_t* p, int *size);


/**
 * @brief USE WITH DMA ONLY! force full signaling to next buffer become available
 */
int ppbuf_dma_force_swap(ppbuf_t* p);

/**
 * @brief get full signal
 */
bool ppbuf_get_full_signal(ppbuf_t *p, bool consume);


/* instantiate a fully initialized and static ping-pong buffer */
#define PPBUF_DECLARE(name,size)								\
		unsigned char ppbuf_mem_##name[size * 2] = {0};			\
		ppbuf_t name = {										\
			.buffer_data = &ppbuf_mem_##name[0],				\
			.ping = 1,											\
			.pong = 0,											\
			.buffer_size = size,								\
			.put_index = 0,										\
			.get_index = 0,										\
			.full_signal = false								\
		}

#endif /* PING_PONG_BUFFER_H_ */
