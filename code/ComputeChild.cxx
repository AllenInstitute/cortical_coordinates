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
#include "idpUtilities.h"

  typedef unsigned int PixelType;
  typedef itk::Image< PixelType, 3 >          ImageType;
  typedef itk::Vector< float>                 VectorType;
  typedef itk::Image< VectorType, 3 >         VectorImageType;

  typedef itk::ImageFileReader<ImageType>     ReaderType;
  typedef itk::ImageFileReader<VectorImageType> VectorReaderType;
  typedef itk::ImageFileWriter<ImageType>     WriterType;

  typedef itk::Index<3>                       IndexType;
  typedef itk::ContinuousIndex<double,3>      ContinuousIndexType;
  typedef itk::Point<double,3>            PointType;
    
int main( int argc, char *argv[] )
{
  if (argc < 4 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " gradientFile maskFile outputFile " << std::endl;
    return EXIT_FAILURE;
    }

  VectorReaderType::Pointer vreader = VectorReaderType::New();
  vreader->SetFileName( argv[1] );

  ReaderType::Pointer mreader = ReaderType::New();
  mreader->SetFileName( argv[2] );

  VectorImageType::Pointer field;
  ImageType::Pointer       mask;
  ImageType::Pointer       output = ImageType::New();
  
  VectorImageType::Pointer child_index = VectorImageType::New();
  VectorType zero_vec;
  zero_vec.Fill( 0.0 );

  try
    {
    vreader->Update();
    mreader->Update();

    field = vreader->GetOutput();
    field->DisconnectPipeline();

    mask = mreader->GetOutput();
    mask->DisconnectPipeline();
    
    itk::idp::MakeImage<ImageType,ImageType>( mask, output, 0 );
    itk::idp::MakeImage<VectorImageType,VectorImageType>( field, child_index, zero_vec );

    vreader = 0;
    mreader = 0;
    
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

  try
    {

    // setup iterators for the mask, gradient and output
    typedef itk::ImageRegionIteratorWithIndex<ImageType> Iterator;
    typedef itk::ImageRegionIteratorWithIndex<VectorImageType> VectorIterator;  
    typedef itk::VectorLinearInterpolateImageFunction<VectorImageType, double> VectorInterpolateType;
    
    ImageType::RegionType region = mask->GetBufferedRegion();
    
/*
    region.SetIndex( 0, 241 );
    region.SetIndex( 1, 681 );
    region.SetIndex( 2, 626 );
    region.SetSize( 0, 1 );
    region.SetSize( 1, 1 );
    region.SetSize( 2, 1 ); 
*/    
    
    VectorIterator fiter( field, region );
    Iterator miter( mask, region );
    Iterator oiter( output, region );
    VectorIterator iiter( child_index, region );
    
    unsigned long count = 0;
    
    while (!miter.IsAtEnd())
      {

       // for each pixel define child as (index - gradient)
      if ( miter.Get() )
         {
         
         ContinuousIndexType cindex = miter.GetIndex();
         PointType point;
         field->TransformContinuousIndexToPhysicalPoint( cindex, point );        
         VectorType v = fiter.Get();
         
         /*
         std::cout << cindex << std::endl;
         std::cout << point << std::endl;
         std::cout << v << std::endl;
         */
         
         v *= field->GetSpacing()[0];
         point -= v;         
         IndexType index;
         field->TransformPhysicalPointToIndex( point, index );         
         
         /*         
         std::cout << cindex << std::endl;
         std::cout << index << std::endl;
         std::cout << point << std::endl;  
         std::cout << v << std::endl;
         */

         if ( region.IsInside( index ) && mask->GetPixel( index ) )
            {
            oiter.Set( mask->ComputeOffset( index ) + 1 );
            VectorType v;
            for( int i = 0; i < 3; i++ )
              {
              v[i] = index[i];
              }
            iiter.Set( v );
            }
            
         count++;  

         // heartbeat
         if ( count % 1000000 == 0 )
          {
          std::cout << "count: " << count << std::endl;
          }        
        
         }         
       
      ++miter;
      ++fiter;
      ++oiter;
      ++iiter;
      
      }
            

    // Write out the last batch before exiting
   WriterType::Pointer writer = WriterType::New();
   writer->SetInput( output );
   writer->SetFileName( argv[3] );
   writer->Update();
   
   itk::idp::WriteImage<VectorImageType>( "child_index.nrrd", child_index, true );

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


