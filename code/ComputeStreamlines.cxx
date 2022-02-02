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
#include "itkVectorLinearInterpolateImageFunction.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "itkPolyLineParametricPath.h"
#include "itkPathIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "idpUtilities.h"

  typedef unsigned int PixelType;
  typedef itk::Image< PixelType, 3 >          ImageType;
  typedef itk::Vector< float>                 VectorType;
  typedef itk::Image< VectorType, 3 >         VectorImageType;
  typedef itk::Image< unsigned int, 3>       MaskImageType;

  typedef itk::ImageFileReader<ImageType>     ReaderType;
  typedef itk::ImageFileReader<VectorImageType> VectorReaderType;
  typedef itk::ImageFileReader<MaskImageType> MaskReaderType;
  typedef itk::ImageFileWriter<ImageType>     WriterType;

  typedef itk::Point<double, 3>               PointType;
  typedef itk::Index<3>                       IndexType;
  typedef itk::ContinuousIndex<double,3>      ContinuousIndexType;
  typedef itk::VectorLinearInterpolateImageFunction<VectorImageType, double> VectorInterpolateType;
  typedef itk::NearestNeighborInterpolateImageFunction<ImageType, double> InterpolateType;
  typedef VectorInterpolateType::OutputType OutputType;
  
  typedef itk::PolyLineParametricPath<3> PathType;
  typedef PathType::VertexType          VertexType;
  typedef itk::PathIterator< ImageType, PathType > PathIteratorType;
  
//#define LOG  
  
 bool render( 
 ImageType::Pointer & labels,
 PathType::Pointer & path,
 PixelType label,
 bool testOnly = false )
 { 
 
 try
   {
       PathIteratorType piter( labels, path );
       piter.GoToBegin();

      while( !piter.IsAtEnd() && label )
        {
        if ( !testOnly )
          {
          piter.Set( label );
          }
#ifdef LOG        
        std::cout << piter.GetIndex() << std::endl;
#endif          
        ++piter;
        }
        
      return true;  
   }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << err << std::endl;
    return false;
    }
  catch( ... )
    {
    std::cerr << "Caught unknown exception." << std::endl;
    return false;
    }   
   
}

void trace(
ContinuousIndexType & seed,
VectorImageType::Pointer & field,
PathType::Pointer & path,
VectorInterpolateType::Pointer & finterpolator,
InterpolateType::Pointer & linterpolator,
PixelType & label )
{

      PointType point;
      ContinuousIndexType index = seed;
      
      field->TransformContinuousIndexToPhysicalPoint( index, point );
      
      path = PathType::New();
      path->AddVertex( index );
      
      label = 0;
      
       for ( int k = 0; k < 100; k++ )
       {
        bool isInside = finterpolator->IsInsideBuffer( point );

        if (isInside)
          {

          OutputType v = finterpolator->Evaluate( point );
          PixelType lvalue = linterpolator->Evaluate( point );
#ifdef LOG         
          std::cout << index << ": " << v << ": " << lvalue << std::endl;
#endif

          if ( lvalue )
            {
            label = lvalue;
            break;   // found a matching label
            }       
            
          for ( int j = 0; j < 3; j++ )
            {
            point[j] -=  25.0 * v[j];
            field->TransformPhysicalPointToContinuousIndex( point, index );
            }

          path->AddVertex( index );

          }
        else
          {
          // error: have slide out of the image buffer
          std::cout << index << ": outside buffer" << std::endl;
          }
        } // for each k

}


int main( int argc, char *argv[] )
{
  if (argc < 3 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " gradientFile seedFile labelFile outputFile " << std::endl;
    return EXIT_FAILURE;
    }

  VectorReaderType::Pointer vreader = VectorReaderType::New();
  vreader->SetFileName( argv[1] );

  MaskReaderType::Pointer mreader = MaskReaderType::New();
  mreader->SetFileName( argv[2] );

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[3] );

  VectorImageType::Pointer field;
  ImageType::Pointer       labels;
  MaskImageType::Pointer   seeds;
  ImageType::Pointer       output = ImageType::New();

  try
    {
    vreader->Update();
    reader->Update();
    mreader->Update();

    field = vreader->GetOutput();
    field->DisconnectPipeline();

    labels = reader->GetOutput();
    labels->DisconnectPipeline();

    seeds = mreader->GetOutput();
    seeds->DisconnectPipeline();
    
    itk::idp::MakeImage<ImageType,ImageType>( labels, output, 0 );

    vreader = 0;
    reader = 0;
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

  // Access gradient field at arbitary positions using inear interpolation
  VectorInterpolateType::Pointer vinterpolator = VectorInterpolateType::New();
  vinterpolator->SetInputImage( field );

  // Access label image at arbitary positions using nearest neighbor interpolation
  InterpolateType::Pointer interpolator = InterpolateType::New();
  interpolator->SetInputImage( labels );

  // Iterate through one top surface seed at a time
  typedef itk::ImageRegionConstIteratorWithIndex< MaskImageType > MaskIterator;
  MaskIterator miter( seeds, seeds->GetBufferedRegion() );

  miter.GoToBegin();

  unsigned long update = 0;

  try
    {

    while( !miter.IsAtEnd() )
    {
     if ( miter.Get() )  // compute streamline for each seed
      {
 
      ContinuousIndexType seedIndex = miter.GetIndex();
      std::cout << "seed: " << seedIndex << std::endl; 
      
      /*
       ContinuousIndexType myIndex;
       myIndex[0] =  153.0;
       myIndex[1] = 259.0;
       myIndex[2] = 296.0;
       if ( seedIndex != myIndex )
         {
         ++miter;
         continue;
         }         
       std::cout << "here" << seedIndex << std::endl;  
      */       
       
      
      unsigned int label = 0;
      PathType::Pointer path;
      
      trace( seedIndex, field, path, vinterpolator, interpolator, label );     
            
      if ( !label )
        {
        // error: still can't find a label
        std::cout << "seedIndex: " << seedIndex << " not matched" << std::endl;
        ++miter; continue;
        }
        
       
      if ( render( output, path, label, true ) )
        {
        // the line is renderable - go do it.
        render( output, path, label, false );
        }  
      
      //output->SetPixel( miter.GetIndex(), label );
        

      } // [if miter.Get()]

    ++miter;
    

    } // [while !miter.IsAtEnd()]

    // Write out the last batch before exiting
   WriterType::Pointer writer = WriterType::New();
   writer->SetInput( output );
   writer->SetFileName( argv[4] );
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


