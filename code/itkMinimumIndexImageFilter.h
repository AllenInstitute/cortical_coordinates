/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $File: //depot/bioapps/infoapps/HBAMapping/main/Common/itkMinimumMergeImageFilter.h $
  Language:  C++
  Date:      $DateTime: 2011/07/14 00:09:05 $
  Version:   $Revision: #2 $

  Copyright (c) Allen Institute for Brain Science. All rights reserved.

=========================================================================*/
#ifndef __itkMinimumIndexImageFilter_h
#define __itkMinimumIndexImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"

namespace itk
{    
  
/** \class MinimumIndexImageFilter
 */
namespace Function {  

template< class TValue, class TLabel>
class MinimumIndex
{
public:

  MinimumIndex() {}
  ~MinimumIndex() {}

  bool operator!=( const MinimumIndex & ) const
    {
    return false;
    }
  bool operator==( const MinimumIndex & other ) const
    {
    return !(*this != other);
    }
   
  inline TValue operator()( const TValue & V1, const TLabel & L1, 
                            const TValue & V2, const TLabel & L2,
                            TLabel & LOutput ) const
    { 
      if ( V1 <= V2 ) 
        { 
        LOutput = L1; 
        return V1; 
        }
      else 
        { 
        LOutput = L2; 
        return V2; 
        } 
    }


};
 
} //end namespace function


template <class TImage, class TLabelImage > 
class ITK_EXPORT MinimumIndexImageFilter :
    public ImageToImageFilter<TImage,TImage> 
{
public:
  /** Standard class typedefs. */
  typedef MinimumIndexImageFilter                       Self;
  typedef InPlaceImageFilter<TImage,TImage>             Superclass;
  typedef SmartPointer<Self>                            Pointer;
  typedef SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(MinimumIndexImageFilter, InPlaceImageFilter);

  /** Some convenient typedefs. */
  typedef TImage                                 ImageType;
  typedef typename ImageType::ConstPointer       ImagePointer;
  typedef typename ImageType::RegionType         ImageRegionType; 
  typedef typename ImageType::PixelType          ImagePixelType;
  typedef ImageRegionType                        OutputImageRegionType;

  typedef TLabelImage                            LabelImageType;
  typedef typename LabelImageType::ConstPointer  LabelImagePointer;
  typedef typename LabelImageType::PixelType     LabelImagePixelType; 
  
  typedef Function::MinimumIndex<ImagePixelType,
                                 LabelImagePixelType>  FunctorType;

  /** Connect one of the operands for pixel-wise addition */
  void SetInput1( const TImage * image );
  void SetInput2( const TImage * image );
  
  /** Get the output label image */
  LabelImageType * GetLabelOutput();

  /** Get the functor object.  The functor is returned by reference.
   * (Functors do not have to derive from itk::LightObject, so they do
   * not necessarily have a reference count. So we cannot return a
   * SmartPointer.) */
  FunctorType& GetFunctor() { return m_Functor; }

  /** Get the functor object.  The functor is returned by reference.
   * (Functors do not have to derive from itk::LightObject, so they do
   * not necessarily have a reference count. So we cannot return a
   * SmartPointer.) */
  const FunctorType& GetFunctor() const
    {
    return m_Functor;
    }

  /** Set the functor object.  This replaces the current Functor with a
   * copy of the specified Functor. This allows the user to specify a
   * functor that has ivars set differently than the default functor.
   * This method requires an operator!=() be defined on the functor
   * (or the compiler's default implementation of operator!=() being
   * appropriate). */
  void SetFunctor(const FunctorType& functor)
    {
    if (m_Functor != functor)
      {
      m_Functor = functor;
      this->Modified();
      }
    }

  /** ImageDimension constants */
  itkStaticConstMacro(
    ImageDimension, unsigned int, TImage::ImageDimension);
  itkStaticConstMacro(
    LabelImageDimension, unsigned int, TLabelImage::ImageDimension);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(SameDimensionCheck,
    (Concept::SameDimension<itkGetStaticConstMacro(ImageDimension),
                            itkGetStaticConstMacro(LabelImageDimension)>));
  /** End concept checking */
#endif

protected:
  MinimumIndexImageFilter();
  virtual ~MinimumIndexImageFilter() {}

  /** MinimumIndexImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );
                                                

private:
  MinimumIndexImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  FunctorType m_Functor;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMinimumIndexImageFilter.txx"
#endif

#endif
