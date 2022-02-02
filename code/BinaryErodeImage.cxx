/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $File: //depot/bioapps/infoapps/HBAMapping/main/Utilities/idpImageDifference.cxx $
  Language:  C++
  Date:      $DateTime: 2011/12/18 23:41:18 $
  Version:   $Revision: #1 $

  Copyright (c) Allen Institute for Brain Science. All rights reserved.

=========================================================================*/

#include "idpUtilities.h"
#include "itkRGBPixel.h"
#include "itksys/String.hxx"
#include <string>

#include "itkBinaryErodeImageFilter.h"
#include "itkBinaryBallStructuringElement.h"

template <typename ImageType > 
int Pipeline( 
 const char * input,
 double & foreground,
 double & background,
 unsigned long & radius,
 const char * output )
{
  typedef typename ImageType::PixelType PixelType;
  typename ImageType::Pointer inputImage;
  typename ImageType::Pointer outputImage;

  itk::idp::ReadImage<ImageType>( input, inputImage );
    
  typedef itk::BinaryBallStructuringElement<PixelType, ImageType::ImageDimension> 
    StructuringElementType;
  StructuringElementType structuringElement;
  structuringElement.SetRadius(radius);
  structuringElement.CreateStructuringElement();

  typedef itk::BinaryErodeImageFilter<ImageType, ImageType, StructuringElementType>
    BinaryDilateImageFilterType;
    
  typename BinaryDilateImageFilterType::Pointer filter = BinaryDilateImageFilterType::New();
  
  filter->SetInput( inputImage );
  filter->SetKernel( structuringElement );
  filter->SetForegroundValue( static_cast<PixelType>(foreground) );
  filter->SetBackgroundValue( static_cast<PixelType>(background) );
  
  filter->Update();
  
  outputImage = filter->GetOutput();
  outputImage->DisconnectPipeline();
          
  itk::idp::WriteImage<ImageType>( output, outputImage );

  return EXIT_SUCCESS;
}

int main( int argc, char *argv[] )
{
  if (argc < 6 )
    {
    std::cout << "Usage: " << argv[0] << " ";
    std::cout << "input foreground background radius output [format] [dimension]";
    std::cout << std::endl;
    return EXIT_FAILURE;
    }

    
  std::string input = argv[1];
  double foreground = atof( argv[2] );
  double background = atof( argv[3] );
  unsigned long radius = atoi( argv[4] );
  std::string output = argv[5];
  
  std::string format = "uchar";
  if (argc > 6 )
    {
    format = argv[6];
    }

  unsigned int dimension = 3;
  if ( argc > 7 )
    {
    dimension = atoi( argv[7] );
    }

  typedef itk::Image< unsigned char, 2 >      UCHARImageType;
  typedef itk::Image< unsigned short, 2 >     USHORTImageType;
  typedef itk::Image< unsigned int,  2 >      UINTImageType;
  typedef itk::Image< unsigned char, 3 >      UCHARVolumeType;
  typedef itk::Image< unsigned short, 3 >     USHORTVolumeType;
  typedef itk::Image< unsigned int,  3 >      UINTVolumeType;
  typedef itk::RGBPixel<unsigned char>        RGBPixelType;
  typedef itk::Image< RGBPixelType, 2 >       RGBImageType;
  typedef itk::Image< float, 3>               FLOATVolumeType;
  typedef itk::Transform<double,2,2>          Transform2DType;
  typedef itk::Transform<double,3,3>          Transform3DType;

  int err;

  if( itksys::SystemTools::Strucmp( format.c_str(), "uchar" ) == 0 && dimension == 2 )
    {
    err = Pipeline<UCHARImageType>( input.c_str(), foreground, background, radius, output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "ushort" ) == 0 && dimension == 2 )
    {
    err = Pipeline<USHORTImageType>( input.c_str(), foreground, background, radius, output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "uint" ) == 0 && dimension == 2 )
    {
    err = Pipeline<UINTImageType>( input.c_str(), foreground, background, radius, output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "uchar" ) == 0 && dimension == 3 )
    {
    err = Pipeline<UCHARVolumeType>( input.c_str(), foreground, background, radius, output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "ushort" ) == 0 && dimension == 3 )
    {
    err = Pipeline<USHORTVolumeType>( input.c_str(), foreground, background, radius, output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "uint" ) == 0 && dimension == 3 )
    {
    err = Pipeline<UINTVolumeType>( input.c_str(), foreground, background, radius, output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "float" ) == 0 && dimension == 3 )
    {
    err = Pipeline<FLOATVolumeType>( input.c_str(), foreground, background, radius, output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else 
    {
    std::cerr << "format: " << format << " and dimension: " << dimension  << " not supported" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}


