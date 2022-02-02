/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $File: //depot/bioapps/infoapps/HBAMapping/main/Common/idpUtilities.txx $
  Language:  C++
  Date:      $DateTime: 2013/08/13 16:05:24 $
  Version:   $Revision: #8 $

  Copyright (c) Allen Institute for Brain Science. All rights reserved.

=========================================================================*/
#ifndef __idpUtilities_txx
#define __idpUtilities_txx

#include "idpUtilities.h"
#include "itkExceptionObject.h"
#include "itkInterpolateImageFunction.h"
#include "itkTransform.h"
#include <itksys/SystemTools.hxx>
#include "itkLinearInterpolateImageFunction.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "itkBSplineInterpolateImageFunction.h"
#include "itkResampleImageFilter.h"
#include "itkVectorResampleImageFilter.h"
#include "itkVectorLinearInterpolateImageFunction.h"
#include "itkVectorNearestNeighborInterpolateImageFunction.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCastImageFilter.h"
#include "itkMinimumImageFilter.h"
#include "itkMaximumImageFilter.h"
#include "itkExtractImageFilter.h"
#include "itkPasteImageFilter.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "itkMedianImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkInvertIntensityImageFilter.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "itkReinitializeLevelSetImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkShiftScaleImageFilter.h"
#include "itkNormalizedCorrelationImageToImageMetric.h"
#include "itkTranslationTransform.h"
#include "itkOverwriteImageFilter.h"
#include "itkMaskImageFilter.h"
#include "itkIsMemberImageFilter.h"
#include "itkTransformFileWriter.h"
#include "itkTransformFileReader.h"

#include "itkMinimumIndexImageFilter.h"

namespace itk
{
namespace idp
{

// helper function to read image from file
template <typename ImageType>
void
ReadImage( 
const char * filename,
typename ImageType::Pointer & image ) 
throw (ExceptionObject)
{
  typedef ImageFileReader<ImageType> ReaderType;
  typename ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( filename );

  try
    {
    reader->Update();
    image = reader->GetOutput();
    image->DisconnectPipeline();
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }

};

// helper function to write image to file
template <typename ImageType>
void
WriteImage( 
const char * filename,
const ImageType * image,
bool compression  ) 
throw( ExceptionObject)
{
  typedef ImageFileWriter<ImageType> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( filename );
  writer->SetInput( image );
  writer->SetUseCompression( compression );

  try
    {
    writer->Update();
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }

};


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
throw( ExceptionObject )
{
    try
    {
    output = ImageType::New();
    output->SetRegions( region );
    output->SetSpacing( spacing );
    output->SetOrigin( origin );
    output->Allocate();
    output->FillBuffer( defaultPixel );
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }
}


template <typename ImageType, typename TemplateImageType>
void
MakeImage(
typename TemplateImageType::Pointer & t,
typename ImageType::Pointer & output,
typename ImageType::PixelType defaultPixel
)
throw( ExceptionObject )
{
    try
    {
    output = ImageType::New();
    output->SetRegions( t->GetBufferedRegion() );
    output->SetSpacing( t->GetSpacing() );
    output->SetOrigin( t->GetOrigin() );
    output->SetDirection( t->GetDirection() );
    output->Allocate();
    output->FillBuffer( defaultPixel );
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }

}

// helper function to resample image
template <typename ImageType>
void
ResampleImage(
typename ImageType::Pointer & input,
typename ImageType::Pointer & ref,
const Transform< double, ImageType::ImageDimension, ImageType::ImageDimension> * trans,
typename ImageType::Pointer & output,
typename ImageType::PixelType pad,
const char * interpolatorType
 )
throw( ExceptionObject )
{
  typedef ResampleImageFilter< ImageType, ImageType > FilterType;
  typename FilterType::Pointer filter = FilterType::New();

  filter->SetTransform( trans );
  filter->SetInput( input );
  filter->SetOutputParametersFromImage( ref );
 // filter->SetSize( ref->GetBufferedRegion().GetSize() );
 // filter->SetOutputOrigin( ref->GetOrigin() );
 // filter->SetOutputSpacing( ref->GetSpacing() );
 // filter->SetOutputStartIndex( ref->GetBufferedRegion().GetIndex() );
  filter->SetDefaultPixelValue( pad );

  typedef InterpolateImageFunction<ImageType,double> InterpolatorType;
  typename InterpolatorType::Pointer interpolator;

  std::string str = interpolatorType;

#define _CreateInterpolator( type ) \
  ( str.compare( #type ) == 0 ) \
    { \
    typedef type##InterpolateImageFunction<ImageType,double> IType; \
    typename IType::Pointer iptr = IType::New(); \
    interpolator = iptr; \
    }

  if ( !str.empty() )
    {
    if _CreateInterpolator( Linear )
    else if _CreateInterpolator( NearestNeighbor )
    else if _CreateInterpolator( BSpline )
    }

#undef _CreateInterpolator

  if ( interpolator.IsNotNull() )
    {
    filter->SetInterpolator( interpolator );
    }

  try
    {
    filter->Update();
    output = filter->GetOutput();
    output->DisconnectPipeline();
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    } 
    
}

// helper function to resample image
template <typename ImageType>
void
VectorResampleImage(
typename ImageType::Pointer & input,
typename ImageType::Pointer & ref,
const Transform< double, ImageType::ImageDimension, ImageType::ImageDimension> * trans,
typename ImageType::Pointer & output,
typename ImageType::PixelType pad,
const char * interpolatorType
 )
throw( ExceptionObject )
{

  typedef VectorResampleImageFilter< ImageType, ImageType > FilterType;
  typename FilterType::Pointer filter = FilterType::New();

  filter->SetTransform( trans );
  filter->SetInput( input );
  filter->SetSize( ref->GetBufferedRegion().GetSize() );
  filter->SetOutputOrigin( ref->GetOrigin() );
  filter->SetOutputSpacing( ref->GetSpacing() );
  filter->SetOutputStartIndex( ref->GetBufferedRegion().GetIndex() );
  filter->SetDefaultPixelValue( pad );

  typedef VectorInterpolateImageFunction<ImageType,double> InterpolatorType;
  typename InterpolatorType::Pointer interpolator;

  std::string str = interpolatorType;

#define _CreateInterpolator( type ) \
  ( str.compare( #type ) == 0 ) \
    { \
    typedef type##InterpolateImageFunction<ImageType,double> IType; \
    typename IType::Pointer iptr = IType::New(); \
    interpolator = iptr; \
    }

  if ( !str.empty() )
    {
    if _CreateInterpolator( VectorLinear )
    }

#undef _CreateInterpolator

  if ( interpolator.IsNotNull() )
    {
    filter->SetInterpolator( interpolator );
    }

  try
    {
    filter->Update();
    output = filter->GetOutput();
    output->DisconnectPipeline();
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }

}

// helper function to cast image type to another
template <typename InputImageType, typename OutputImageType>
void
CastImage(
typename InputImageType::Pointer & input,
typename OutputImageType::Pointer & output
) throw( ExceptionObject )
{
  typedef CastImageFilter<InputImageType,OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInput( input );
  
  try
    {
    filter->Update();
    output = filter->GetOutput();
    output->DisconnectPipeline();
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }

}

// helper function to median filter image
template <typename ImageType>
void
MedianFilter(
typename ImageType::Pointer & input,
typename ImageType::SizeType & radius,
typename ImageType::Pointer & output
) throw( ExceptionObject )
{
  typedef MedianImageFilter<ImageType,ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInput( input );
  filter->SetRadius( radius );
  
  try
    {
    filter->Update();
    output = filter->GetOutput();
    output->DisconnectPipeline();
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }

}


// helper function compute the minimum
template <typename ImageType>
void
MinimumImage(
typename ImageType::Pointer & input1,
typename ImageType::Pointer & input2,
typename ImageType::Pointer & output
) throw( ExceptionObject )
{
  typedef MinimumImageFilter<ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInput1( input1 );
  filter->SetInput2( input2 );
  
  try
    {
    filter->Update();
    output = filter->GetOutput();
    output->DisconnectPipeline();
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }

}

// helper function compute the maximum
template <typename ImageType>
void
MaximumImage(
typename ImageType::Pointer & input1,
typename ImageType::Pointer & input2,
typename ImageType::Pointer & output
) throw( ExceptionObject )
{
  typedef MaximumImageFilter<ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInput1( input1 );
  filter->SetInput2( input2 );
  
  try
    {
    filter->Update();
    output = filter->GetOutput();
    output->DisconnectPipeline();
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }

}


// helper function to extract region of interest
template <typename ImageType, typename OutputImageType>
void
ExtractImage(
typename ImageType::Pointer & input,
typename ImageType::RegionType & region,
typename OutputImageType::Pointer & output
) throw( ExceptionObject )
{
  typedef ExtractImageFilter<ImageType,OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInput( input );
  filter->SetExtractionRegion( region );
  
  try
    {
    filter->Update();
    output = filter->GetOutput();
    output->DisconnectPipeline();
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }

}

// helper function to paste region of interest
template <typename ImageType>
void
PasteImage(
typename ImageType::Pointer & dest,
const typename ImageType::IndexType & destIndex,
typename ImageType::Pointer & src,
const typename ImageType::RegionType & srcRegion,
typename ImageType::Pointer & output
) throw( ExceptionObject )
{

  typedef PasteImageFilter<ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetDestinationImage( dest );
  filter->SetSourceImage( src );
  filter->SetDestinationIndex( destIndex );
  filter->SetSourceRegion( srcRegion );
  
  try
    {
    filter->Update();
    output = filter->GetOutput();
    output->DisconnectPipeline();
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }


}


// helper function to cast image type to another
template <typename InputImageType, typename OutputImageType>
void
VectorIndexSelectionCastImage(
typename InputImageType::Pointer & input,
unsigned int index,
typename OutputImageType::Pointer & output
) throw( ExceptionObject )
{
  typedef VectorIndexSelectionCastImageFilter<InputImageType,OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInput( input );
  filter->SetIndex( index );
  
  try
    {
    filter->Update();
    output = filter->GetOutput();
    output->DisconnectPipeline();
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }

}


// helper function to binary threshold
template <typename InputImageType, typename OutputImageType>
void
BinaryThreshold(
typename InputImageType::Pointer & input,
typename InputImageType::PixelType lowerThreshold,
typename InputImageType::PixelType upperThreshold,
typename OutputImageType::PixelType insideValue,
typename OutputImageType::PixelType outsideValue,
typename OutputImageType::Pointer & output
) throw( ExceptionObject )
{

  typedef BinaryThresholdImageFilter<InputImageType,OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInput( input );
  filter->SetLowerThreshold( lowerThreshold );
  filter->SetUpperThreshold( upperThreshold );
  filter->SetInsideValue( insideValue );
  filter->SetOutsideValue( outsideValue );
  
  try
    {
    filter->Update();
    output = filter->GetOutput();
    output->DisconnectPipeline();
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }

}

// helper function to invert image intensity
template <typename ImageType>
void
InvertImage( 
typename ImageType::Pointer & input,
typename ImageType::Pointer & output
) throw( ExceptionObject )
{
  typedef InvertIntensityImageFilter<ImageType,ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInput( input );
  
  try
    {
    filter->Update();
    output = filter->GetOutput();
    output->DisconnectPipeline();
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }

}

// select on component out of a vector
template <typename InputImageType, typename OutputImageType>
void
VectorIndexSelection( 
typename InputImageType::Pointer & input,
unsigned int index,
typename OutputImageType::Pointer & output
) throw( ExceptionObject )
{
  typedef VectorIndexSelectionCastImageFilter<InputImageType,OutputImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInput( input );
  filter->SetIndex( index );
  
  try
    {
    filter->Update();
    output = filter->GetOutput();
    output->DisconnectPipeline();
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }

}

// signed distance function
template <typename ImageType>
void 
SignedDistance(
typename ImageType::Pointer & input,
typename ImageType::Pointer & output,
float maximumDistance
) throw( ExceptionObject )
{

  typedef itk::ReinitializeLevelSetImageFilter<ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();

  filter->SetInput( input );
  filter->NarrowBandingOn();
  filter->SetNarrowBandwidth( 2 * maximumDistance );

  try
    {
    filter->Update();
    output = filter->GetOutput();
    output->DisconnectPipeline();
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }


}

// helper function compute difference between two images
template <typename ImageType>
void
SubtractImage(
typename ImageType::Pointer & input1,
typename ImageType::Pointer & input2,
typename ImageType::Pointer & output
) throw( ExceptionObject )
{
  typedef SubtractImageFilter<ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInput1( input1 );
  filter->SetInput2( input2 );
  
  try
    {
    filter->Update();
    output = filter->GetOutput();
    output->DisconnectPipeline();
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }

}

// helper function compute difference between two images
template <typename ImageType>
void
ShiftScaleImage(
typename ImageType::Pointer & input,
float shift,
float scale,
typename ImageType::Pointer & output
) throw( ExceptionObject )
{
  typedef ShiftScaleImageFilter<ImageType,ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInput( input );
  filter->SetShift( shift );
  filter->SetScale( scale );
  
  try
    {
    filter->Update();
    output = filter->GetOutput();
    output->DisconnectPipeline();
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }

}

// helper function to compute the correlation between two images
template <typename ImageType>
void
ComputeCorrelation( 
const ImageType * image1,
const ImageType * image2,
double & value 
) throw (ExceptionObject)
{

  typedef NormalizedCorrelationImageToImageMetric<ImageType,ImageType> MetricType;
  typename MetricType::Pointer metric = MetricType::New();
  metric->SetFixedImage( image1 );
  metric->SetMovingImage( image2 );

  typedef TranslationTransform< double, ImageType::ImageDimension > TransformType;
  typename TransformType::Pointer transform = TransformType::New();
  metric->SetTransform( transform );
  typename TransformType::ParametersType parameters( metric->GetNumberOfParameters() );
  parameters.Fill( 0.0 );

  typedef LinearInterpolateImageFunction< ImageType > InterpolatorType;
  typename InterpolatorType::Pointer interpolator = InterpolatorType::New();
  metric->SetInterpolator( interpolator );
  
  try
    {
    metric->ComputeGradientOff();
    metric->SubtractMeanOn();
    metric->SetFixedImageRegion( image1->GetBufferedRegion() );
    metric->Initialize();

    value = metric->GetValue( parameters );
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }



}

// helper function compose mask images
template <typename ImageType>
void
OverwriteImage(
typename ImageType::Pointer & input1,
typename ImageType::Pointer & input2,
typename ImageType::Pointer & output
) throw( ExceptionObject )
{
  typedef OverwriteImageFilter<ImageType,ImageType,ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInput1( input1 );
  filter->SetInput2( input2 );
  
  try
    {
    filter->Update();
    output = filter->GetOutput();
    output->DisconnectPipeline();
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }

}

// helper function compute the minimum image merge
template <typename ImageType, typename LabelType>
void
MinimumIndexImage(
typename ImageType::Pointer & input1,
typename ImageType::Pointer & input2,
typename LabelType::Pointer & outputLabel
) throw( ExceptionObject )
{

  typedef MinimumIndexImageFilter<ImageType,LabelType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInput1( input1 );
  filter->SetInput2( input2 );
  
  try
    {
    filter->Update();
    outputLabel = filter->GetLabelOutput();
    outputLabel->DisconnectPipeline();
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }

}

template <typename ImageType, typename MaskType>
void
MaskImage(
typename ImageType::Pointer & input,
typename ImageType::Pointer & mask,
typename ImageType::Pointer & output,
typename ImageType::PixelType outsideValue
) throw( ExceptionObject )
{
  typedef MaskImageFilter<ImageType,MaskType,ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInput1( input );
  filter->SetInput2( mask );
  filter->SetOutsideValue( outsideValue );
  
  try
    {
    filter->Update();
    output = filter->GetOutput();
    output->DisconnectPipeline();
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }
}

template <typename ImageType>
void
FillRegion(
typename ImageType::Pointer & ref,
typename ImageType::Pointer & output,
typename ImageType::RegionType region,
typename ImageType::PixelType foreground,
typename ImageType::PixelType background
)
throw( ExceptionObject )
{
 
  try
    {
    output = ImageType::New();
    output->SetRegions( ref->GetBufferedRegion() );
    output->SetSpacing( ref->GetSpacing() );
    output->SetOrigin( ref->GetOrigin() );
    output->Allocate();
    output->FillBuffer( background );
    region.Crop( output->GetBufferedRegion() );

    typedef ImageRegionIterator<ImageType> Iterator;
    Iterator it( output, region );
  
    while( !it.IsAtEnd() )
      {
      it.Set( foreground );
      ++it;
      }

    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }

}

template <typename ImageType>
void
IsMember(
typename ImageType::Pointer & input,
typename std::vector<typename ImageType::PixelType> & array,
typename ImageType::Pointer & output,
typename ImageType::PixelType foreground,
typename ImageType::PixelType background
)
throw( ExceptionObject)
{

  typedef IsMemberImageFilter<ImageType,ImageType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInput( input );
  filter->SetInputArray( array );
  filter->SetIsNotMemberValue( background );
  filter->SetIsMemberValue( foreground );
  
  try
    {
    filter->Update();
    output = filter->GetOutput();
    output->DisconnectPipeline();
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }

}

template <typename ImageType>
void
CropImage(
typename ImageType::Pointer & input,
typename ImageType::IndexType & start,
typename ImageType::SizeType & size,
typename ImageType::Pointer & output
)
throw( ExceptionObject )
{
  typename ImageType::RegionType outputRegion;
  outputRegion.SetIndex( start );
  outputRegion.SetSize( size );
  
  bool ok = outputRegion.Crop( input->GetBufferedRegion() );
  if (!ok)
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Error: Crop box is completely outside input image domain.", ITK_LOCATION );
    throw e;
    }

  try
    {
    output = ImageType::New();
    output->CopyInformation( input );
    output->SetRegions( outputRegion );
    output->Allocate();
    
    typedef itk::ImageRegionConstIterator< ImageType > InputIterator;
    typedef itk::ImageRegionIterator< ImageType > OutputIterator;

    InputIterator iiter( input, outputRegion );
    OutputIterator oiter( output, outputRegion );

    while( !oiter.IsAtEnd() )
     {
     oiter.Set( iiter.Get() );
     ++iiter;
     ++oiter;
     }    
    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }

}

// helper function to read transform to file
template< typename TransformType >
void ReadTransform(
const char * filename,
typename TransformType::Pointer & transform
)throw( ExceptionObject )
{
  typedef TransformFileReader ReaderType;
  typename ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( filename );

  try
    {
    reader->Update();

    typedef ReaderType::TransformListType TransformListType;
    TransformListType * tlist = reader->GetTransformList();
    TransformListType::const_iterator it = tlist->begin();

    transform = static_cast<TransformType*>((*it).GetPointer());

    }
  catch( ExceptionObject & excp )
    {
    throw excp;
    }
  catch( ... )
    {
    ExceptionObject e( __FILE__, __LINE__, 
                              "Caught unknown exception", ITK_LOCATION );
    throw e;
    }

}


} // end namespace idp
} //end namespace itk
           

#endif

  
