/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLaplacianUpdateImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006-03-31 14:31:04 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLaplacianUpdateImageFilter_h
#define __itkLaplacianUpdateImageFilter_h

#include "itkInPlaceImageFilter.h"
#include "itkArray.h"

namespace itk
{

/** \class LaplacianUpdateImageFilter
 * \brief Perform postprocessing before next iteration of laplacian algorithm
 *
 * This filter takes three inputs:
 * [1] Output of the neighborhood filter
 * [2] The fix boundary image
 * [3] The solution from last iteration
 * [4] the neumann boundary image
 * [5] the neumann boundary update
 *
 * This filter has one output:
 * [1] Image representing the current solution
 *
 * After Update(), method GetEpsilon() returns the average absolute difference
 * before the current and last iteration.
 *
 * This filter performs the calculation inplace. That is first input is destroy
 * the create the output.
 *
 * \ingroup IntensityImageFilters
 * \sa ShiftScaleImageFilter
 * \sa InPlaceImageFilter
 */
template <class TInputImage>
class ITK_EXPORT LaplacianUpdateImageFilter:
    public InPlaceImageFilter<TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef LaplacianUpdateImageFilter         Self;
  typedef InPlaceImageFilter<TInputImage>  Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Typedef to describe the output and input image region types. */
  typedef typename TInputImage::RegionType InputImageRegionType;
  typedef typename TInputImage::RegionType OutputImageRegionType;

  /** Typedef to describe the pointer to the input/output. */  
  typedef typename TInputImage::Pointer InputImagePointer;
  typedef typename TInputImage::Pointer OutputImagePointer;

  /** Typedef to describe the type of pixel. */
  typedef typename TInputImage::PixelType InputImagePixelType;
  typedef typename TInputImage::PixelType OutputImagePixelType;

  /** Typedef to describe the output and input image index and size types. */
  typedef typename TInputImage::IndexType InputImageIndexType;
  typedef typename TInputImage::SizeType InputImageSizeType;
  typedef typename TInputImage::OffsetType InputImageOffsetType;
  typedef typename TInputImage::IndexType OutputImageIndexType;
  typedef typename TInputImage::SizeType OutputImageSizeType;
  typedef typename TInputImage::OffsetType OutputImageOffsetType;

  /** Type to use form computations. */
  typedef typename NumericTraits<OutputImagePixelType>::RealType RealType;
      
  /** Image related typedefs. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension ) ;

  /** Run-time type information (and related methods). */
  itkTypeMacro(LaplacianUpdateImageFilter, InPlaceImageFilter);

  /** Get the average absolute difference between current and previous solution */
  itkGetMacro( Epsilon, RealType );

  /** Set input #1 - output of neighborhood operator */
  void SetInput1( const TInputImage * image );

  /** Set input #2 - the boundary image */
  void SetInput2( const TInputImage * image );

  /** Set input #3 - solution from last iteration */
  void SetInput3( const TInputImage * image );
  
  /** Set input #4 - the neumann boundary image */
  void SetInput4( const TInputImage * image );

  /** Set input #5 - the neumann boundary update */
  void SetInput5( const TInputImage * image );  

protected:
  LaplacianUpdateImageFilter();
  ~LaplacianUpdateImageFilter();
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Initialize some accumulators before the threads run. */
  void BeforeThreadedGenerateData ();
  
  /** Tally data accumulated in threads. */
  void AfterThreadedGenerateData ();
  
  /** Multi-thread version GenerateData. */
  void  ThreadedGenerateData (const OutputImageRegionType& 
                              outputRegionForThread,
                              int threadId) ;

private:
  LaplacianUpdateImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  RealType            m_Epsilon;
  Array<RealType>     m_SumOfAbsoluteDifference;
  Array<long>         m_Count;

};

  
} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLaplacianUpdateImageFilter.txx"
#endif
  
#endif
