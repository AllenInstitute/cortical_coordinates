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
#include "itkShrinkImageFilter.h"
#include "itkImageSliceIteratorWithIndex.h"
#include "itkMaskImageFilter.h"
#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkResampleImageFilter.h"
#include "itkTranslationTransform.h"
#include "itkNearestNeighborInterpolateImageFunction.h"

int main( int argc, char *argv[] )
{
  if (argc < 4 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
	  std::cerr << "inputFile foreground outputFile" << std::endl;
    return EXIT_FAILURE;
    }

  typedef unsigned int PixelType;
  typedef itk::Image< PixelType, 3 >          ImageType;
  typedef itk::ImageFileReader<ImageType>     ReaderType;
  typedef itk::ImageFileWriter<ImageType>     WriterType;
  typedef itk::ImageRegionIterator<ImageType> Iterator;
  typedef itk::ImageSliceIteratorWithIndex<ImageType> SliceIterator;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();
  
  unsigned int foreground = atoi( argv[2] );
  
  ImageType::Pointer output = reader->GetOutput();
  output->DisconnectPipeline();

  unsigned int counter = 0;
  SliceIterator iter( output, output->GetBufferedRegion() );
  iter.SetFirstDirection( 0 );
  iter.SetSecondDirection( 2 );
  iter.GoToBegin();

  while( !iter.IsAtEnd() )
    {
    while( !iter.IsAtEndOfSlice() )
      {
      while( !iter.IsAtEndOfLine() )
        {
        if ( iter.Get() == foreground )
          {
          counter++;
          iter.Set( counter );
          //std::cout << counter << ":" << iter.GetIndex() << std::endl;         
          }
        else
          {
          iter.Set( 0 );
          }
        ++iter;
        }
      iter.NextLine();
      }
    iter.NextSlice();
    }

  std::cout << "labelCount: " << counter << std::endl;

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( output );
  writer->SetFileName( argv[3] );
  
  try
    {
    writer->Update(); 
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


