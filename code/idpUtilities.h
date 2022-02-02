/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $File: //depot/bioapps/infoapps/HBAMapping/main/Common/idpUtilities.h $
  Language:  C++
  Date:      $DateTime: 2013/08/13 16:05:24 $
  Version:   $Revision: #8 $

  Copyright (c) Allen Institute for Brain Science. All rights reserved.

=========================================================================*/
#ifndef __idpUtilities_h
#define __idpUtilities_h

#include "itkExceptionObject.h"
#include "itkInterpolateImageFunction.h"
#include "itkTransform.h"
#include <itksys/SystemTools.hxx>

namespace itk
{
namespace idp
{

// helper function to change between linux and windows paths
void ChangePaths( std::string & str );

// helper function to read image from file
template <typename ImageType>
void
ReadImage( 
const char * filename,
typename ImageType::Pointer & image ) 
throw (ExceptionObject);

// helper function to write image to file
template <typename ImageType>
void
WriteImage( 
const char * filename,
const ImageType * image,
bool compression = false ) 
throw( ExceptionObject);


// helper function make an image using a reference
template <typename ImageType>
void
MakeImage(
typename ImageType::Pointer & output,
typename ImageType::RegionType  & region,
typename ImageType::SpacingType & spacing,
typename ImageType::PointType & origin,
typename ImageType::PixelType defaultPixel
)
throw( ExceptionObject );

// helper function make an image using a reference
template <typename ImageType, typename TemplateImageType>
void
MakeImage(
typename TemplateImageType::Pointer & t,
typename ImageType::Pointer & output,
typename ImageType::PixelType defaultPixel
)
throw( ExceptionObject );


// helper function to resample image
template <typename ImageType>
void
ResampleImage(
typename ImageType::Pointer & input,
typename ImageType::Pointer & ref,
const Transform< double, ImageType::ImageDimension, ImageType::ImageDimension> * trans,
typename ImageType::Pointer & output,
typename ImageType::PixelType pad = 0,
const char * interpolatorType = "Linear"
 )
throw( ExceptionObject );

// helper function to resample image
template <typename ImageType>
void
VectorResampleImage(
typename ImageType::Pointer & input,
typename ImageType::Pointer & ref,
const Transform< double, ImageType::ImageDimension, ImageType::ImageDimension> * trans,
typename ImageType::Pointer & output,
typename ImageType::PixelType pad = 0,
const char * interpolatorType = "VectorLinear"
 )
throw( ExceptionObject );

// helper function to cast an image
template <typename InputImageType, typename OutputImageType>
void
CastImage(
typename InputImageType::Pointer & input,
typename OutputImageType::Pointer & output
) throw( ExceptionObject );

// helper function compute the minimum
template <typename ImageType>
void
MinimumImage(
typename ImageType::Pointer & input1,
typename ImageType::Pointer & input2,
typename ImageType::Pointer & output
) throw( ExceptionObject );

// helper function compute the maximum
template <typename ImageType>
void
MaximumImage(
typename ImageType::Pointer & input1,
typename ImageType::Pointer & input2,
typename ImageType::Pointer & output
) throw( ExceptionObject );

// helper function to extract region of interest
template <typename ImageType, typename OutputImageType>
void
ExtractImage(
typename ImageType::Pointer & input,
typename ImageType::RegionType & region,
typename OutputImageType::Pointer & output
) throw( ExceptionObject );

// helper function to paste region of interest
template <typename ImageType>
void
PasteImage(
typename ImageType::Pointer & dest,
const typename ImageType::IndexType & destIndex,
typename ImageType::Pointer & src,
const typename ImageType::RegionType & srcRegion,
typename ImageType::Pointer & output
) throw( ExceptionObject );

// helper function to extract one channel
template <typename InputImageType, typename OutputImageType>
void
VectorIndexSelectionCastImage(
typename InputImageType::Pointer & input,
unsigned int index,
typename OutputImageType::Pointer & output
)  throw( ExceptionObject );

// helper function to median filter an image
template <typename ImageType>
void
MedianFilter(
typename ImageType::Pointer & input,
unsigned int radius,
typename ImageType::Pointer & output
) throw( ExceptionObject );

// helper function to binary threshold
template <typename InputImageType, typename OutputImageType>
void
BinaryThreshold(
typename InputImageType::Pointer & input,
typename InputImageType::PixelType & lowerThreshold,
typename InputImageType::PixelType & upperThreshold,
typename OutputImageType::PixelType & insideValue,
typename OutputImageType::PixelType & outsideValue,
typename OutputImageType::Pointer & output
) throw( ExceptionObject );

// helper function to invert image intensity
template <typename ImageType>
void
InvertImage( 
typename ImageType::Pointer & input,
typename ImageType::Pointer & output
) throw( ExceptionObject );

// select on component out of a vector
template <typename InputImageType, typename OutputImageType>
void
VectorIndexSelection( 
typename InputImageType::Pointer & input,
unsigned int index,
typename OutputImageType::Pointer & output
) throw( ExceptionObject );

// signed distance function
template <typename ImageType>
void 
SignedDistance(
typename ImageType::Pointer & input,
typename ImageType::Pointer & output,
float maximumDistance
) throw( ExceptionObject );

// helper function to subtract 2 images
template <typename ImageType>
void
SubtractImage(
typename ImageType::Pointer & input1,
typename ImageType::Pointer & input2,
typename ImageType::Pointer & output
) throw( ExceptionObject );

// helper function to apply shift and scale
template <typename ImageType>
void
ShiftScaleImage(
typename ImageType::Pointer & input,
float shift,
float scale,
typename ImageType::Pointer & output
) throw( ExceptionObject );

// helper function to compute the correlation between two images
template <typename ImageType>
void
ComputeCorrelation( 
const ImageType * image1,
const ImageType * image2,
double & value 
) throw (ExceptionObject);

// helper function compute the minimum
template <typename ImageType>
void
OverwriteImage(
typename ImageType::Pointer & input1,
typename ImageType::Pointer & input2,
typename ImageType::Pointer & output
) throw( ExceptionObject );

// helper function compute the minimum image merge
template <typename ImageType, typename LabelType>
void
MinimumIndexImage(
typename ImageType::Pointer & input1,
typename ImageType::Pointer & input2,
typename LabelType::Pointer & outputLabel
) throw( ExceptionObject );

// helper function to mask an image
template <typename ImageType, typename MaskType>
void
MaskImage(
typename ImageType::Pointer & input,
typename ImageType::Pointer & mask,
typename ImageType::Pointer & output,
typename ImageType::PixelType outsideValue
)
throw( ExceptionObject );

// helper function to fill a specifiy region
template <typename ImageType>
void
FillRegion(
typename ImageType::Pointer & ref,
typename ImageType::Pointer & output,
typename ImageType::RegionType region,
typename ImageType::PixelType foreground,
typename ImageType::PixelType background
)
throw( ExceptionObject );

template <typename ImageType>
void
IsMember(
typename ImageType::Pointer & input,
typename std::vector<typename ImageType::PixelType> & array,
typename ImageType::Pointer & output,
typename ImageType::PixelType foreground,
typename ImageType::PixelType background
)
throw( ExceptionObject );

template <typename ImageType>
void
CropImage(
typename ImageType::Pointer & input,
typename ImageType::IndexType & start,
typename ImageType::SizeType & size,
typename ImageType::Pointer & output
)
throw( ExceptionObject );

// helper function to read transform to file
template< typename TransformType >
void ReadTransform(
const char * filename,
typename TransformType::Pointer & transform
)throw( ExceptionObject );



} // end namespace idp
} //end namespace itk
           
#ifndef ITK_MANUAL_INSTANTIATION
#include "idpUtilities.txx"
#endif

#endif

  
