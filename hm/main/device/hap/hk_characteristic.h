/*
 Copyright (C) 2017 Tieto Poland Sp. z o. o.
*/

/** @file hk_characteristic.h

    @brief Homekit characteristic API.*/

#ifndef HOMEKIT_HK_CHARACTERISTIC_H
#define HOMEKIT_HK_CHARACTERISTIC_H

#include "../hk_api.h"

#define HK_MAX_CHARACTERISTICS 100

// forward declaration
typedef struct hk_acc_base hk_acc_base_t;

/** HomeKit permission flags. */
typedef struct
{
    uint8_t paired_read : 1;     ///< Permission flag read.
    uint8_t paired_write : 1;    ///< Permission flag write.
    uint8_t events : 1;          ///< Permission flag events.
    uint8_t additional_auth : 1; ///< Permission flag additional authorization.
    uint8_t timed_write : 1;     ///< Permission flag timed_write.
    uint8_t hidden : 1;          ///< Permission flag hidden.
} hk_permission_t;

/** Homekit unit types defined in R9 HAP spec */
typedef enum {
    HK_UNIT_UNITLESS,
    HK_UNIT_CELSIUS,
    HK_UNIT_PERCENTAGE,
    HK_UNIT_ARCDEGREES,
    HK_UNIT_LUX,
    HK_UNIT_SECONDS
} hk_unit_t;

/** Value formats specified in HAP R9 spec. */
typedef enum {
    HK_VALUE_FORMAT_NULL,  ///< NULL format for empty value.
    HK_VALUE_FORMAT_BOOL,  ///< Bool.
    HK_VALUE_FORMAT_U8,    ///< Unsigned 8-bit integer.
    HK_VALUE_FORMAT_U16,   ///< Unsigned 16-bit integer.
    HK_VALUE_FORMAT_U32,   ///< Unsigned 32-bit integer.
    HK_VALUE_FORMAT_U64,   ///< Unsigned 64-bit integer.
    HK_VALUE_FORMAT_I32,   ///< Signed 32-bit integer.
    HK_VALUE_FORMAT_FLOAT, ///< Signed 64-bit floating point number.
    HK_VALUE_FORMAT_STR,   ///< Unicode character string, encoded as UTF-8.
    HK_VALUE_FORMAT_TLV8,  ///< Base64-encoded set of one or more TLV8's.
    HK_VALUE_FORMAT_DATA   ///< Base64-encoded data blob.
} hk_value_format_t;

typedef enum {
    HK_HAP_STATUS_OK = 0,
    HK_HAP_STATUS_UNABLE_TO_COMMUNICATE = -70402,
    HK_HAP_STATUS_R_ONLY_CHR = -70404,
    HK_HAP_STATUS_W_ONLY_CHR = -70405,
    HK_HAP_STATUS_OUT_OF_RESOURCES = -70407,
    HK_HAP_STATUS_RESOURCES_DOES_NOT_EXIST = -70409,
    HK_HAP_STATUS_INVALID_VALUE = -70410
} hk_hap_status_t;

typedef float vfloat_t; // TODO(ks): import hap_float_t from 4010 project.
typedef char *vtlv8_t;  // TODO(ks): import form previous project or write new.
typedef char *vdata_t;  // TODO(ks): write something.
typedef int64_t hk_chr_status_t;

#define HK_VALUE                                                                                                       \
    union {                                                                                                            \
        bool b;                                                                                                        \
        uint8_t u8;                                                                                                    \
        uint16_t u16;                                                                                                  \
        uint32_t u32;                                                                                                  \
        uint64_t u64;                                                                                                  \
        int32_t i32;                                                                                                   \
        vfloat_t f;                                                                                                    \
        char *str;                                                                                                     \
        vtlv8_t tlv8;                                                                                                  \
        vdata_t data;                                                                                                  \
    }

typedef HK_VALUE hk_value_union_t;

/** HomeKit value representation. */
typedef struct
{
    hk_value_format_t format; ///< Value format type.
    HK_VALUE;                 ///< Anonymous value union.
} hk_value_t;

/** Characteristic metadata structure. */
typedef struct
{
    uint8_t uuid[16];            ///< Characteristic type UUID.
    hk_permission_t permissions; ///< Characteristic permissions.
    hk_unit_t unit;              ///< Characteristic unit.
    hk_value_format_t format;    ///< Value format type.
    struct
    {
        uint8_t min : 1;
        uint8_t max : 1;
        uint8_t step : 1;
        uint8_t valid_values : 1;
    } validate;                     ///< Validation flags.
    hk_value_union_t min;           ///< Minimum value.
    hk_value_union_t max;           ///< Maximum value.
    hk_value_union_t step;          ///< Minimum step.
    size_t max_len;                 ///< Maximum length for string and data types.
    hk_value_union_t *valid_values; ///< Valid values array.
    size_t valid_values_size;       ///< Size of valid values array.
} hk_chr_metadata_t;

/** Characteristic  on read callback. */
typedef hk_hap_status_t (*hk_chr_on_read_callback_t)(void *);
/** Characteristic on write callback. */
typedef hk_hap_status_t (*hk_chr_on_write_callback_t)(void *);

/** Characteristic base structure. */
typedef struct
{
    hk_instance_id_t iid;              ///< Characteristic instance ID.
    hk_value_t value;                  ///< Characteristic value.
    const hk_chr_metadata_t *metadata; ///< Characteristic metadata pointer.
    hk_chr_on_read_callback_t on_read;
    hk_chr_on_write_callback_t on_write;
} hk_chr_base_t;

typedef struct
{
    hk_instance_id_t iid;
    hk_accessory_id_t aid;
} hk_id_t;

typedef struct
{
    hk_id_t *id;
    size_t id_size;
    bool meta;
    bool perms;
    bool type;
    bool ev;
} hk_chr_request_t;

typedef struct
{
    hk_id_t id;
    hk_value_t value;
    bool ev;
    bool remote;
    hk_hap_status_t status;
} hk_chr_write_t;

typedef struct
{
    hk_id_t id;
    hk_chr_base_t *chr;
    hk_hap_status_t status;
} hk_chr_respond_t;

/** Characteristics metadata definition. */
extern const hk_chr_metadata_t hk_chr_metadata_administrator_only_access;
extern const hk_chr_metadata_t hk_chr_metadata_audio_feedback;
extern const hk_chr_metadata_t hk_chr_metadata_brightness;
extern const hk_chr_metadata_t hk_chr_metadata_cooling_threshold_temperature;
extern const hk_chr_metadata_t hk_chr_metadata_current_door_state;
extern const hk_chr_metadata_t hk_chr_metadata_current_heating_cooling_state;
extern const hk_chr_metadata_t hk_chr_metadata_current_relative_humidity;
extern const hk_chr_metadata_t hk_chr_metadata_current_temperature;
extern const hk_chr_metadata_t hk_chr_metadata_firmware_revision;
extern const hk_chr_metadata_t hk_chr_metadata_hardware_revision;
extern const hk_chr_metadata_t hk_chr_metadata_heating_threshold_temperature;
extern const hk_chr_metadata_t hk_chr_metadata_hue;
extern const hk_chr_metadata_t hk_chr_metadata_identify;
extern const hk_chr_metadata_t hk_chr_metadata_lock_control_point;
extern const hk_chr_metadata_t hk_chr_metadata_lock_current_state;
extern const hk_chr_metadata_t hk_chr_metadata_lock_last_known_action;
extern const hk_chr_metadata_t hk_chr_metadata_lock_management_auto_security_timeout;
extern const hk_chr_metadata_t hk_chr_metadata_lock_target_state;
extern const hk_chr_metadata_t hk_chr_metadata_logs;
extern const hk_chr_metadata_t hk_chr_metadata_manufacturer;
extern const hk_chr_metadata_t hk_chr_metadata_model;
extern const hk_chr_metadata_t hk_chr_metadata_motion_detected;
extern const hk_chr_metadata_t hk_chr_metadata_name;
extern const hk_chr_metadata_t hk_chr_metadata_obstruction_detected;
extern const hk_chr_metadata_t hk_chr_metadata_on;
extern const hk_chr_metadata_t hk_chr_metadata_outlet_in_use;
extern const hk_chr_metadata_t hk_chr_metadata_rotation_direction;
extern const hk_chr_metadata_t hk_chr_metadata_rotation_speed;
extern const hk_chr_metadata_t hk_chr_metadata_saturation;
extern const hk_chr_metadata_t hk_chr_metadata_serial_number;
extern const hk_chr_metadata_t hk_chr_metadata_target_door_state;
extern const hk_chr_metadata_t hk_chr_metadata_target_heating_cooling_state;
extern const hk_chr_metadata_t hk_chr_metadata_target_relative_humidity;
extern const hk_chr_metadata_t hk_chr_metadata_target_temperature;
extern const hk_chr_metadata_t hk_chr_metadata_temperature_display_units;
extern const hk_chr_metadata_t hk_chr_metadata_version;
extern const hk_chr_metadata_t hk_chr_metadata_air_particulate_density;
extern const hk_chr_metadata_t hk_chr_metadata_air_particulate_size;
extern const hk_chr_metadata_t hk_chr_metadata_security_system_current_state;
extern const hk_chr_metadata_t hk_chr_metadata_security_system_target_state;
extern const hk_chr_metadata_t hk_chr_metadata_battery_level;
extern const hk_chr_metadata_t hk_chr_metadata_carbon_monoxide_detected;
extern const hk_chr_metadata_t hk_chr_metadata_contact_sensor_state;
extern const hk_chr_metadata_t hk_chr_metadata_current_ambient_light_level;
extern const hk_chr_metadata_t hk_chr_metadata_current_horizontal_tilt_angle;
extern const hk_chr_metadata_t hk_chr_metadata_current_position;
extern const hk_chr_metadata_t hk_chr_metadata_current_vertical_tilt_angle;
extern const hk_chr_metadata_t hk_chr_metadata_hold_position;
extern const hk_chr_metadata_t hk_chr_metadata_leak_detected;
extern const hk_chr_metadata_t hk_chr_metadata_occupancy_detected;
extern const hk_chr_metadata_t hk_chr_metadata_position_state;
extern const hk_chr_metadata_t hk_chr_metadata_programmable_switch_event;
extern const hk_chr_metadata_t hk_chr_metadata_status_active;
extern const hk_chr_metadata_t hk_chr_metadata_smoke_detected;
extern const hk_chr_metadata_t hk_chr_metadata_status_fault;
extern const hk_chr_metadata_t hk_chr_metadata_status_jammed;
extern const hk_chr_metadata_t hk_chr_metadata_status_low_battery;
extern const hk_chr_metadata_t hk_chr_metadata_status_tampered;
extern const hk_chr_metadata_t hk_chr_metadata_target_horizontal_tilt_angle;
extern const hk_chr_metadata_t hk_chr_metadata_target_position;
extern const hk_chr_metadata_t hk_chr_metadata_target_vertical_tilt_angle;
extern const hk_chr_metadata_t hk_chr_metadata_security_system_alarm_type;
extern const hk_chr_metadata_t hk_chr_metadata_charging_state;
extern const hk_chr_metadata_t hk_chr_metadata_carbon_monoxide_level;
extern const hk_chr_metadata_t hk_chr_metadata_carbon_monoxide_peak_level;
extern const hk_chr_metadata_t hk_chr_metadata_carbon_dioxide_detected;
extern const hk_chr_metadata_t hk_chr_metadata_carbon_dioxide_level;
extern const hk_chr_metadata_t hk_chr_metadata_carbon_dioxide_peak_level;
extern const hk_chr_metadata_t hk_chr_metadata_air_quality;
extern const hk_chr_metadata_t hk_chr_metadata_streaming_status;
extern const hk_chr_metadata_t hk_chr_metadata_supported_video_stream_configuration;
extern const hk_chr_metadata_t hk_chr_metadata_supported_audio_stream_configuration;
extern const hk_chr_metadata_t hk_chr_metadata_supported_rtp_configuration;
extern const hk_chr_metadata_t hk_chr_metadata_setup_endpoints;
extern const hk_chr_metadata_t hk_chr_metadata_selected_rtp_stream_configuration;
extern const hk_chr_metadata_t hk_chr_metadata_volume;
extern const hk_chr_metadata_t hk_chr_metadata_mute;
extern const hk_chr_metadata_t hk_chr_metadata_night_vision;
extern const hk_chr_metadata_t hk_chr_metadata_optical_zoom;
extern const hk_chr_metadata_t hk_chr_metadata_digital_zoom;
extern const hk_chr_metadata_t hk_chr_metadata_image_rotation;
extern const hk_chr_metadata_t hk_chr_metadata_image_mirroring;
extern const hk_chr_metadata_t hk_chr_metadata_accessory_flags;
extern const hk_chr_metadata_t hk_chr_metadata_lock_physical_controls;
extern const hk_chr_metadata_t hk_chr_metadata_current_air_purifier_state;
extern const hk_chr_metadata_t hk_chr_metadata_current_slat_state;
extern const hk_chr_metadata_t hk_chr_metadata_slat_type;
extern const hk_chr_metadata_t hk_chr_metadata_filter_life_level;
extern const hk_chr_metadata_t hk_chr_metadata_filter_change_indication;
extern const hk_chr_metadata_t hk_chr_metadata_reset_filter_indication;
extern const hk_chr_metadata_t hk_chr_metadata_target_air_purifier_state;
extern const hk_chr_metadata_t hk_chr_metadata_target_fan_state;
extern const hk_chr_metadata_t hk_chr_metadata_current_fan_state;
extern const hk_chr_metadata_t hk_chr_metadata_active;
extern const hk_chr_metadata_t hk_chr_metadata_swing_mode;
extern const hk_chr_metadata_t hk_chr_metadata_current_tilt_angle;
extern const hk_chr_metadata_t hk_chr_metadata_target_tilt_angle;
extern const hk_chr_metadata_t hk_chr_metadata_ozone_density;
extern const hk_chr_metadata_t hk_chr_metadata_nitrogen_dioxide_density;
extern const hk_chr_metadata_t hk_chr_metadata_sulphur_dioxide_density;
extern const hk_chr_metadata_t hk_chr_metadata_pm_2_5_density;
extern const hk_chr_metadata_t hk_chr_metadata_pm_10_density;
extern const hk_chr_metadata_t hk_chr_metadata_voc_density;
extern const hk_chr_metadata_t hk_chr_metadata_service_label_index;
extern const hk_chr_metadata_t hk_chr_metadata_service_label_namespace;
extern const hk_chr_metadata_t hk_chr_metadata_color_temperature;

/** Finds pointer to characteristic by instance ID in specified accessory.

    @param [in] accessory Accessory instance pointer.
    @param [in] iid Characteristic instance ID.

    @return Pointer of characteristic instance with specified instance ID. */
HK_NOT_NULL_ARGS(1)
hk_chr_base_t *hk_chr_find_by_iid(const hk_acc_base_t *accessory, hk_instance_id_t iid);

void hk_srv_list(const hk_acc_base_t *accessory);

/** Validates characteristic value depending on metadata.

    @param value Pointer of value to validate.
    @param metadata Characteristic metadata for value validation.

    @return True if value is valid. False otherwise. */
HK_NOT_NULL_ARGS(1, 2)
bool hk_chr_validate_value(const hk_value_t *value, const hk_chr_metadata_t *metadata);

/** Compares characteristic values.

    @param [in] value_1 Pointer to 1st value.
    @param [in] value_2 Pointer to 2nd value.

    @return True if values are equal. False otherwise. */
HK_NOT_NULL_ARGS(1, 2)
bool hk_chr_compare_value_equal(const hk_value_t *value_1, const hk_value_t *value_2);

/** Copies characteristic value.

    @param [in] dst Destination value pointer.
    @param [in] src Source value pointer.

    @return True if copying succeed. False otherwise. */
HK_NOT_NULL_ARGS(1, 2)
bool hk_chr_copy_value(hk_value_t *dst, const hk_value_t *src);

/** Copies characteristic's metadata.

    @param dst Pointer to destination metadata.
    @param src Pointer to source metadata.

    @return True if copying ok. False otherwise. */
HK_NOT_NULL_ARGS(1, 2)
bool hk_chr_copy_metadata(hk_chr_metadata_t *dst, const hk_chr_metadata_t *src);

/** Allocates memory for characteristic.

    @return Memory pointer of type \ref hk_chr_base_t. */
hk_chr_base_t *hk_chr_new(void);

/** Frees memory of characteristic.

    @param [in] chr Pointer to characteristic.

    @return None. */
void hk_chr_delete(hk_chr_base_t *chr);

hk_chr_request_t *hk_chr_request_new(size_t size);
void hk_chr_request_delete(hk_chr_request_t *r_chr);
/** Short characteristic UUIDs defined in HAP R9 spec. */

bool hk_chr_respond_check_status(const hk_chr_respond_t const *chr, const hk_chr_request_t const *chr_request);
enum
{
    HK_CHARACTERISTIC_UUID_ADMINISTRATOR_ONLY_ACCESS = 0x01,
    HK_CHARACTERISTIC_UUID_AUDIO_FEEDBACK = 0x05,
    HK_CHARACTERISTIC_UUID_BRIGHTNESS = 0x08,
    HK_CHARACTERISTIC_UUID_COOLING_THRESHOLD_TEMPERATURE = 0x0D,
    HK_CHARACTERISTIC_UUID_CURRENT_DOOR_STATE = 0x0E,
    HK_CHARACTERISTIC_UUID_CURRENT_HEATING_COOLING_STATE = 0x0F,
    HK_CHARACTERISTIC_UUID_CURRENT_RELATIVE_HUMIDITY = 0x10,
    HK_CHARACTERISTIC_UUID_CURRENT_TEMPERATURE = 0x11,
    HK_CHARACTERISTIC_UUID_FIRMWARE_REVISION = 0x52,
    HK_CHARACTERISTIC_UUID_HARDWARE_REVISION = 0x53,
    HK_CHARACTERISTIC_UUID_HEATING_THRESHOLD_TEMPERATURE = 0x12,
    HK_CHARACTERISTIC_UUID_HUE = 0x13,
    HK_CHARACTERISTIC_UUID_IDENTIFY = 0x14,
    HK_CHARACTERISTIC_UUID_LOCK_CONTROL_POINT = 0x19,
    HK_CHARACTERISTIC_UUID_LOCK_CURRENT_STATE = 0x1D,
    HK_CHARACTERISTIC_UUID_LOCK_LAST_KNOWN_ACTION = 0x1C,
    HK_CHARACTERISTIC_UUID_LOCK_MANAGEMENT_AUTO_SECURITY_TIMEOUT = 0x1A,
    HK_CHARACTERISTIC_UUID_LOCK_TARGET_STATE = 0x1E,
    HK_CHARACTERISTIC_UUID_LOGS = 0x1F,
    HK_CHARACTERISTIC_UUID_MANUFACTURER = 0x20,
    HK_CHARACTERISTIC_UUID_MODEL = 0x21,
    HK_CHARACTERISTIC_UUID_MOTION_DETECTED = 0x22,
    HK_CHARACTERISTIC_UUID_NAME = 0x23,
    HK_CHARACTERISTIC_UUID_OBSTRUCTION_DETECTED = 0x24,
    HK_CHARACTERISTIC_UUID_ON = 0x25,
    HK_CHARACTERISTIC_UUID_OUTLET_IN_USE = 0x26,
    HK_CHARACTERISTIC_UUID_ROTATION_DIRECTION = 0x28,
    HK_CHARACTERISTIC_UUID_ROTATION_SPEED = 0x29,
    HK_CHARACTERISTIC_UUID_SATURATION = 0x2F,
    HK_CHARACTERISTIC_UUID_SERIAL_NUMBER = 0x30,
    HK_CHARACTERISTIC_UUID_TARGET_DOOR_STATE = 0x32,
    HK_CHARACTERISTIC_UUID_TARGET_HEATING_COOLING_STATE = 0x33,
    HK_CHARACTERISTIC_UUID_TARGET_RELATIVE_HUMIDITY = 0x34,
    HK_CHARACTERISTIC_UUID_TARGET_TEMPERATURE = 0x35,
    HK_CHARACTERISTIC_UUID_TEMPERATURE_DISPLAY_UNITS = 0x36,
    HK_CHARACTERISTIC_UUID_VERSION = 0x37,
    HK_CHARACTERISTIC_UUID_AIR_PARTICULATE_DENSITY = 0x64,
    HK_CHARACTERISTIC_UUID_AIR_PARTICULATE_SIZE = 0x65,
    HK_CHARACTERISTIC_UUID_SECURITY_SYSTEM_CURRENT_STATE = 0x66,
    HK_CHARACTERISTIC_UUID_SECURITY_SYSTEM_TARGET_STATE = 0x67,
    HK_CHARACTERISTIC_UUID_BATTERY_LEVEL = 0x68,
    HK_CHARACTERISTIC_UUID_CARBON_MONOXIDE_DETECTED = 0x69,
    HK_CHARACTERISTIC_UUID_CONTACT_SENSOR_STATE = 0x6A,
    HK_CHARACTERISTIC_UUID_CURRENT_AMBIENT_LIGHT_LEVEL = 0x6B,
    HK_CHARACTERISTIC_UUID_CURRENT_HORIZONTAL_TILT_ANGLE = 0x6C,
    HK_CHARACTERISTIC_UUID_CURRENT_POSITION = 0x6D,
    HK_CHARACTERISTIC_UUID_CURRENT_VERTICAL_TILT_ANGLE = 0x6E,
    HK_CHARACTERISTIC_UUID_HOLD_POSITION = 0x6F,
    HK_CHARACTERISTIC_UUID_LEAK_DETECTED = 0x70,
    HK_CHARACTERISTIC_UUID_OCCUPANCY_DETECTED = 0x71,
    HK_CHARACTERISTIC_UUID_POSITION_STATE = 0x72,
    HK_CHARACTERISTIC_UUID_PROGRAMMABLE_SWITCH_EVENT = 0x73,
    HK_CHARACTERISTIC_UUID_STATUS_ACTIVE = 0x75,
    HK_CHARACTERISTIC_UUID_SMOKE_DETECTED = 0x76,
    HK_CHARACTERISTIC_UUID_STATUS_FAULT = 0x77,
    HK_CHARACTERISTIC_UUID_STATUS_JAMMED = 0x78,
    HK_CHARACTERISTIC_UUID_STATUS_LOW_BATTERY = 0x79,
    HK_CHARACTERISTIC_UUID_STATUS_TAMPERED = 0x7A,
    HK_CHARACTERISTIC_UUID_TARGET_HORIZONTAL_TILT_ANGLE = 0x7B,
    HK_CHARACTERISTIC_UUID_TARGET_POSITION = 0x7C,
    HK_CHARACTERISTIC_UUID_TARGET_VERTICAL_TILT_ANGLE = 0x7D,
    HK_CHARACTERISTIC_UUID_SECURITY_SYSTEM_ALARM_TYPE = 0x8E,
    HK_CHARACTERISTIC_UUID_CHARGING_STATE = 0x8F,
    HK_CHARACTERISTIC_UUID_CARBON_MONOXIDE_LEVEL = 0x90,
    HK_CHARACTERISTIC_UUID_CARBON_MONOXIDE_PEAK_LEVEL = 0x91,
    HK_CHARACTERISTIC_UUID_CARBON_DIOXIDE_DETECTED = 0x92,
    HK_CHARACTERISTIC_UUID_CARBON_DIOXIDE_LEVEL = 0x93,
    HK_CHARACTERISTIC_UUID_CARBON_DIOXIDE_PEAK_LEVEL = 0x94,
    HK_CHARACTERISTIC_UUID_AIR_QUALITY = 0x95,
    HK_CHARACTERISTIC_UUID_STREAMING_STATUS = 0x120,
    HK_CHARACTERISTIC_UUID_SUPPORTED_VIDEO_STREAM_CONFIGURATION = 0x114,
    HK_CHARACTERISTIC_UUID_SUPPORTED_AUDIO_STREAM_CONFIGURATION = 0x115,
    HK_CHARACTERISTIC_UUID_SUPPORTED_RTP_CONFIGURATION = 0x116,
    HK_CHARACTERISTIC_UUID_SETUP_ENDPOINTS = 0x118,
    HK_CHARACTERISTIC_UUID_SELECTED_RTP_STREAM_CONFIGURATION = 0x117,
    HK_CHARACTERISTIC_UUID_VOLUME = 0x119,
    HK_CHARACTERISTIC_UUID_MUTE = 0x11A,
    HK_CHARACTERISTIC_UUID_NIGHT_VISION = 0x11B,
    HK_CHARACTERISTIC_UUID_OPTICAL_ZOOM = 0x11C,
    HK_CHARACTERISTIC_UUID_DIGITAL_ZOOM = 0x11D,
    HK_CHARACTERISTIC_UUID_IMAGE_ROTATION = 0x11E,
    HK_CHARACTERISTIC_UUID_IMAGE_MIRRORING = 0x11F,
    HK_CHARACTERISTIC_UUID_ACCESSORY_FLAGS = 0xA6,
    HK_CHARACTERISTIC_UUID_LOCK_PHYSICAL_CONTROLS = 0xA7,
    HK_CHARACTERISTIC_UUID_CURRENT_AIR_PURIFIER_STATE = 0xA9,
    HK_CHARACTERISTIC_UUID_CURRENT_SLAT_STATE = 0xAA,
    HK_CHARACTERISTIC_UUID_SLAT_TYPE = 0xC0,
    HK_CHARACTERISTIC_UUID_FILTER_LIFE_LEVEL = 0xAB,
    HK_CHARACTERISTIC_UUID_FILTER_CHANGE_INDICATION = 0xAC,
    HK_CHARACTERISTIC_UUID_RESET_FILTER_INDICATION = 0xAD,
    HK_CHARACTERISTIC_UUID_TARGET_AIR_PURIFIER_STATE = 0xA8,
    HK_CHARACTERISTIC_UUID_TARGET_FAN_STATE = 0xBF,
    HK_CHARACTERISTIC_UUID_CURRENT_FAN_STATE = 0xAF,
    HK_CHARACTERISTIC_UUID_ACTIVE = 0xB0,
    HK_CHARACTERISTIC_UUID_SWING_MODE = 0xB6,
    HK_CHARACTERISTIC_UUID_CURRENT_TILT_ANGLE = 0xC1,
    HK_CHARACTERISTIC_UUID_TARGET_TILT_ANGLE = 0xC2,
    HK_CHARACTERISTIC_UUID_OZONE_DENSITY = 0xC3,
    HK_CHARACTERISTIC_UUID_NITROGEN_DIOXIDE_DENSITY = 0xC4,
    HK_CHARACTERISTIC_UUID_SULPHUR_DIOXIDE_DENSITY = 0xC5,
    HK_CHARACTERISTIC_UUID_PM_2_5_DENSITY = 0xC6,
    HK_CHARACTERISTIC_UUID_PM_10_DENSITY = 0xC7,
    HK_CHARACTERISTIC_UUID_VOC_DENSITY = 0xC8,
    HK_CHARACTERISTIC_UUID_SERVICE_LABEL_INDEX = 0xCB,
    HK_CHARACTERISTIC_UUID_SERVICE_LABEL_NAMESPACE = 0xCD,
    HK_CHARACTERISTIC_UUID_COLOR_TEMPERATURE = 0xCE
};

#endif /* HOMEKIT_HK_CHARACTERISTIC_H */
