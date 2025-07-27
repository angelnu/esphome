import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button
from esphome.const import CONF_ID, CONF_MESSAGE
from . import SiedleInhomeBus, siedle_inhome_bus_ns, CONF_SIEDLE_INHOME_BUS_ID, CONFIG_MESSAGE_SCHEMA, new_Pmessage

DEPENDENCIES = ["siedle_inhome_bus"]

ButtonClass = siedle_inhome_bus_ns.class_("SiedleInhomeBusButton", button.Button, cg.Component)

CONFIG_SCHEMA = (
    button.button_schema(ButtonClass)
    .extend(cv.COMPONENT_SCHEMA)
    .extend(
        {
            cv.GenerateID(CONF_SIEDLE_INHOME_BUS_ID): cv.use_id(SiedleInhomeBus),
            cv.Required(CONF_MESSAGE): CONFIG_MESSAGE_SCHEMA,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_SIEDLE_INHOME_BUS_ID])
    msg = new_Pmessage(config[CONF_MESSAGE])
    var = cg.new_Pvariable(config[CONF_ID])
    cg.add(var.set_parent(parent))
    cg.add(var.set_message(msg))

    await cg.register_component(var, config)
    await button.register_button(var, config)
