/*
 * Copyright (C) 2017 Tieto Poland Sp. z o. o.
 */

/** @file hk_api.h

    @brief HomeKit API.

    HomeKit application programming interface. */

#ifndef HOMEKIT_HK_API_H
#define HOMEKIT_HK_API_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define SETUP_CODE_STRING_LENGTH 10    ///< Length of the setup code.
#define FRIENDLY_NAME_STRING_LENGTH 64 ///< Length of device friendly name.
#define MANUFACTURER_STRING_LENGTH 64  ///< Length of manufacturer name.
#define MODEL_STRING_LENGTH 64         ///< Length of model name.
#define WAC_DEVICE_ID_LENGTH 6         ///< Length of device id.

#ifdef __GNUC__
#define HK_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#define HK_NOT_NULL_ARGS(...) __attribute__((nonnull(__VA_ARGS__)))
#else
#define HK_WARN_UNUSED_RESULT
#define HK_NOT_NULL_ARGS(...)
#endif

typedef struct hk_acc_base hk_acc_base_t;
/** @addtogroup homekit_accessory_protocol_api
@{ */

/** HomeKit accessory ID. */
typedef uint64_t hk_accessory_id_t;

/** HomeKit instance ID. */
typedef uint64_t hk_instance_id_t;

// Define used in metadata initialization with short UUID.
#define UINT16_TO_UUID(x)                                                                                              \
    {                                                                                                                  \
        (uint8_t)0, (uint8_t)0, (uint8_t)((x) >> 8), (uint8_t)(x)                                                      \
    }

/** API return codes. */
typedef enum {
    HK_RET_SUCCESS,             ///< Success.
    HK_RET_FAIL,                ///< Fail.
    HK_RET_RESOURCES_EXHAUSTED, ///< Not enough resources to perform operation.
    HK_RET_INVALID_STATE,       ///< Function called with invalid state (either within arguments or global).
    HK_RET_TOO_MANY_ATTEMPTS,   ///< An operation has been attempted too many times or is no longer allowed.
} hk_ret_t;

/** HomeKit status codes. */
typedef enum {
    HK_STATUS_SUCCESS = 0,                 ///< Success.
    HK_STATUS_INSUFFICIENT_AUTHENTICATION, ///< Insufficient authentication.
    HK_STATUS_RESOURCE_UNREACHABLE,        ///< Resource unreachable.
    HK_STATUS_RESOURCE_BUSY,               ///< Resource busy.
    HK_STATUS_WRITE_ERROR,                 ///< Write error.
    HK_STATUS_READ_ERROR,                  ///< Read error.
    HK_STATUS_NOTIFICATION_ERROR,          ///< Notification error.
    HK_STATUS_OUT_OF_RESOURCES,            ///< Out of resources.
    HK_STATUS_OPERATION_TIME_OUT,          ///< Operation time out.
    HK_STATUS_INVALID_INSTANCE_ID,         ///< Invalid instance ID.
    HK_STATUS_INVALID_REQUEST,             ///< Invalid request.
    HK_STATUS_INSUFFICIENT_AUTHORIZATION   ///< Insufficient authorization.
} hk_status_code_t;

/** Represents HomeKit public API.
    It is responsible for configuring, initializing all HomeKit subsystems and allows the application to set up
    handlers for HomeKit operations such as reads, writes or sending notifications. */
typedef void (*hk_callback_t)(hk_ret_t, void *);

typedef struct
{
    const char setup_code[SETUP_CODE_STRING_LENGTH];       ///< Setup code in format XXX-XX-XX.
    int interface_id;                                      ///< BLE or IP.
    const char friendly_name[FRIENDLY_NAME_STRING_LENGTH]; ///< Name of the device.
    const char manufacturer[MANUFACTURER_STRING_LENGTH];   ///< Name of the manufacturer.
    const char model[MODEL_STRING_LENGTH];                 ///< Device model name.
    const uint8_t wac_device_id[WAC_DEVICE_ID_LENGTH];     ///< Hap device id.
} hk_configuration_t;

typedef struct
{
    char *name;
    // TODO(ks): add required fields.
} hk_accessory_information_t;

typedef struct
{
    const char *device_id;
    // qurt_pipe_t message_queue;
} hk_core_handle_t;

typedef struct
{
    hk_core_handle_t handle;
    hk_callback_t callback;
    void *user_data;
} hk_application_context_t;

/** Initializes all HomeKit subsystems.

    @param [in]cfg  Contains configuration information used by HomeKit.

    @remarks Can only be called once. Calling it more than once on an object will result in Undefined Behavior
    @remarks It is synchronous - it will block until HomeKit is running or an error occurs
    @return Pointer to hk_core_handle_t if HomeKit has been initiated successfully or NULL otherwise. */
HK_WARN_UNUSED_RESULT
HK_NOT_NULL_ARGS(1)
hk_core_handle_t *hk_core_init(const hk_configuration_t *cfg);

/** Runs the main event loop of the application.

    When an application thread enters this loop, it becomes ready to handle HomeKit requests. Never returns.
    If a function needs to be called within the thread that called this function, \ref hk_dispatch() should be
    used instead.

    @param [in]context  Context of application.

    @remarks Only one thread may call this function.
    @return None. */
HK_NOT_NULL_ARGS(1)
void hk_run(hk_application_context_t *context);

/** Dispatches a function on the thread that called \ref hk_run().

    This function is useful when a function must be called on the application thread to prevent data races or avoid
    the required synchronization.

    @param [in]context   Context of application.
    @param [in]callback  The function to be run on the application thread.
    @param [in]user_data Pointer to user data.

    @remarks Thread-safe, non-reentrant (may be called from threads but not from ISRs).
    @return  None. */
HK_NOT_NULL_ARGS(1, 2)
void hk_dispatch(hk_application_context_t *context, hk_callback_t callback, void *user_data);

/** Register accessory.

    @param [in]context   Context of application.
    @param [in]aid       Accessory id.
    @param [in]callback  The function to be run on the application thread.
    @param [in]user_data Pointer to user data.

    @remarks Thread-safe, non-reentrant (may be called from threads but not from ISRs).
    @return  None. */
HK_NOT_NULL_ARGS(1, 3)
void hk_reregister_accessory(hk_application_context_t *context, hk_accessory_id_t aid, hk_callback_t *callback,
                             void *user_data);

/** Reregister accessory.

    @param [in]context   Context of application.
    @param [out]aid      Accessory id.
    @param [in]callback  The function to be run on the application thread.
    @param [in]user_data Pointer to user data.

    @remarks Thread-safe, non-reentrant (may be called from threads but not from ISRs).
    @return  None. */
HK_NOT_NULL_ARGS(1, 2, 3)
void hk_register_accessory(hk_application_context_t *context, hk_accessory_id_t *aid, hk_callback_t *callback,
                           void *user_data);

/** Deregister accessory.

    @param [in]context   Context of application.
    @param [in]aid       Accessory id.
    @param [in]callback  The function to be run on the application thread.
    @param [in]user_data Pointer to user data.

    @remarks Thread-safe, non-reentrant (may be called from threads but not from ISRs).
    @return  None. */
HK_NOT_NULL_ARGS(1, 3)
void hk_deregister_accessory(hk_application_context_t *context, hk_accessory_id_t aid, hk_callback_t *callback,
                             void *user_data);

/** @} */ // End of homekit_accessory_protocol_api

#endif /* HOMEKIT_HK_API_H */
