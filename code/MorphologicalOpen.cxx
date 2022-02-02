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
#include "itkBinaryBallStructuringElement.h"
#include "itkBinaryErodeImageFilter.h"
#include "itkBinaryDilateImageFilter.h"

template <typename ImageType > 
int Pipeline( 
 const char * input,
 int & label,
 int & radius,
 const char * output )
{
  typename ImageType::Pointer inputImage;
  typename ImageType::Pointer outputImage;
  
    itk::idp::ReadImage<ImageType>( input, inputImage );

  typedef itk::BinaryBallStructuringElement< typename ImageType::PixelType, 
                                             ImageType::ImageDimension  > ElementType;
  ElementType ball;
  ball.SetRadius( radius );
  ball.CreateStructuringElement(); 

  typedef itk::BinaryErodeImageFilter< ImageType, ImageType, ElementType > FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInput( inputImage );
  filter->SetKernel( ball );
  filter->SetErodeValue( label );
  
  typedef itk::BinaryDilateImageFilter< ImageType, ImageType, ElementType > Filter2Type;
  typename Filter2Type::Pointer filter2 = Filter2Type::New();
  filter2->SetInput( filter->GetOutput() );
  filter2->SetKernel( ball );
  filter2->SetDilateValue( label );  

  filter->Update();

  outputImage = filter2->GetOutput();   


    itk::idp::WriteImage<ImageType>( output, outputImage );

  return EXIT_SUCCESS;
}

int main( int argc, char *argv[] )
{
  if (argc < 4 )
    {
    std::cout << "Usage: " << argv[0] << " ";
    std::cout << "input label radius output [format] [dimension]";
    std::cout << std::endl;
    return EXIT_FAILURE;
    }

    
  std::string input = argv[1];
  int label = atof( argv[2] ); 
  int radius = atof( argv[3] );
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
    err = Pipeline<UCHARImageType>( input.c_str(), label, radius, output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "ushort" ) == 0 && dimension == 2 )
    {
    err = Pipeline<USHORTImageType>( input.c_str(), label, radius,  output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "uint" ) == 0 && dimension == 2 )
    {
    err = Pipeline<UINTImageType>( input.c_str(), label, radius,  output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "uchar" ) == 0 && dimension == 3 )
    {
    err = Pipeline<UCHARVolumeType>( input.c_str(), label, radius,  output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "ushort" ) == 0 && dimension == 3 )
    {
    err = Pipeline<USHORTVolumeType>( input.c_str(), label, radius,  output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "uint" ) == 0 && dimension == 3 )
    {
    err = Pipeline<UINTVolumeType>( input.c_str(), label, radius,  output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "float" ) == 0 && dimension == 3 )
    {
    err = Pipeline<FLOATVolumeType>( input.c_str(), label, radius,  output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else 
    {
    std::cerr << "format: " << format << " and dimension: " << dimension  << " not supported" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}


