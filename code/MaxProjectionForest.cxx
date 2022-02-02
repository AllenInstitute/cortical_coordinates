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

typedef itk::Image< unsigned int, 2 > ForestType;
typedef itk::Image< unsigned int, 3> ForestIndexType;

template <typename ImageType > 
int Pipeline( 
 std::string & inputFile,
 std::string & forestDirectory,
 std::string & forestIndexFile,
 std::string & outputFile,
 std::string & maxIndexFile,
 std::string & parentMaxFile,
 std::string & childMaxFile )
 {
  // declare image objects
  typename ImageType::Pointer input;
  typename ForestIndexType::Pointer forestIndex;
  typename ImageType::Pointer childMax;
  typename ImageType::Pointer parentMax;
  typename ForestIndexType::Pointer childMaxIndex;
  typename ForestIndexType::Pointer parentMaxIndex;
  typename ImageType::Pointer output;
  typename ForestIndexType::Pointer maxIndex;

  typedef  typename ImageType::PixelType PixelType;
  
  // read files
  itk::idp::ReadImage<ImageType>( inputFile.c_str(), input );
  std::cout << inputFile << " ok" << std::endl;    
  itk::idp::ReadImage<ForestIndexType>( forestIndexFile.c_str(), forestIndex );
  std::cout << forestIndexFile << " ok" << std::endl;    
  
  // initialize output images
  itk::idp::MakeImage<ImageType,ImageType>( input, childMax, 0 );
  itk::idp::MakeImage<ImageType,ImageType>( input, parentMax, 0 );
  itk::idp::MakeImage<ImageType,ImageType>( input, output, 0 );
  itk::idp::MakeImage<ForestIndexType,ImageType>( input, childMaxIndex, 0 );
  itk::idp::MakeImage<ForestIndexType,ImageType>( input, parentMaxIndex, 0 );
  itk::idp::MakeImage<ForestIndexType,ImageType>( input, maxIndex, 0 );
  
  PixelType * inputBuffer = input->GetBufferPointer();
  PixelType * childMaxBuffer = childMax->GetBufferPointer();
  PixelType * parentMaxBuffer = parentMax->GetBufferPointer();
  PixelType * outputBuffer = output->GetBufferPointer();
  unsigned int * forestIndexBuffer = forestIndex->GetBufferPointer();
  unsigned int * childMaxIndexBuffer = childMaxIndex->GetBufferPointer();
  unsigned int * parentMaxIndexBuffer =  parentMaxIndex->GetBufferPointer();
  unsigned int * maxIndexBuffer = maxIndex->GetBufferPointer();
  
  unsigned long count = 0;
  
  itksys::Directory directory;
  directory.Load( forestDirectory.c_str() );
  unsigned long numberOfFiles = directory.GetNumberOfFiles();
  
  for ( unsigned int i = 0; i < numberOfFiles; i++ )
    {
    
    std::string forestFile = forestDirectory;
    forestFile += "/";
    forestFile += directory.GetFile(i);
    
    if ( itksys::SystemTools::FileIsDirectory( forestFile.c_str() ) )
      {
      continue;
      }
      
    ForestType::Pointer forest; 
    itk::idp::ReadImage<ForestType>( forestFile.c_str(), forest );
    
    std::cout << forestFile << ": ";
    std::cout << forest->GetBufferedRegion().GetSize() << std::endl;
  
    // iterate for each path to compute child and parent max
    typedef itk::ImageLinearIteratorWithIndex<ForestType> ForestIterator;
    ForestIterator fiter( forest, forest->GetBufferedRegion() );
    fiter.SetDirection( 0 );
    fiter.GoToBegin();    
    
    while (!fiter.IsAtEnd())
      {
      // new path
      //std::cout << fiter.GetIndex() << ": ";
      //std::cout << fiter.Get() << " ";
      //std::cout << forestIndexBuffer[ fiter.Get() - 1 ] << std::endl;
      
      PixelType child_max_value = 0;
      PixelType parent_max_value = 0;
      unsigned long rootPtr = (fiter.Get() - 1);
      
      unsigned long child_max_index = 0;
      unsigned long parent_max_index = 0;
      
      while (!fiter.IsAtEndOfLine())
        {
        
        if ( fiter.Get() )
          {          
          PixelType input_value = inputBuffer[ fiter.Get() - 1 ];
          
          if ( input_value > child_max_value )
            {
            child_max_value = input_value;
            child_max_index = fiter.Get();
            }
            
          if ( input_value > parent_max_value )
            {
            parent_max_value = input_value;
            parent_max_index = fiter.Get();
            }
            
          if ( parent_max_value > parentMaxBuffer[ fiter.Get() - 1] )
            {
            parentMaxBuffer[fiter.Get() - 1] = parent_max_value;
            parentMaxIndexBuffer[fiter.Get() - 1] = parent_max_index;
            }
            
          if ( parent_max_value > outputBuffer[ fiter.Get() - 1 ] )
            {
            outputBuffer[fiter.Get() - 1] = parent_max_value;
            maxIndexBuffer[fiter.Get() - 1] = parent_max_index;
            }
            
          }
          
        ++fiter;
        
        }
        
      childMaxBuffer[ rootPtr ] = child_max_value;
      childMaxIndexBuffer[ rootPtr ] = child_max_index;
      
      if ( child_max_value > outputBuffer[ rootPtr ] )
        {
        outputBuffer[rootPtr] = child_max_value;
        maxIndexBuffer[rootPtr] = child_max_index;
        }
      
      
      fiter.NextLine();
      count++;
      
      }

   }
   
  std::cout << "#path processed: " << count << std::endl;
  
  // write out files
  itk::idp::WriteImage<ImageType>( outputFile.c_str(), output, true );
  
  if ( !childMaxFile.empty() )
    {
    itk::idp::WriteImage<ImageType>( childMaxFile.c_str(), childMax, true );
    }
    
  if ( !parentMaxFile.empty() )
    {
    itk::idp::WriteImage<ImageType>( parentMaxFile.c_str(), parentMax, true );
    }
  
  if( !maxIndexFile.empty() )
    {
    itk::idp::WriteImage<ForestIndexType>( maxIndexFile.c_str(), maxIndex, true );
    } 
    
 return EXIT_SUCCESS;
 
 }
  
int main( int argc, char *argv[] )
{
  if (argc < 5 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputFile forestDir forestIndexFile outputFile [format] [maxIndexFile] [parentMaxFile] [childMaxFile]" << std::endl;
    return EXIT_FAILURE;
    }
    
    std::string inputFile = argv[1];
    std::string forestDirectory = argv[2];
    std::string forestIndexFile = argv[3];
    std::string outputFile = argv[4];
    std::string format = "float";
    std::string maxIndexFile = "";
    std::string parentMaxFile = "";
    std::string childMaxFile = "";
    
    int ind = 5;
    if (argc > ind)
      {
      format = argv[ind];
      }
      
    ind++;  
    if ( argc > ind )
      {
      maxIndexFile = argv[ind];
      }
    
    ind++;
    if ( argc > ind )
      {
      parentMaxFile = argv[ind];
      }
      
    ind++;
    if ( argc > ind )
      {
      childMaxFile = argv[ind];
      }

    typedef itk::Image< unsigned char, 3 >      UCHARVolumeType;
    typedef itk::Image< unsigned short, 3 >     USHORTVolumeType;
    typedef itk::Image< unsigned int,  3 >      UINTVolumeType;
    typedef itk::Image< float, 3>               FLOATVolumeType;
 
    
  try
    {
    int err;
    
   if( itksys::SystemTools::Strucmp( format.c_str(), "uchar" ) == 0 )
    {
    err = Pipeline<UCHARVolumeType>( inputFile, forestDirectory, forestIndexFile,
       outputFile, maxIndexFile, parentMaxFile, childMaxFile  );
    if ( err ) return EXIT_FAILURE;
    }
   else if( itksys::SystemTools::Strucmp( format.c_str(), "ushort" ) == 0 )
    {
    err = Pipeline<USHORTVolumeType>( inputFile, forestDirectory, forestIndexFile,
       outputFile, maxIndexFile, parentMaxFile, childMaxFile  );
    if ( err ) return EXIT_FAILURE;
    }  
   else if( itksys::SystemTools::Strucmp( format.c_str(), "uint" ) == 0)
    {
    err = Pipeline<UINTVolumeType>( inputFile, forestDirectory, forestIndexFile,
       outputFile, maxIndexFile, parentMaxFile, childMaxFile  );
    if ( err ) return EXIT_FAILURE;
    }
   else if ( itksys::SystemTools::Strucmp( format.c_str(), "float" ) == 0 )
    {
    err = Pipeline<FLOATVolumeType>( inputFile, forestDirectory, forestIndexFile,
       outputFile, maxIndexFile, parentMaxFile, childMaxFile  );
    if ( err ) return EXIT_FAILURE;
    }      
  else 
    {
    std::cerr << "format: " << format << " not supported" << std::endl;
    return EXIT_FAILURE;
    }

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
