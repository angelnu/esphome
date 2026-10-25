import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_DISTANCE,
    DEVICE_CLASS_DISTANCE,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_TOTAL_INCREASING,
    ENTITY_CATEGORY_DIAGNOSTIC,
)

from . import VL53L1XPololuComponent, CONF_VL53L1X_POLOLU_ID

DEPENDENCIES = ["vl53l1x_pololu"]

CONF_RANGE_STATUS = "range_status"
CONF_SIGNAL_RATE = "signal_rate"
CONF_AMBIENT_RATE = "ambient_rate"
CONF_FRAMES = "frames"
CONF_RECOVERY_COUNT = "recovery_count"
CONF_SPEED = "speed"
CONF_ACCELERATION = "acceleration"

UNIT_MCPS = "MCPS"  # mega counts per second

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_VL53L1X_POLOLU_ID): cv.use_id(VL53L1XPololuComponent),
        cv.Required(CONF_DISTANCE): sensor.sensor_schema(
            unit_of_measurement="mm",
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_DISTANCE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        # Range status of the latest measurement (0 = valid; see RangeStatus).
        cv.Optional(CONF_RANGE_STATUS): sensor.sensor_schema(
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        # Returned signal rate (target reflectance/quality). Drops with weak/far targets.
        cv.Optional(CONF_SIGNAL_RATE): sensor.sensor_schema(
            unit_of_measurement=UNIT_MCPS,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        # Ambient light rate. Rises in bright conditions — useful for diagnosing
        # ambient-light range loss (long-mode max range collapses under strong light).
        cv.Optional(CONF_AMBIENT_RATE): sensor.sensor_schema(
            unit_of_measurement=UNIT_MCPS,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        # Total ranging frames since boot. Climbs steadily while ranging is healthy.
        cv.Optional(CONF_FRAMES): sensor.sensor_schema(
            accuracy_decimals=0,
            state_class=STATE_CLASS_TOTAL_INCREASING,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        # Total in-place freeze recoveries since boot. Flat = healthy.
        cv.Optional(CONF_RECOVERY_COUNT): sensor.sensor_schema(
            accuracy_decimals=0,
            state_class=STATE_CLASS_TOTAL_INCREASING,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        # Derived speed/acceleration (ported from the original custom vl53l1x).
        cv.Optional(CONF_SPEED): sensor.sensor_schema(
            unit_of_measurement="m/s",
            icon="mdi:speedometer",
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_ACCELERATION): sensor.sensor_schema(
            unit_of_measurement="m/s\u00b2",
            icon="mdi:chart-line-variant",
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
    }
)

_SETTERS = {
    CONF_RANGE_STATUS: "set_range_status_sensor",
    CONF_SIGNAL_RATE: "set_signal_rate_sensor",
    CONF_AMBIENT_RATE: "set_ambient_rate_sensor",
    CONF_FRAMES: "set_frames_sensor",
    CONF_RECOVERY_COUNT: "set_recovery_count_sensor",
    CONF_SPEED: "set_speed_sensor",
    CONF_ACCELERATION: "set_acceleration_sensor",
}


async def to_code(config):
    hub = await cg.get_variable(config[CONF_VL53L1X_POLOLU_ID])
    cg.add(hub.set_distance_sensor(await sensor.new_sensor(config[CONF_DISTANCE])))
    for key, setter in _SETTERS.items():
        if conf := config.get(key):
            cg.add(getattr(hub, setter)(await sensor.new_sensor(conf)))
