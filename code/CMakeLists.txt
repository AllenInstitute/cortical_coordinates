PROJECT(ShortestPath)

cmake_minimum_required(VERSION 2.8)

#----------------------
# Find ITK
#---------------------- 
FIND_PACKAGE(ITK)
IF(ITK_FOUND)
INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
MESSAGE(FATAL_ERROR
"ITK not found. Please set ITK_DIR.")
ENDIF(ITK_FOUND)

#----------------------------
# Specify tinyxml path
#-------------------------
SET(TINYXML_PATH CACHE PATH "Path to tinyXML installation")

SET( TINYXML_SRCS
${TINYXML_PATH}/tinyxml.cpp
${TINYXML_PATH}/tinyxmlerror.cpp
${TINYXML_PATH}/tinyxmlparser.cpp
${TINYXML_PATH}/tinystr.cpp
)

#------------------------
# Set output directories
#-----------------------------
SET (EXECUTABLE_OUTPUT_PATH ${ShortestPath_BINARY_DIR}/bin CACHE PATH "Single output directory for building all executables.")

INCLUDE_DIRECTORIES(
${TINYXML_PATH}
)

ADD_EXECUTABLE( OverwriteImage
OverwriteImage.cxx
${TINYXML_SRCS}
)
TARGET_LINK_LIBRARIES( OverwriteImage ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( IsMemberImage
IsMemberImage.cxx
${TINYXML_SRCS}
)
TARGET_LINK_LIBRARIES( IsMemberImage ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( BinaryThresholdImage
BinaryThresholdImage.cxx
${TINYXML_SRCS}
)
TARGET_LINK_LIBRARIES( BinaryThresholdImage ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( BinaryDilateImage
BinaryDilateImage.cxx
${TINYXML_SRCS}
)
TARGET_LINK_LIBRARIES( BinaryDilateImage ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( BinaryErodeImage
BinaryErodeImage.cxx
${TINYXML_SRCS}
)
TARGET_LINK_LIBRARIES( BinaryErodeImage ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( MaskImage
MaskImage.cxx
${TINYXML_SRCS}
)
TARGET_LINK_LIBRARIES( MaskImage ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( FindSurfaceVoxels
FindSurfaceVoxels.cxx
${TINYXML_SRCS}
)
TARGET_LINK_LIBRARIES( FindSurfaceVoxels ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( ComputeLaplacian
ComputeLaplacian.cxx
${TINYXML_SRCS}
)
TARGET_LINK_LIBRARIES( ComputeLaplacian ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( NormalizedGradient
NormalizedGradient.cxx
${TINYXML_SRCS}
)
TARGET_LINK_LIBRARIES( NormalizedGradient ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( LabelPatches
LabelPatches.cxx
${TINYXML_SRCS}
)
TARGET_LINK_LIBRARIES( LabelPatches ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( ComputeStreamlines
ComputeStreamlines.cxx
${TINYXML_SRCS}
)
TARGET_LINK_LIBRARIES( ComputeStreamlines ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( ComputeChild
ComputeChild.cxx
${TINYXML_SRCS}
)
TARGET_LINK_LIBRARIES( ComputeChild ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( GenerateForest
GenerateForest.cxx
${TINYXML_SRCS}
)
TARGET_LINK_LIBRARIES( GenerateForest ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( MaxProjectionForest
MaxProjectionForest.cxx
${TINYXML_SRCS}
)
TARGET_LINK_LIBRARIES( MaxProjectionForest ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})


ADD_EXECUTABLE( SubSampleStreams
SubSampleStreams.cxx
${TINYXML_SRCS}
)
TARGET_LINK_LIBRARIES( SubSampleStreams ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( SignedDistance
SignedDistance.cxx
${TINYXML_SRCS}
)
TARGET_LINK_LIBRARIES( SignedDistance ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( MaxProjection
MaxProjection.cxx
${TINYXML_SRCS}
)
TARGET_LINK_LIBRARIES( MaxProjection ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( FillStreamlines
FillStreamlines.cxx
${TINYXML_SRCS}
)
TARGET_LINK_LIBRARIES( FillStreamlines ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( ResampleImage
ResampleImage.cxx
${TINYXML_SRCS}
)
TARGET_LINK_LIBRARIES( ResampleImage ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( MaxProjectionAlongStreamlines
MaxProjectionAlongStreamlines.cxx
${TINYXML_SRCS}
)
TARGET_LINK_LIBRARIES( MaxProjectionAlongStreamlines ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( MorphologicalOpen
MorphologicalOpen.cxx
${TINYXML_SRCS}
)
TARGET_LINK_LIBRARIES( MorphologicalOpen ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( FastMarching
FastMarching.cxx
)
TARGET_LINK_LIBRARIES( FastMarching ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( CropImage
CropImage.cxx
)
TARGET_LINK_LIBRARIES( CropImage ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( CreateImageFromIndex
CreateImageFromIndex.cxx
)
TARGET_LINK_LIBRARIES( CreateImageFromIndex ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( MakeIndexImage
MakeIndexImage.cxx
)
TARGET_LINK_LIBRARIES( MakeIndexImage ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( CreateReverseIndexImage
CreateReverseIndexImage.cxx
)
TARGET_LINK_LIBRARIES( CreateReverseIndexImage ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( UpdateMaster
UpdateMaster.cxx
)
TARGET_LINK_LIBRARIES( UpdateMaster ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( OpenLabels
OpenLabels.cxx
)
TARGET_LINK_LIBRARIES( OpenLabels ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})

ADD_EXECUTABLE( BoundaryGradient
BoundaryGradient.cxx
${TINYXML_SRCS}
)
TARGET_LINK_LIBRARIES( BoundaryGradient ITKCommon ITKNumerics ITKIO ${ITK_JP2_LIBRARIES})


