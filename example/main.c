/**
 * @brief main application to test ping-pong-buffer
 */

#include <stdio.h>
#include "ping_pong_buffer.h"

/* creates a 32 byte ping-pong-buffer*/
PPBUF_DECLARE(test,32);


int main(void)
{
	unsigned char data = 0;
	unsigned char buf  = 0;

	/* fill the active buffer with some raw value */
	while(ppbuf_get_full_signal(&test,false) != true) {
		ppbuf_insert_active(&test, &data, sizeof(data));
		printf("data inserted on ping buffer: %hu", data);
		/* insert odd values in active buffer */
		data += 2;

		/* gets the data on the pong buffer */
		ppbuf_remove_inactive(&test, &buf, sizeof(buf));
		printf("		data got from pong buffer: %hu \n\r", buf);
	}

	/* bufffer full lets swap it */
	ppbuf_get_full_signal(&test,true);

	printf("SWAPPING BUFFERS! \n\r");


	data = 1;

	/* fill the active buffer with some raw value */
	while(ppbuf_get_full_signal(&test,false) != true) {
		ppbuf_insert_active(&test, &data, sizeof(data));
		printf("data inserted on ping buffer: %hu", data);
		/* insert even values in active buffer */
		data += 2;

		/* gets the data on the pong buffer */
		ppbuf_remove_inactive(&test, &buf, sizeof(buf));
		printf("		data got from pong buffer: %hu \n\r", buf);
	}


	return 0;
}
