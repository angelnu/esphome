import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ID, CONF_MESSAGE
from . import SiedleInhomeBus, CONF_SIEDLE_INHOME_BUS_ID, CONFIG_MESSAGE_SCHEMA, new_Pmessage

DEPENDENCIES = ["siedle_inhome_bus"]

CONFIG_SCHEMA = (
    binary_sensor.binary_sensor_schema()
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
    await binary_sensor.register_binary_sensor(var, config)
    cg.add(parent.register_binary_sensor(var, msg))