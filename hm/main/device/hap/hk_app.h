#ifndef HK_APP_H
#define HK_APP_H

#include "../hk_api.h"
#include "../hap/hk_characteristic.h"
#include "../hap/hk_accessory.h"

#define HK_VERSION "0.0.1"

/** Application characteristic read callback. */
typedef hk_status_code_t (*hk_app_chr_read_callback_t)(const hk_chr_base_t *, hk_value_t *);
/** Application characteristic write callback. */
typedef hk_status_code_t (*hk_app_chr_write_callback_t)(const hk_chr_base_t *, const hk_value_t *);

typedef struct
{
    hk_chr_base_t base; ///< Characteristic base type.
    // TODO(SB): add persistent storage keys and other needed fields

    void *user_driver_context;
    void *user_driver;
    void *user_device;
} hk_app_chr_t;

hk_instance_id_t hk_app_get_next_iid(void);
hk_app_chr_t *hk_app_chr_new(void);
hk_acc_base_t *hk_app_create_acc(hk_accessory_id_t aid);
void hk_app_chr_delete(hk_app_chr_t *chr);
void hk_app_srv_delete(hk_srv_base_t *srv);
hk_ret_t hk_app_add_srv(hk_acc_base_t *acc, hk_srv_base_t *srv);
hk_ret_t hk_app_add_chr(hk_srv_base_t *srv, hk_app_chr_t *chr);

typedef struct
{
    int data; // TODO(DJ): Replace with core/app ctx handles once Core API is finalized
} hk_application_data_t;



/** Application characteristic base structure. */


/** Instance ID generator for application accessory.

    @return Instance ID of type \ref hk_instance_id_t. */
hk_instance_id_t hk_app_get_next_iid(void);

/** Application characteristic constructor.

    @return Pointer to characteristic instance of type \ref hk_app_chr_t. */
hk_app_chr_t *hk_app_chr_new(void);

/** Application characteristic destructor.

    @param [in] chr Pointer to characteristic instance to delete.

    @return None. */
void hk_app_chr_delete(hk_app_chr_t *chr);

/** Application service destructor.

    @param [in] srv Pointer to service instance to delete.

    @return None. */
void hk_app_srv_delete(hk_srv_base_t *srv);

/** Request dispatcher for accessory services.

    @param [in] accessory Accessory instance pointer.
    @param [in] request Request message to dispatch.

    @return One of defined status codes: \ref hk_status_code_t. */
// hk_status_code_t hk_app_request_dispatcher(const hk_acc_base_t *accessory, hk_message_t *request);

void *homekit_app_main_task(void *param);

#endif // HK_APP_H
