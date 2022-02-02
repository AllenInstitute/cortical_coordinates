/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $File$
  Language:  C++
  Date:      $DateTime$
  Version:   $Revision$

  Copyright (c) Allen Institute for Brain Science. All rights reserved.

=========================================================================*/

#include "idpUtilities.h"
#include "itkRGBPixel.h"
#include "itksys/String.hxx"
#include <string>


template <typename ImageType > 
int Pipeline( 
 const char * infile, 
 typename ImageType::IndexType & start,
 typename ImageType::SizeType & size,
 const char * outfile )
{
  typename ImageType::Pointer input;
  typename ImageType::Pointer output;

  itk::idp::ReadImage<ImageType>( infile, input );
  
  std::cout << input->GetBufferedRegion().GetSize() << std::endl;
  
  itk::idp::CropImage<ImageType>( input, start, size, output );
  std::cout << "crop done" << std::endl;

  itk::idp::WriteImage<ImageType>( outfile, output ); 

  return EXIT_SUCCESS;
}



int main( int argc, char *argv[] )
{
  if (argc < 9 )
    {
    std::cout << "Usage: " << argv[0] << " ";
    std::cout << "inputFile outputFile startx starty startz sizex sizey sizez";
    std::cout << "[format]";
    std::cout << std::endl;
    return EXIT_FAILURE;
    }

  std::string infile = argv[1];
  std::string outfile = argv[2];
  
  itk::Index<3> start;
  itk::Size<3> size;
  start[0] = atoi(argv[3]);
  start[1] = atoi(argv[4]);
  start[2] = atoi(argv[5]);
  size[0] = atoi(argv[6]);
  size[1] = atoi(argv[7]);
  size[2] = atoi(argv[8]);  
  
  std::string format = "uchar";
  if (argc > 9 )
    {
    format = argv[9];
    }


   std::cout << infile << std::endl; 
   std::cout << outfile << std::endl;
   std::cout << start << std::endl;
   std::cout << size << std::endl;
   std::cout << format << std::endl;

    
  typedef itk::Image< unsigned char, 3 >      UCHARVolumeType;
  typedef itk::Image< unsigned short, 3 >     USHORTVolumeType;
  typedef itk::Image< unsigned int,  3 >      UINTVolumeType;
  typedef itk::Image< float, 3>               FLOATVolumeType;

  int err;

  if( itksys::SystemTools::Strucmp( format.c_str(), "uchar" ) == 0 )
    {
    err = Pipeline<UCHARVolumeType>( infile.c_str(), start, size, outfile.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "ushort" ) == 0 )
    {
    err = Pipeline<UCHARVolumeType>( infile.c_str(), start, size, outfile.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "uint" ) == 0 )
    {
    err = Pipeline<UCHARVolumeType>( infile.c_str(), start, size, outfile.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
  else if( itksys::SystemTools::Strucmp( format.c_str(), "float" ) == 0  )
    {
    err = Pipeline<UCHARVolumeType>( infile.c_str(), start, size, outfile.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
   else 
    {
    std::cerr << "format: " << format << " not supported" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}


