/*
 * Copyright (C) 2017 Tieto Poland Sp. z o. o.
 */

#ifndef HK_QUEUE_H_
#define HK_QUEUE_H_
#define QUOEUE_MAX_LENGTH 64

#include "../hk_api.h"
#include <pthread.h>

typedef struct queue_item queue_item_t;

typedef struct
{
    queue_item_t *tail, *head;
    size_t current_length;
    size_t max_length;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pthread_mutex_t mutex_c; // mutex for condition variable
} queue_t;

HK_NOT_NULL_ARGS(1)
void queue_init(queue_t *queue, size_t max_length);
HK_NOT_NULL_ARGS(1)
hk_ret_t queue_push(queue_t *queue, void *data, size_t data_size);
HK_NOT_NULL_ARGS(1)
hk_ret_t queue_pop(queue_t *queue, void *data, size_t *data_size);
HK_NOT_NULL_ARGS(1)
bool queue_is_empty(queue_t *queue);
hk_ret_t queue_test();
HK_NOT_NULL_ARGS(1)
size_t queue_pop_data_size(queue_t *queue);
HK_NOT_NULL_ARGS(1)
void queue_wait_until_empty(queue_t *queue);
queue_t *queue_new(void);

#endif /* HK_QUEUE_H_ */
