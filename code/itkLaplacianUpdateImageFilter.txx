/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLaplacianUpdateImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2003-12-15 14:13:18 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkLaplacianUpdateImageFilter_txx
#define _itkLaplacianUpdateImageFilter_txx
#include "itkLaplacianUpdateImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"

namespace itk {

template<class TInputImage>
LaplacianUpdateImageFilter<TInputImage>
::LaplacianUpdateImageFilter()
{
  this->SetNumberOfRequiredInputs( 5 );
  this->InPlaceOn();
  m_Epsilon = NumericTraits<RealType>::Zero;
}

template<class TInputImage>
LaplacianUpdateImageFilter<TInputImage>
::~LaplacianUpdateImageFilter()
{
}

template<class TInputImage>
void
LaplacianUpdateImageFilter<TInputImage>
::SetInput1( const TInputImage * image )
{
  this->SetNthInput(0, const_cast<TInputImage *>( image ) );
}

template<class TInputImage>
void
LaplacianUpdateImageFilter<TInputImage>
::SetInput2( const TInputImage * image )
{
  this->SetNthInput(1, const_cast<TInputImage *>( image ) );
}

template<class TInputImage>
void
LaplacianUpdateImageFilter<TInputImage>
::SetInput3( const TInputImage * image )
{
  this->SetNthInput(2, const_cast<TInputImage *>( image ) );
}

template<class TInputImage>
void
LaplacianUpdateImageFilter<TInputImage>
::SetInput4( const TInputImage * image )
{
  this->SetNthInput(3, const_cast<TInputImage *>( image ) );
}

template<class TInputImage>
void
LaplacianUpdateImageFilter<TInputImage>
::SetInput5( const TInputImage * image )
{
  this->SetNthInput(4, const_cast<TInputImage *>( image ) );
}

template<class TInputImage>
void
LaplacianUpdateImageFilter<TInputImage>
::BeforeThreadedGenerateData ()
{
  int numberOfThreads = this->GetNumberOfThreads();

  //  Allocate and initialize the thread temporaries
  m_SumOfAbsoluteDifference.SetSize(numberOfThreads);
  m_SumOfAbsoluteDifference.Fill(0);
  m_Count.SetSize(numberOfThreads);
  m_Count.Fill(0);
}

template<class TInputImage>
void
LaplacianUpdateImageFilter<TInputImage>
::AfterThreadedGenerateData ()
{
  int numberOfThreads = this->GetNumberOfThreads();

  m_Epsilon = 0.0;
  long totalCount = 0;

  // Accumulate counts for each thread
  for( int i = 0; i < numberOfThreads; i++)
    {
    m_Epsilon += m_SumOfAbsoluteDifference[i];
    totalCount += m_Count[i];
    }

  if ( totalCount )
    {
    m_Epsilon /= static_cast<RealType>( totalCount );
    }
  else
   {
   m_Epsilon = 0.0;
   }
}

template<class TInputImage>
void
LaplacianUpdateImageFilter<TInputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       int threadId) 
{

  ImageRegionConstIterator<TInputImage>  it1 (this->GetInput(0), outputRegionForThread);
  ImageRegionConstIterator<TInputImage>  it2 (this->GetInput(1), outputRegionForThread);
  ImageRegionConstIterator<TInputImage>  it3 (this->GetInput(2), outputRegionForThread);
  ImageRegionConstIterator<TInputImage>  it4 (this->GetInput(3), outputRegionForThread);
  ImageRegionConstIterator<TInputImage>  it5 (this->GetInput(4), outputRegionForThread);  
  ImageRegionIterator<TInputImage> ot (this->GetOutput(), outputRegionForThread);
  
  // support progress methods/callbacks
  ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
          
  while (!it1.IsAtEnd())
    {

    if ( ot.Get() > 10000.0 )
      {
      // set value outside the mask to zero
      ot.Set( 0.0 );
      }
    else
      {
      m_SumOfAbsoluteDifference[threadId] += vnl_math_abs( ot.Get() - it3.Get() );
      m_Count[threadId]++;
      }
     

    if( it2.Get() )
      {
      // copy the fix boundary values
      ot.Set( it2.Get() );
      } 
    else if ( it4.Get() )
      {    
      // copy the neumann update value
      ot.Set( it5.Get() );
      } 

    ++it1;
    ++it2;
    ++it3;
    ++it4;
    ++it5;
    ++ot;

    progress.CompletedPixel();
    }
}

template <class TInputImage>
void 
LaplacianUpdateImageFilter<TInputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Epsilon: " << m_Epsilon << std::endl;
}


}// end namespace itk
#endif
