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
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkMaskNeighborhoodOperatorImageFilter.h"
#include "itkBinaryCrossStructuringElement.h"
#include "itkLaplacianUpdateImageFilter.h"
#include "idpUtilities.h"
#include "itkCovariantVector.h"
#include "itkNeighborhoodIterator.h"


typedef unsigned int PixelType;
typedef itk::Image< PixelType, 3 >          ImageType;
typedef itk::Image< float, 3>               FloatType;
typedef itk::ImageRegionConstIterator<ImageType> InputIterator;
typedef itk::ImageRegionIterator<FloatType> FloatIterator;
typedef itk::NeighborhoodIterator<ImageType> NeighborhoodIterator;
typedef itk::NeighborhoodIterator<FloatType> FloatNeighborhoodIterator;
typedef itk::CovariantVector<float,3> GradientPixelType;
typedef itk::Image< GradientPixelType, 3> GradientType;
typedef itk::ImageRegionIterator<GradientType> GradientIterator;


void CreateFixedBoundaryImage( 
 FloatType::Pointer & image,
 const ImageType * seeds )
{

   image->SetRegions( seeds->GetBufferedRegion() );
   image->SetSpacing( seeds->GetSpacing() );
   image->SetOrigin( seeds->GetOrigin() );
   image->SetDirection( seeds->GetDirection() );
   image->Allocate();
   image->FillBuffer( 0.0 );
   InputIterator siter( seeds, seeds->GetBufferedRegion() );
   FloatIterator iter( image, image->GetBufferedRegion() );

   for( iter.GoToBegin(); !iter.IsAtEnd(); ++iter, ++siter )
      {
      if( siter.Get() == 1 )
        {
        iter.Set( 5000.0 ); 
        }
      else if( siter.Get() == 3 )
        {
        iter.Set( -5000.0 );
        }
      }

}

void ComputeNeumanBounaryUpdate( 
FloatType::Pointer & laplacian, 
ImageType::Pointer & mask,
ImageType::Pointer & fixedBoundary,
FloatType::Pointer &neumannBoundary, 
GradientType::Pointer & boundaryNormal, 
FloatType::Pointer & neumannUpdate )
{
  if ( boundaryNormal.IsNotNull() )
    {
    // set up iterators
    FloatType::RegionType region = laplacian->GetBufferedRegion();
    FloatType::SizeType radius;
    radius.Fill( 1 );
    
    FloatNeighborhoodIterator laplacianIter( radius, laplacian, region ); // current laplacian solution
    NeighborhoodIterator maskIter( radius, mask, region ); // mask region
    NeighborhoodIterator fixedBoundaryIter( radius, fixedBoundary, region ); // fixed boundary map

    
    FloatIterator neumannBoundaryIter( neumannBoundary, region ); // mask of the neumannBoundary voxels
    GradientIterator boundaryNormalIter( boundaryNormal, region ); // boundary normal
    FloatIterator neumannUpdateIter( neumannUpdate, region ); // updated solution at boundary voxels
    
    // set up neighborhood offsets
    std::vector< std::vector<NeighborhoodIterator::OffsetType> > offsets;
    offsets.resize( 3 );
    for ( int i = 0; i < 3; i++ )
      {
      offsets[i].resize(2);
      NeighborhoodIterator::OffsetType t;
      t.Fill(0);
      t[i] = -1;
      offsets[i][0] = t;
      
      t.Fill(0);
      t[i] = 1;
      offsets[i][1] = t;
      }
      
    unsigned int voxelCount = 0;
    unsigned int validCount = 0;
    
    while( !laplacianIter.IsAtEnd() )
      {
      

      if ( neumannBoundaryIter.Get() ) // boundary voxel
        { 
        voxelCount++;
        
        GradientType::PixelType normal = boundaryNormalIter.Get();
        
        // count how many inside voxel are in the stencil         
        unsigned int stencilCount = 0;
        float values[3];
        
        for ( int i = 0; i < 3; i++ )
          {
          
          // check if voxel in negative normal direction is available
          bool found = false;
          
          int direction = 1;
          if ( normal[i] > 0.0 ) {
             direction = 0;
          }
          
          if ( maskIter.GetPixel( offsets[i][direction] ) || fixedBoundaryIter.GetPixel( offsets[i][direction] ) )
            {
            found = true;
            values[i] = laplacianIter.GetPixel( offsets[i][direction] );
            stencilCount++;
            }
          
          // otherwise check the voxel on the other side
          if ( !found )
            {
            if (direction == 1) 
              { 
              direction = 0; 
              }
            else
              {
              direction = 1;
              }
            }
            
          if ( !found && ( maskIter.GetPixel( offsets[i][direction] ) || fixedBoundaryIter.GetPixel( offsets[i][direction] ) ) )
            {
            found = true;
            values[i] = laplacianIter.GetPixel( offsets[i][direction] );
            stencilCount++;
            }

          // else abandon voxel
          if ( !found )
            {
            break;
            }            
                             
          } // for each dimension
          
          if ( stencilCount == 3 )
            {
            validCount++;
            double numerator = 0.0;
            double denominator = 0.0;
            
            for ( int i = 0; i < 3; i++ )
              {
              numerator += values[i] * ( 1.0/3.0 + fabs( normal[i] ) );
              denominator += ( 1.0/3.0 + fabs( normal[i] ) );
              }
              
            neumannUpdateIter.Set( numerator / denominator );  
            
            }
        
 /*      
        std::cout << maskIter.GetIndex() << ":" << neumannUpdateIter.Get() << " " <<  normal << " " << stencilCount << " "; 
        for ( int i = 0; i < 3; i++ )
          {
          for ( int j = 0; j < 2; j++ )
            {
            std::cout << static_cast<unsigned int>(maskIter.GetPixel( offsets[i][j] ) || fixedBoundaryIter.GetPixel( offsets[i][j] )) << ",";
            }
          }
        std::cout << std::endl;
*/        
       

        
        } // boundary voxel
      
      ++laplacianIter;
      ++maskIter;
      ++fixedBoundaryIter;
      ++neumannBoundaryIter;
      ++boundaryNormalIter;
      ++neumannUpdateIter;
      }
      
//    std::cout << "voxelCount" << ":" << voxelCount << std::endl;    
 //   std::cout << "validCount" << ":" << validCount << std::endl;  
    
    } // boundaryNormal not null
} 

int main( int argc, char *argv[] )
{
  if (argc < 4 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
	  std::cerr << " ROIFile seedFile niter outputFile [threshold] [boundaryNormal] [intializeFile] " << std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::ImageFileReader<ImageType>     ReaderType;
  typedef itk::ImageFileReader<FloatType>     FloatReaderType;
  typedef itk::ImageFileWriter<FloatType>     WriterType;
  typedef itk::MaskNeighborhoodOperatorImageFilter< FloatType, ImageType, FloatType> FilterType;
  typedef itk::BinaryCrossStructuringElement<float, 3> OperatorType;
  typedef itk::LaplacianUpdateImageFilter< FloatType > UpdaterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  ReaderType::Pointer seeds = ReaderType::New();
  seeds->SetFileName( argv[2] );
   
  try
    {
    seeds->Update();
    reader->Update();
    
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

   double threshold = 0.5;
   if( argc > 5 )
    {
    threshold = atof( argv[5] );
    }

   ImageType::Pointer seedImage = seeds->GetOutput();
   ImageType::Pointer maskImage = reader->GetOutput();
    
   FloatType::Pointer laplacian;
   FloatType::Pointer fixedBoundary = FloatType::New();  // fixed boundary values
   CreateFixedBoundaryImage( fixedBoundary, seedImage );
   
   FloatType::Pointer neumannBoundary = FloatType::New(); // neumann boundary pixels
   itk::idp::BinaryThreshold<ImageType,FloatType>( seedImage, 4, 4, 4, 0, neumannBoundary );
   
   GradientType::Pointer boundaryNormal;
   
   FloatType::Pointer neumannUpdate;
   
   if ( argc > 6 )
     {
     itk::idp::ReadImage<GradientType>( argv[6], boundaryNormal );
     }   

   if ( argc > 7 )
    {
    // initialize laplacian from file
    FloatReaderType::Pointer freader = FloatReaderType::New();
    freader->SetFileName( argv[7] );
    try
      {
      freader->Update();
      laplacian = freader->GetOutput();
      laplacian->DisconnectPipeline();
      freader = 0;
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
    }
   else
    {
      laplacian = FloatType::New();
      CreateFixedBoundaryImage( laplacian, seeds->GetOutput() );
    }
    
           

   // set up operator
   OperatorType op;
   op.CreateStructuringElement();
   op[op.GetCenterNeighborhoodIndex()] = 0.0;
 
   OperatorType::Iterator opIter;
   OperatorType::Iterator opEnd = op.End();
   for( opIter = op.Begin(); opIter != opEnd; ++opIter )
    {
    (*opIter) /= 6.0;
    }

   int numberOfIterations = atoi(argv[3]);
   double eps = 5000.0;
   
   itk::idp::MakeImage<FloatType,FloatType>( laplacian, neumannUpdate, 0.0 );

   for( int k = 0; k < numberOfIterations; k++ )
    {
    std::cout << " iter: " << k;

    // compute neuman bounary updates
    neumannUpdate->FillBuffer( 0.0 );
    ComputeNeumanBounaryUpdate( laplacian, maskImage, seedImage, neumannBoundary, boundaryNormal, neumannUpdate );
    
    //itk::idp::WriteImage<FloatType>( "dummy.mhd", neumannUpdate );
    
    // compute interior updates
    FilterType::Pointer filter = FilterType::New();
    filter->SetInput( laplacian );
    filter->SetMaskImage( reader->GetOutput() );
    filter->SetInput( laplacian );
    filter->SetOperator( op );
    filter->SetDefaultValue( 10001.0 );
  
    // combine results
    UpdaterType::Pointer updater = UpdaterType::New();
    updater->SetInput1( filter->GetOutput() );
    updater->SetInput2( fixedBoundary );
    updater->SetInput3( laplacian );
    updater->SetInput4( neumannBoundary );
    updater->SetInput5( neumannUpdate );

    try
      {
      updater->Update();
      eps = updater->GetEpsilon();
      laplacian = updater->GetOutput();
      laplacian->DisconnectPipeline();
      std::cout << " eps: " << eps << std::endl;
      if ( eps < threshold )
        {
        break;
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

    }

   
   WriterType::Pointer writer = WriterType::New();
   writer->SetInput( laplacian );
   writer->SetFileName( argv[4] );

  try
    {
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


