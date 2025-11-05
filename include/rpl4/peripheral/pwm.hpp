#ifndef RPL4_PERIPHERAL_PWM_HPP_
#define RPL4_PERIPHERAL_PWM_HPP_

#include <array>
#include <cstdint>
#include <memory>

#include "rpl4/registers/registers_pwm.hpp"

namespace rpl {

class Pwm : public std::enable_shared_from_this<Pwm> {
 public:
  enum class Port : size_t {
    kPwm0 = 0,
    kPwm1 = 1,
  };

  enum class Channel : uint8_t {
    kChannel1 = 1,
    kChannel2 = 2,
  };

  /**
   * @brief Get the Pwm instance of specified port.
   * @details To save memory, only the port instance obtained with GetInstance()
   *          is created. If a port instance has already been created, the same
   *          instance will be returned.
   *
   * @param port PWM port
   * @return std::shared_ptr<Pwm>
   */
  static std::shared_ptr<Pwm> GetInstance(Port port);

  Pwm(const Pwm&) = delete;
  Pwm& operator=(const Pwm&) = delete;
  Pwm(Pwm&&) = delete;
  Pwm& operator=(Pwm&&) = delete;
  ~Pwm() = default;

  /**
   * @brief Get the PwmRegisterMap pointer.
   *
   * @return PwmRegisterMap*
   */
  inline PwmRegisterMap* GetRegister() const { return register_map_; }

  /**
   * @brief Configure GPIO pin for PWM output
   *
   * @param pin GPIO pin number
   * @return true if successful, false otherwise
   */
  static bool ConfigureGpioPin(uint8_t pin);

  /**
   * @brief Initialize PWM clock
   *
   * @param frequency Clock frequency in Hz
   */
  void InitializeClock(double frequency);

  /**
   * @brief Enable PWM channel
   *
   * @param channel PWM channel
   */
  void Enable(Channel channel);

  /**
   * @brief Disable PWM channel
   *
   * @param channel PWM channel
   */
  void Disable(Channel channel);

  /**
   * @brief Set PWM frequency
   *
   * @param channel PWM channel
   * @param frequency Frequency in Hz
   */
  void SetFrequency(Channel channel, double frequency);

  /**
   * @brief Set PWM duty cycle
   *
   * @param channel PWM channel
   * @param duty Duty cycle (0.0 to 1.0)
   */
  void SetDutyCycle(Channel channel, double duty);

  /**
   * @brief Set PWM range (period)
   *
   * @param channel PWM channel
   * @param range Range value
   */
  void SetRange(Channel channel, uint32_t range);

  /**
   * @brief Set PWM data (pulse width)
   *
   * @param channel PWM channel
   * @param data Data value
   */
  void SetData(Channel channel, uint32_t data);

  /**
   * @brief Get PWM range
   *
   * @param channel PWM channel
   * @return Range value
   */
  uint32_t GetRange(Channel channel);

  /**
   * @brief Get PWM data
   *
   * @param channel PWM channel
   * @return Data value
   */
  uint32_t GetData(Channel channel);

  /**
   * @brief Set PWM mode
   *
   * @param channel PWM channel
   * @param mode PWM mode
   */
  void SetMode(Channel channel, PwmRegisterMap::CTL::MODE mode);

  /**
   * @brief Set PWM polarity
   *
   * @param channel PWM channel
   * @param polarity Polarity setting
   */
  void SetPolarity(Channel channel, PwmRegisterMap::CTL::POLA polarity);

  /**
   * @brief Enable M/S (Mark/Space) mode
   *
   * @param channel PWM channel
   * @param enable true to enable, false to disable
   */
  void SetMSMode(Channel channel, bool enable);

  /**
   * @brief Enable PWM FIFO
   *
   * @param channel PWM channel
   */
  void EnableFifo(Channel channel);

  /**
   * @brief Disable PWM FIFO
   *
   * @param channel PWM channel
   */
  void DisableFifo(Channel channel);

  /**
   * @brief Clear PWM FIFO
   */
  void ClearFifo();

  /**
   * @brief Write data to FIFO
   *
   * @param data Data to write
   */
  void WriteFifo(uint32_t data);

  /**
   * @brief Check if FIFO is full
   *
   * @return true if full, false otherwise
   */
  bool IsFifoFull();

  /**
   * @brief Check if FIFO is empty
   *
   * @return true if empty, false otherwise
   */
  bool IsFifoEmpty();

  /**
   * @brief Enable DMA
   *
   * @param dreq_threshold DREQ threshold (number of free FIFO slots)
   * @param panic_threshold Panic threshold
   */
  void EnableDma(uint8_t dreq_threshold = 7, uint8_t panic_threshold = 7);

  /**
   * @brief Disable DMA
   */
  void DisableDma();

  /**
   * @brief Get physical address of FIFO register for DMA
   *
   * @return Physical address of FIFO register
   */
  uint32_t GetFifoPhysicalAddress() const;

 private:
  Pwm(PwmRegisterMap* register_map, Port port);

  static constexpr size_t kNumOfInstances = 2;
  static std::array<std::shared_ptr<Pwm>, kNumOfInstances> instances_;

  PwmRegisterMap* register_map_;
  Port port_;
  double clock_frequency_;
  static constexpr double kDefaultClockFrequency = 25000000.0;  // 25 MHz

  // Allow factory struct to access private constructor
  friend struct PwmFactory;
};

// Helper struct to enable make_shared with private constructor
struct PwmFactory {
  static std::shared_ptr<Pwm> Create(PwmRegisterMap* register_map, Pwm::Port port);
};

}  // namespace rpl

#endif  // RPL4_PERIPHERAL_PWM_HPP_
