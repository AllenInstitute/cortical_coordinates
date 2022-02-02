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

typedef float PixelType;
typedef itk::Image< PixelType, 3 >      ImageType;
typedef itk::Image< unsigned int, 3>    IndexImageType;

  
int main( int argc, char *argv[] )
{
  if (argc < 3 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputFile outputFile" << std::endl;
    return EXIT_FAILURE;
    }
    
    std::string inputFile = argv[1];
    std::string outputFile = argv[2];   
    
  try
    {
    // declare image objects
    ImageType::Pointer input;
    IndexImageType::Pointer output;  
    
    // read files
    itk::idp::ReadImage<ImageType>( inputFile.c_str(), input );
    std::cout << inputFile << " ok" << std::endl;    
    
    // initialize output images
    itk::idp::MakeImage<IndexImageType,ImageType>( input, output, 0 );    
    
    typedef itk::ImageRegionIteratorWithIndex<ImageType> Iterator;
    typedef itk::ImageRegionIterator<IndexImageType> IndexIterator;
    
    IndexIterator oiter ( output, output->GetBufferedRegion() );
    Iterator iiter( input, input->GetBufferedRegion() );
    
    while( !iiter.IsAtEnd() )
      {
      if ( iiter.Get() )
        {
        oiter.Set( input->ComputeOffset( iiter.GetIndex() ) + 1 );
        }     
      ++iiter;
      ++oiter;
      }
 
   
    // write out files
    itk::idp::WriteImage<IndexImageType>( outputFile.c_str(), output, true );
    
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
