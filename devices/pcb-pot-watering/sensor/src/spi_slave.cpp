#include "spi_slave.h"
#include <functional>
#include <map>
#include <assert.h>

#if defined SPI1_BASE
  std::function<void ()> HAL_SPI1_IRQHandler = 0;
  extern "C" void SPI1_IRQHandler(void)
  {
  if (HAL_SPI1_IRQHandler) HAL_SPI1_IRQHandler();
  }
#endif

#if defined SPI2_BASE
  std::function<void ()> HAL_SPI2_IRQHandler = 0;
  extern "C" void SPI2_IRQHandler(void)
  {
  if (HAL_SPI2_IRQHandler) HAL_SPI2_IRQHandler();
  }
#endif

#if defined SPI3_BASE
  std::function<void ()> HAL_SPI3_IRQHandler = 0;
  extern "C" void SPI3_IRQHandler(void)
  {
  if (HAL_SPI3_IRQHandler) HAL_SPI3_IRQHandler();
  }
#endif

#if defined SPI4_BASE
  std::function<void ()> HAL_SPI4_IRQHandler = 0;
  extern "C" void SPI4_IRQHandler(void)
  {
  if (HAL_SPI4_IRQHandler) HAL_SPI4_IRQHandler();
  }
#endif

#if defined SPI5_BASE
  std::function<void ()> HAL_SPI5_IRQHandler = 0;
  extern "C" void SPI5_IRQHandler(void)
  {
  if (HAL_SPI5_IRQHandler) HAL_SPI5_IRQHandler();
  }
#endif

#if defined SPI6_BASE
  std::function<void ()> HAL_SPI6_IRQHandler = 0;
  extern "C" void SPI6_IRQHandler(void)
  {
  if (HAL_SPI6_IRQHandler) HAL_SPI6_IRQHandler();
  }
#endif


/**
  * @brief  Default constructor. Uses pin configuration of variant.h.
  */
SPI_Slave::SPI_Slave() : SPI_Slave(MOSI, MISO, SCK) {}

/**
  * @brief  Constructor to create another SPI instance attached to another SPI
  *         peripheral different of the default SPI. All pins must be attached to
  *         the same SPI peripheral. See datasheet of the microcontroller.
  * @param  mosi: SPI mosi pin. Accepted format: number or Arduino format (Dx)
  *         or ST format (Pxy).
  * @param  miso: SPI miso pin. Accepted format: number or Arduino format (Dx)
  *         or ST format (Pxy).
  * @param  sclk: SPI clock pin. Accepted format: number or Arduino format (Dx)
  *         or ST format (Pxy).
  * @param  ssel: SPI ssel pin (optional). Accepted format: number or
  *         Arduino format (Dx) or ST format (Pxy). By default is set to NC.
  *         This pin must correspond to a hardware CS pin which can be managed
  *         by the SPI peripheral itself. See the datasheet of the microcontroller
  *         or look at PinMap_SPI_SSEL[] inside the file PeripheralPins.c
  *         corresponding to the board. If you configure this pin you can't use
  *         another CS pin and don't pass a CS pin as parameter to any functions
  *         of the class.
  */
SPI_Slave::SPI_Slave(uint8_t mosi, uint8_t miso, uint8_t sclk, uint8_t ssel)
{
  setMISO(miso);
  setMOSI(mosi);
  setSCLK(sclk);
  setSSEL(ssel);
  setBitOrder(MSBFIRST);
  setDataMode(SPI_MODE0);
  setTimeout(SPI_DEFAULT_TIMEOUT);
}

void SPI_Slave::begin(){

  //Enable PINs
  pinmap_pinout(_spi.pin_mosi, PinMap_SPI_MOSI);
  pinmap_pinout(_spi.pin_miso, PinMap_SPI_MISO);
  pinmap_pinout(_spi.pin_sclk, PinMap_SPI_SCLK);
  if (_spi.pin_ssel != NC) pinmap_pinout(_spi.pin_ssel, PinMap_SPI_SSEL);
  
  //Get belonging SPI
  void *spi_mosi = pinmap_peripheral(_spi.pin_mosi, PinMap_SPI_MOSI);
  void *spi_miso = pinmap_peripheral(_spi.pin_miso, PinMap_SPI_MISO);
  void *spi_sclk = pinmap_peripheral(_spi.pin_sclk, PinMap_SPI_SCLK);
  void *spi_ssel = pinmap_peripheral(_spi.pin_ssel, PinMap_SPI_SSEL);

  if (spi_mosi == NP || spi_miso == NP || spi_sclk == NP) {
    //ERROR: at least one SPI pin has no peripheral
    Error_Handler();
  }

  void *spi_data = pinmap_merge_peripheral(spi_mosi, spi_miso);
  void *spi_cntl = pinmap_merge_peripheral(spi_sclk, spi_ssel);

  void* spi_instance = pinmap_merge_peripheral(spi_data, spi_cntl);
  if (spi_instance == NP) {
    //ERROR: pins do not belong to one SPI
    Error_Handler();
  }

  //Init handle
  _spi.handle.Instance = (SPI_TypeDef*)spi_instance;
  _spi.handle.Init.Mode = SPI_MODE_SLAVE;
  _spi.handle.Init.Direction = SPI_DIRECTION_2LINES;
  _spi.handle.Init.DataSize = SPI_DATASIZE_8BIT;
  if ((_spi.mode == SPI_MODE0) || (_spi.mode == SPI_MODE1)) {
    _spi.handle.Init.CLKPolarity       = SPI_POLARITY_LOW;
  } else {
    _spi.handle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
  }
  if ((_spi.mode == SPI_MODE0) || (_spi.mode == SPI_MODE2)) {
    _spi.handle.Init.CLKPhase          = SPI_PHASE_1EDGE;
  } else {
    _spi.handle.Init.CLKPhase          = SPI_PHASE_2EDGE;
  }
  if (_spi.pin_ssel != NC) {
    _spi.handle.Init.NSS = SPI_NSS_HARD_INPUT;
  } else {
    _spi.handle.Init.NSS = SPI_NSS_SOFT;
  }
  if (_spi.order == MSBFIRST) {
    _spi.handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
  } else {
    _spi.handle.Init.FirstBit = SPI_FIRSTBIT_LSB;
  }
  _spi.handle.Init.TIMode = SPI_TIMODE_DISABLE;
  _spi.handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  _spi.handle.Init.CRCPolynomial = 7;
  _spi.handle.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  #if defined(STM32F0xx) || defined(STM32F3xx) || defined(STM32F7xx) ||\
    defined(STM32G0xx) || defined(STM32H7xx) || defined(STM32L4xx) ||\
    defined(STM32WBxx) || defined(STM32MP1xx)
    _spi.handle.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  #endif


//Init SPI clock and interrupt
#if defined SPI1_BASE
  if (_spi.handle.Instance == SPI1) {
    __HAL_RCC_SPI1_CLK_ENABLE();
    assert(HAL_SPI1_IRQHandler == 0);
    HAL_SPI1_IRQHandler = std::bind(HAL_SPI_IRQHandler, &_spi.handle);
  }
#endif

#if defined SPI2_BASE
  if (_spi.handle.Instance == SPI2) {
    __HAL_RCC_SPI2_CLK_ENABLE();
    assert(HAL_SPI2_IRQHandler == 0);
    HAL_SPI2_IRQHandler = std::bind(HAL_SPI_IRQHandler, &_spi.handle);
  }
#endif

#if defined SPI3_BASE
  if (_spi.handle.Instance == SPI3) {
    __HAL_RCC_SPI3_CLK_ENABLE();
    assert(HAL_SPI3_IRQHandler == 0);
    HAL_SPI3_IRQHandler = std::bind(HAL_SPI_IRQHandler, &_spi.handle);
  }
#endif

#if defined SPI4_BASE
  if (_spi.handle.Instance == SPI4) {
    __HAL_RCC_SPI4_CLK_ENABLE();
    assert(HAL_SPI4_IRQHandler == 0);
    HAL_SPI4_IRQHandler = std::bind(HAL_SPI_IRQHandler, &_spi.handle);
  }
#endif

#if defined SPI5_BASE
  if (_spi.handle.Instance == SPI5) {
    __HAL_RCC_SPI5_CLK_ENABLE();
    assert(HAL_SPI5_IRQHandler == 0);
    HAL_SPI5_IRQHandler = std::bind(HAL_SPI_IRQHandler, &_spi.handle);
  }
#endif

#if defined SPI6_BASE
  if (_spi.handle.Instance == SPI6) {
    __HAL_RCC_SPI6_CLK_ENABLE();
    assert(HAL_SPI6_IRQHandler == 0);
    HAL_SPI6_IRQHandler = std::bind(HAL_SPI_IRQHandler, &_spi.handle);
  }
#endif

  if (HAL_SPI_Init(&_spi.handle) != HAL_OK)
  {
    Error_Handler();
  }

    HAL_NVIC_SetPriority(SPI1_IRQn, 100, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
}

void SPI_Slave::end() {
  SPI_HandleTypeDef *handle = &(_spi.handle);

  HAL_SPI_DeInit(handle);

// Reset SPI, disable clock and remove interrupt handler
#if defined SPI1_BASE
  if (handle->Instance == SPI1) {
    __HAL_RCC_SPI1_FORCE_RESET();
    __HAL_RCC_SPI1_RELEASE_RESET();
    __HAL_RCC_SPI1_CLK_DISABLE();
    HAL_SPI1_IRQHandler = 0;
  }
#endif
#if defined SPI2_BASE
  if (handle->Instance == SPI2) {
    __HAL_RCC_SPI2_FORCE_RESET();
    __HAL_RCC_SPI2_RELEASE_RESET();
    __HAL_RCC_SPI2_CLK_DISABLE();
    HAL_SPI2_IRQHandler = 0;
  }
#endif

#if defined SPI3_BASE
  if (handle->Instance == SPI3) {
    __HAL_RCC_SPI3_FORCE_RESET();
    __HAL_RCC_SPI3_RELEASE_RESET();
    __HAL_RCC_SPI3_CLK_DISABLE();
    HAL_SPI3_IRQHandler = 0;
  }
#endif

#if defined SPI4_BASE
  if (handle->Instance == SPI4) {
    __HAL_RCC_SPI4_FORCE_RESET();
    __HAL_RCC_SPI4_RELEASE_RESET();
    __HAL_RCC_SPI4_CLK_DISABLE();
    HAL_SPI4_IRQHandler = 0;
  }
#endif

#if defined SPI5_BASE
  if (handle->Instance == SPI5) {
    __HAL_RCC_SPI5_FORCE_RESET();
    __HAL_RCC_SPI5_RELEASE_RESET();
    __HAL_RCC_SPI5_CLK_DISABLE();
    HAL_SPI5_IRQHandler = 0;
  }
#endif

#if defined SPI6_BASE
  if (handle->Instance == SPI6) {
    __HAL_RCC_SPI6_FORCE_RESET();
    __HAL_RCC_SPI6_RELEASE_RESET();
    __HAL_RCC_SPI6_CLK_DISABLE();
    HAL_SPI6_IRQHandler = 0;
  }
#endif
}

uint8_t SPI_Slave::transfer8(uint8_t data_out){
  uint8_t data_in;
  transfer(sizeof(data_out), &data_out, &data_in);
  return data_in;
}
uint16_t SPI_Slave::transfer16(uint16_t data_out){
  uint16_t data_in;
  data_in = transfer8((data_out>>8)) << 8;
  data_in += transfer8((data_out));
  return data_in;
}
uint32_t SPI_Slave::transfer32(uint32_t data_out){
  uint32_t data_in;
  data_in = transfer16((data_out>>16)) << 16;
  data_in += transfer16((data_out));
  return data_in;
}
void SPI_Slave::transfer(size_t count, uint8_t *bufout, uint8_t *bufin){
  if ((count == 0) || (bufout==0 && bufin==0)) {
    return;
  } else if (bufout == 0){
    HAL_SPI_Receive(&(_spi.handle), bufin, count, _spi.timeout);
  }else if (bufin == 0){
    HAL_SPI_Transmit(&(_spi.handle), bufout, count, _spi.timeout);
  }else {
    HAL_SPI_TransmitReceive(&(_spi.handle), bufout, bufin, count, _spi.timeout);
  }
}



static std::map<SPI_HandleTypeDef*, std::function<void()>> rx_callbacks;
static std::map<SPI_HandleTypeDef*, std::function<void()>> tx_callbacks;

HAL_StatusTypeDef status;
void SPI_Slave::transfer_IT(size_t count, uint8_t *bufout, void (*tx_callback)(), uint8_t *bufin, void (*rx_callback)()){
  if ((count == 0) || (bufout==0 && bufin==0)) {
    return;
  } else if (bufout == 0){
    status = HAL_SPI_Receive_IT(&(_spi.handle), bufin, count);
    if (status == HAL_OK && rx_callback) rx_callbacks[&(_spi.handle)] = rx_callback;
  } else if (bufin == 0){
    status = HAL_SPI_Transmit_IT(&(_spi.handle), bufout, count);
    if (status == HAL_OK && tx_callback) tx_callbacks[&(_spi.handle)] = tx_callback;
  }else {
    status = HAL_SPI_TransmitReceive_IT(&(_spi.handle), bufout, bufin, count);
    if (status == HAL_OK && rx_callback) rx_callbacks[&(_spi.handle)] = rx_callback;
    if (status == HAL_OK && tx_callback) tx_callbacks[&(_spi.handle)] = tx_callback;
  }

  if (status == HAL_OK) {
    ;
  } else {
    assert(false);
  }
}

extern "C" void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    auto callback_iter = rx_callbacks.find(hspi);
    if (callback_iter != rx_callbacks.end()) {
      auto callback = callback_iter -> second;
      rx_callbacks.erase(hspi);
      callback();
    }
}

extern "C" void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    auto callback_iter = tx_callbacks.find(hspi);
    if (callback_iter != tx_callbacks.end()) {
      auto callback = callback_iter -> second;
      tx_callbacks.erase(hspi);
      callback();
    }
}

extern "C" void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi){
  HAL_SPI_RxCpltCallback(hspi);
  HAL_SPI_TxCpltCallback(hspi);
}

extern "C" void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi) {
  assert(false);
}

