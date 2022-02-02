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
#include "itkNeighborhoodIterator.h"
#include <iomanip>

int main( int argc, char *argv[] )
{

  // Parse command line arguments using itksys tool
  std::string inputFile = ""; 
  std::string outputFile = "";
  std::string orphanedFile = "";
  int foreground = 2;
  
  std::string outputDirectory = "";
  int debugLevel = 1;  
  int threads = 5;

  {
  typedef itksys::CommandLineArguments argT;

  argT arg;
  arg.Initialize( argc, argv );

  arg.AddArgument( "--threads", argT::SPACE_ARGUMENT, &threads, "maximum number of threads" );
  arg.AddArgument( "--debugLevel", argT::SPACE_ARGUMENT, &debugLevel, "debugging level" ); 
  arg.AddArgument( "--outputDirectory", argT::SPACE_ARGUMENT, &outputDirectory, "output directory for debugging" ); 
  arg.AddArgument( "--foreground", argT::SPACE_ARGUMENT, &foreground, "label of foreground" ); 
  
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
    std::cerr << "[--options] inputFile outputFile orphanedFile";
    std::cerr << std::endl;
    std::cerr << arg.GetHelp() << std::endl;
    arg.DeleteRemainingArguments( lastArgc, &lastArgv );
    return EXIT_FAILURE;
    }

  inputFile = lastArgv[ lastArgc - 3];
  outputFile = lastArgv[ lastArgc - 2];
  orphanedFile = lastArgv[ lastArgc - 1];  
  
  std::cout << "threads: " << threads << std::endl;
  std::cout << "inputFile: " << inputFile << std::endl;
  std::cout << "outputFile: " << outputFile << std::endl;
  std::cout << "orphanedFile: " << orphanedFile << std::endl;
  std::cout << "foreground: " << foreground << std::endl;  
  std::cout << "outputDirectory: " << outputDirectory << std::endl;
  std::cout << "debugLevel: " << debugLevel << std::endl;    
  
  arg.DeleteRemainingArguments( lastArgc, &lastArgv );
  }

  try
    {
    
    // Limit the number of threads used
    itk::MultiThreader::SetGlobalMaximumNumberOfThreads( threads );
    
    typedef itk::Image<unsigned int,3> AnnotationVolumeType;
    typedef itk::Image<float,3> FloatVolumeType;
    
    AnnotationVolumeType::Pointer input;
    AnnotationVolumeType::Pointer output;
    AnnotationVolumeType::Pointer orphaned;    
 
    itk::idp::ReadImage<AnnotationVolumeType>( inputFile.c_str(), input );
    itk::idp::MakeImage<AnnotationVolumeType,AnnotationVolumeType>( input, output, 0 );
    itk::idp::MakeImage<AnnotationVolumeType,AnnotationVolumeType>( input, orphaned, 0 );

    // -----    
   std::cout << "Identify boundary pixels ..." << std::endl;
   
   typedef itk::ImageRegionIterator<AnnotationVolumeType> AnnotationIterator;
   typedef itk::NeighborhoodIterator<AnnotationVolumeType> NeighbourhoodIterator;
   typedef AnnotationVolumeType::RegionType RegionType;
   
   RegionType computeRegion = input->GetBufferedRegion();
   
   AnnotationVolumeType::SizeType radius;
   radius.Fill( 1 );
   
   NeighbourhoodIterator iiter( radius, input, computeRegion );   // input annotation
   AnnotationIterator biter( output, computeRegion );  // output label
   AnnotationIterator piter( orphaned, computeRegion ); // orphaned label
   
   iiter.GoToBegin();
   biter.GoToBegin();
   piter.GoToBegin(); 

   std::vector< NeighbourhoodIterator::OffsetType > offsets;
     for ( int i = 0; i < 3; i++ )
      {
      for ( int j = -1; j < 2; j = j + 2 )
        {      
        NeighbourhoodIterator::OffsetType t;
        t.Fill(0);
        t[i] = j;
        offsets.push_back( t );
        }
      }     
    
   while ( !iiter.IsAtEnd() )
     {
     
     if ( iiter.GetCenterPixel() == static_cast<unsigned int>(foreground) )
       {
       
       // initialized label counter
       typedef std::map<unsigned int, unsigned int> MapType;
       MapType labelCounter;
       
       for ( unsigned int i = 0; i < offsets.size(); i++ )
         {
         
           unsigned int lab = iiter.GetPixel( offsets[i] );
         
           if ( lab == static_cast<unsigned int>( foreground ) )
           {
           continue;
           }
           
      
         MapType::iterator it;
         
         // has this label been seen already?
         it = labelCounter.find( lab );
         
         if ( it == labelCounter.end() )
           {
           labelCounter[lab] = 0;
           it = labelCounter.find( lab );
           }
           
         // increment counter  
         it->second += 1;
         }
       
       if ( !labelCounter.empty() )
         {
         unsigned int maxCount = 0;
         unsigned int maxLabel = 0;
         
         for ( MapType::iterator it = labelCounter.begin(); it != labelCounter.end(); ++it )
           {
           if ( it->second > maxCount )
             {
             maxCount = it->second;
             maxLabel = it->first;
             }
           }
           
          if ( maxCount > 0 )
            {
            biter.Set( maxLabel );
            }

          if ( labelCounter.find( 0 ) != labelCounter.end() )
            {
            piter.Set( 1 );
            }            

         }

      }
     
     ++iiter;
     ++biter;
     ++piter;   
     }
 
    itk::idp::WriteImage<AnnotationVolumeType>( outputFile.c_str(), output );
    itk::idp::WriteImage<AnnotationVolumeType>( orphanedFile.c_str(), orphaned );

                    
   std::cout << "[done]" << std::endl;
   
   // -----  

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
