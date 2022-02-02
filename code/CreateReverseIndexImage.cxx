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

  
int main( int argc, char *argv[] )
{
  if (argc < 4 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputIndexFile referenceFile outputIndexFile" << std::endl;
    return EXIT_FAILURE;
    }
    
    std::string inputFile = argv[1];
    std::string referenceFile = argv[2];
    std::string outputFile = argv[3];   
    
  try
    {
    // declare image objects
    IndexImageType::Pointer input;
    IndexImageType::Pointer reference;
    IndexImageType::Pointer output;  
    
    // read files
    itk::idp::ReadImage<IndexImageType>( inputFile.c_str(), input );
    std::cout << inputFile << " ok" << std::endl;    
    
    itk::idp::ReadImage<IndexImageType>( referenceFile.c_str(), reference );
    std::cout << referenceFile << " ok" << std::endl;
    
    // initialize output images
    itk::idp::MakeImage<IndexImageType,IndexImageType>( reference, output, 0 );    
    
    typedef itk::ImageRegionIteratorWithIndex<IndexImageType> Iterator;
    Iterator iiter( input, input->GetBufferedRegion() );
    
    unsigned int * outputBuffer = output->GetBufferPointer();
    
    while( !iiter.IsAtEnd() )
      {
      if ( iiter.Get() )
        {
        outputBuffer[iiter.Get() - 1] = ( input->ComputeOffset( iiter.GetIndex() ) + 1 );
        }     
      ++iiter;
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
