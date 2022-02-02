/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: LocateBoundaryUCHAR3.cxx,v $
  Language:  C++
  Date:      $Date: 2004/06/27 19:52:43 $
  Version:   $Revision: 1.5 $

  Copyright (c) Allen Institute for Brain Science. All rights reserved.

=========================================================================*/
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "idpUtilities.h"

int main( int argc, char *argv[] )
{
  if (argc < 5 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
	  std::cerr << "labelFile factor inputFile outputFile" << std::endl;
    return EXIT_FAILURE;
    }

  typedef unsigned int PixelType;
  typedef itk::Image< PixelType, 3 >          ImageType;
  typedef itk::ImageFileReader<ImageType>     ReaderType;
  typedef itk::ImageFileWriter<ImageType>     WriterType;
  typedef itk::ImageRegionIterator<ImageType> Iterator;
  
  std::string labelFile = argv[1];
  unsigned int factor = atoi( argv[2] );  
  std::string inputFile = argv[3];
  std::string outputFile = argv[4];
  
  ImageType::Pointer label;
  ImageType::Pointer input;
  ImageType::Pointer output;
  ImageType::Pointer mask;
  
  try
    {

    // read in input files
    itk::idp::ReadImage<ImageType>( labelFile.c_str(), label );
    itk::idp::ReadImage<ImageType>( inputFile.c_str(), input );
    
    // prepare output
    itk::idp::MakeImage<ImageType,ImageType>( input, output, 0 );
    
    // subsample label file
    Iterator iter( label, label->GetBufferedRegion() );
    unsigned int counter = 0;
    unsigned int newLabel = 0;
    
    while (!iter.IsAtEnd())
      {
      if ( iter.Get() )
        {
        
        ImageType::IndexType index = iter.GetIndex();
        
        if ( index[0] % factor == 0 &&
             index[1] % factor == 0 &&
             index[2] % factor == 0 )
          {
          std::cout << iter.GetIndex() << std::endl;
        
          itk::idp::BinaryThreshold<ImageType,ImageType>( input, 
            iter.Get(), iter.Get(), ++newLabel, 0, mask );
          
          itk::idp::OverwriteImage<ImageType>( output, mask, output );
          }          
        
        ++counter;
        }
      
      ++iter;
      }
    
    itk::idp::WriteImage<ImageType>( outputFile.c_str(), output );
    
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


