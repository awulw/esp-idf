/*
 * Copyright (C) 2017 Tieto Poland Sp. z o. o.
 */
#include "../utility/hk_queue.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>

struct queue_item
{
    struct queue_item *previous, *next;
    void *data;
    size_t data_size;
};

void queue_init(queue_t *queue, size_t max_length)
{
    queue->max_length = max_length;
    queue->current_length = 0;
    queue->tail = NULL;
    queue->head = NULL;
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_mutex_init(&queue->mutex_c, NULL);
    pthread_cond_init(&queue->cond, NULL);
}

queue_t *queue_new(void)
{
    queue_t *queue = malloc(sizeof(queue_t));
    if (queue == NULL)
        return NULL;
    queue_init(queue, QUOEUE_MAX_LENGTH);
    return queue;
}

hk_ret_t queue_push(queue_t *queue, void *data, size_t data_size)
{
    pthread_mutex_lock(&queue->mutex);
    if (queue->current_length > queue->max_length)
    {
        pthread_mutex_unlock(&queue->mutex);
        return HK_RET_RESOURCES_EXHAUSTED;
    }
    if (data_size < 1)
    {
        pthread_mutex_unlock(&queue->mutex);
        return HK_RET_INVALID_STATE;
    }
    queue_item_t *new_item = calloc(1, sizeof(queue_item_t));
    new_item->data = malloc(data_size);
    if (new_item == NULL)
    {
        pthread_mutex_unlock(&queue->mutex);
        return HK_RET_RESOURCES_EXHAUSTED;
    }
    new_item->next = NULL;
    new_item->data_size = data_size;
    memcpy(new_item->data, data, data_size);
    if (queue->current_length == 0)
    {
        queue->tail = new_item;
    }
    else // The queue is not empty
    {
        queue->head->next = new_item;
        new_item->previous = queue->head;
    }
    queue->head = new_item;
    queue->current_length++;
    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
    return HK_RET_SUCCESS;
}
hk_ret_t queue_pop(queue_t *queue, void *data, size_t *data_size)
{
    pthread_mutex_lock(&queue->mutex);
    queue_item_t *old_item = queue->tail;
    if (old_item == NULL)
    {
        pthread_mutex_unlock(&queue->mutex);
        return HK_RET_FAIL;
    }
    if (queue->current_length > 0)
    {
        queue->current_length--;
        if (data != NULL)
        {
            memcpy(data, old_item->data, old_item->data_size);
        }
        if (old_item->next != NULL)
        {
            queue->tail = old_item->next;
        }
        else
        { // Last item
            queue->tail = NULL;
            queue->head = NULL;
        }
        if (data_size != NULL)
        {
            *data_size = old_item->data_size;
        }
        free(old_item->data);
        free(old_item);
        pthread_mutex_unlock(&queue->mutex);
        return HK_RET_SUCCESS;
    }
    pthread_mutex_unlock(&queue->mutex);
    return HK_RET_FAIL;
}
size_t queue_pop_data_size(queue_t *queue)
{
    if (queue == NULL)
    {
        return 0;
    }
    if (queue->current_length > 0)
    {
        return queue->tail->data_size;
    }
    return 0;
}

bool queue_is_empty(queue_t *queue)
{
    if (queue->current_length == 0)
        return true;
    return false;
}

void queue_wait_until_empty(queue_t *queue)
{
    if (queue_is_empty(queue))
        pthread_cond_wait(&queue->cond, &queue->mutex_c);
}
