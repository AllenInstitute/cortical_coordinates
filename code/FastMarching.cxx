/*=========================================================================

  Program:   Informatics Data Pipeline (IDP)
  Module:    $File: //depot/bioapps/infoapps/HBAMapping/main/Modules/idpNisslAlignmentModule.cxx $
  Language:  C++
  Date:      $DateTime: 2012/01/20 10:38:40 $
  Version:   $Revision: #1 $

  Copyright (c) Allen Institute for Brain Science. All rights reserved.

=========================================================================*/

#include <iostream>
#include <itksys/CommandLineArguments.hxx>

#include "itkImage.h"
#include "tinyxml.h"
#include "idpXMLUtilities.h"
#include "idpUtilities.h"
#include "itkFastMarchingImageFilter.h"
#include "itkThresholdImageFilter.h"
#include <iomanip>

int main( int argc, char *argv[] )
{
  // input arguments:
  //  - the speed map
  //  - starting seeds map
  //  - stopping value  
  //  - output distance file name

  // Parse command line arguments using itksys tool
  std::string speedFile = ""; 
  std::string seedsFile = "";
  std::string outputFile = "";
  double stoppingValue = 100.0;
  
  int debugLevel = 1;  
  int threads = 5;
  
 {
  typedef itksys::CommandLineArguments argT;

  argT arg;
  arg.Initialize( argc, argv );

  arg.AddArgument( "--threads", argT::SPACE_ARGUMENT, &threads, "maximum number of threads" );
  arg.AddArgument( "--debugLevel", argT::SPACE_ARGUMENT, &debugLevel, "debugging level" ); 
  arg.AddArgument( "--stoppingValue", argT::SPACE_ARGUMENT, &stoppingValue, "stopping value" ); 
  
  arg.StoreUnusedArguments( true );

  if ( !arg.Parse() )
    {
    std::cerr << "Problem parsing command line arguments" << std::endl;
    return EXIT_FAILURE;
    }

  char ** lastArgv;
  int lastArgc;
  arg.GetUnusedArguments( &lastArgc, &lastArgv );
  
  if ( lastArgc < 4 )
    {
    std::cerr << "Error: missing input arguments" << std::endl;
    std::cerr << "Usage: " << argv[0] << " ";
    std::cerr << "[--options] speedFile seedsFile outputFile";
    std::cerr << std::endl;
    std::cerr << arg.GetHelp() << std::endl;
    arg.DeleteRemainingArguments( lastArgc, &lastArgv );
    return EXIT_FAILURE;
    }

  speedFile = lastArgv[ lastArgc - 3];
  seedsFile = lastArgv[ lastArgc - 2];
  outputFile = lastArgv[ lastArgc - 1];  
  
  std::cout << "threads: " << threads << std::endl;
  std::cout << "speedFile: " << speedFile << std::endl;
  std::cout << "seedsFile: " << seedsFile << std::endl;
  std::cout << "outputFile: " << outputFile << std::endl;
  std::cout << "stoppingValue: " << stoppingValue << std::endl;  
  std::cout << "debugLevel: " << debugLevel << std::endl;    
  
  arg.DeleteRemainingArguments( lastArgc, &lastArgv );
  }
  
  try
    {

    // Limit the number of threads used
    itk::MultiThreader::SetGlobalMaximumNumberOfThreads( threads );

    typedef itk::Image<unsigned char,3> AnnotationVolumeType;
    typedef itk::Image<float,3> FloatVolumeType;
    
    AnnotationVolumeType::Pointer seeds;
    FloatVolumeType::Pointer speed;
    FloatVolumeType::Pointer output;
    
    std::cout << "Read in the images ..." << std::endl;
    itk::idp::ReadImage<AnnotationVolumeType>( seedsFile.c_str(), seeds );
    itk::idp::ReadImage<FloatVolumeType>( speedFile.c_str(), speed );

    // Convert seeds into trial points for fastmarching filter
    std::cout << "Make a container of trial points ..." << std::endl;
    
    typedef itk::FastMarchingImageFilter<FloatVolumeType,FloatVolumeType> FilterType;
    typedef FilterType::NodeContainer NodeContainer;
    typedef FilterType::NodeType      NodeType;
    NodeContainer::Pointer container = NodeContainer::New();
    
    container->Initialize();
    
    typedef itk::ImageRegionConstIteratorWithIndex<AnnotationVolumeType> AnnotationIterator;
    AnnotationIterator aiter( seeds, seeds->GetBufferedRegion() );
    
    unsigned long count = 0L;
    aiter.GoToBegin();
    
    while( !aiter.IsAtEnd() )
     {
     if ( aiter.Get() ) // seed voxel
       {
       NodeType node;
       double seedValue = 0.0;
       node.SetValue( seedValue );
       node.SetIndex( aiter.GetIndex() );
       container->InsertElement( count, node );
       count++;
       }       
        
     ++aiter;   
     }
     
     std::cout << container->Size() << std::endl;
    
    // use fastmarching filter
    std::cout << "Start Fast Marching ..." << std::endl;
    FilterType::Pointer filter = FilterType::New();
    filter->SetTrialPoints( container );
    filter->SetStoppingValue( stoppingValue );
    filter->SetInput( speed );
    
    filter->Update();
    
    typedef itk::ThresholdImageFilter<FloatVolumeType> ThresholdType;
    ThresholdType::Pointer threshold = ThresholdType::New();
    
    threshold->SetInput( filter->GetOutput() );
    threshold->ThresholdAbove( stoppingValue );
    threshold->SetOutsideValue( stoppingValue );

    output = threshold->GetOutput();

    itk::idp::WriteImage<FloatVolumeType>( outputFile.c_str(), output );    
    
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }
  catch( ... )
    {
    std::cerr << "Caught unknown exception" << std::endl;
    return EXIT_FAILURE;
    }


  return EXIT_SUCCESS;

}
