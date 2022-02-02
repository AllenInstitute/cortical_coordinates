/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkIsMemberImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2004/06/02 13:01:44 $
  Version:   $Revision: 1.9 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkIsMemberImageFilter_txx
#define _itkIsMemberImageFilter_txx

#include "itkIsMemberImageFilter.h"

namespace itk
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
IsMemberImageFilter<TInputImage, TOutputImage>
::IsMemberImageFilter()
{
  m_IsNotMemberValue  = NumericTraits<OutputPixelType>::Zero;
  m_IsMemberValue     = NumericTraits<OutputPixelType>::max();
  m_InputArray        = InputArrayType(0);
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void 
IsMemberImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "IsNotMemberValue: "
     << static_cast<typename NumericTraits<OutputPixelType>::PrintType>(m_IsNotMemberValue) << std::endl;
  os << indent << "IsMemberValue: "
     << static_cast<typename NumericTraits<OutputPixelType>::PrintType>(m_IsMemberValue) << std::endl;
  os << indent << "InputArray: [";
  for( unsigned int k = 0; k < m_InputArray.size() - 1; k++ )
      {
      os << m_InputArray[k] << ", ";
      }
  if ( m_InputArray.size() > 0 )
      {
      os << m_InputArray[m_InputArray.size()-1];
      }
   os << "]" << std::endl;
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void 
IsMemberImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  // Setup up the functor
  this->GetFunctor().SetIsMemberValue( m_IsMemberValue );
  this->GetFunctor().SetIsNotMemberValue( m_IsNotMemberValue );

  this->GetFunctor().SetInputArray( m_InputArray );
  
}


} // end namespace itk

#endif
