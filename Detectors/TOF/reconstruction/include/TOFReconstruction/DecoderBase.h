// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/// @file   Decoder.h
/// @author Roberto Preghenella
/// @since  2020-02-24
/// @brief  TOF compressed data decoder

#ifndef O2_TOF_DECODERBASE
#define O2_TOF_DECODERBASE

#include <fstream>
#include <string>
#include <cstdint>
#include <vector>
#include "Headers/RAWDataHeader.h"
#include "DataFormatsTOF/CompressedDataFormat.h"

namespace o2
{
namespace tof
{
namespace compressed
{

class DecoderBase
{

 public:
  DecoderBase() = default;
  virtual ~DecoderBase() = default;

  inline bool run()
  {
    rewind();
    while (!processHBF())
      ;
    return false;
  };

  inline void rewind()
  {
    decoderRewind();
  };

  void setDecoderVerbose(bool val) { mDecoderVerbose = val; };
  void setDecoderBuffer(const char* val) { mDecoderBuffer = val; };
  void setDecoderBufferSize(long val) { mDecoderBufferSize = val; };

 protected:
  /** handlers **/

  virtual void handlerHBFHeader() {};
  virtual void handlerCrateHeader() {};
  virtual void handlerFrameHeader() {};  
  virtual void handlerCrateTrailer() {};
  virtual void handlerHBFTrailer() {};
  
  /** old API, deprecated **/

  virtual void rdhHandler(const o2::header::RAWDataHeader* rdh){};
  virtual void headerHandler(const CrateHeader_t* crateHeader, const CrateOrbit_t* crateOrbit){};

  virtual void frameHandler(const CrateHeader_t* crateHeader, const CrateOrbit_t* crateOrbit,
                            const FrameHeader_t* frameHeader, const PackedHit_t* packedHits){};

  virtual void trailerHandler(const CrateHeader_t* crateHeader, const CrateOrbit_t* crateOrbit,
                              const CrateTrailer_t* crateTrailer, const Diagnostic_t* diagnostics,
                              const Error_t* errors){};

  /** very old API, deprecated **/

  virtual void trailerHandler(const CrateHeader_t* crateHeader, const CrateOrbit_t* crateOrbit,
                              const CrateTrailer_t* crateTrailer, const Diagnostic_t* diagnostics){};

  bool processHBF();

  /** decoder private functions and data members **/
  inline void decoderRewind() { mDecoderPointer = reinterpret_cast<const uint32_t*>(mDecoderBuffer); };

  const char* mDecoderBuffer = nullptr;
  long mDecoderBufferSize;
  const uint32_t* mDecoderPointer = nullptr;
  const uint32_t* mDecoderPointerMax = nullptr;
  const uint32_t* mDecoderPointerNext = nullptr;
  bool mDecoderVerbose = false;
  bool mDecoderError = false;
  bool mDecoderFatal = false;
  char mDecoderSaveBuffer[1048576];
  uint32_t mDecoderSaveBufferDataSize = 0;
  uint32_t mDecoderSaveBufferDataLeft = 0;

  /** local pointers **/
  const HBFHeader_t *mHBFHeader = nullptr;
  const HBFOrbit_t *mHBFOrbit = nullptr;
  const HBFTrigger_t *mHBFTrigger = nullptr;
  const HBFPayload_t *mHBFPayload = nullptr;
  const CrateHeader_t *mCrateHeader = nullptr;
  const FrameHeader_t *mFrameHeader = nullptr;
  const PackedHit_t *mPackedHits = nullptr;
  const CrateTrailer_t *mCrateTrailer = nullptr;
  const Diagnostic_t *mDiagnostics = nullptr;
  const Error_t *mErrors = nullptr;
  const HBFHeader_t *mHBFTrailer = nullptr;
  
};

} // namespace compressed
} // namespace tof
} // namespace o2

#endif /** O2_TOF_DECODERBASE **/
