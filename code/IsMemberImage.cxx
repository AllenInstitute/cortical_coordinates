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
 const char * input1, 
 const char * input2,
 const char * output )
{
  typename ImageType::Pointer inputImage1;
  typename ImageType::Pointer outputImage;
  
  // read in the textfile
  std::vector<typename ImageType::PixelType> array;
  
  std::fstream myfile( input2, std::ios_base::in );
  
  typename ImageType::PixelType a;
  while ( myfile >> a )
    {
    std::cout << a << std::endl;
    array.push_back( a );    
    }

  itk::idp::ReadImage<ImageType>( input1, inputImage1 );
  itk::idp::IsMember<ImageType>( inputImage1, array, outputImage, 1, 0 );
  itk::idp::WriteImage<ImageType>( output, outputImage );

  return EXIT_SUCCESS;
}

int main( int argc, char *argv[] )
{
  if (argc < 4 )
    {
    std::cout << "Usage: " << argv[0] << " ";
    std::cout << "input textFile output [format] [dimension]";
    std::cout << std::endl;
    return EXIT_FAILURE;
    }

  std::string input1 = argv[1];
  std::string input2 = argv[2];
  std::string output = argv[3];
  
  std::string format = "uchar";
  if (argc > 4 )
    {
    format = argv[4];
    }

  unsigned int dimension = 3;
  if ( argc > 5 )
    {
    dimension = atoi( argv[5] );
    }

  typedef itk::Image< unsigned char, 2 >      UCHARImageType;
  typedef itk::Image< unsigned short, 2 >     USHORTImageType;
  typedef itk::Image< unsigned int,  2 >      UINTImageType;
  typedef itk::Image< unsigned char, 3 >      UCHARVolumeType;
  typedef itk::Image< unsigned short, 3 >     USHORTVolumeType;
  typedef itk::Image< unsigned int,  3 >      UINTVolumeType;
  typedef itk::Image< float, 3>               FLOATVolumeType;

  int err;

  if( itksys::SystemTools::Strucmp( format.c_str(), "uchar" ) == 0 && dimension == 2 )
    {
    err = Pipeline<UCHARImageType>( input1.c_str(), input2.c_str(), 
                                output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "ushort" ) == 0 && dimension == 2 )
    {
    err = Pipeline<USHORTImageType>( input1.c_str(), input2.c_str(), 
                                output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "uint" ) == 0 && dimension == 2 )
    {
    err = Pipeline<UINTImageType>( input1.c_str(), input2.c_str(), 
                                output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "uchar" ) == 0 && dimension == 3 )
    {
    err = Pipeline<UCHARVolumeType>( input1.c_str(), input2.c_str(), 
                                output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "ushort" ) == 0 && dimension == 3 )
    {
    err = Pipeline<USHORTVolumeType>( input1.c_str(), input2.c_str(), 
                                output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "uint" ) == 0 && dimension == 3 )
    {
    err = Pipeline<UINTVolumeType>( input1.c_str(), input2.c_str(), 
                                output.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "float" ) == 0 && dimension == 3 )
    {
    err = Pipeline<FLOATVolumeType>( input1.c_str(), input2.c_str(), 
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


