/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: ComputeStreamlinesUSHORT3.cxx,v $
  Language:  C++
  Date:      $Date: 2004/06/27 19:52:43 $
  Version:   $Revision: 1.5 $

  Copyright (c) Allen Institute for Brain Science. All rights reserved.

=========================================================================*/
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkVector.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageLinearIteratorWithIndex.h"
#include "idpUtilities.h"
#include <itksys/SystemTools.hxx>
#include <itksys/Directory.hxx>

typedef itk::Image< unsigned int, 3>    IndexImageType;

template <typename ImageType > 
int Pipeline( 
 const char * inputFile,
 const char * indexFile,
 const char * outputFile )
 {
 
  typedef typename ImageType::PixelType PixelType;
  
   // declare image objects
  typename ImageType::Pointer input;
  typename IndexImageType::Pointer indexImage;
  typename ImageType::Pointer output;  
  
  // read files
  itk::idp::ReadImage<ImageType>( inputFile, input );
  std::cout << inputFile << " ok" << std::endl;    
  itk::idp::ReadImage<IndexImageType>( indexFile, indexImage );
  std::cout << indexFile << " ok" << std::endl;    
  
  // initialize output images
  itk::idp::MakeImage<ImageType,IndexImageType>( indexImage, output, 0 );    
  PixelType * inputBuffer = input->GetBufferPointer();
  
  typedef itk::ImageRegionIterator<ImageType> Iterator;
  typedef itk::ImageRegionIterator<IndexImageType> IndexIterator;
  
  Iterator oiter ( output, output->GetBufferedRegion() );
  IndexIterator iiter( indexImage, indexImage->GetBufferedRegion() );
  
  while( !iiter.IsAtEnd() )
    {
    if ( iiter.Get() )
      {
      oiter.Set( inputBuffer[ iiter.Get() - 1 ] );
      }     
    ++iiter;
    ++oiter;
    }


  // write out files
  itk::idp::WriteImage<ImageType>( outputFile, output, true );
  
  return EXIT_SUCCESS;
 
 }

  
int main( int argc, char *argv[] )
{
  if (argc < 4 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputFile indexFile outputFile [format]" << std::endl;
    return EXIT_FAILURE;
    }
    
    std::string inputFile = argv[1];
    std::string indexFile = argv[2];
    std::string outputFile = argv[3];

    std::string format = "float";
    
    if (argc > 4)
      {
      format = argv[4];
      }
      
    typedef itk::Image< unsigned char, 3 >      UCHARVolumeType;
    typedef itk::Image< unsigned short, 3 >     USHORTVolumeType;
    typedef itk::Image< unsigned int,  3 >      UINTVolumeType;
    typedef itk::Image< float, 3>               FLOATVolumeType;
    
    std::cout << format << std::endl;
        
  try
    {
    int err;
    
   if( itksys::SystemTools::Strucmp( format.c_str(), "uchar" ) == 0 )
    {
    err = Pipeline<UCHARVolumeType>( inputFile.c_str(), indexFile.c_str(), outputFile.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
   else if( itksys::SystemTools::Strucmp( format.c_str(), "ushort" ) == 0 )
    {
    err = Pipeline<USHORTVolumeType>( inputFile.c_str(), indexFile.c_str(), outputFile.c_str() );
    if ( err ) return EXIT_FAILURE;
    }  
   else if( itksys::SystemTools::Strucmp( format.c_str(), "uint" ) == 0 )
    {
    err = Pipeline<UINTVolumeType>( inputFile.c_str(), indexFile.c_str(), outputFile.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
   else if ( itksys::SystemTools::Strucmp( format.c_str(), "float" ) == 0 )
    {
    err = Pipeline<FLOATVolumeType>( inputFile.c_str(), indexFile.c_str(), outputFile.c_str() );
    if ( err ) return EXIT_FAILURE;
    }      
  else 
    {
    std::cerr << "format: " << format << " not supported" << std::endl;
    return EXIT_FAILURE;
    }
    
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }
  catch( ... )
    {
    std::cerr << "Caught unknown exception." << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
