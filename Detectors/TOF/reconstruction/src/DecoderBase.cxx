// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/// @file   Decoder.cxx
/// @author Roberto Preghenella
/// @since  2020-02-24
/// @brief  TOF compressed data decoder base class

#include "TOFReconstruction/DecoderBase.h"
#include "DetectorsRaw/HBFUtils.h"
#include "DetectorsRaw/RDHUtils.h"

#include <cstring>
#include <iostream>

//#define DECODER_PARANOID
#define DECODER_VERBOSE

#ifdef DECODER_PARANOID
#warning "Building code with DecoderParanoid option. This may limit the speed."
#endif
#ifdef DECODER_VERBOSE
#warning "Building code with DecoderVerbose option. This may limit the speed."
#endif

#define colorReset "\033[0m"
#define colorRed "\033[1;31m"
#define colorGreen "\033[1;32m"
#define colorYellow "\033[1;33m"
#define colorBlue "\033[1;34m"

namespace o2
{
namespace tof
{
namespace compressed
{

bool DecoderBase::processHBF()
{

  mDecoderVerbose = true;
  
#ifdef DECODER_VERBOSE
  if (mDecoderVerbose) {
    std::cout << colorBlue << "--- PROCESS HBF"
              << colorReset
              << std::endl;
  }
#endif

  /** check HBF Header signature **/
  if ((*mDecoderPointer & 0x80000000) != 0x80000000) {
#ifdef DECODER_VERBOSE
    if (mDecoderVerbose) {
      printf(" %08x [ERROR] \n", *mDecoderPointer);
    }
#endif
    return true;
  }

  /** HBF Header detected **/
  mHBFHeader = reinterpret_cast<const HBFHeader_t*>(mDecoderPointer);
#ifdef DECODER_VERBOSE
  if (mDecoderVerbose) {
    mHBFHeader->print();
  }
#endif
  mDecoderPointer++;
  
  /** HBF Orbit expected **/
  mHBFOrbit = reinterpret_cast<const HBFOrbit_t*>(mDecoderPointer);
#ifdef DECODER_VERBOSE
  if (mDecoderVerbose) {
    mHBFOrbit->print();
  }
#endif
  mDecoderPointer++;

  /** HBF Trigger expected **/
  mHBFTrigger = reinterpret_cast<const HBFTrigger_t*>(mDecoderPointer);
#ifdef DECODER_VERBOSE
  if (mDecoderVerbose) {
    mHBFTrigger->print();
  }
#endif
  mDecoderPointer++;

  /** HBF Payload expected **/
  mHBFPayload = reinterpret_cast<const HBFPayload_t*>(mDecoderPointer);
#ifdef DECODER_VERBOSE
  if (mDecoderVerbose) {
    mHBFPayload->print();
  }
#endif
  mDecoderPointer++;

  /** HBF Header handler **/
  handlerHBFHeader();

  
  /** loop over HBF data **/
  while (true) {
    
    /** Crate Header detected **/
    if (*mDecoderPointer & 0x80000000) {
      mCrateHeader = reinterpret_cast<const CrateHeader_t*>(mDecoderPointer);
#ifdef DECODER_VERBOSE
      if (mDecoderVerbose) {
	mCrateHeader->print();
      }
#endif
      mDecoderPointer++;

      /** Header Crate handler **/
      handlerCrateHeader();
      
      /** loop over Crate data **/
      while (true) {
	
	/** Crate Trailer detected **/
	if (*mDecoderPointer & 0x80000000) {
	  mCrateTrailer = reinterpret_cast<const CrateTrailer_t*>(mDecoderPointer);
#ifdef DECODER_VERBOSE
	  if (mDecoderVerbose) {
	    mCrateTrailer->print();
	  }
#endif
	  mDecoderPointer++;
	  mDiagnostics = reinterpret_cast<const Diagnostic_t*>(mDecoderPointer);
#ifdef DECODER_VERBOSE
	  if (mDecoderVerbose) {
	    for (int i = 0; i < mCrateTrailer->numberOfDiagnostics; ++i) {
	      auto diagnostic = reinterpret_cast<const Diagnostic_t*>(mDiagnostics + i);
	      diagnostic->print();
	    }
	  }
#endif
	  mDecoderPointer += mCrateTrailer->numberOfDiagnostics;
	  mErrors = reinterpret_cast<const Error_t*>(mDecoderPointer);
#ifdef DECODER_VERBOSE
	  if (mDecoderVerbose) {
	    for (int i = 0; i < mCrateTrailer->numberOfErrors; ++i) {
	      auto error = reinterpret_cast<const Error_t*>(mErrors + i);
	      error->print();
	    }
	  }
#endif
	  mDecoderPointer += mCrateTrailer->numberOfErrors;

	  /** Crate Trailer Handler **/
	  handlerCrateTrailer();
	  
	  break;
	}

	/** frame header detected **/
	mFrameHeader = reinterpret_cast<const FrameHeader_t*>(mDecoderPointer);
#ifdef DECODER_VERBOSE
	if (mDecoderVerbose) {
	  mFrameHeader->print();
	}
#endif
	mDecoderPointer++;
	mPackedHits = reinterpret_cast<const PackedHit_t*>(mDecoderPointer);
#ifdef DECODER_VERBOSE
	if (mDecoderVerbose) {
	  for (int i = 0; i < mFrameHeader->numberOfHits; ++i) {
	    auto packedHit = reinterpret_cast<const PackedHit_t*>(mPackedHits + i);
	    packedHit->print();
	  }
	}
#endif
	mDecoderPointer += mFrameHeader->numberOfHits;

	/** Frame Header handler **/
	handlerFrameHeader();

	
      } /** end of loop over Crate data **/
      
      continue;
    }

    /** HBF Trailer detected **/
    auto hbfTrailer = reinterpret_cast<const HBFTrailer_t*>(mDecoderPointer);
#ifdef DECODER_VERBOSE
    if (mDecoderVerbose) {
      hbfTrailer->print();
    }
#endif
    mDecoderPointer++;

    /** HBF Trailer handler **/
    handlerHBFTrailer();
    
    break;
    
  } /** end of loop over HBF data **/

#ifdef DECODER_VERBOSE
  if (mDecoderVerbose) {
    std::cout << colorBlue
              << "--- END PROCESS HBF"
              << colorReset
              << std::endl;
  }
#endif

  /** check next HBF is within buffer **/
  if (reinterpret_cast<const char*>(mDecoderPointer) < mDecoderBuffer + mDecoderBufferSize)
    return false;

  /** otherwise return **/
  return true;

}

} // namespace compressed
} // namespace tof
} // namespace o2
