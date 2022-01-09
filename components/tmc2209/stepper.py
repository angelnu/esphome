from esphome import pins
from esphome.components import stepper, uart, cover
import esphome.config_validation as cv
from esphome import automation
import esphome.codegen as cg
from esphome.core import CORE
from esphome.const import CONF_ID, CONF_ENABLE_PIN, CONF_STEP_PIN, CONF_DIR_PIN, CONF_INVERTED

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["cover"]
cg.add_library("SPI", None) #not used but included by TMCStepper lib
cg.add_library("teemuatlut/TMCStepper@^0.7.3", None)

CONF_INDEX_PIN = "index_pin"
CONF_DIAG_PIN = "diag_pin"
CONF_FREEWHEELING_MODE = "freewheeling_mode"
CONF_DRIVER_ADDRESS = "driver_address"
CONF_R_SENSE = "r_sense"
CONF_STALL_THRESHOLD = "stall_threshold"
CONF_MICROSTEPS = "microsteps"
CONF_RMS_CURRENT = "rms_current"
CONF_COVER = "cover"

tmc2209_ns = cg.esphome_ns.namespace("tmc2209")
TMC2209 = tmc2209_ns.class_("TMC2209", stepper.Stepper, uart.UARTDevice, cg.Component)
StepperCover = tmc2209_ns.class_("StepperCover", cover.Cover, cg.PollingComponent)

CalibrateAction = tmc2209_ns.class_("CalibrateAction", automation.Action)


def validate_index_counter_pin(value):
    value = pins.internal_gpio_input_pin_schema(value)
    if CORE.is_esp8266 and value[CONF_NUMBER] >= 16:
        raise cv.Invalid(
            "Pins GPIO16 and GPIO17 cannot be used as index counters on ESP8266."
        )
    return value

def validate_driver_address(value):
    if value < 0 or value > 3:
        raise cv.Invalid(
            "Driver address must be 0 to 3 according to MS1 and MS2"
        )
    return value

CONFIG_SCHEMA = cv.All(
    stepper.STEPPER_SCHEMA.extend ({
        cv.Required(CONF_ID): cv.declare_id(TMC2209),
        # Match to your driver
        # SilentStepStick series use 0.11
        # UltiMachine Einsy and Archim2 boards use 0.2
        # Panucatt BSD2660 uses 0.1
        # Watterott TMC5160 uses 0.075
        cv.Required(CONF_R_SENSE): cv.float_range(min=0,max=1),
        cv.Required(CONF_ENABLE_PIN): pins.gpio_output_pin_schema,
        cv.Optional(CONF_INDEX_PIN): validate_index_counter_pin,
        cv.Optional(CONF_DIAG_PIN): pins.gpio_input_pin_schema,
        cv.Optional(CONF_STEP_PIN): pins.gpio_output_pin_schema,
        cv.Optional(CONF_DIR_PIN): pins.gpio_output_pin_schema,
        cv.Optional(CONF_FREEWHEELING_MODE, default="normal"): cv.enum({"normal": 0, "freewheeling": 1, "shorted_coil_ls": 2, "shorted_coil_hs": 3}),
        cv.Optional(CONF_INVERTED, False): cv.boolean,
        cv.Optional(CONF_DRIVER_ADDRESS, default=0): validate_driver_address,
        # StallGuard4 threshold [0... 255] level for stall detection. It compensates for
        # motor specific characteristics and controls sensitivity. A higher value gives a higher
        # sensitivity. A higher value makes StallGuard4 more sensitive and requires less torque to
        # indicate a stall. The double of this value is compared to SG_RESULT.
        # The stall output becomes active if SG_RESULT fall below this value.
        cv.Optional(CONF_STALL_THRESHOLD, default=0): cv.int_range(min=0,max=255),
        cv.Optional(CONF_MICROSTEPS, default=0): cv.one_of(0,2,4,8,16,32,64,128,256, int=True),
        cv.Optional(CONF_RMS_CURRENT, default="0.8 A"): cv.current,
        cv.Optional(CONF_COVER): cover.COVER_SCHEMA.extend({
                cv.GenerateID(): cv.declare_id(StepperCover),
        })       
        .extend(cv.polling_component_schema("1s"))
    })
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await stepper.register_stepper(var, config)
    await uart.register_uart_device(var, config)

    cg.add(var.set_r_sense(config[CONF_R_SENSE]))
    cg.add(var.set_freewheeling_mode(config[CONF_FREEWHEELING_MODE]))
    cg.add(var.set_inverted(config[CONF_INVERTED]))

    enable_pin = await cg.gpio_pin_expression(config[CONF_ENABLE_PIN])
    cg.add(var.set_enable_pin(enable_pin))

    if CONF_INDEX_PIN in config:
        index_pin = await cg.gpio_pin_expression(config[CONF_INDEX_PIN])
        cg.add(var.set_index_pin(index_pin))

    if CONF_DIAG_PIN in config:
        diag_pin = await cg.gpio_pin_expression(config[CONF_DIAG_PIN])
        cg.add(var.set_diag_pin(diag_pin))
    
    if CONF_STEP_PIN in config:
        step_pin = await cg.gpio_pin_expression(config[CONF_STEP_PIN])
        cg.add(var.set_step_pin(step_pin))
    
    if CONF_DIR_PIN in config:
        dir_pin = await cg.gpio_pin_expression(config[CONF_DIR_PIN])
        cg.add(var.set_dir_pin(dir_pin))
    
    cg.add(var.set_driver_address(config[CONF_DRIVER_ADDRESS]))
    cg.add(var.set_stall_threshold(config[CONF_STALL_THRESHOLD]))
    cg.add(var.set_microsteps(config[CONF_MICROSTEPS]))
    cg.add(var.set_rms_current(config[CONF_RMS_CURRENT]))

    if CONF_COVER in config:
        config_cover = config[CONF_COVER]
        cover_var = cg.new_Pvariable(config_cover[CONF_ID])
        await cg.register_component(cover_var, config_cover)
        await cover.register_cover(cover_var, config_cover)

        cg.add(cover_var.set_stepper(var))

@automation.register_action(
    "stepper.calibrate",
    CalibrateAction,
    automation.maybe_simple_id(
        {
            cv.Required(CONF_ID): cv.use_id(TMC2209),
        }
    ) ,
)
async def stepper_calibrate_to_code(config, action_id, template_arg, args):
    parent = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, parent)
    return var