/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOverwriteImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:28:43 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkOverwriteImageFilter_h
#define __itkOverwriteImageFilter_h

#include "itkBinaryFunctorImageFilter.h"
#include "itkNumericTraits.h"


namespace itk
{
  
/** \class OverwriteImageFilter
 * \brief Implements pixel-wise overwrite of two images.
 *
 * This class is parametrized over the types of the two 
 * input images and the type of the output image. 
 * Numeric conversions (castings) are done by the C++ defaults.
 * 
 * The operation over one pixel will is
 *
 * if ( input2_pixel is non-zero )
 *  output_pixel = static_cast<OutputPixelType>( input2_pixel )
 * else
 *  output_pixel = static_cast<OutputPixelType>( input1_pixel )
 *
 * \warning No numeric overflow checking is performed in this filter.
 *
 * \ingroup IntensityImageFilters  Multithreaded
 */
namespace Functor {  
  
template< class TInput1, class TInput2, class TOutput >
class Overwrite
{
public:
  typedef typename NumericTraits< TInput1 >::AccumulateType AccumulatorType;
  Overwrite() {};
  ~Overwrite() {};
  inline TOutput operator()( const TInput1 & A, const TInput2 & B)
  {
    if( B )  // if B is not (zero) background value
      {
      // overwrite pixel with B value
      return static_cast<TOutput>( B );
      }
    return static_cast<TOutput>( A );
  }
}; 

}
template <class TInputImage1, class TInputImage2, class TOutputImage>
class ITK_EXPORT OverwriteImageFilter :
    public
BinaryFunctorImageFilter<TInputImage1,TInputImage2,TOutputImage, 
                         Functor::Overwrite< 
  typename TInputImage1::PixelType, 
  typename TInputImage2::PixelType,
  typename TOutputImage::PixelType>   >


{
public:
  /** Standard class typedefs. */
  typedef OverwriteImageFilter  Self;
  typedef BinaryFunctorImageFilter<TInputImage1,TInputImage2,TOutputImage, 
                                   Functor::Overwrite< 
    typename TInputImage1::PixelType, 
    typename TInputImage2::PixelType,
    typename TOutputImage::PixelType>   
  >  Superclass;
  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
protected:
  OverwriteImageFilter() {}
  virtual ~OverwriteImageFilter() {}

private:
  OverwriteImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk


#endif
