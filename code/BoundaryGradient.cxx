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
#include "itkDerivativeOperator.h"
#include "itkConstNeighborhoodIterator.h"

int main( int argc, char *argv[] )
{

  if (argc < 6 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputFile boundaryFile maskFile gradientFile outputFile " << std::endl;
    return EXIT_FAILURE;
    }

  std::string inputFile = argv[1];
  std::string boundaryFile = argv[2];
  std::string maskFile = argv[3];
  std::string gradientFile = argv[4];
  std::string outputFile = argv[5];

  typedef float PixelType;
  typedef itk::Image< PixelType, 3 >          ImageType;
  typedef itk::GradientImageFilter< ImageType,float, float> FilterType;
  typedef FilterType::OutputImageType GradientImageType;
  typedef GradientImageType::PixelType GradientType;
  
  typedef itk::ImageFileReader<ImageType>     ReaderType;
  typedef itk::ImageFileReader<GradientImageType> GradientReaderType;
  typedef itk::ImageFileWriter<GradientImageType>     WriterType;
  
  try
    {
    
    // read in images
    ImageType::Pointer input;
    ImageType::Pointer boundary;
    ImageType::Pointer mask;
    GradientImageType::Pointer gradient;
    
    std::cout << "reading input file" << std::endl;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( inputFile.c_str() );
    reader->Update();
    
    input = reader->GetOutput();
    input->DisconnectPipeline();
    
    std::cout << "reading boundary file" << std::endl;
    reader = ReaderType::New();
    reader->SetFileName( boundaryFile.c_str() );
    reader->Update();
    
    boundary = reader->GetOutput();
    boundary->DisconnectPipeline();
    
    std::cout << "reading mask file" << std::endl;
    reader = ReaderType::New();
    reader->SetFileName( maskFile.c_str() );
    reader->Update();
    
    mask = reader->GetOutput();
    mask->DisconnectPipeline(); 

    std::cout << "reading gradient file" << std::endl;
    GradientReaderType::Pointer greader = GradientReaderType::New();
    greader->SetFileName( gradientFile.c_str() );
    greader->Update();

    gradient = greader->GetOutput();
    gradient->DisconnectPipeline();     
    
    // set up iterators
    typedef itk::ImageRegionIterator<ImageType> Iterator;
    typedef itk::ImageRegionIterator<GradientImageType> GIterator;
    typedef itk::NeighborhoodIterator<ImageType> NIterator;
    
    ImageType::RegionType region;
    region = input->GetBufferedRegion();
    
    /*
    region.SetSize(0,1);
    region.SetSize(1,1);
    region.SetSize(2,1);
    region.SetIndex(0,244);
    region.SetIndex(1,673);
    region.SetIndex(2,629); 
    */    
    
    itk::Size<3> radius;
    radius.Fill( 3 );
    
    NIterator miter( radius, mask, region );
    NIterator iiter( radius, input, region );
    
    Iterator biter( boundary, region );
    GIterator giter( gradient, region );
    
    GradientImageType::PixelType zeroVector;
    zeroVector.Fill( 0.0 );
    
    int c = (iiter.Size() / 2);
   
    unsigned long count = 0;
    
    while (!iiter.IsAtEnd())
      {
      
      if ( biter.Get() && miter.GetCenterPixel() )  // boundary voxel
        {
        /*
          std::cout << iiter.GetIndex() << std::endl;
          std::cout << iiter.GetCenterPixel() << std::endl;
          std::cout << iiter.GetPixel(c) << std::endl;
          std::cout << biter.Get() << std::endl;
          std::cout << miter.GetCenterPixel() << std::endl;
          std::cout << giter.Get() << std::endl;
         */ 
         
          
          GradientImageType::PixelType gvalue;
          
          for(unsigned int i = 0; i < 3; i++)
          {
          int s = iiter.GetStride(i);
          if ( miter.GetPixel(c-s) && miter.GetPixel(c+s) )
            {
            gvalue[i] = 0.5 * (iiter.GetPixel(c+s) - iiter.GetPixel(c-s)) / input->GetSpacing()[i];
            }
          else if ( miter.GetPixel(c-s) && ! miter.GetPixel(c+s) )
           {
           gvalue[i] = ( iiter.GetPixel(c) - iiter.GetPixel(c-s) ) / input->GetSpacing()[i];
           }
          else if ( !miter.GetPixel(c-s) && miter.GetPixel(c+s) )
           {
           gvalue[i] = ( iiter.GetPixel(c-s) - iiter.GetPixel(c) ) / input->GetSpacing()[i];
           }       
       //   std::cout << iiter.GetIndex(c-s) << ": " << miter.GetPixel(c-s) << ", " << iiter.GetPixel(c-s) << std::endl;
       //   std::cout << iiter.GetIndex(c) << ": " << miter.GetPixel(c) << ", " << iiter.GetPixel(c) << std::endl;     
      //    std::cout << iiter.GetIndex(c+s) << ": " << miter.GetPixel(c+s) << ", " << iiter.GetPixel(c+s) << std::endl;
          }
          
      //    std::cout << gvalue << std::endl;
          
          GradientImageType::PixelType tvalue;
          input->TransformLocalVectorToPhysicalVector( gvalue, tvalue );
          
      //    std::cout << tvalue << std::endl;
          
          float m = tvalue.GetNorm();
          tvalue /= m;
          
       //  std::cout << tvalue << std::endl;
          
          giter.Set( tvalue );
          count++;
          
          if (count % 1000 == 0)
            {
            std::cout << count << std::endl;
            }
        
        }
      else if ( miter.GetCenterPixel() == 0 )
        {
        giter.Set( zeroVector );
        }
              
      ++iiter;
      ++biter;
      ++miter;
      ++giter;
      }
      
    // write output file
    WriterType::Pointer writer = WriterType::New();
    writer->SetInput( gradient );
    writer->SetFileName( outputFile.c_str() );
    writer->UseCompressionOn(); 
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


