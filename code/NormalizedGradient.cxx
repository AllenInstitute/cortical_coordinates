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
#include "itkGradientImageFilter.h"
#include "itkImageRegionIterator.h"

int main( int argc, char *argv[] )
{
  if (argc < 3 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
	  std::cerr << " inputFile outputFile " << std::endl;
    return EXIT_FAILURE;
    }

  typedef float PixelType;
  typedef itk::Image< PixelType, 3 >          ImageType;
  typedef itk::GradientImageFilter< ImageType,float, float> FilterType;
  typedef FilterType::OutputImageType GradientImageType;
  typedef GradientImageType::PixelType GradientType;

  typedef itk::ImageFileReader<ImageType>     ReaderType;
  typedef itk::ImageFileWriter<GradientImageType>     WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
 
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );

  GradientImageType::Pointer gradient;

  try
    {
    //filter->SetUseImageSpacingOff();
    //filter->UseImageDirectionOff();
    filter->Update();
    gradient = filter->GetOutput();
    gradient->DisconnectPipeline();
    filter = 0;
    reader = 0;
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

  typedef itk::ImageRegionIterator<GradientImageType> Iterator;
  Iterator iter( gradient, gradient->GetBufferedRegion() );
  iter.GoToBegin();

  while( !iter.IsAtEnd() )
    {
    GradientType g = iter.Get();
    float m = g.GetNorm();
    if( vnl_math_abs(m) > 1e-10 )
      {
      g /= m;
      }
    else
      {
      g.Fill( 0.0 );
      }
    iter.Set( g );
    ++iter;
    }

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( gradient );
  writer->SetFileName( argv[2] );
  writer->UseCompressionOn();
  
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


