import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button
from esphome.const import CONF_ID, CONF_COMMAND, CONF_SOURCE
from . import SiedleInhomeBus, siedle_inhome_bus_ns, CONF_SIEDLE_INHOME_BUS_ID, CONF_SOURCE_BUS, CONF_DESTINATION, CONF_DESTINATION_BUS

DEPENDENCIES = ["siedle_inhome_bus"]

ButtonClass = siedle_inhome_bus_ns.class_("SiedleInhomeBusButton", button.Button, cg.Component)

CONFIG_SCHEMA = (
    button.button_schema(ButtonClass)
    .extend(cv.COMPONENT_SCHEMA)
    .extend(
        {
            cv.GenerateID(CONF_SIEDLE_INHOME_BUS_ID): cv.use_id(SiedleInhomeBus),
            cv.Required(CONF_COMMAND): cv.All(cv.hex_int_range(max=63), cv.uint32_t),
            cv.Required(CONF_DESTINATION): cv.All(cv.hex_int_range(max=31), cv.uint8_t),
            cv.Optional(CONF_DESTINATION_BUS, default=0b1000): cv.All(cv.hex_int_range(max=15), cv.uint8_t),
            cv.Required(CONF_SOURCE): cv.All(cv.hex_int_range(max=31), cv.uint8_t),
            cv.Optional(CONF_SOURCE_BUS, default=0b1000): cv.All(cv.hex_int_range(max=15), cv.uint8_t),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    parent = await cg.get_variable(config[CONF_SIEDLE_INHOME_BUS_ID])
    cg.add(var.set_parent(parent))
    cg.add(var.set_command(config[CONF_COMMAND], config[CONF_DESTINATION], config[CONF_DESTINATION_BUS], config[CONF_SOURCE], config[CONF_SOURCE_BUS]))

    await cg.register_component(var, config)
    await button.register_button(var, config)
