#pragma once

#include "esphome/components/climate_ir/climate_ir.h"

namespace esphome {
namespace espmhi {


// Mitsubishi Heavy timing constants
const uint16_t ESPMHI_HEADER_MARK         =3200; // 3200
const uint16_t ESPMHI_HEADER_SPACE        =1600; // 1600
const uint16_t ESPMHI_BIT_MARK         =400 ; // 400
const uint16_t ESPMHI_ONE_SPACE        =1200; // 1200
const uint16_t ESPMHI_ZERO_SPACE       =400 ; // 400
const uint16_t ESPMHI_MIN_GAP       =17500 ; // 17500


// Mitsubishi Heavy codes
// Auto:101/Heat:110/Cool:011
// const uint8_t ESPMHI_MODE_AUTO         =0x05;// 0x07?
// const uint8_t ESPMHI_MODE_HEAT         =0x03;
const uint8_t ESPMHI_MODE_COOL         =0x06;
// const uint8_t ESPMHI_MODE_DRY          =0x07;// 0x05?
// const uint8_t ESPMHI_MODE_FAN          =0x04;

const uint32_t ESPMHI_MODE_OFF          =0x08; // Power OFF
// const uint32_t ESPMHI_MODE_ON           =0x00; // Power ON

const uint8_t ESPMHI_FAN_AUTO       =0xE0; // Fan speed
const uint8_t ESPMHI_FAN1           =0xA0;
const uint8_t ESPMHI_FAN2           =0x80;
const uint8_t ESPMHI_FAN3           =0x60;
// const uint8_t ESPMHI_HIPOWER        =0x40; 
// const uint8_t ESPMHI_ECONO          =0x00;

const uint8_t ESPMHI_CLEAN_ON       =0x00;
const uint8_t ESPMHI_CLEAN_OFF      =0x20;

// const uint8_t ESPMHI_SILENT_ON      =0x00;

const uint8_t ESPMHI_VS_SWING       =0x0A; // Vertical swing
const uint8_t ESPMHI_VS_UP          =0x02;
const uint8_t ESPMHI_VS_MUP         =0x18;
const uint8_t ESPMHI_VS_MIDDLE      =0x10;
const uint8_t ESPMHI_VS_MDOWN       =0x08;
const uint8_t ESPMHI_VS_DOWN        =0x00;
const uint8_t ESPMHI_VS_STOP        =0x1A;

const uint8_t ESPMHI_HS_SWING       =0x4C; // Horizontal swing - 3D AUTO
const uint8_t ESPMHI_HS_MIDDLE      =0x48;
const uint8_t ESPMHI_HS_LEFT        =0xC8;
const uint8_t ESPMHI_HS_MLEFT       =0x88;
const uint8_t ESPMHI_HS_MRIGHT      =0x08;
const uint8_t ESPMHI_HS_RIGHT       =0xC4;
const uint8_t ESPMHI_HS_STOP        =0xCC;
// const uint8_t ESPMHI_HS_LEFTRIGHT   =0x84;
// const uint8_t ESPMHI_HS_RIGHTLEFT   =0x44;
// const uint8_t ESPMHI_HS_3DAUTO      =0x04;


// Temperature
const uint8_t ESPMHI_TEMP_MIN = 18;  // Celsius
const uint8_t ESPMHI_TEMP_MAX = 30;  // Celsius

class espmhiClimate : public climate_ir::ClimateIR {
 public:
  espmhiClimate() : climate_ir::ClimateIR(ESPMHI_TEMP_MIN, ESPMHI_TEMP_MAX) {}
  void set_horizontal_default(HorizontalDirection horizontal_direction) {
    this->default_horizontal_direction_ = horizontal_direction;
  }
  void set_vertical_default(VerticalDirection vertical_direction) {
    this->default_vertical_direction_ = vertical_direction;
  }
  void set_clean_default(CleanValues clean_values) {
    this->default_clean_values_ = clean_values;
  }
  void set_fan_default(FanValues fan_values) {
    this->default_fan_values_ = fan_values;
  }

 protected:
  /// Transmit via IR the state of this climate controller.
  void transmit_state() override;
  HorizontalDirection default_horizontal_direction_;
  VerticalDirection default_vertical_direction_;
  CleanValues default_clean_values_;
  FanValues default_fan_values_;
};

}  // namespace espmhi
}  // namespace esphome