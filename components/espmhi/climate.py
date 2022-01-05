import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate_ir
from esphome.const import CONF_ID

AUTO_LOAD = ["climate_ir"]

espmhi_ns = cg.esphome_ns.namespace("espmhi")
espmhiClimate = espmhi_ns.class_("espmhiClimate", climate_ir.ClimateIR)

CONF_HORIZONTAL_DEFAULT = "horizontal_default"
HorizontalDirections = espmhi_ns.enum("HorizontalDirections")
HORIZONTAL_DIRECTIONS = {
    "swing": HorizontalDirections.ESPMHI_HS_SWING,
    "middle": HorizontalDirections.ESPMHI_HS_MIDDLE,
    "left": HorizontalDirections.ESPMHI_HS_LEFT,
    "mleft": HorizontalDirections.ESPMHI_HS_MLEFT,
    "mright": HorizontalDirections.ESPMHI_HS_MRIGHT,
    "right": HorizontalDirections.ESPMHI_HS_RIGHT,
    "stop": HorizontalDirections.ESPMHI_HS_STOP,
}

CONF_VERTICAL_DEFAULT = "vertical_default"
VerticalDirections = espmhi_ns.enum("VerticalDirections")
VERTICAL_DIRECTIONS = {
    "swing": VerticalDirections.ESPMHI_VS_SWING,
    "up": VerticalDirections.ESPMHI_VS_UP,
    "mup": VerticalDirections.ESPMHI_VS_MUP,
    "middle": VerticalDirections.ESPMHI_VS_MIDDLE,
    "mdown": VerticalDirections.ESPMHI_VS_MDOWN,
    "down": VerticalDirections.ESPMHI_VS_DOWN,
    "stop": VerticalDirections.ESPMHI_VS_STOP,
}

CONF_CLEAN_DEFAULT = "clean_default"
CleanValues = espmhi_ns.enum("CleanValues")
CLEAN_VALUES = {
    "on": CleanValues.ESPMHI_CLEAN_ON,
    "off": CleanValues.ESPMHI_CLEAN_OFF,
}

CONF_FAN_DEFAULT = "fan_default"
FanValues = espmhi_ns.enum("FanValues")
FAN_VALUES = {
    "auto": FanValues.ESPMHI_FAN_AUTO,
    "low": FanValues.ESPMHI_FAN1,
    "medium": FanValues.ESPMHI_FAN2,
    "high": FanValues.ESPMHI_FAN3,
}

CONFIG_SCHEMA = climate_ir.CLIMATE_IR_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(espmhiClimate),
        cv.Required(CONF_HORIZONTAL_DEFAULT): cv.enum(HORIZONTAL_DIRECTIONS),
        cv.Required(CONF_VERTICAL_DEFAULT): cv.enum(VERTICAL_DIRECTIONS),
        cv.Required(CONF_CLEAN_DEFAULT): cv.enum(CLEAN_VALUES),
        cv.Required(CONF_FAN_DEFAULT): cv.enum(FAN_VALUES),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await climate_ir.register_climate_ir(var, config)
    cg.add(var.set_horizontal_default(config[CONF_HORIZONTAL_DEFAULT]))
    cg.add(var.set_vertical_default(config[CONF_VERTICAL_DEFAULT]))
    cg.add(var.set_protocol(config[CONF_CLEAN_DEFAULT]))
    cg.add(var.set_protocol(config[CONF_FAN_DEFAULT]))
