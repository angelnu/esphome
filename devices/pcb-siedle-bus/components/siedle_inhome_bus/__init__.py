from esphome import pins
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_SENSING_PIN, CONF_RX_PIN, CONF_TX_PIN, CONF_DUMP, CONF_COMMAND, CONF_SOURCE

MULTI_CONF = True
CONF_SIEDLE_INHOME_BUS_ID = "siedle_inhome_bus_id"
AUTO_LOAD = ["binary_sensor", "button"]

siedle_inhome_bus_ns = cg.esphome_ns.namespace("siedle_inhome_bus")
SiedleInhomeBus = siedle_inhome_bus_ns.class_("SiedleInhomeBus", cg.Component)

SiedleInhomeBusMessage = siedle_inhome_bus_ns.class_("SiedleInhomeBusMessage")

CONF_SOURCE_BUS = "source_bus"
CONF_DESTINATION = "destination"
CONF_DESTINATION_BUS = "destination_bus"

CONF_CARRIER_PIN = "carrier_pin"
CONF_LOAD_PIN = "load_pin"
CONFIG_MESSAGE_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(SiedleInhomeBusMessage),
        cv.Required(CONF_COMMAND): cv.All(cv.hex_int_range(max=63), cv.uint32_t),
        cv.Required(CONF_DESTINATION): cv.All(cv.hex_int_range(max=31), cv.uint8_t),
        cv.Optional(CONF_DESTINATION_BUS, default=0b1000): cv.All(cv.hex_int_range(max=15), cv.uint8_t),
        cv.Required(CONF_SOURCE): cv.All(cv.hex_int_range(max=31), cv.uint8_t),
        cv.Optional(CONF_SOURCE_BUS, default=0b1000): cv.All(cv.hex_int_range(max=15), cv.uint8_t),
    }
)

def new_Pmessage(config):
    return cg.new_Pvariable(
        config[CONF_ID],
        config[CONF_COMMAND],
        config[CONF_DESTINATION],
        config[CONF_DESTINATION_BUS],
        config[CONF_SOURCE],
        config[CONF_SOURCE_BUS],
    )

CONF_CARRIER_PIN = "carrier_pin"
CONF_LOAD_PIN = "load_pin"
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(SiedleInhomeBus),
        cv.Required(CONF_CARRIER_PIN): pins.internal_gpio_input_pin_schema,
        cv.Required(CONF_RX_PIN): pins.internal_gpio_input_pin_schema,
        cv.Required(CONF_LOAD_PIN): pins.internal_gpio_output_pin_schema,
        cv.Required(CONF_TX_PIN): pins.internal_gpio_output_pin_schema,
        cv.Optional(CONF_DUMP, default=True): cv.boolean,
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
    cg.add(var.set_dump(config[CONF_DUMP]))