/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $File: //depot/bioapps/infoapps/HBAMapping/main/Common/idpUtilities.cxx $
  Language:  C++
  Date:      $DateTime: 2013/03/11 20:42:46 $
  Version:   $Revision: #4 $

  Copyright (c) Allen Institute for Brain Science. All rights reserved.

=========================================================================*/
#ifndef __idpUtilities_cxx
#define __idpUtilities_cxx

#include "idpUtilities.h"


namespace itk
{
namespace idp
{

/*--------------------------
 * Helper function to change path between cluster and windows machines
 * --------------------------
 */
void ChangePaths( std::string & str )
{
#ifdef WIN32
  if (!itksys::SystemTools::StringStartsWith( str.c_str(), "//titan/CNS/" ) )
    {
    itksys::SystemTools::ReplaceString( str, "/projects/devmouse/vol1/", "//titan/CNS/devmouse/" );
    itksys::SystemTools::ReplaceString( str, "/projects/0351/vol1/", "//titan/CNS/0351/" );
    itksys::SystemTools::ReplaceString( str, "/projects/0351x/vol1/", "//titan/CNS/0351x/" );
    itksys::SystemTools::ReplaceString( str, "/projects/0376/vol1/", "//titan/CNS/0376/" );
    itksys::SystemTools::ReplaceString( str, "/projects/aibssan/", "//titan/CNS/" );  
    itksys::SystemTools::ReplaceString( str, "/aibssan/", "//titan/CNS/aibssan/" );
    }
#endif
}

} // end namespace idp
} //end namespace itk

#endif

  
