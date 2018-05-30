/*
 * cmd_device.c
 *
 *  Created on: May 28, 2018
 *      Author: adam
 */
#include "esp_console.h"
#include "../dispatcher/hk_dispatcher.h"
#include "../utility/hk_queue.h"
#include "../utility/hk_byte_stream.h"
#include "../device/device.h"

extern hk_dispatcher_acc_map_t *g_acc_map;
extern void hk_console_printf(const char *text, ...);

static int list_acc(int argc, char** argv)
{
	hk_dispatcher_transaction(g_acc_map, HK_DISPATCHER_GET_ACCESSORIES, NULL, 0, 0);
    return 0;
}

static int read_chr(int argc, char** argv)
{


	if (argc > 1)
		{
			hk_dispatcher_transaction(g_acc_map, HK_DISPATCHER_GET_CHARACTERISTICS, (byte_t *)argv[1], strlen(argv[1]), 0);
		}

    return 0;
}

static int write_chr(int argc, char** argv)
{
	char buf[128];

	if (argc > 2)
		{
			if (strlen(argv[1]) + strlen(argv[2]) > sizeof(buf) - 1)
				return 0;
			strcpy(buf, argv[1]);
			strcpy(buf + strlen(argv[1]), " ");
			strcpy(buf + strlen(argv[1]) + 1, argv[2]);
			hk_console_printf("BUF: [%s]", buf);
			hk_dispatcher_transaction(g_acc_map, HK_DISPATCHER_PUT_CHARACTERISTICS, (byte_t *)buf, strlen(buf), 0);
		}
    return 0;
}

static void register_read_chr()
{
    const esp_console_cmd_t cmd = {
        .command = "chr",
        .help = "Read dispatcher characteristics \nex. chr id=1.11, chr id=1.3",
        .hint = NULL,
        .func = &read_chr,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

static void register_write_chr()
{
    const esp_console_cmd_t cmd = {
        .command = "put",
        .help = "Write chr",
        .hint = NULL,
        .func = &write_chr,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

static void register_list_acc()
{
    const esp_console_cmd_t cmd = {
        .command = "acc",
        .help = "List all dispatcher registered accessory",
        .hint = NULL,
        .func = &list_acc,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

void register_device()
{
	register_list_acc();
	register_read_chr();
	register_write_chr();
}
