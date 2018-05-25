/*
 Copyright (C) 2017 Tieto Poland Sp. z o. o.
*/

#include <assert.h>
#include <string.h>

#include "hk_characteristic.h"
#include "hk_accessory.h"
extern void hk_console_printf(const char *text, ...);

/* clang-format off */
const hk_chr_metadata_t hk_chr_metadata_administrator_only_access = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_ADMINISTRATOR_ONLY_ACCESS),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_BOOL,
  .unit = HK_UNIT_UNITLESS};

const hk_chr_metadata_t hk_chr_metadata_audio_feedback = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_AUDIO_FEEDBACK),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_BOOL,
  .unit = HK_UNIT_UNITLESS};

const hk_chr_metadata_t hk_chr_metadata_brightness = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_BRIGHTNESS),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_I32,
  .unit = HK_UNIT_PERCENTAGE,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.i32 = 0,
  .max.i32 = 100,
  .step.i32 = 1};

const hk_chr_metadata_t hk_chr_metadata_cooling_threshold_temperature = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_COOLING_THRESHOLD_TEMPERATURE),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_CELSIUS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.f = 10,
  .max.f = 35,
  .step.f = 0.1};

const hk_chr_metadata_t hk_chr_metadata_current_door_state = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_CURRENT_DOOR_STATE),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 4,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_current_heating_cooling_state = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_CURRENT_HEATING_COOLING_STATE),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 2,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_current_relative_humidity = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_CURRENT_RELATIVE_HUMIDITY),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_PERCENTAGE,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.f = 0,
  .max.f = 100,
  .step.f = 1};

const hk_chr_metadata_t hk_chr_metadata_current_temperature = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_CURRENT_TEMPERATURE),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_CELSIUS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.f = 0,
  .max.f = 100,
  .step.f = 0.1};

const hk_chr_metadata_t hk_chr_metadata_firmware_revision = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_FIRMWARE_REVISION),
  .permissions = {1, 0, 0, 0, 0, 0},
  .format = HK_VALUE_FORMAT_STR,
  .unit = HK_UNIT_UNITLESS};

const hk_chr_metadata_t hk_chr_metadata_hardware_revision = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_HARDWARE_REVISION),
  .permissions = {1, 0, 0, 0, 0, 0},
  .format = HK_VALUE_FORMAT_STR,
  .unit = HK_UNIT_UNITLESS};

const hk_chr_metadata_t hk_chr_metadata_heating_threshold_temperature = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_HEATING_THRESHOLD_TEMPERATURE),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_CELSIUS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.f = 0,
  .max.f = 25,
  .step.f = 0.1};

const hk_chr_metadata_t hk_chr_metadata_hue = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_HUE),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_ARCDEGREES,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.f = 0,
  .max.f = 360,
  .step.f = 1};

const hk_chr_metadata_t hk_chr_metadata_identify = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_IDENTIFY),
  .permissions = {0, 1, 0, 0, 0, 0},
  .format = HK_VALUE_FORMAT_BOOL,
  .unit = HK_UNIT_UNITLESS};

const hk_chr_metadata_t hk_chr_metadata_lock_control_point = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_LOCK_CONTROL_POINT),
  .permissions = {0, 1, 0, 0, 0, 0},
  .format = HK_VALUE_FORMAT_TLV8,
  .unit = HK_UNIT_UNITLESS};

const hk_chr_metadata_t hk_chr_metadata_lock_current_state = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_LOCK_CURRENT_STATE),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 3,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_lock_last_known_action = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_LOCK_LAST_KNOWN_ACTION),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 8,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_lock_management_auto_security_timeout = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_LOCK_MANAGEMENT_AUTO_SECURITY_TIMEOUT),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U32,
  .unit = HK_UNIT_SECONDS};

const hk_chr_metadata_t hk_chr_metadata_lock_target_state = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_LOCK_TARGET_STATE),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_logs = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_LOGS),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_TLV8,
  .unit = HK_UNIT_UNITLESS};

const hk_chr_metadata_t hk_chr_metadata_manufacturer = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_MANUFACTURER),
  .permissions = {1, 0, 0, 0, 0, 0},
  .format = HK_VALUE_FORMAT_STR,
  .unit = HK_UNIT_UNITLESS,
  .max_len = 64};

const hk_chr_metadata_t hk_chr_metadata_model = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_MODEL),
  .permissions = {1, 0, 0, 0, 0, 0},
  .format = HK_VALUE_FORMAT_STR,
  .unit = HK_UNIT_UNITLESS,
  .max_len = 64};

const hk_chr_metadata_t hk_chr_metadata_motion_detected = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_MOTION_DETECTED),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_BOOL,
  .unit = HK_UNIT_UNITLESS};

const hk_chr_metadata_t hk_chr_metadata_name = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_NAME),
  .permissions = {1, 0, 0, 0, 0, 0},
  .format = HK_VALUE_FORMAT_STR,
  .unit = HK_UNIT_UNITLESS,
  .max_len = 64,
};

const hk_chr_metadata_t hk_chr_metadata_obstruction_detected = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_OBSTRUCTION_DETECTED),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_BOOL,
  .unit = HK_UNIT_UNITLESS};

const hk_chr_metadata_t hk_chr_metadata_on = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_ON),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_BOOL,
  .unit = HK_UNIT_UNITLESS};

const hk_chr_metadata_t hk_chr_metadata_outlet_in_use = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_OUTLET_IN_USE),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_BOOL,
  .unit = HK_UNIT_UNITLESS};

const hk_chr_metadata_t hk_chr_metadata_rotation_direction = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_ROTATION_DIRECTION),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_I32,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.i32 = 0,
  .max.i32 = 1,
  .step.i32 = 1};

const hk_chr_metadata_t hk_chr_metadata_rotation_speed = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_ROTATION_SPEED),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_PERCENTAGE,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.f = 0,
  .max.f = 100,
  .step.f = 1};

const hk_chr_metadata_t hk_chr_metadata_saturation = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_SATURATION),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_PERCENTAGE,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.f = 0,
  .max.f = 100,
  .step.f = 1};

const hk_chr_metadata_t hk_chr_metadata_serial_number = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_SERIAL_NUMBER),
  .permissions = {1, 0, 0, 0, 0, 0},
  .format = HK_VALUE_FORMAT_STR,
  .unit = HK_UNIT_UNITLESS,
  .max_len = 64};

const hk_chr_metadata_t hk_chr_metadata_target_door_state = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_TARGET_DOOR_STATE),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_target_heating_cooling_state = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_TARGET_HEATING_COOLING_STATE),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 3,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_target_relative_humidity = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_TARGET_RELATIVE_HUMIDITY),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_PERCENTAGE,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.f = 0,
  .max.f = 100,
  .step.f = 1};

const hk_chr_metadata_t hk_chr_metadata_target_temperature = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_TARGET_TEMPERATURE),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_CELSIUS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.f = 10.0,
  .max.f = 38.0,
  .step.f = 0.1};

const hk_chr_metadata_t hk_chr_metadata_temperature_display_units = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_TEMPERATURE_DISPLAY_UNITS),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_version = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_VERSION),
  .permissions = {1, 0, 0, 0, 0, 0},
  .format = HK_VALUE_FORMAT_STR,
  .unit = HK_UNIT_UNITLESS,
  .max_len = 64};

const hk_chr_metadata_t hk_chr_metadata_air_particulate_density = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_AIR_PARTICULATE_DENSITY),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = false, .valid_values = false},
  .min.f = 0,
  .max.f = 1000};

const hk_chr_metadata_t hk_chr_metadata_air_particulate_size = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_AIR_PARTICULATE_SIZE),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_security_system_current_state = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_SECURITY_SYSTEM_CURRENT_STATE),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 4,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_security_system_target_state = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_SECURITY_SYSTEM_TARGET_STATE),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 3,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_battery_level = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_BATTERY_LEVEL),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_PERCENTAGE,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 100,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_carbon_monoxide_detected = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_CARBON_MONOXIDE_DETECTED),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_contact_sensor_state = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_CONTACT_SENSOR_STATE),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_current_ambient_light_level = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_CURRENT_AMBIENT_LIGHT_LEVEL),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_LUX,
  .validate = {.min = true, .max = true, .step = false, .valid_values = false},
  .min.f = 0.0001,
  .max.f = 100000};

const hk_chr_metadata_t hk_chr_metadata_current_horizontal_tilt_angle = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_CURRENT_HORIZONTAL_TILT_ANGLE),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_I32,
  .unit = HK_UNIT_ARCDEGREES,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.i32 = -90,
  .max.i32 = 90,
  .step.i32 = 1};

const hk_chr_metadata_t hk_chr_metadata_current_position = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_CURRENT_POSITION),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_PERCENTAGE,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 100,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_current_vertical_tilt_angle = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_CURRENT_VERTICAL_TILT_ANGLE),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_I32,
  .unit = HK_UNIT_ARCDEGREES,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.i32 = -90,
  .max.i32 = 90,
  .step.i32 = 1};

const hk_chr_metadata_t hk_chr_metadata_hold_position = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_HOLD_POSITION),
  .permissions = {0, 1, 0, 0, 0, 0},
  .format = HK_VALUE_FORMAT_BOOL,
  .unit = HK_UNIT_UNITLESS};

const hk_chr_metadata_t hk_chr_metadata_leak_detected = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_LEAK_DETECTED),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_occupancy_detected = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_OCCUPANCY_DETECTED),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_position_state = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_POSITION_STATE),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 2,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_programmable_switch_event = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_PROGRAMMABLE_SWITCH_EVENT),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 2,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_status_active = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_STATUS_ACTIVE),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_BOOL,
  .unit = HK_UNIT_UNITLESS};

const hk_chr_metadata_t hk_chr_metadata_smoke_detected = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_SMOKE_DETECTED),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_status_fault = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_STATUS_FAULT),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_status_jammed = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_STATUS_JAMMED),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_status_low_battery = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_STATUS_LOW_BATTERY),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_status_tampered = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_STATUS_TAMPERED),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_target_horizontal_tilt_angle = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_TARGET_HORIZONTAL_TILT_ANGLE),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_I32,
  .unit = HK_UNIT_ARCDEGREES,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.i32 = -90,
  .max.i32 = 90,
  .step.i32 = 1};

const hk_chr_metadata_t hk_chr_metadata_target_position = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_TARGET_POSITION),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_PERCENTAGE,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 100,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_target_vertical_tilt_angle = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_TARGET_VERTICAL_TILT_ANGLE),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_I32,
  .unit = HK_UNIT_ARCDEGREES,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.i32 = -90,
  .max.i32 = 90,
  .step.i32 = 1};

const hk_chr_metadata_t hk_chr_metadata_security_system_alarm_type = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_SECURITY_SYSTEM_ALARM_TYPE),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_charging_state = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_CHARGING_STATE),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 2,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_carbon_monoxide_level = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_CARBON_MONOXIDE_LEVEL),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = false, .valid_values = false},
  .min.f = 0,
  .max.f = 100};

const hk_chr_metadata_t hk_chr_metadata_carbon_monoxide_peak_level = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_CARBON_MONOXIDE_PEAK_LEVEL),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = false, .valid_values = false},
  .min.f = 0,
  .max.f = 100};

const hk_chr_metadata_t hk_chr_metadata_carbon_dioxide_detected = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_CARBON_DIOXIDE_DETECTED),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_carbon_dioxide_level = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_CARBON_DIOXIDE_LEVEL),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = false, .valid_values = false},
  .min.f = 0,
  .max.f = 100000};

const hk_chr_metadata_t hk_chr_metadata_carbon_dioxide_peak_level = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_CARBON_DIOXIDE_PEAK_LEVEL),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = false, .valid_values = false},
  .min.f = 0,
  .max.f = 100000};

const hk_chr_metadata_t hk_chr_metadata_air_quality = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_AIR_QUALITY),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 5,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_streaming_status = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_STREAMING_STATUS),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_TLV8,
  .unit = HK_UNIT_UNITLESS};

const hk_chr_metadata_t hk_chr_metadata_supported_video_stream_configuration = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_SUPPORTED_VIDEO_STREAM_CONFIGURATION),
  .permissions = {1, 0, 0, 0, 0, 0},
  .format = HK_VALUE_FORMAT_TLV8,
  .unit = HK_UNIT_UNITLESS};

const hk_chr_metadata_t hk_chr_metadata_supported_audio_stream_configuration = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_SUPPORTED_AUDIO_STREAM_CONFIGURATION),
  .permissions = {1, 0, 0, 0, 0, 0},
  .format = HK_VALUE_FORMAT_TLV8,
  .unit = HK_UNIT_UNITLESS};

const hk_chr_metadata_t hk_chr_metadata_supported_rtp_configuration = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_SUPPORTED_RTP_CONFIGURATION),
  .permissions = {1, 0, 0, 0, 0, 0},
  .format = HK_VALUE_FORMAT_TLV8,
  .unit = HK_UNIT_UNITLESS};

const hk_chr_metadata_t hk_chr_metadata_setup_endpoints = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_SETUP_ENDPOINTS),
  .permissions = {1, 1, 0, 0, 0, 0},
  .format = HK_VALUE_FORMAT_TLV8,
  .unit = HK_UNIT_UNITLESS};

const hk_chr_metadata_t hk_chr_metadata_selected_rtp_stream_configuration = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_SELECTED_RTP_STREAM_CONFIGURATION),
  .permissions = {0, 1, 0, 0, 0, 0},
  .format = HK_VALUE_FORMAT_TLV8,
  .unit = HK_UNIT_UNITLESS};

const hk_chr_metadata_t hk_chr_metadata_volume = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_VOLUME),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_PERCENTAGE,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 100,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_mute = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_MUTE),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_BOOL,
  .unit = HK_UNIT_UNITLESS};

const hk_chr_metadata_t hk_chr_metadata_night_vision = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_NIGHT_VISION),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_BOOL,
  .unit = HK_UNIT_UNITLESS};

const hk_chr_metadata_t hk_chr_metadata_optical_zoom = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_OPTICAL_ZOOM),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_UNITLESS};

const hk_chr_metadata_t hk_chr_metadata_digital_zoom = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_DIGITAL_ZOOM),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_UNITLESS};

static hk_value_union_t g_hk_valid_values_image_rotation[] = {
  {.f = 0.0},
  {.f = 90.0},
  {.f = 180.0},
  {.f = 270}};

const hk_chr_metadata_t hk_chr_metadata_image_rotation = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_IMAGE_ROTATION),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = false, .max = false, .step = false, .valid_values = true},
  .valid_values = g_hk_valid_values_image_rotation,
  .valid_values_size = 4};

const hk_chr_metadata_t hk_chr_metadata_image_mirroring = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_IMAGE_MIRRORING),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_BOOL,
  .unit = HK_UNIT_ARCDEGREES};

static hk_value_union_t g_hk_valid_values_accessory_flags[] = {
  {.u32 = 0x01}};

const hk_chr_metadata_t hk_chr_metadata_accessory_flags = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_ACCESSORY_FLAGS),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U32,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = false, .max = false, .step = false, .valid_values = true},
  .valid_values = g_hk_valid_values_accessory_flags,
  .valid_values_size = 1};

const hk_chr_metadata_t hk_chr_metadata_lock_physical_controls = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_LOCK_PHYSICAL_CONTROLS),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_current_air_purifier_state = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_CURRENT_AIR_PURIFIER_STATE),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 2,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_current_slat_state = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_CURRENT_SLAT_STATE),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 2,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_slat_type = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_SLAT_TYPE),
  .permissions = {1, 0, 0, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_filter_life_level = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_FILTER_LIFE_LEVEL),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.f = 0,
  .max.f = 100,
  .step.f = 1};

const hk_chr_metadata_t hk_chr_metadata_filter_change_indication = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_FILTER_CHANGE_INDICATION),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_reset_filter_indication = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_RESET_FILTER_INDICATION),
  .permissions = {0, 1, 0, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = false, .valid_values = false},
  .min.u8 = 1,
  .max.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_target_air_purifier_state = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_TARGET_AIR_PURIFIER_STATE),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_target_fan_state = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_TARGET_FAN_STATE),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_current_fan_state = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_CURRENT_FAN_STATE),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 2,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_active = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_ACTIVE),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_swing_mode = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_SWING_MODE),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_current_tilt_angle = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_CURRENT_TILT_ANGLE),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_I32,
  .unit = HK_UNIT_ARCDEGREES,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.i32 = -90,
  .max.i32 = 90,
  .step.i32 = 1};

const hk_chr_metadata_t hk_chr_metadata_target_tilt_angle = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_TARGET_TILT_ANGLE),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_I32,
  .unit = HK_UNIT_ARCDEGREES,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.i32 = -90,
  .max.i32 = 90,
  .step.i32 = 1};

const hk_chr_metadata_t hk_chr_metadata_ozone_density = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_OZONE_DENSITY),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = false, .valid_values = false},
  .min.f = 0,
  .max.f = 1000};

const hk_chr_metadata_t hk_chr_metadata_nitrogen_dioxide_density = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_NITROGEN_DIOXIDE_DENSITY),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = false, .valid_values = false},
  .min.f = 0,
  .max.f = 1000};

const hk_chr_metadata_t hk_chr_metadata_sulphur_dioxide_density = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_SULPHUR_DIOXIDE_DENSITY),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = false, .valid_values = false},
  .min.f = 0,
  .max.f = 1000};

const hk_chr_metadata_t hk_chr_metadata_pm_2_5_density = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_PM_2_5_DENSITY),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = false, .valid_values = false},
  .min.f = 0,
  .max.f = 1000};

const hk_chr_metadata_t hk_chr_metadata_pm_10_density = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_PM_10_DENSITY),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = false, .valid_values = false},
  .min.f = 0,
  .max.f = 1000};

const hk_chr_metadata_t hk_chr_metadata_voc_density = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_VOC_DENSITY),
  .permissions = {1, 0, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_FLOAT,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = false, .valid_values = false},
  .min.f = 0,
  .max.f = 1000};

const hk_chr_metadata_t hk_chr_metadata_service_label_index = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_SERVICE_LABEL_INDEX),
  .permissions = {1, 0, 0, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = false, .valid_values = false},
  .min.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_service_label_namespace = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_SERVICE_LABEL_NAMESPACE),
  .permissions = {1, 0, 0, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U8,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = false, .valid_values = false},
  .min.u8 = 0,
  .max.u8 = 1,
  .step.u8 = 1};

const hk_chr_metadata_t hk_chr_metadata_color_temperature = {
  .uuid = UINT16_TO_UUID(HK_CHARACTERISTIC_UUID_COLOR_TEMPERATURE),
  .permissions = {1, 1, 1, 0, 0, 0},
  .format = HK_VALUE_FORMAT_U32,
  .unit = HK_UNIT_UNITLESS,
  .validate = {.min = true, .max = true, .step = true, .valid_values = false},
  .min.u32 = 50,
  .max.u32 = 400,
  .step.u32 = 1};
/* clang-format on */

hk_chr_base_t *hk_chr_find_by_iid(const hk_acc_base_t *accessory, hk_instance_id_t iid)
{
    for (size_t i = 0; i < accessory->srv_size; ++i)
    {
        for (size_t j = 0; j < accessory->srv[i]->chr_size; ++j)
        {
            if (accessory->srv[i]->chr[j]->iid == iid)
            {
                return accessory->srv[i]->chr[j];
            }
        }
    }
    return NULL;
}

void hk_chr_print(hk_chr_base_t *chr)
{
    hk_console_printf("iid %d", chr->iid);
    hk_value_format_t format = chr->metadata->format;
    if (format == HK_VALUE_FORMAT_STR)
        hk_console_printf(" str : %s", chr->value.str);
    else if (format == HK_VALUE_FORMAT_BOOL)
        hk_console_printf(" bool : %d", chr->value.b);
}

void hk_srv_list(const hk_acc_base_t *accessory)
{
    hk_console_printf("\nAccessory aid %d\n", accessory->aid);
    for (size_t i = 0; i < accessory->srv_size; ++i)
    {
        hk_console_printf("  Service iid %d\n", accessory->srv[i]->iid);
        for (size_t j = 0; j < accessory->srv[i]->chr_size; ++j)
        {
            hk_console_printf("    ");
            hk_chr_print(accessory->srv[i]->chr[j]);
            hk_console_printf("\n");
        }
    }
}

static bool hk_chr_validate_by_min(const hk_value_t *value, const hk_chr_metadata_t *metadata)
{
    if (metadata->validate.min == false)
    {
        return true;
    }
    switch (metadata->format)
    {
        case HK_VALUE_FORMAT_NULL:
            return false;

        case HK_VALUE_FORMAT_BOOL:
            return true;

        case HK_VALUE_FORMAT_U8:
            return value->u8 >= metadata->min.u8;

        case HK_VALUE_FORMAT_U16:
            return value->u16 >= metadata->min.u16;

        case HK_VALUE_FORMAT_U32:
            return value->u32 >= metadata->min.u32;

        case HK_VALUE_FORMAT_U64:
            return value->u64 >= metadata->min.u64;

        case HK_VALUE_FORMAT_I32:
            return value->i32 >= metadata->min.i32;

        case HK_VALUE_FORMAT_FLOAT:
            return value->f >= metadata->min.f;

        case HK_VALUE_FORMAT_STR:
        case HK_VALUE_FORMAT_TLV8:
        case HK_VALUE_FORMAT_DATA:
            return true;
    }
    return false;
}

static bool hk_chr_validate_by_max(const hk_value_t *value, const hk_chr_metadata_t *metadata)
{
    if (metadata->validate.max == false)
    {
        return true;
    }
    switch (metadata->format)
    {
        case HK_VALUE_FORMAT_NULL:
            return false;

        case HK_VALUE_FORMAT_BOOL:
            return true;

        case HK_VALUE_FORMAT_U8:
            return value->u8 <= metadata->max.u8;

        case HK_VALUE_FORMAT_U16:
            return value->u16 <= metadata->max.u16;

        case HK_VALUE_FORMAT_U32:
            return value->u32 <= metadata->max.u32;

        case HK_VALUE_FORMAT_U64:
            return value->u64 <= metadata->max.u64;

        case HK_VALUE_FORMAT_I32:
            return value->i32 <= metadata->max.i32;

        case HK_VALUE_FORMAT_FLOAT:
            return value->f <= metadata->max.f;

        case HK_VALUE_FORMAT_STR:
        case HK_VALUE_FORMAT_TLV8:
        case HK_VALUE_FORMAT_DATA:
            return true;
    }
    return false;
}

static bool hk_chr_validate_by_step(const hk_value_t *value, const hk_chr_metadata_t *metadata)
{
    if (metadata->validate.step == false)
    {
        return true;
    }
    switch (metadata->format)
    {
        case HK_VALUE_FORMAT_NULL:
            return true;

        case HK_VALUE_FORMAT_BOOL:
            return true;

        case HK_VALUE_FORMAT_U8:
            return !((value->u8 - metadata->min.u8) % metadata->step.u8);

        case HK_VALUE_FORMAT_U16:
            return !((value->u16 - metadata->min.u16) % metadata->step.u16);

        case HK_VALUE_FORMAT_U32:
            return !((value->u32 - metadata->min.u32) % metadata->step.u32);

        case HK_VALUE_FORMAT_U64:
            return !((value->u64 - metadata->min.u64) % metadata->step.u64);

        case HK_VALUE_FORMAT_I32:
            return !((value->i32 - metadata->min.i32) % metadata->step.i32);

        case HK_VALUE_FORMAT_FLOAT:
        // TODO(SB): add implementation - return !((value->f - metadata->min.f) % metadata->step.f);

        case HK_VALUE_FORMAT_STR:
        case HK_VALUE_FORMAT_TLV8:
        case HK_VALUE_FORMAT_DATA:
            return true;
    }
    return false;
}

static bool hk_chr_validate_by_max_len(const hk_value_t *value, const hk_chr_metadata_t *metadata)
{
    switch (metadata->format)
    {
        case HK_VALUE_FORMAT_NULL:
            return false;

        case HK_VALUE_FORMAT_BOOL:
        case HK_VALUE_FORMAT_U8:
        case HK_VALUE_FORMAT_U16:
        case HK_VALUE_FORMAT_U32:
        case HK_VALUE_FORMAT_U64:
        case HK_VALUE_FORMAT_I32:
        case HK_VALUE_FORMAT_FLOAT:
            return true;

        case HK_VALUE_FORMAT_STR:
            if (value->str == NULL)
            {
                return true;
            }
            else
            {
                return strlen(value->str) <= metadata->max_len;
            }

        case HK_VALUE_FORMAT_TLV8:
            // TODO(SB): add implementation
            return true;

        case HK_VALUE_FORMAT_DATA:
            // TODO(SB): add implementation
            return true;
    }
    return false;
}

static bool hk_chr_validate_by_values(const hk_value_t *value, const hk_chr_metadata_t *metadata)
{
    if (metadata->validate.valid_values == false)
    {
        return true;
    }
    switch (metadata->format)
    {
        case HK_VALUE_FORMAT_NULL:
            return false;

        case HK_VALUE_FORMAT_BOOL:
            return true;

        case HK_VALUE_FORMAT_U8:
            for (size_t i = 0; i < metadata->valid_values_size; ++i)
            {
                if (metadata->valid_values[i].u8 == value->u8)
                {
                    return true;
                }
            }
            return false;

        case HK_VALUE_FORMAT_U16:
            for (size_t i = 0; i < metadata->valid_values_size; ++i)
            {
                if (metadata->valid_values[i].u16 == value->u16)
                {
                    return true;
                }
            }
            return false;

        case HK_VALUE_FORMAT_U32:
            for (size_t i = 0; i < metadata->valid_values_size; ++i)
            {
                if (metadata->valid_values[i].u32 == value->u32)
                {
                    return true;
                }
            }
            return false;

        case HK_VALUE_FORMAT_U64:
            for (size_t i = 0; i < metadata->valid_values_size; ++i)
            {
                if (metadata->valid_values[i].u64 == value->u64)
                {
                    return true;
                }
            }
            return false;

        case HK_VALUE_FORMAT_I32:
            for (size_t i = 0; i < metadata->valid_values_size; ++i)
            {
                if (metadata->valid_values[i].i32 == value->i32)
                {
                    return true;
                }
            }
            return false;

        case HK_VALUE_FORMAT_FLOAT:
        // TODO(SB): add implementation - return value->f >= metadata->max.f;

        case HK_VALUE_FORMAT_STR:
        case HK_VALUE_FORMAT_TLV8:
        case HK_VALUE_FORMAT_DATA:
            return true;
    }
    return false;
}

bool hk_chr_validate_value(const hk_value_t *value, const hk_chr_metadata_t *metadata)
{
    assert(value != NULL);
    assert(metadata != NULL);

    bool valid = true;
    int i = 0;
    valid &= hk_chr_validate_by_min(value, metadata);
    valid &= hk_chr_validate_by_max(value, metadata);
    valid &= hk_chr_validate_by_step(value, metadata);
    valid &= hk_chr_validate_by_max_len(value, metadata);
    valid &= hk_chr_validate_by_values(value, metadata);

    return valid;
}

bool hk_chr_compare_value_equal(const hk_value_t *value_1, const hk_value_t *value_2)
{
    if (value_1->format != value_2->format)
    {
        return false;
    }
    // TODO(SB): add implementation
    switch (value_1->format)
    {
        case HK_VALUE_FORMAT_NULL:
            break;

        case HK_VALUE_FORMAT_BOOL:
            break;

        case HK_VALUE_FORMAT_U8:
            break;

        case HK_VALUE_FORMAT_U16:
            break;

        case HK_VALUE_FORMAT_U32:
            break;

        case HK_VALUE_FORMAT_U64:
            break;

        case HK_VALUE_FORMAT_I32:
            break;

        case HK_VALUE_FORMAT_FLOAT:
            break;

        case HK_VALUE_FORMAT_STR:
            break;

        case HK_VALUE_FORMAT_TLV8:
            break;

        case HK_VALUE_FORMAT_DATA:
            break;
    }
    return true;
}

bool hk_chr_copy_value(hk_value_t *dst, const hk_value_t *src)
{
    if (dst->format == src->format)
    {
        memcpy(dst, src, sizeof(*dst));
        return true;
    }
    // TODO(SB): add implementation
    switch (dst->format)
    {
        case HK_VALUE_FORMAT_NULL:
            break;

        case HK_VALUE_FORMAT_BOOL:
        case HK_VALUE_FORMAT_U8:
        case HK_VALUE_FORMAT_U16:
        case HK_VALUE_FORMAT_U32:
        case HK_VALUE_FORMAT_U64:
        case HK_VALUE_FORMAT_I32:
            dst->i32 = src->i32;
            break;
        case HK_VALUE_FORMAT_FLOAT:
            dst->f = (float)src->i32;
            break;
        case HK_VALUE_FORMAT_STR:
            break;
        case HK_VALUE_FORMAT_TLV8:
            break;
        case HK_VALUE_FORMAT_DATA:
            break;
    }
    return true;
}

bool hk_chr_copy_metadata(hk_chr_metadata_t *dst, const hk_chr_metadata_t *src)
{
    memcpy(dst, src, sizeof(hk_chr_metadata_t));
    if (src->valid_values != NULL)
    {
        dst->valid_values = calloc(sizeof(hk_value_union_t), src->valid_values_size);
        if (dst->valid_values == NULL)
        {
            return false;
        }
        memcpy(dst->valid_values, src->valid_values, sizeof(hk_instance_id_t) * dst->valid_values_size);
    }

    return true;
}

hk_chr_base_t *hk_chr_new(void)
{
    return calloc(1, sizeof(hk_chr_base_t));
}

void hk_chr_delete(hk_chr_base_t *chr)
{
    if (chr == NULL)
    {
        return;
    }
    switch (chr->value.format)
    {
        case HK_VALUE_FORMAT_NULL:
        case HK_VALUE_FORMAT_BOOL:
        case HK_VALUE_FORMAT_U8:
        case HK_VALUE_FORMAT_U16:
        case HK_VALUE_FORMAT_U32:
        case HK_VALUE_FORMAT_U64:
        case HK_VALUE_FORMAT_I32:
        case HK_VALUE_FORMAT_FLOAT:
            // nothing to do more
            break;

        case HK_VALUE_FORMAT_STR:
            if (chr->value.str != NULL)
            {
                free(chr->value.str);
            }
            break;

        case HK_VALUE_FORMAT_TLV8:
            // TODO(SB): add implementation
            break;

        case HK_VALUE_FORMAT_DATA:
            // TODO(SB): add implementation
            break;
    }
    free(chr);
}

hk_chr_request_t *hk_chr_request_new(size_t size)
{
    hk_chr_request_t *r_chr = calloc(1, sizeof(hk_chr_request_t));
    if (r_chr == NULL)
        return NULL;
    r_chr->id = calloc(size, sizeof(hk_id_t));
    if (r_chr->id == NULL)
        return NULL;
    return r_chr;
}

void hk_chr_request_delete(hk_chr_request_t *r_chr)
{
    free(r_chr->id);
    free(r_chr);
}

bool hk_chr_respond_check_status(const hk_chr_respond_t const *chr, const hk_chr_request_t const *chr_request)
{
    for (size_t i = 0; i < chr_request->id_size; i++)
    {
        if (chr[i].status != 0)
            return true;
    }
    return false;
}
