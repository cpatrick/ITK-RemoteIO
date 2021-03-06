/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __itkMorphologyImageFilter_txx
#define __itkMorphologyImageFilter_txx

#include <limits.h>

#include "itkNumericTraits.h"
#include "itkMorphologyImageFilter.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkProgressReporter.h"

namespace itk
{
template< class TInputImage, class TOutputImage, class TKernel >
MorphologyImageFilter< TInputImage, TOutputImage, TKernel >
::MorphologyImageFilter()
{
  m_DefaultBoundaryCondition.SetConstant(NumericTraits< PixelType >::Zero);
  m_BoundaryCondition = &m_DefaultBoundaryCondition;
}

template< class TInputImage, class TOutputImage, class TKernel >
void
MorphologyImageFilter< TInputImage, TOutputImage, TKernel >
::ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread,
                       ThreadIdType threadId)
{
  // Neighborhood iterators
  NeighborhoodIteratorType b_iter;

  // Find the boundary "faces"
  typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType >::FaceListType faceList;
  NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType > fC;
  faceList = fC( this->GetInput(), outputRegionForThread, this->GetKernel().GetRadius() );

  typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType >::FaceListType::iterator fit;

  ImageRegionIterator< TOutputImage > o_iter;

  ProgressReporter progress( this, threadId, outputRegionForThread.GetNumberOfPixels() );

  // Process the boundary faces, these are N-d regions which border the
  // edge of the buffer

  const KernelIteratorType kernelBegin = this->GetKernel().Begin();
  const KernelIteratorType kernelEnd = this->GetKernel().End();

  for ( fit = faceList.begin(); fit != faceList.end(); ++fit )
    {
    b_iter = NeighborhoodIteratorType(this->GetKernel().GetRadius(),
                                      this->GetInput(), *fit);

    o_iter = ImageRegionIterator< OutputImageType >(this->GetOutput(), *fit);
    b_iter.OverrideBoundaryCondition(m_BoundaryCondition);
    b_iter.GoToBegin();

    while ( !o_iter.IsAtEnd() )
      {
      o_iter.Set( this->Evaluate(b_iter, kernelBegin, kernelEnd) );
      ++b_iter;
      ++o_iter;
      progress.CompletedPixel();
      }
    }
}

template< class TInputImage, class TOutputImage, class TKernel >
void
MorphologyImageFilter< TInputImage, TOutputImage, TKernel >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Boundary condition: " << typeid( *m_BoundaryCondition ).name() << std::endl;
}
} // end namespace itk
#endif
