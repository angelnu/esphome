import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ID, CONF_COMMAND
from . import SiedleInhomeBus, CONF_SIEDLE_INHOME_BUS_ID

DEPENDENCIES = ["siedle_inhome_bus"]

CONFIG_SCHEMA = (
    binary_sensor.binary_sensor_schema()
    .extend(
        {
            cv.GenerateID(CONF_SIEDLE_INHOME_BUS_ID): cv.use_id(SiedleInhomeBus),
            cv.Required(CONF_COMMAND): cv.All(cv.hex_int, cv.uint32_t),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_SIEDLE_INHOME_BUS_ID])
    var = cg.new_Pvariable(config[CONF_ID])
    await binary_sensor.register_binary_sensor(var, config)
    cg.add(parent.register_binary_sensor(config[CONF_COMMAND], var))