/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkIsMemberImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2004/03/05 21:44:57 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkIsMemberImageFilter_h
#define __itkIsMemberImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkConceptChecking.h"
#include "itkArray.h"

namespace itk
{
  
/** \class IsMemberImageFilter
 *
 * \brief Detect if pixels are members of a specific set.
 *
 * This filter produces an output image whose pixels
 * are either one of two values ( IsMemberValue or IsNotMemberValue ), 
 * depending on whether of not the corresponding input image pixel
 * is an element of a user specified set.
 * 
 * This filter is templated over the input image type
 * and the output image type.
 * The filter expect both images to have the same number of dimensions.
 *
 * \ingroup IntensityImageFilters  Multithreaded
 */
namespace Functor {  
  
template< class TInput, class TOutput>
class IsMember
{
public:
  typedef std::vector<TInput> InputArrayType;

  IsMember() 
    { 
    m_InputArray = InputArrayType(0); 
    };

  ~IsMember() {};

  void SetInputArray( const InputArrayType& values )
    { m_InputArray = values; }

  void SetIsMemberValue( const TOutput & value )
  { m_IsMemberValue = value; }

  void SetIsNotMemberValue( const TOutput & value )
  { m_IsNotMemberValue = value; }

  inline TOutput operator()( const TInput & A )
  {
    const unsigned int n = m_InputArray.size();

    for( unsigned int k = 0; k < n; k++ )
      {
      if ( A == m_InputArray[k] )
        {
        return m_IsMemberValue;
        }
      }

    return m_IsNotMemberValue;
  }

private:
  InputArrayType    m_InputArray;
  TOutput           m_IsMemberValue;
  TOutput           m_IsNotMemberValue;

};
}

template <class TInputImage, class TOutputImage>
class ITK_EXPORT IsMemberImageFilter :
    public
UnaryFunctorImageFilter<TInputImage,TOutputImage, 
                        Functor::IsMember< 
  typename TInputImage::PixelType, 
  typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef IsMemberImageFilter  Self;
  typedef UnaryFunctorImageFilter<TInputImage,TOutputImage, 
                                  Functor::IsMember< 
    typename TInputImage::PixelType, 
    typename TOutputImage::PixelType>   
  >  Superclass;
  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(IsMemberImageFilter, UnaryFunctorImageFilter);

  /** Pixel types. */
  typedef typename TInputImage::PixelType  InputPixelType;
  typedef typename TOutputImage::PixelType OutputPixelType;
  
  /* Input array containing the values to be detected in the images. */
  typedef std::vector<InputPixelType> InputArrayType;
  virtual void SetInputArray( InputArrayType & array )
    {
    m_InputArray = array;
    this->Modified();
    }
  virtual const InputArrayType & GetInputArray() const
    {
    return m_InputArray;
    }

  /** Set the "is not member" pixel value. The default value 
   * NumericTraits<OutputPixelType>::Zero. */
  itkSetMacro(IsNotMemberValue,OutputPixelType);
  
  /** Get the "is not member"  pixel value. */
  itkGetMacro(IsNotMemberValue,OutputPixelType);

  /** Set the "is member" pixel value. The default value 
   * NumericTraits<OutputPixelType>::max() */
  itkSetMacro(IsMemberValue,OutputPixelType);
  
  /** Get the "is member" pixel value. */
  itkGetMacro(IsMemberValue,OutputPixelType);
  
protected:
  IsMemberImageFilter();
  virtual ~IsMemberImageFilter() {}
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** This method is used to set the state of the filter before 
   * multi-threading. */
  virtual void BeforeThreadedGenerateData();

private:
  IsMemberImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  InputArrayType      m_InputArray;
  OutputPixelType     m_IsMemberValue;
  OutputPixelType     m_IsNotMemberValue;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkIsMemberImageFilter.txx"
#endif

#endif
