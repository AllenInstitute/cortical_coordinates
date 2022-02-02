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
  if (argc < 6 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
	  std::cerr << "inputFile inputLabelFile outputLabelFile outputFile maxVoxelFile" << std::endl;
    return EXIT_FAILURE;
    }
    
  std::string inputFile = argv[1];
  std::string labelFile = argv[2];
  std::string outputLabelFile = argv[3];
  std::string outputFile = argv[4];
  std::string maxVoxelFile = argv[5]; 
 
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
    itk::idp::ReadImage<LabelType>( outputLabelFile.c_str(), outputLabel );
 
    itk::idp::MakeImage<ImageType,LabelType>( outputLabel, output, 0 ); 
    itk::idp::MakeImage<LabelType,ImageType>( input, maxVoxel, 0 );
    

    // make a map of max value/index per streamline label
    // setup iterators
    typedef itk::ImageRegionIteratorWithIndex<ImageType> IteratorType;
    typedef itk::ImageRegionIteratorWithIndex<LabelType> LabelIteratorType;
    
    IteratorType iiter( input, input->GetBufferedRegion() );
    LabelIteratorType liter( label, label->GetBufferedRegion() );

    typedef std::map<unsigned int, unsigned short> MaxValueMapType;
    typedef std::map<unsigned int, ImageType::IndexType> MaxIndexMapType;
    
    MaxValueMapType maxValueMap;
    MaxIndexMapType maxIndexMap;
    ImageType::IndexType dummy;
    dummy.Fill( 0 );
    
    while( !iiter.IsAtEnd() )
      {
      if ( liter.Get() && iiter.Get() )
        {
        
        MaxValueMapType::iterator it = maxValueMap.find( liter.Get() );
        if ( it == maxValueMap.end() )
          {
          maxValueMap[liter.Get()] = 0;
          maxIndexMap[liter.Get()] = dummy;
          it = maxValueMap.find( liter.Get() );
          }
        
       if ( iiter.Get() > it->second )
         {
         it->second = iiter.Get();
         maxIndexMap[liter.Get()] = iiter.GetIndex();
         }         
          
        }
        
      ++iiter;
      ++liter;
      }
      
    // fill in output and maxVoxel  
    LabelIteratorType oliter ( outputLabel, outputLabel->GetBufferedRegion() );
    IteratorType         oiter( output, output->GetBufferedRegion() );
    
    while ( !oliter.IsAtEnd() )
      {
      if ( oliter.Get() )
        {
        MaxValueMapType::iterator it = maxValueMap.find( oliter.Get() );
        if ( it != maxValueMap.end() )
          {
          oiter.Set( it->second );
          maxVoxel->SetPixel( maxIndexMap[oliter.Get()], oliter.Get() );
          }
        }
      ++oliter;
      ++oiter;
      }
    
    itk::idp::WriteImage<ImageType>( outputFile.c_str(), output );
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


