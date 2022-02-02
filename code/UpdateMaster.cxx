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


template <typename ImageType > 
int Pipeline( 
 const char * masterFile,
 const char * updateFile,
 const char * outputFile,
 unsigned long deleteValue )
 {
 
  typedef typename ImageType::PixelType PixelType;
  
   // declare image objects
  typename ImageType::Pointer master;
  typename ImageType::Pointer update;
  
  // read files
  itk::idp::ReadImage<ImageType>( masterFile, master );
  std::cout << masterFile << " ok" << std::endl;    
  itk::idp::ReadImage<ImageType>( updateFile, update );
  std::cout << updateFile << " ok" << std::endl;    
  
  typedef itk::ImageRegionIterator<ImageType> Iterator;
  
  Iterator miter ( master, master->GetBufferedRegion() );
  Iterator uiter ( update, update->GetBufferedRegion() );
  
  while( !miter.IsAtEnd() )
    {
    if ( uiter.Get() && uiter.Get() != deleteValue )
      {
      miter.Set( uiter.Get() );
      }
    else if ( uiter.Get() == deleteValue )
      {
      miter.Set( 0 );
      }      
    ++miter;
    ++uiter;
    }

  // write out files
  itk::idp::WriteImage<ImageType>( outputFile, master, true );
  
  return EXIT_SUCCESS;
 
 }

  
int main( int argc, char *argv[] )
{
  if (argc < 5 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " masterFile updateFile outputFile deleteValue [format]" << std::endl;
    return EXIT_FAILURE;
    }
    
    std::string masterFile = argv[1];
    std::string updateFile = argv[2];
    std::string outputFile = argv[3];
    unsigned long deleteValue = atol(argv[4]);

    std::string format = "ushort";
    
    if (argc > 5)
      {
      format = argv[5];
      }
      
    typedef itk::Image< unsigned char, 3 >      UCHARVolumeType;
    typedef itk::Image< unsigned short, 3 >     USHORTVolumeType;
    typedef itk::Image< unsigned int,  3 >      UINTVolumeType;
    
    std::cout << format << std::endl;
        
  try
    {
    int err;
    
   if( itksys::SystemTools::Strucmp( format.c_str(), "uchar" ) == 0 )
    {
    err = Pipeline<UCHARVolumeType>( masterFile.c_str(), updateFile.c_str(), outputFile.c_str(), deleteValue );
    if ( err ) return EXIT_FAILURE;
    }
   else if( itksys::SystemTools::Strucmp( format.c_str(), "ushort" ) == 0 )
    {
    err = Pipeline<USHORTVolumeType>( masterFile.c_str(), updateFile.c_str(), outputFile.c_str(), deleteValue );
    if ( err ) return EXIT_FAILURE;
    }  
   else if( itksys::SystemTools::Strucmp( format.c_str(), "uint" ) == 0 )
    {
    err = Pipeline<UINTVolumeType>( masterFile.c_str(), updateFile.c_str(), outputFile.c_str(), deleteValue );
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
