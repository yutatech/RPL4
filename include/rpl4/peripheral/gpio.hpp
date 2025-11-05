#ifndef RPL4_PERIPHERAL_GPIO_HPP_
#define RPL4_PERIPHERAL_GPIO_HPP_

#include <array>
#include <cstdint>
#include <memory>

namespace rpl {

// Forward declaration for factory
class Gpio;

// Helper struct to enable make_shared with private constructor
struct GpioFactory {
  static std::shared_ptr<Gpio> Create(uint8_t pin);
};

class Gpio : public std::enable_shared_from_this<Gpio> {
 public:
  /**
   * @brief Resistor Select
   */
  enum class PullRegister : uint8_t {
    kNoRegister = 0b00,  // No resistor is selected
    kPullUp = 0b01,      // Pull up resistor is selected
    kPullDown = 0b10,    // Pull down resistor is selected
  };

  /**
   * @brief Function Select
   */
  enum class AltFunction : uint8_t {
    kInput = 0b000,
    kOutput = 0b001,
    kAlt0 = 0b100,
    kAlt1 = 0b101,
    kAlt2 = 0b110,
    kAlt3 = 0b111,
    kAlt4 = 0b011,
    kAlt5 = 0b010,
  };
  /**
   * @brief Get the Gpio instance of specified pin.
   * @details To save memory, only the pin instance obtained with GetInstance()
   *          is created. If a pin instance has already been created, the same
   *          instance will be returned.
   *
   * @param pin GPIO pin number (0 ~ 57)
   * @return std::shared_ptr<Gpio>
   */
  static std::shared_ptr<Gpio> GetInstance(uint8_t pin);

  Gpio(const Gpio&) = delete;
  Gpio& operator=(const Gpio&) = delete;
  Gpio(Gpio&&) = delete;
  Gpio& operator=(Gpio&&) = delete;
  ~Gpio() = default;

  /**
   * @brief Read the pin state
   *
   * @return true
   * @return false
   */
  bool Read();

  /**
   * @brief Set the output state.
   *
   * @param output output state. true: HIGH, false: LOW
   * @retval true : output state is set to HIGH
   * @retval false : output state is set to LOW
   */
  bool Write(bool output);

  /**
   * @brief Configure the alternative function mode of the gpio pin.
   *
   * @param alt_function Alternattive function mode
   */
  void SetAltFunction(AltFunction alt_function);

  /**
   * @brief Configure the pull register of the gpio pin.
   *
   * @param pull_register Pull register mode
   */
  void SetPullRegister(PullRegister pull_register);

  /**
   * @brief Configure the alternative function mode with a gpio pin number.
   *
   * @param pin Gpio pin number
   * @param alt_function Alternative function mode
   */
  static void SetAltFunction(uint8_t pin, AltFunction alt_function);

  /**
   * @brief Configure the pull register with a gpio pin number.
   *
   * @param pin Gpio pin number
   * @param pull_register Pull register mode
   */
  static void SetPullRegister(uint8_t pin, PullRegister pull_register);

  bool operator=(bool output) { return Write(output); }

 private:
  /**
   * @brief Construct a new Gpio object
   *
   * @param pin GPIO pin number (0 ~ 57)
   */
  Gpio(uint8_t pin);

  static constexpr size_t kNumOfInstances = 58;
  static std::array<std::shared_ptr<Gpio>, kNumOfInstances> instances_;

  uint8_t pin_;

  // Allow factory struct to access private constructor
  friend struct GpioFactory;
};

}  // namespace rpl

#endif  // RPL4_PERIPHERAL_GPIO_HPP_