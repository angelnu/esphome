from esphome import pins
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_SENSING_PIN, CONF_RX_PIN, CONF_TX_PIN

siedle_inhome_bus_ns = cg.esphome_ns.namespace("siedle_inhome_bus")
SiedleInhomeBus = siedle_inhome_bus_ns.class_("SiedleInhomeBus", cg.Component)

CONF_CARRIER_PIN = "carrier_pin"
CONF_LOAD_PIN = "load_pin"
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(SiedleInhomeBus),
        cv.Required(CONF_CARRIER_PIN): pins.internal_gpio_input_pin_schema,
        cv.Required(CONF_RX_PIN): pins.internal_gpio_input_pin_schema,
        cv.Required(CONF_LOAD_PIN): pins.internal_gpio_output_pin_schema,
        cv.Required(CONF_TX_PIN): pins.internal_gpio_output_pin_schema,
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    pin = await cg.gpio_pin_expression(config[CONF_CARRIER_PIN])
    cg.add(var.set_carrier_pin(pin))
    pin = await cg.gpio_pin_expression(config[CONF_RX_PIN])
    cg.add(var.set_rx_pin(pin))
    pin = await cg.gpio_pin_expression(config[CONF_LOAD_PIN])
    cg.add(var.set_load_pin(pin))
    pin = await cg.gpio_pin_expression(config[CONF_TX_PIN])
    cg.add(var.set_tx_pin(pin))