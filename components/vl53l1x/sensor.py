import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    CONF_ID,
    STATE_CLASS_MEASUREMENT,
    ICON_ARROW_EXPAND_VERTICAL,
    CONF_ADDRESS,
    CONF_TIMEOUT,
    CONF_ENABLE_PIN,
    CONF_SPEED,
    CONF_ACCELERATION,
    ICON_ACCELERATION,
    UNIT_METER_PER_SECOND_SQUARED
)
from esphome import pins

DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["i2c_platform"]
cg.add_library("file:///config/libraries/VL53L1X", None)

vl53l1x_ns = cg.esphome_ns.namespace("vl53l1x")
VL53L1XSensor = vl53l1x_ns.class_(
    "VL53L1XSensor", sensor.Sensor, cg.PollingComponent, i2c.I2CDevice
)

UNIT_MILIMETER = "mm"
ICON_LIDAR_DISTANCE = "mdi:signal-distance-variant"
CONF_INTERRUPT_PIN = "interrupt_pin"
CONF_LONG_RANGE = "long_range"
CONF_ERROR = "errors"
ICON_SPEED ="mdi:speedometer"
UNIT_METER_PER_SECOND = "m/s"


def check_keys(obj):
    if obj[CONF_ADDRESS] != 0x29 and CONF_ENABLE_PIN not in obj:
        msg = "Address other then 0x29 requires enable_pin definition to allow sensor\r"
        msg += "re-addressing. Also if you have more then one VL53 device on the same\r"
        msg += "i2c bus, then all VL53 devices must have enable_pin defined."
        raise cv.Invalid(msg)
    return obj


CONFIG_SCHEMA = cv.All(
    sensor.sensor_schema(
        unit_of_measurement=UNIT_MILIMETER,
        icon=ICON_LIDAR_DISTANCE,
        accuracy_decimals=0,
        state_class=STATE_CLASS_MEASUREMENT,
    )
    .extend(
        {
            cv.GenerateID(): cv.declare_id(VL53L1XSensor),
            cv.Optional(CONF_LONG_RANGE, default=True): cv.boolean,
            cv.Optional(CONF_ENABLE_PIN): pins.gpio_output_pin_schema,
            cv.Optional(CONF_INTERRUPT_PIN): pins.gpio_input_pin_schema,
            cv.Optional(CONF_SPEED): sensor.sensor_schema(
                unit_of_measurement=UNIT_METER_PER_SECOND,
                icon=ICON_SPEED,
                accuracy_decimals=2,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_ACCELERATION): sensor.sensor_schema(
                unit_of_measurement=UNIT_METER_PER_SECOND_SQUARED ,
                icon=ICON_ACCELERATION,
                accuracy_decimals=2,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_ERROR): sensor.sensor_schema(
                #unit_of_measurement=UNIT_METER_PER_SECOND_SQUARED ,
                #icon=ICON_ACCELERATION,
                accuracy_decimals=0,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
        }
    )
    .extend(cv.polling_component_schema("1s"))
    .extend(i2c.i2c_device_schema(0x29)),
    check_keys,
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    cg.add(var.set_long_range(config[CONF_LONG_RANGE]))

    if CONF_ENABLE_PIN in config:
        enable = await cg.gpio_pin_expression(config[CONF_ENABLE_PIN])
        cg.add(var.set_enable_pin(enable))
    if CONF_INTERRUPT_PIN in config:
        interrupt = await cg.gpio_pin_expression(config[CONF_INTERRUPT_PIN])
        cg.add(var.set_interrupt_pin(interrupt))

    await sensor.register_sensor(var, config)
    await i2c.register_i2c_device(var, config)

    if CONF_SPEED in config:
        sens = await sensor.new_sensor(config[CONF_SPEED])
        cg.add(var.set_speed_sensor(sens))

    if CONF_ACCELERATION in config:
        sens = await sensor.new_sensor(config[CONF_ACCELERATION])
        cg.add(var.set_acceleration_sensor(sens))

    if CONF_ERROR in config:
        sens = await sensor.new_sensor(config[CONF_ERROR])
        cg.add(var.set_error_sensor(sens))