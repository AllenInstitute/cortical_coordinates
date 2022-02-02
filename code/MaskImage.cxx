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

template <typename ImageType > 
int Pipeline( 
 const char * input,
 const char * mask,
 double & outsideValue,
 const char * output )
{
  typename ImageType::Pointer inputImage;
  typename ImageType::Pointer maskImage;
  typename ImageType::Pointer outputImage;

    itk::idp::ReadImage<ImageType>( input, inputImage );
    itk::idp::ReadImage<ImageType>( mask, maskImage );
    itk::idp::MaskImage<ImageType,ImageType>( inputImage, maskImage, outputImage,
                                         static_cast<typename ImageType::PixelType>(outsideValue));
    itk::idp::WriteImage<ImageType>( output, outputImage );

  return EXIT_SUCCESS;
}

int main( int argc, char *argv[] )
{
  if (argc < 5 )
    {
    std::cout << "Usage: " << argv[0] << " ";
    std::cout << "input mask outsideValue output [format] [dimension]";
    std::cout << std::endl;
    return EXIT_FAILURE;
    }

    
  std::string input = argv[1];
  std::string mask = argv[2];  
  double outsideValue = atof( argv[3] );  
  std::string output = argv[4];
  
  std::string format = "uchar";
  if (argc > 5 )
    {
    format = argv[5];
    }

  unsigned int dimension = 3;
  if ( argc > 6 )
    {
    dimension = atoi( argv[6] );
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
    err = Pipeline<UCHARImageType>( input.c_str(), mask.c_str(), outsideValue, 
                                    output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "ushort" ) == 0 && dimension == 2 )
    {
    err = Pipeline<USHORTImageType>( input.c_str(), mask.c_str(), outsideValue, 
                                    output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "uint" ) == 0 && dimension == 2 )
    {
    err = Pipeline<UINTImageType>( input.c_str(), mask.c_str(), outsideValue, 
                                    output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "uchar" ) == 0 && dimension == 3 )
    {
    err = Pipeline<UCHARVolumeType>( input.c_str(), mask.c_str(), outsideValue, 
                                    output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "ushort" ) == 0 && dimension == 3 )
    {
    err = Pipeline<USHORTVolumeType>( input.c_str(), mask.c_str(), outsideValue, 
                                    output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "uint" ) == 0 && dimension == 3 )
    {
    err = Pipeline<UINTVolumeType>( input.c_str(), mask.c_str(), outsideValue, 
                                    output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "float" ) == 0 && dimension == 3 )
    {
    err = Pipeline<FLOATVolumeType>( input.c_str(), mask.c_str(), outsideValue, 
                                    output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else 
    {
    std::cerr << "format: " << format << " and dimension: " << dimension  << " not supported" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}


