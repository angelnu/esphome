import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import i2c
from esphome.const import CONF_ID
from esphome.core import TimePeriod

CODEOWNERS = ["@jasonwbarnett"]
DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["sensor"]
# Allow several instances (one per I2C bus / sensor), validated and code-genned
# per entry: needed for the trashcan's two VL53L1X on separate buses.
MULTI_CONF = True

CONF_VL53L1X_POLOLU_ID = "vl53l1x_pololu_id"
CONF_DISTANCE_MODE = "distance_mode"
CONF_TIMING_BUDGET = "timing_budget"
CONF_INTER_MEASUREMENT = "inter_measurement"
CONF_ENABLE_PIN = "enable_pin"

vl53l1x_pololu_ns = cg.esphome_ns.namespace("vl53l1x_pololu")
VL53L1XPololuComponent = vl53l1x_pololu_ns.class_(
    "VL53L1XPololuComponent", cg.PollingComponent, i2c.I2CDevice
)

# Maps to the Pololu VL53L1X::DistanceMode enum (Short=0, Medium=1, Long=2).
DISTANCE_MODES = {"short": 0, "medium": 1, "long": 2}


def _validate(config):
    tb_ms = config[CONF_TIMING_BUDGET].total_milliseconds
    im_ms = config[CONF_INTER_MEASUREMENT].total_milliseconds
    # ST UM2356 sec 2.2: the inter-measurement period must be longer than the
    # timing budget + 4 ms (the sensor's own API rejects anything else).
    if im_ms <= tb_ms + 4:
        raise cv.Invalid(
            f"inter_measurement ({im_ms}ms) must be greater than "
            f"timing_budget + 4ms (> {tb_ms + 4}ms) per ST UM2356"
        )
    return config


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(VL53L1XPololuComponent),
            cv.Optional(CONF_DISTANCE_MODE, default="long"): cv.enum(
                DISTANCE_MODES, lower=True
            ),
            cv.Optional(CONF_TIMING_BUDGET, default="50ms"): cv.All(
                cv.positive_time_period_microseconds,
                cv.Range(
                    min=TimePeriod(microseconds=20000),
                    max=TimePeriod(microseconds=1000000),
                ),
            ),
            cv.Optional(CONF_INTER_MEASUREMENT, default="100ms"): cv.All(
                cv.positive_time_period_milliseconds,
                cv.Range(
                    min=TimePeriod(milliseconds=20),
                    max=TimePeriod(milliseconds=5000),
                ),
            ),
            # XSHUT: held high in setup() to boot the sensor (boards that wire it).
            cv.Optional(CONF_ENABLE_PIN): pins.gpio_output_pin_schema,
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x29)),
    _validate,
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    if CONF_ENABLE_PIN in config:
        cg.add(var.set_enable_pin(await cg.gpio_pin_expression(config[CONF_ENABLE_PIN])))
    cg.add(var.set_distance_mode(config[CONF_DISTANCE_MODE]))
    cg.add(var.set_timing_budget_us(config[CONF_TIMING_BUDGET].total_microseconds))
    cg.add(var.set_inter_measurement_ms(config[CONF_INTER_MEASUREMENT].total_milliseconds))
