#include "espmhi.h"
#include "esphome/core/log.h"

namespace esphome {
namespace espmhi {

static const char *const TAG = "espmhi.climate";

const uint32_t ESPMHI_OFF = 0x00;

const uint8_t ESPMHI_COOL = 0x18;       //b00011000
const uint8_t ESPMHI_DRY = 0x10;        //b00010000
const uint8_t ESPMHI_AUTO = 0x20;       //b00100000
const uint8_t ESPMHI_HEAT = 0x08;       //b00001000
const uint8_t ESPMHI_FAN_AUTO = 0x00;   //b00000000

// Pulse parameters in usec
const uint16_t ESPMHI_BIT_MARK = 430;
const uint16_t ESPMHI_ONE_SPACE = 1250;
const uint16_t ESPMHI_ZERO_SPACE = 390;
const uint16_t ESPMHI_HEADER_MARK = 3500;
const uint16_t ESPMHI_HEADER_SPACE = 1700;
const uint16_t ESPMHI_MIN_GAP = 17500;

void espmhiClimate::transmit_state() {
  uint32_t remote_state[18] = {0x23, 0xCB, 0x26, 0x01, 0x00, 0x20, 0x08, 0x00, 0x30,
                               0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  switch (this->mode) {
    case climate::CLIMATE_MODE_COOL:
      remote_state[6] = ESPMHI_COOL;
      break;
    case climate::CLIMATE_MODE_HEAT:
      remote_state[6] = ESPMHI_HEAT;
      break;
    case climate::CLIMATE_MODE_HEAT_COOL:
      remote_state[6] = ESPMHI_AUTO;
      break;
    case climate::CLIMATE_MODE_OFF:
    default:
      remote_state[5] = ESPMHI_OFF;
      break;
  }

  remote_state[7] = (uint8_t) roundf(clamp<float>(this->target_temperature, ESPMHI_TEMP_MIN, ESPMHI_TEMP_MAX) -
                                     ESPMHI_TEMP_MIN);

  ESP_LOGV(TAG, "Sending Mitsubishi target temp: %.1f state: %02X mode: %02X temp: %02X", this->target_temperature,
           remote_state[5], remote_state[6], remote_state[7]);

  // Checksum
  for (int i = 0; i < 17; i++) {
    remote_state[17] += remote_state[i];
  }

  auto transmit = this->transmitter_->transmit();
  auto data = transmit.get_data();

  data->set_carrier_frequency(38000);
  // repeat twice
  for (uint16_t r = 0; r < 2; r++) {
    // Header
    data->mark(ESPMHI_HEADER_MARK);
    data->space(ESPMHI_HEADER_SPACE);
    // Data
    for (uint8_t i : remote_state)
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