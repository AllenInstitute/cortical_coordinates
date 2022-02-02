/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: LocateBoundaryUCHAR3.cxx,v $
  Language:  C++
  Date:      $Date: 2004/06/27 19:52:43 $
  Version:   $Revision: 1.5 $

  Copyright (c) Allen Institute for Brain Science. All rights reserved.

=========================================================================*/
#include "idpUtilities.h"

int main( int argc, char *argv[] )
{
  if (argc < 7 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
	  std::cerr << "inputFile inputLabelFile direction outputFile outputLabelFile maxVoxelFile" << std::endl;
    return EXIT_FAILURE;
    }
    
  std::string inputFile = argv[1];
  std::string labelFile = argv[2];
  int direction = atoi( argv[3] );
  std::string outputFile = argv[4];
  std::string outputLabelFile = argv[5];
  std::string maxVoxelFile = argv[6]; 
 
  typedef itk::Image<unsigned short, 3> ImageType; 
  typedef itk::Image<unsigned int, 3> LabelType;  
  
  try
    {
    ImageType::Pointer input;
    LabelType::Pointer label;
    ImageType::Pointer output;
    LabelType::Pointer outputLabel;
    LabelType::Pointer maxVoxel;

    // intialize    
    itk::idp::ReadImage<ImageType>( inputFile.c_str(), input );
    itk::idp::ReadImage<LabelType>( labelFile.c_str(), label );
    
    itk::idp::MakeImage<LabelType,ImageType>( input, maxVoxel, 0 );
    
    ImageType::RegionType region = input->GetBufferedRegion();
    ImageType::SpacingType spacing = input->GetSpacing();
    ImageType::PointType origin = input->GetOrigin();
    
    region.SetSize( direction, 1 );
    region.SetIndex( direction, 0 );
    itk::idp::MakeImage<ImageType>( output, region, spacing, origin, 0 );
    output->SetDirection( input->GetDirection() );
    
    itk::idp::MakeImage<LabelType,ImageType>( output, outputLabel, 0 );

    // setup iterators
    typedef itk::ImageLinearIteratorWithIndex<ImageType> IteratorType;
    
    IteratorType iiter (input, input->GetBufferedRegion());    
    iiter.SetDirection( direction );

    
    while( !iiter.IsAtEnd() )
      {
      
      iiter.GoToBeginOfLine();
      ImageType::IndexType index = iiter.GetIndex();
      
      ImageType::IndexType maxIndex = index;
      unsigned short maxValue = 0;
      
      while ( !iiter.IsAtEndOfLine() )
        {
        if ( iiter.Get() > maxValue )
          {
          maxValue = iiter.Get();
          maxIndex = iiter.GetIndex();
          }          
        ++iiter;    
        }
       
      if ( maxValue > 0 )
        {      
        output->SetPixel( index, maxValue );
        outputLabel->SetPixel( index, label->GetPixel( maxIndex ) );
        maxVoxel->SetPixel( maxIndex, label->GetPixel( maxIndex ) );
        }        

      iiter.NextLine();

      }
    
    itk::idp::WriteImage<ImageType>( outputFile.c_str(), output );
    itk::idp::WriteImage<LabelType>( outputLabelFile.c_str(), outputLabel );
    itk::idp::WriteImage<LabelType>( maxVoxelFile.c_str(), maxVoxel );
    
    
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


