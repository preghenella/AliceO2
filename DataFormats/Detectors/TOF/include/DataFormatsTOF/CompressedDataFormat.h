// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/// @file   CmpDataFormat.h
/// @author Roberto Preghenella
/// @since  2019-12-18
/// @brief  TOF compressed data format

#ifndef O2_TOF_COMPRESSEDDATAFORMAT
#define O2_TOF_COMPRESSEDDATAFORMAT

#include <cstdint>

namespace o2
{
namespace tof
{
namespace compressed
{

struct Word_t {
  uint32_t undefined : 31;
  uint32_t wordType : 1;
};

struct HBFHeader_t {
  uint32_t localPacketCounter : 8;
  uint32_t rdhPacketCounter : 8;
  uint32_t undefined : 4;
  uint32_t versionID : 4;
  uint32_t drmID : 7;
  uint32_t mustBeOne : 1;
  static const uint32_t slotEnableMask = 0x0; // deprecated
  void print() const { printf(" %08x HBF Header           (drmID=%u, versionID=%u, rdhPacketCnt=%u, localPacketCnt=%u) \n",
			      *reinterpret_cast<const uint32_t*>(this),
			      drmID, versionID, rdhPacketCounter, localPacketCounter); };
};

struct HBFOrbit_t {
  uint32_t orbitID : 32;
  void print() const { printf(" %08x HBF Orbit            (orbit=%u) \n",
			      *reinterpret_cast<const uint32_t*>(this),
			      orbitID); };
};

struct HBFTrigger_t {
  uint32_t triggerType : 32;
  void print() const { printf(" %08x HBF Trigger          (trigger=%u) \n",
			      *reinterpret_cast<const uint32_t*>(this),
			      triggerType); };
};

struct HBFPayload_t {
  uint32_t payload : 32;
  void print() const { printf(" %08x HBF Payload          (payload=%u) \n",
			      *reinterpret_cast<const uint32_t*>(this),
			      payload); };
};

struct CrateHeader_t {
  uint32_t bunchID : 12;
  uint32_t slotPartMask : 11;
  uint32_t undefined : 5;
  uint32_t deltaOrbit : 3;
  uint32_t mustBeOne : 1;
  void print() const { printf(" %08x Crate Header         (deltaOrbit=0x%x, slotPartMask=0x%x, bunchID=%u) \n",
			      *reinterpret_cast<const uint32_t*>(this),
			      0x100, slotPartMask, bunchID); };
};

struct FrameHeader_t {
  uint32_t numberOfHits : 16;
  uint32_t frameID : 8;
  uint32_t trmID : 4;
  uint32_t deltaBC : 3;
  uint32_t mustBeZero : 1;
  void print() const { printf(" %08x Frame Header         (deltaBC=0x%x, trmID=%u, frameID=%u, numberOfHits=%u) \n",
			      *reinterpret_cast<const uint32_t*>(this),
			      0x100, trmID, frameID, numberOfHits); };
};

struct PackedHit_t {
  uint32_t tot : 11;
  uint32_t time : 13;
  uint32_t channel : 3;
  uint32_t tdcID : 4;
  uint32_t chain : 1;
  void print() const { printf(" %08x Packed Hit           (chain=%u, tdcID=%u, channel=%u, time=%u, tot=%u) \n",
			      *reinterpret_cast<const uint32_t*>(this),
			      chain, tdcID, channel, time, tot); };
};

struct CrateTrailer_t {
  uint32_t numberOfDiagnostics : 4;
  uint32_t eventCounter : 12;
  uint32_t numberOfErrors : 9;
  uint32_t undefined : 15;
  uint32_t mustBeOne : 1;
  void print() const { printf(" %08x Crate Trailer        (numberOfErrors=%u, eventCounter=%u, numberOfDiagnostics=%u) \n",
			      *reinterpret_cast<const uint32_t*>(this),
			      numberOfErrors, eventCounter, numberOfDiagnostics); };
};

struct Diagnostic_t {
  uint32_t slotID : 4;
  uint32_t faultBits : 28;
  void print() const { printf(" %08x Diagnostic           (slotID=%u, faultBits=%x) \n",
			      *reinterpret_cast<const uint32_t*>(this),
			      slotID, faultBits); };
};

struct Error_t {
  uint32_t errorFlags : 15;
  uint32_t undefined : 4;
  uint32_t slotID : 4;
  uint32_t chain : 1;
  uint32_t tdcID : 4;
  uint32_t mustBeSix : 4;
  void print() const { printf(" %08x Error                (tdcID=%u, chain=%u, slotID=%u, errorFlags=%x) \n",
			      *reinterpret_cast<const uint32_t*>(this),
			      tdcID, chain, slotID, errorFlags); };
};

struct HBFTrailer_t {
  uint32_t receivedTriggers : 4;
  uint32_t servedTriggers : 4;
  uint32_t fatalErrors : 4;
  uint32_t decodeErrors : 4;
  uint32_t numberOfRDHPackets : 4;
  uint32_t undefined : 11;
  uint32_t mustBeZero : 1;
  void print() const { printf(" %08x HBF Trailer          (nRDHPackets=%u servedTrgs=%u, receivedTrgs=%u) \n",
			      *reinterpret_cast<const uint32_t*>(this),
			      numberOfRDHPackets, servedTriggers, receivedTriggers); };
};
 
/** union **/

union Union_t {
  uint32_t data;
  Word_t word;
  CrateHeader_t crateHeader;
  FrameHeader_t frameHeader;
  PackedHit_t packedHit;
  CrateTrailer_t crateTrailer;
};

} // namespace compressed

namespace diagnostic
{

/** DRM diagnostic bits, 12 bits [4-15] **/
enum EDRMDiagnostic_t {
  DRM_HEADER_MISSING = 1 << 4, // start from BIT(4)
  DRM_TRAILER_MISSING = 1 << 5,
  DRM_FEEID_MISMATCH = 1 << 6,
  DRM_ORBIT_MISMATCH = 1 << 7,
  DRM_CRC_MISMATCH = 1 << 8,
  DRM_ENAPARTMASK_DIFFER = 1 << 9,
  DRM_CLOCKSTATUS_WRONG = 1 << 10,
  DRM_FAULTSLOTMASK_NOTZERO = 1 << 11,
  DRM_READOUTTIMEOUT_NOTZERO = 1 << 12,
  DRM_EVENTWORDS_MISMATCH = 1 << 13,
  DRM_MAXDIAGNOSTIC_BIT = 1 << 16 // end before BIT(16)
};

/** LTM diagnostic bits **/
enum ELTMDiagnostic_t {
  LTM_HEADER_MISSING = 1 << 4, // start from BIT(4)
  LTM_TRAILER_MISSING = 1 << 5,
  LTM_HEADER_UNEXPECTED = 1 << 7,
  LTM_MAXDIAGNOSTIC_BIT = 1 << 16 // end before BIT(16)
};

/** TRM diagnostic bits, 12 bits [4-15] **/
enum ETRMDiagnostic_t {
  TRM_HEADER_MISSING = 1 << 4, // start from BIT(4)
  TRM_TRAILER_MISSING = 1 << 5,
  TRM_CRC_MISMATCH = 1 << 6,
  TRM_HEADER_UNEXPECTED = 1 << 7,
  TRM_EVENTCNT_MISMATCH = 1 << 8,
  TRM_EMPTYBIT_NOTZERO = 1 << 9,
  TRM_LBIT_NOTZERO = 1 << 10,
  TRM_FAULTSLOTBIT_NOTZERO = 1 << 11,
  TRM_EVENTWORDS_MISMATCH = 1 << 12,
  TRM_DIAGNOSTIC_SPARE1 = 1 << 13,
  TRM_DIAGNOSTIC_SPARE2 = 1 << 14,
  TRM_DIAGNOSTIC_SPARE3 = 1 << 15,
  TRM_MAXDIAGNOSTIC_BIT = 1 << 16 // end before BIT(16)
};

/** TRM Chain diagnostic bits, 8 bits [16-23] chainA [24-31] chainB **/
enum ETRMChainDiagnostic_t {
  TRMCHAIN_HEADER_MISSING = 1 << 16, // start from BIT(14), BIT(24)
  TRMCHAIN_TRAILER_MISSING = 1 << 17,
  TRMCHAIN_STATUS_NOTZERO = 1 << 18,
  TRMCHAIN_EVENTCNT_MISMATCH = 1 << 19,
  TRMCHAIN_TDCERROR_DETECTED = 1 << 20,
  TRMCHAIN_BUNCHCNT_MISMATCH = 1 << 21,
  TRMCHAIN_DIAGNOSTIC_SPARE1 = 1 << 22,
  TRMCHAIN_DIAGNOSTIC_SPARE2 = 1 << 23,
  TRMCHAIN_MAXDIAGNOSTIC_BIT = 1 << 24 // end before BIT(23), BIT(32)
};

} // namespace diagnostic

} // namespace tof
} // namespace o2

#endif /** O2_TOF_CMPDATAFORMAT **/
