from esphome import automation, pins
import esphome.codegen as cg
from esphome.components import output
import esphome.config_validation as cv
from esphome.const import (
    CONF_CHANNEL,
    CONF_FREQUENCY,
    CONF_ID,
    CONF_PHASE_ANGLE,
    CONF_PIN,
)

DEPENDENCIES = ["esp32"]


mcpwm_ns = cg.esphome_ns.namespace("mcpwm")
MCPWMOutput = mcpwm_ns.class_("MCPWMOutput", output.FloatOutput, cg.Component)

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend(
    {
        cv.Required(CONF_ID): cv.declare_id(MCPWMOutput),
        cv.Required(CONF_PIN): pins.internal_gpio_output_pin_schema,
        cv.Optional(CONF_FREQUENCY, default="1kHz"): cv.frequency,
        cv.Optional(CONF_CHANNEL): cv.int_range(min=0, max=1),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    gpio = await cg.gpio_pin_expression(config[CONF_PIN])
    var = cg.new_Pvariable(config[CONF_ID], gpio)
    await cg.register_component(var, config)
    await output.register_output(var, config)
    if CONF_CHANNEL in config:
        cg.add(var.set_channel(config[CONF_CHANNEL]))
    cg.add(var.set_frequency(config[CONF_FREQUENCY]))
