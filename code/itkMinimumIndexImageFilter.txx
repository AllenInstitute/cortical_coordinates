/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $File: //depot/bioapps/infoapps/HBAMapping/main/Common/itkMinimumMergeImageFilter.txx $
  Language:  C++
  Date:      $DateTime: 2011/07/14 00:09:05 $
  Version:   $Revision: #2 $

  Copyright (c) Allen Institute for Brain Science. All rights reserved.

=========================================================================*/
#ifndef __itkMinimumIndexImageFilter_txx
#define __itkMinimumIndexImageFilter_txx

#include "itkMinimumIndexImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkProgressReporter.h"

namespace itk
{


/**
 * Constructor
 */
template <class TImage, class TLabelImage > 
MinimumIndexImageFilter<TImage,TLabelImage>
::MinimumIndexImageFilter()
{
  this->SetNumberOfRequiredInputs( 2 );
  this->SetNumberOfRequiredOutputs( 2 );

  typename LabelImageType::Pointer ptr = LabelImageType::New();
  this->ProcessObject::SetNthOutput( 1, ptr.GetPointer() ); 

}


/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TImage, class TLabelImage > 
void
MinimumIndexImageFilter<TImage,TLabelImage>
::SetInput1( const TImage * image ) 
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(0, const_cast<TImage *>( image ));
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TImage, class TLabelImage > 
void
MinimumIndexImageFilter<TImage,TLabelImage>
::SetInput2( const TImage * image ) 
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(1, const_cast<TImage *>( image ));
}


/**
 * Get output label image
 */
template <class TImage, class TLabelImage > 
typename MinimumIndexImageFilter<TImage,TLabelImage>
::LabelImageType *
MinimumIndexImageFilter<TImage,TLabelImage>
::GetLabelOutput()
{
    LabelImageType * labelOutputPtr 
    = static_cast<TLabelImage *>( this->ProcessObject::GetOutput(1) );

    return labelOutputPtr;
    
}


/**
 * ThreadedGenerateData Performs the pixel-wise addition
 */
template <class TImage, class TLabelImage > 
void
MinimumIndexImageFilter<TImage,TLabelImage>
::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread,
                        int threadId)
{

  ImagePointer inputPtr1
    = dynamic_cast<const TImage*>(ProcessObject::GetInput(0));
  ImagePointer inputPtr2
    = dynamic_cast<const TImage*>(ProcessObject::GetInput(1));


  TImage * outputPtr = this->GetOutput();
  TLabelImage * labelOutputPtr 
    = static_cast<TLabelImage *>( ProcessObject::GetOutput(1) );
  
  ImageRegionConstIterator<TImage> inputIt1(inputPtr1, outputRegionForThread);
  ImageRegionConstIterator<TImage> inputIt2(inputPtr2, outputRegionForThread);

  ImageRegionIterator<TImage> outputIt(outputPtr, outputRegionForThread);
  ImageRegionIterator<TLabelImage> labelOutputIt(labelOutputPtr, outputRegionForThread);

  ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inputIt1.GoToBegin();
  inputIt2.GoToBegin();
  outputIt.GoToBegin();
  labelOutputIt.GoToBegin();

  while( !inputIt1.IsAtEnd() ) 
    {
    LabelImagePixelType lab;    
    outputIt.Set( m_Functor( inputIt1.Get(), 1,
                             inputIt2.Get(), 2, lab ) );
    labelOutputIt.Set( lab );                     
    ++inputIt2;
    ++inputIt1;
    ++outputIt;
    ++labelOutputIt;
    progress.CompletedPixel(); // potential exception thrown here
    }

}

} // end namespace itk

#endif
