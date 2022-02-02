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
#include "itkImageSliceIteratorWithIndex.h"
#include "itkVectorLinearInterpolateImageFunction.h"
#include "idpUtilities.h"
#include <stdio.h>
#include <itksys/SystemTools.hxx>

  typedef unsigned int PixelType;
  typedef itk::Image< PixelType, 3 >          ImageType;
  typedef itk::ImageFileReader<ImageType>     ReaderType;
  typedef ImageType::IndexType                IndexType;
  typedef itk::ContinuousIndex<double,3>      ContinuousIndexType;
  typedef itk::Point<double, 3>               PointType;  
  
  typedef itk::Vector< float>                 GradientType;
  typedef itk::Image< GradientType, 3 >       GradientImageType;
   
  typedef itk::Image< PixelType, 2>  MapType;
  typedef itk::ImageLinearIteratorWithIndex<MapType> MapIterator;


int main( int argc, char *argv[] )
{
  if (argc < 5 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << "gradientFile maskFile outputFile indexFile " << std::endl;
    return EXIT_FAILURE;
    }
    
  std::string gradientFile = argv[1];
  std::string maskFile = argv[2];
  std::string outputFile = argv[3];
  std::string indexFile = argv[4];

  GradientImageType::Pointer field;
  ImageType::Pointer       mask;
  ImageType::Pointer       index = ImageType::New();

  try
    {
    itk::idp::ReadImage<ImageType>( maskFile.c_str(), mask );
    itk::idp::ReadImage<GradientImageType>( gradientFile.c_str(), field );    
    itk::idp::MakeImage<ImageType,ImageType>( mask, index, 0 );

    // create output directory
    itksys::SystemTools::MakeDirectory( argv[3] );  

    std::cout << "initialized" << std::endl;
    
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

    // set up map data structure
    typedef std::vector<PixelType> VectorType;
    typedef std::vector<VectorType> ForestType;
    
    // setup mask and index iterators
    typedef itk::ImageSliceIteratorWithIndex<ImageType> Iterator;
    ImageType::RegionType region = mask->GetBufferedRegion();
    
/*    
    region.SetIndex( 0, 0 );
    region.SetIndex( 1, 692 );
    region.SetIndex( 2, 0 );
    region.SetSize( 0, 619 );
    region.SetSize( 1, 300 );
    region.SetSize( 2, 799 );   
    region.Crop( mask->GetBufferedRegion() );
*/    

/* 
    region.SetIndex( 0, 178 );
    region.SetIndex( 1, 695 );
    region.SetIndex( 2, 625 );
    region.SetSize( 0, 1 );
    region.SetSize( 1, 1 );
    region.SetSize( 2, 1 );   
    region.Crop( mask->GetBufferedRegion() );   
*/    
    
    Iterator miter( mask, region );
    Iterator iiter( index, region );
    
    miter.SetFirstDirection( 0 );
    miter.SetSecondDirection( 1 );
    miter.GoToBegin();
    
    iiter.SetFirstDirection( 0 );
    iiter.SetSecondDirection( 1 );
    iiter.GoToBegin();
    
    // setup gradient field interpolator
    typedef itk::VectorLinearInterpolateImageFunction<GradientImageType, double> GradientInterpolator;
    GradientInterpolator::Pointer ginterp = GradientInterpolator::New();
    ginterp->SetInputImage( field );
    
    double spacing = mask->GetSpacing()[0];
    
    while ( !miter.IsAtEnd() )
      {
      
      // start a new matrix for each new slice
      ForestType forest;
      unsigned int count = 0;
      unsigned int max_length = 0; 
      ImageType::IndexType index = miter.GetIndex();      
      
      while (!miter.IsAtEndOfSlice() )
        {
        while(!miter.IsAtEndOfLine() )
          {
          if ( miter.Get() )
            {
            
            // update index map to current forest pointer
            count += 1;
            iiter.Set( count );
            
            // start new path in a vector
            VectorType my_vector;
            ContinuousIndexType my_child_index = miter.GetIndex();
            PixelType my_child = mask->ComputeOffset( miter.GetIndex() ) + 1;
            
            PixelType last_child = 0;
            unsigned int sameChildCount = 0;
           
            // collect vertices into the vector until you reach the end of the path (no more children)
            while ( my_child )
                {
                
               // std::cout << my_child_index << " to ";
                
                my_vector.push_back( my_child );
                
                // step on unit in along gradient
                PointType point;
                mask->TransformContinuousIndexToPhysicalPoint( my_child_index, point );
                
                GradientType g = ginterp->Evaluate( point );
                g *= spacing;
                point -= g;
                mask->TransformPhysicalPointToContinuousIndex( point, my_child_index );
                
                IndexType index;
                mask->TransformPhysicalPointToIndex( point, index );
                
                last_child = my_child;
                my_child = mask->ComputeOffset( index ) + 1;
                
                if ( my_child == last_child )
                  {
                  sameChildCount++;
                  }
                else
                  {
                  sameChildCount = 0;
                  }
                
                // terminate if we are the same child for 10 iterations
                if ( sameChildCount > 10 )
                  {
                  my_child = 0;
                  }
                
                //std::cout << my_child_index << std::endl;
                
                // terminate if child is not in mask
                if ( !mask->GetPixel(index) ) 
                  {
                  my_child = 0;
                  }
                
                // terminate if path gets too long
                if ( my_vector.size() > 200 )
                  {
                  my_child = 0;
                  }
                  
                }
                
            // add my path to the forest    
            forest.push_back( my_vector );
            
            if ( 0 )
                {
                // print out vector
                std::cout << count << ": ";
                for ( VectorType::const_iterator i = my_vector.begin(); 
                        i != my_vector.end(); ++i)
                    {
                    std::cout << mask->ComputeIndex(*i - 1) << " -> ";
                    }
                std::cout << std::endl;
                }

            
            if ( my_vector.size() > max_length )
                {
                max_length = my_vector.size();
                }        
            
            }
        
          ++miter;
          ++iiter;
          }
          miter.NextLine();
          iiter.NextLine();
        }
        
        if (count)
          {
          
          std::cout << index << " count: " << count << " max_length: " << max_length << std::endl;
          
          // write the forest into a 2D matrix structure
          MapType::Pointer matrix = MapType::New();
          
          MapType::RegionType matrix_region;
          matrix_region.SetSize( 0, max_length );
          matrix_region.SetSize( 1, count );
          
          matrix->SetRegions( matrix_region );
          matrix->Allocate();
          matrix->FillBuffer( 0 );
          
          MapIterator xiter( matrix, matrix_region );
          xiter.SetDirection( 0 );
          xiter.GoToBegin();
          
          ForestType::iterator fiter = forest.begin();
          
          while (!xiter.IsAtEnd())
            {
            VectorType::iterator viter = (*fiter).begin();
            while (!xiter.IsAtEndOfLine() && viter != (*fiter).end())
              {
              xiter.Set( *viter );
              ++xiter;
              viter++;
              }
            xiter.NextLine();
            fiter++;
            }        
          
          // write out the 2D matrix
          char buffer[10];
          sprintf( buffer, "%04ld", index[2] );
          
          std::string filename = argv[3];
          filename += "/";
          filename += buffer;
          filename += ".mha";
         
          itk::idp::WriteImage<MapType>( filename.c_str(), matrix, true );        

          }
        
        miter.NextSlice();
        iiter.NextSlice();
        
      }    
    
    // Write out index map
   itk::idp::WriteImage<ImageType>( argv[4], index, true );
   

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


