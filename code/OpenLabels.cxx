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
#include "itkLabelImageToLabelMapFilter.h"
#include "itkBinaryErodeImageFilter.h"
#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryBallStructuringElement.h"


template <typename ImageType > 
int Pipeline( 
 const char * inputFile,
 unsigned int radius,
 const char * outputFile )
 {
 
  typedef typename ImageType::PixelType PixelType;
  
   // declare image objects
  typename ImageType::Pointer input;
  typename ImageType::Pointer output;
  
  // read files
  itk::idp::ReadImage<ImageType>( inputFile, input );
  std::cout << inputFile << " ok" << std::endl;
  
  // make output image
  itk::idp::MakeImage<ImageType,ImageType>( input, output, 0 );

  typedef itk::LabelImageToLabelMapFilter<ImageType> InputConverterType;
  typedef typename InputConverterType::OutputImageType LabelMapType;
  typedef typename LabelMapType::LabelObjectType LabelObjectType;

  typename InputConverterType::Pointer inputConverter = InputConverterType::New();
  inputConverter->SetInput( input );
  inputConverter->Update();
  
  typename LabelMapType::Pointer labelMap = inputConverter->GetOutput();
  
  std::cout << "There are " << labelMap->GetNumberOfLabelObjects() << " objects." << std::endl;
  
  typename LabelMapType::LabelObjectContainerType::const_iterator it;
  const typename LabelMapType::LabelObjectContainerType & labelObjectContainer = labelMap->GetLabelObjectContainer();
  
  std::cout << "radius: " << radius << std::endl;
  
  typedef itk::BinaryBallStructuringElement<PixelType, ImageType::ImageDimension> 
    StructuringElementType;
  StructuringElementType structuringElement;
  structuringElement.SetRadius( radius );
  structuringElement.CreateStructuringElement();  
  
  for( it = labelObjectContainer.begin(); it != labelObjectContainer.end(); it++ )
    {
    const PixelType & label = it->first;
    std::cout << "processing label: " << label << std::endl;
    
    typename ImageType::Pointer binary;
    
    // create binary image
    itk::idp::BinaryThreshold<ImageType,ImageType>( input, label, label, label, 0, binary );
    
    // morphological open
    typedef itk::BinaryDilateImageFilter<ImageType,ImageType,StructuringElementType> DilatorType;
    
    typedef itk::BinaryErodeImageFilter<ImageType,ImageType,StructuringElementType> EroderType;
    
    typename DilatorType::Pointer dilator = DilatorType::New();
    typename EroderType::Pointer eroder = EroderType::New();
    
    // then open to fill in holes
    dilator->SetInput( binary );
    dilator->SetKernel( structuringElement );
    dilator->SetForegroundValue( label );
    dilator->SetBackgroundValue( 0 );
    eroder->SetInput( dilator->GetOutput() );
    eroder->SetKernel( structuringElement );
    eroder->SetForegroundValue( label );
    eroder->SetBackgroundValue( 0 );   
    eroder->Update();
    binary = eroder->GetOutput();
    binary->DisconnectPipeline();
    
    // overwrite image output
    itk::idp::OverwriteImage<ImageType>( output, binary, output );
    
    }

  

  // write out files
  itk::idp::WriteImage<ImageType>( outputFile, output, true );
  
  return EXIT_SUCCESS;
 
 }

  
int main( int argc, char *argv[] )
{
  if (argc < 3 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputFile outputFile [radius] [format]" << std::endl;
    return EXIT_FAILURE;
    }
    
    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    
    unsigned int radius = 1;
    
    if (argc > 3 )
      {
      radius = atoi(argv[3]);
      }

    std::string format = "ushort";
    
    if ( argc > 4 )
      {
      format = argv[4];
      }
      
    typedef itk::Image< unsigned char, 3 >      UCHARVolumeType;
    typedef itk::Image< unsigned short, 3 >     USHORTVolumeType;
    typedef itk::Image< unsigned int,  3 >      UINTVolumeType;
    
    std::cout << format << std::endl;
        
  try
    {
    int err;
    
   if( itksys::SystemTools::Strucmp( format.c_str(), "uchar" ) == 0 )
    {
    err = Pipeline<UCHARVolumeType>( inputFile.c_str(), radius, outputFile.c_str() );
    if ( err ) return EXIT_FAILURE;
    }
   else if( itksys::SystemTools::Strucmp( format.c_str(), "ushort" ) == 0 )
    {
    err = Pipeline<USHORTVolumeType>( inputFile.c_str(), radius, outputFile.c_str() );
    if ( err ) return EXIT_FAILURE;
    }  
   else if( itksys::SystemTools::Strucmp( format.c_str(), "uint" ) == 0 )
    {
    err = Pipeline<UINTVolumeType>( inputFile.c_str(), radius, outputFile.c_str() );
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
