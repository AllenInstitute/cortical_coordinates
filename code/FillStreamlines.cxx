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
  if (argc < 5 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
	  std::cerr << "inputAnnotationFile inputLabelFile labelFile outputFile " << std::endl;
    return EXIT_FAILURE;
    }
    
  std::string inputFile = argv[1];
  std::string inputLabelFile = argv[2];  
  std::string labelFile = argv[3];
  std::string outputFile = argv[4];

  typedef itk::Image<unsigned int, 3> ImageType;  
  
  try
    {
    
    ImageType::Pointer input;
    ImageType::Pointer inputLabel;
    ImageType::Pointer label;
    ImageType::Pointer output;

    // intialize    
    itk::idp::ReadImage<ImageType>( inputFile.c_str(), input );
    itk::idp::ReadImage<ImageType>( inputLabelFile.c_str(), inputLabel );    
    itk::idp::ReadImage<ImageType>( labelFile.c_str(), label );
   
    itk::idp::MakeImage<ImageType,ImageType>( label, output, 0 );
    
    // setup iterators
    typedef itk::ImageRegionIterator<ImageType> IteratorType;
    IteratorType iiter (input, input->GetBufferedRegion());
    IteratorType liter (inputLabel, inputLabel->GetBufferedRegion());   

    // create a streamline-label map
    typedef std::vector<unsigned int> LabelArray;
    typedef std::map<unsigned int, LabelArray> MapType;
    
    MapType streamlineAnnotationMap;
    
    while ( !iiter.IsAtEnd() )
      {
      
      if ( iiter.Get() && liter.Get() )
        {
         // check is streamline label is in the map already
         MapType::iterator it = streamlineAnnotationMap.find( liter.Get() );
         if ( it == streamlineAnnotationMap.end() );
           {
           LabelArray arr;
           streamlineAnnotationMap[liter.Get()] = arr;
           it = streamlineAnnotationMap.find( liter.Get() );
           }
           
         // add streamline label to the map  
         it->second.push_back( iiter.Get() );  
        
        }
      
      ++iiter;
      ++liter;
      }

 /*
    // for each streamline label in the map
    for ( MapType::iterator it = streamlineAnnotationMap.begin();
          it != streamlineAnnotationMap.end(); ++it )
       {
       
       // pick the first annotation label for each streamline label
       unsigned int sline = it->first;
       unsigned int anno = it->second[0];
       std::cout << sline << ":" << anno << std::endl;
    
       // find output streamline with annotation label
       ImageType::Pointer masked;
       itk::idp::BinaryThreshold<ImageType,ImageType>( label, sline, sline, anno, 0, masked );
       itk::idp::OverwriteImage<ImageType>( output, masked, output );
       
       } 
 */      
 
    IteratorType oiter (output, output->GetBufferedRegion());
    IteratorType oliter (label, label->GetBufferedRegion());  

    while ( !oiter.IsAtEnd() )
      {

      if ( oliter.Get() )
        {
        MapType::iterator it = streamlineAnnotationMap.find( oliter.Get() );
        
        if ( it != streamlineAnnotationMap.end() )
          {
          oiter.Set( it->second[0] );
          }
        
        }

      ++oiter;
      ++oliter;
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


