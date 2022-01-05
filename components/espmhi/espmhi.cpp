#include "espmhi.h"
#include "esphome/core/log.h"

namespace esphome {
namespace espmhi {

static const char *const TAG = "espmhi.climate";

void espmhiClimate::transmit_state() {
  uint8_t powerMode     = ESPMHI_MODE_ON;
  uint8_t operatingMode = ESPMHI_MODE_COOL;
  uint8_t fanSpeed      = ESPMHI_FAN_AUTO;
  uint8_t temperature   = 25;
  uint8_t swingV        = ESPMHI_VS_STOP;
  uint8_t swingH        = ESPMHI_HS_STOP;
  uint8_t cleanMode     = ESPMHI_CLEAN_OFF;

  switch (this->mode) {
    case climate::CLIMATE_MODE_COOL:
      operatingMode = ESPMHI_MODE_COOL;
      break;
    case climate::CLIMATE_MODE_FAN_ONLY:
      operatingMode = ESPMHI_MODE_FAN;
      break;
    case climate::CLIMATE_MODE_AUTO:
      operatingMode = ESPMHI_MODE_AUTO;
      break;
    case climate::CLIMATE_MODE_DRY:
      operatingMode = ESPMHI_MODE_DRY;
      break;
    case climate::CLIMATE_MODE_OFF:
    default:
      operatingMode = ESPMHI_MODE_OFF;
      break;
  }

  uint8_t temp = (this->target_temperature);
  if (temp > ESPMHI_TEMP_MIN && temp < ESPMHI_TEMP_MAX) {
    temperature = (~((temp - 17) << 4)) & 0xF0;;
  }

  if (this->fan_mode == climate::CLIMATE_FAN_AUTO) {
      fanSpeed = ESPMHI_FAN_AUTO;
  } else if (this->fan_mode == climate::CLIMATE_FAN_LOW) {
      fanSpeed = ESPMHI_FAN1;
  } else if (this->fan_mode == climate::CLIMATE_FAN_MEDIUM) {
      fanSpeed = ESPMHI_FAN2;
  } else if (this->fan_mode == climate::CLIMATE_FAN_HIGH) { 
      fanSpeed = ESPMHI_FAN3;
  } else {
      fanSpeed = ESPMHI_FAN_AUTO;
  }

  if (this->swing_mode == climate::CLIMATE_SWING_BOTH){
      swingV = ESPMHI_VS_SWING;
      swingH = ESPMHI_HS_SWING;
  } else if (this->swing_mode == climate::CLIMATE_SWING_HORIZONTAL){
      swingH = ESPMHI_HS_SWING;
  } else if (this->swing_mode == climate::CLIMATE_SWING_VERTICAL){
      swingV = ESPMHI_VS_SWING;
  } else {
      swingV = ESPMHI_VS_STOP;
      swingH = ESPMHI_HS_STOP;
  }

  ESP_LOGV(TAG, "Sending Mitsubishi target temp: %.1f state: %02X mode: %02X temp: %02X", this->target_temperature,
           powerMode, operatingMode, temperature);

  uint8_t espmhiTemplate[] = { 0x52, 0xAE, 0xC3, 0x26, 0xD9, 0x11, 0x00, 0x07, 0x00, 0x00, 0x00 };
  //                                         0     1     2     3     4     5     6     7     8     9    10

  // Horizontal & vertical air flow + allergen + clean + 3D
  espmhiTemplate[5] |= swingH | (swingV & 0b00000010) | cleanMode;

  // Vertical air flow + fan speed
  espmhiTemplate[7] |= fanSpeed | (swingV & 0b00011000);

  // Power state + operating mode + temperature
  espmhiTemplate[9] |= operatingMode | powerMode | temperature;

  // There is no checksum, but some bytes are inverted
  espmhiTemplate[6] = ~espmhiTemplate[5];
  espmhiTemplate[8] = ~espmhiTemplate[7];
  espmhiTemplate[10] = ~espmhiTemplate[9];

  auto transmit = this->transmitter_->transmit();
  auto data = transmit.get_data();

  data->set_carrier_frequency(38000);
  // repeat twice
  for (uint16_t r = 0; r < 2; r++) {
    // Header
    data->mark(ESPMHI_HEADER_MARK);
    data->space(ESPMHI_HEADER_SPACE);
    // Data
    for (uint8_t i : espmhiTemplate)
      for (uint8_t j = 0; j < 8; j++) {
        data->mark(ESPMHI_BIT_MARK);
        bool bit = i & (1 << j);
        data->space(bit ? ESPMHI_ONE_SPACE : ESPMHI_ZERO_SPACE);
      }
    // Footer
    if (r == 0) {
      data->mark(ESPMHI_BIT_MARK);
      data->space(ESPMHI_MIN_GAP);  // Pause before repeating
    }
  }
  data->mark(ESPMHI_BIT_MARK);

  transmit.perform();
}

}  // namespace espmhi
}  // namespace esphome