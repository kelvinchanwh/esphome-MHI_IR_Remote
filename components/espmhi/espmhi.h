#pragma once

#include "esphome/components/climate_ir/climate_ir.h"

namespace esphome {
namespace espmhi {

// Temperature
const uint8_t ESPMHI_TEMP_MIN = 16;  // Celsius
const uint8_t ESPMHI_TEMP_MAX = 31;  // Celsius

class espmhiClimate : public climate_ir::ClimateIR {
 public:
  espmhiClimate() : climate_ir::ClimateIR(ESPMHI_TEMP_MIN, ESPMHI_TEMP_MAX) {}

 protected:
  /// Transmit via IR the state of this climate controller.
  void transmit_state() override;
};

}  // namespace espmhi
}  // namespace esphome