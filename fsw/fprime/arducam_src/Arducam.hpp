// ======================================================================
// \title  Arducam.hpp
// \author jstar
// \brief  hpp file for Arducam component implementation class
// ======================================================================

#ifndef Components_Arducam_HPP
#define Components_Arducam_HPP

#include "arducam_src/ArducamComponentAc.hpp"

extern "C"{
#include "cam_device.h"
#include "cam_registers.h"
}

typedef struct
{
    uint8_t                     CommandErrorCount;
    uint8_t                     CommandCount;
} __attribute__((packed)) SAMPLE_Hk_tlm_t;
#define SAMPLE_HK_TLM_LNGTH sizeof(SAMPLE_Hk_tlm_t)

namespace Components {

  class Arducam :
    public ArducamComponentBase
  {

    public:

    SAMPLE_Hk_tlm_t HkTelemetryPkt;

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct Arducam object
      Arducam(
          const char* const compName //!< The component name
      );

      //! Destroy Arducam object
      ~Arducam();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for commands
      // ----------------------------------------------------------------------
      void NOOP_cmdHandler(
          FwOpcodeType opcode, // The opcode
          U32 cmdSeq // The command sequence number
      ) override;

      void I2C_cmdHandler(
          FwOpcodeType opcode, // The opcode
          U32 cmdSeq // The command sequence number
      ) override;

      void IMAGE_cmdHandler(
        FwOpcodeType opcode, // The opcode
        U32 cmdSeq, // The command sequence number
        U32 image_size //
      ) override;

      void SPI_cmdHandler(
          FwOpcodeType opcode, // The opcode
          U32 cmdSeq // The command sequence number
      ) override;

      void RESET_COUNTERS_cmdHandler(
        FwOpcodeType opcode,
        U32 cmdSeq
      ) override;

      void REPORT_HOUSEKEEPING_cmdHandler(
        FwOpcodeType opcode,
        U32 cmdSeq
      ) override;

      void HARDWARE_CHECKOUT_cmdHandler(
        FwOpcodeType opcode,
        U32 cmdSeq
      ) override;

  };

}

#endif
