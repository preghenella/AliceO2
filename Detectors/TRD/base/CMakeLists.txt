SET(MODULE_NAME TRDBase)

O2_SETUP(NAME ${MODULE_NAME})

SET(SRCS
  src/TRDPadPlane.cxx
  src/TRDGeometryBase.cxx
  src/TRDGeometry.cxx
  src/TRDGeometryFlat.cxx
  src/TRDCommonParam.cxx
  src/TRDSimParam.cxx
  src/Digit.cxx
)

SET(HEADERS
  include/${MODULE_NAME}/TRDPadPlane.h
  include/${MODULE_NAME}/TRDGeometryBase.h
  include/${MODULE_NAME}/TRDGeometry.h
  include/${MODULE_NAME}/TRDGeometryFlat.h
  include/${MODULE_NAME}/TRDSimParam.h
  include/${MODULE_NAME}/TRDCommonParam.h
  include/${MODULE_NAME}/Digit.h
)

SET(LINKDEF src/TRDBaseLinkDef.h)
SET(LIBRARY_NAME ${MODULE_NAME})
SET(BUCKET_NAME trd_base_bucket)

O2_GENERATE_LIBRARY()
