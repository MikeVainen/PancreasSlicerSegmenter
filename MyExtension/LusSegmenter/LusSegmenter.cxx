#include "itkImageFileWriter.h"

#include "itkSmoothingRecursiveGaussianImageFilter.h"

#include "itkPluginUtilities.h"

#include "LusSegmenterCLP.h"
#include "itkExtractImageFilter.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <Windows.h>
#include <list>
#include "vnl/vnl_math.h"

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkImageLinearIteratorWithIndex.h"

#include "itkGradientAnisotropicDiffusionImageFilter.h"
#include <itkCurvatureAnisotropicDiffusionImageFilter.h>
#include "itkMedianImageFilter.h"

#include "itkBinaryThresholdImageFilter.h"
#include "itkThresholdImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include <itkGradientMagnitudeImageFilter.h>
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"

#include <itkGrayscaleErodeImageFilter.h>
#include <itkGrayscaleDilateImageFilter.h>

#include <itkVotingBinaryHoleFillingImageFilter.h>
#include <itkCastImageFilter.h>
#include <itkSigmoidImageFilter.h>

#include "itkHoughTransform2DCirclesImageFilter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkDiscreteGaussianImageFilter.h"

#include <itkBinaryCrossStructuringElement.h>
#include "itkFlatStructuringElement.h"
#include <itkBinaryBallStructuringElement.h>
#include "itkLineSpatialObject.h"
#include "itkLineSpatialObjectPoint.h"
#include "itkLineIterator.h"
#include "itkSurfaceSpatialObject.h"



using namespace std;


namespace
{

template <class T>


int DoIt( int argc, char * argv[], T )
{
  PARSE_ARGS;

  typedef          T           InputPixelType;
  typedef          T           OutputPixelType;
  typedef unsigned short       ImagePixelType;
  typedef          float       AnisotropicPixelType;
  typedef          float       AccumulatorPixelType;
  
  const   unsigned int         Dimension = 2;
  const	  unsigned int		   MinimumBorderValue = 45;

  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::Image<ImagePixelType, Dimension> ImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;
  typedef itk::Image<AnisotropicPixelType,Dimension> GradientImageType;

  typedef itk::ImageFileReader<InputImageType> ReaderType;
  typedef itk::ImageFileWriter<OutputImageType> WriterType;

  typedef itk::ImageLinearIteratorWithIndex<InputImageType> IteratorType;
  typedef itk::ImageLinearIteratorWithIndex<ImageType> MaskIteratorType;
  typedef itk::CurvatureAnisotropicDiffusionImageFilter<ImageType, ImageType> AnisoCurvatureType;
  typedef itk::GradientAnisotropicDiffusionImageFilter<ImageType, ImageType> AnisoGradientType;
  typedef itk::MedianImageFilter<ImageType, ImageType> MedianFilterType;

   //Better results in terms of smoothing are oftenly obtained by applying the Curvature version of the anisotropic filter

  typedef itk::VotingBinaryHoleFillingImageFilter<ImageType,ImageType >  FillingFilterType;
  typedef itk::FlatStructuringElement<Dimension> FlatStructuringElementType;
  typedef itk::BinaryCrossStructuringElement<ImagePixelType, Dimension> StructuringElementType;
  typedef itk::GrayscaleErodeImageFilter<ImageType,ImageType, FlatStructuringElementType> ErodeFilterType;
  typedef itk::GrayscaleDilateImageFilter<ImageType,ImageType, FlatStructuringElementType> DilateFilterType;

  typedef itk::CastImageFilter<GradientImageType,OutputImageType> CasterType;
  typedef itk::GradientMagnitudeImageFilter<ImageType,GradientImageType> GradMagType;
  typedef itk::GradientMagnitudeRecursiveGaussianImageFilter<ImageType,GradientImageType> RecursiveGradMadType;

  typedef itk::BinaryThresholdImageFilter <ImageType, ImageType> BinaryThresholdImageFilterType;
 
  BinaryThresholdImageFilterType::Pointer thresholdFilter = BinaryThresholdImageFilterType::New();

   
  

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  FillingFilterType::Pointer filler = FillingFilterType::New();
  ErodeFilterType::Pointer eroder = ErodeFilterType::New();
  DilateFilterType::Pointer dilater = DilateFilterType::New();
  AnisoCurvatureType::Pointer aniCur = AnisoCurvatureType::New();
  AnisoGradientType::Pointer aniGrad = AnisoGradientType::New();
  MedianFilterType::Pointer medianFilter = MedianFilterType::New();
  CasterType::Pointer caster = CasterType::New();
  GradMagType::Pointer gradMagnitude = GradMagType::New();
  RecursiveGradMadType::Pointer recursiveGradMagnitude = RecursiveGradMadType::New();


  InputImageType::Pointer image;
  ImageType::Pointer maskImage = ImageType::New();
  ImageType::Pointer finalMaskImage = ImageType::New();
  ImageType::Pointer resultImage = ImageType::New();
  OutputImageType::Pointer outImage = OutputImageType::New();
  
  typedef OutputImageType::IndexType imageIndexType;
  InputImageType::IndexType localIndex;
  ImageType::IndexType centerIndex;

  typedef itk::Image< AccumulatorPixelType, Dimension > AccumulatorImageType;
  typedef itk::LineSpatialObject< Dimension >   LineType;
  typedef itk::Point< double, ImageType::ImageDimension > PointType;

  typedef itk::SurfaceSpatialObject<Dimension> SurfaceType;
  typedef SurfaceType::Pointer SurfacePointer;
  typedef itk::SurfaceSpatialObjectPoint<Dimension> SurfacePointType;
  typedef itk::CovariantVector<double,Dimension> VectorType;
  SurfacePointer Surface = SurfaceType::New();
  SurfaceType::PointListType list;

  reader->SetFileName( inputImage.c_str() );
   try
    {
		reader->Update();
		image = reader->GetOutput();
    }
   catch ( itk::ExceptionObject &err)
    {
		 std::cout << "ExceptionObject caught a !" << std::endl;
		std::cout << err << std::endl;
		return -1;
    }


 //We create a mask and output image of the same size as the one we're setting in

  ImageType::RegionType region = image->GetLargestPossibleRegion();

  // Pixel data is allocated
  maskImage->SetRegions( region );
  maskImage->Allocate();
  finalMaskImage->SetRegions( region );
  finalMaskImage->Allocate();
  resultImage->SetRegions( region );
  resultImage->Allocate();
  outImage->SetRegions( region );
  outImage->Allocate();


  itk::ImageRegionIterator<ImageType> imageIterator(maskImage,maskImage->GetLargestPossibleRegion());
 
  // Make the black base for the mask image
  while(!imageIterator.IsAtEnd())
    {
     imageIterator.Set(0);
	 finalMaskImage->SetPixel(imageIterator.GetIndex(),0);
	 resultImage->SetPixel(imageIterator.GetIndex(),0);
	 maskImage->SetPixel(imageIterator.GetIndex(),0);
	 outImage->SetPixel(imageIterator.GetIndex(),0);
     ++imageIterator;
    }

   OutputImageType::IndexType start;
  start[0] = 0 ;
  start[1] = bY ;
  // Software Guide : EndCodeSnippet


  // Software Guide : BeginCodeSnippet
  OutputImageType::SizeType size;
  size[0] = region.GetSize()[0]  ;
  size[1] = eY-bY;
  // Software Guide : EndCodeSnippet


  InputImageType::RegionType desiredInputRegion;
  desiredInputRegion.SetSize(  size  );
  desiredInputRegion.SetIndex( start );

  ImageType::RegionType desiredMaskRegion;
  desiredMaskRegion.SetSize(  size  );
  desiredMaskRegion.SetIndex( start );


  // Software Guide : BeginCodeSnippet
  ImageType::SizeType indexRadius;
  ImageType::SizeType medianIndexRadius;

  indexRadius[0] = radius; // radius along x
  indexRadius[1] = radius; // radius along y

  medianIndexRadius[0]= radius - 3;
  medianIndexRadius[1]= radius - 3;

  filler->SetRadius( indexRadius );
  medianFilter->SetRadius(medianIndexRadius);
  // Software Guide : EndCodeSnippet


  // Software Guide : BeginCodeSnippet
  filler->SetBackgroundValue(   0 );
  filler->SetForegroundValue( 255 );
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginCodeSnippet
  filler->SetMajorityThreshold( 2 );
  // Software Guide : EndCodeSnippet



  int i=0, j=0, NUM= radius/2; 
  FlatStructuringElementType::RadiusType rad; 
  rad.Fill(NUM);

  //we design the int matrix that will be used to create de Structuring Element
  vector<vector<int>> diamondStructure(NUM+NUM+1, vector<int>(NUM+NUM+1));
 
  for(i=-NUM; i<=NUM; i++){
	  for(j=-NUM; j<=NUM; j++){
		  if( abs(i)+abs(j)<=NUM){
			 diamondStructure[NUM+i][NUM+j] = 1;
		  }
          else { 
			 diamondStructure[NUM+i][NUM+j] = 0;
		  }
		}
	}

  FlatStructuringElementType elementoEstructurante = FlatStructuringElementType::Box(rad);  

  i=0;
  j=0;

  //Now we introduce the elements into the element with that exact diamond structure
  for(FlatStructuringElementType::Iterator iter = 
elementoEstructurante.Begin(); iter != elementoEstructurante.End(); ++iter, j++) 
  { 
		if(j==(2*NUM+1)){
			j=0;
			++i;
			std::cout << endl ;
		}
		*iter = diamondStructure[i][j];
		//we print the look of the structuring element
		std::cout << *iter;
  } 

  eroder->SetKernel(elementoEstructurante);
  dilater->SetKernel(elementoEstructurante);

  thresholdFilter->SetLowerThreshold(lowerthreshold);
  thresholdFilter->SetUpperThreshold(upperthreshold);
  thresholdFilter->SetInsideValue(255);
  thresholdFilter->SetOutsideValue(0);



  IteratorType inputIt( image, desiredInputRegion ); 
  inputIt.SetDirection(1);

  MaskIteratorType maskIt( finalMaskImage, desiredInputRegion);
  maskIt.SetDirection(1);

  for ( maskIt.GoToBegin(), inputIt.GoToBegin() ; !inputIt.IsAtEnd();  ++maskIt, ++inputIt )
    {
		maskIt.Set(inputIt.Get()); 
		if(inputIt.IsAtEndOfLine()){
			inputIt.NextLine();
			maskIt.NextLine();
		}
   }

  thresholdFilter->SetInput(finalMaskImage);
  filler->SetInput(thresholdFilter->GetOutput());
  eroder->SetInput(filler->GetOutput());
  dilater->SetInput(eroder->GetOutput());
  finalMaskImage =  dilater->GetOutput() ;
  finalMaskImage->Update();

  //lus::nonContourDetectedObtainMask(finalMaskImage, eY);
  vector<int> boundarypositions;
  boundarypositions = lus::contourDetection(finalMaskImage, eY, boundarypositions);
  if(nIts > 0)lus::ignoreBoundaries (finalMaskImage, eY, boundarypositions, nIts);

  MaskIteratorType fmaskIt( finalMaskImage, desiredInputRegion);
  fmaskIt.SetDirection(1);
  
  IteratorType outIt( outImage, desiredInputRegion ); 
  outIt.SetDirection(1);

  for ( fmaskIt.GoToBegin(), outIt.GoToBegin() ; !fmaskIt.IsAtEnd();  ++fmaskIt, ++outIt )
    {
		maskImage->SetPixel(fmaskIt.GetIndex(),fmaskIt.Get()/255*image->GetPixel(fmaskIt.GetIndex()));
		if (gC==2) outIt.Set(maskImage->GetPixel(fmaskIt.GetIndex()));

		if(fmaskIt.IsAtEndOfLine()){
			outIt.NextLine();
			fmaskIt.NextLine();
		}
   }


  if(gC!=2){

		thresholdFilter->SetLowerThreshold(90);
		thresholdFilter->SetUpperThreshold(200);
		medianFilter->SetInput(maskImage);
		thresholdFilter->SetInput(medianFilter->GetOutput());
		//thresholdFilter->SetOutsideValue(255);
		//thresholdFilter->SetInsideValue(0);
		thresholdFilter->Update();

		filler->SetInput(thresholdFilter->GetOutput());
		filler->Update();
		eroder->SetInput( filler->GetOutput());
		eroder->Update();
		gradMagnitude->SetInput(eroder->GetOutput());
		caster->SetInput(gradMagnitude->GetOutput());
		gradMagnitude->Update();
			

		resultImage= eroder->GetOutput();

			
		caster->Update();

		typedef itk::HoughTransform2DCirclesImageFilter<OutputPixelType,AccumulatorPixelType> HoughTransformFilterType;
		HoughTransformFilterType::Pointer houghFilter= HoughTransformFilterType::New();
		houghFilter->SetInput(caster->GetOutput());

		houghFilter->SetNumberOfCircles( ncircles );
		houghFilter->SetMinimumRadius(   minrad );
		houghFilter->SetMaximumRadius(   maxrad );

		houghFilter->Update();
		AccumulatorImageType::Pointer localAccumulator = houghFilter->GetOutput();

		HoughTransformFilterType::CirclesListType circles;
		circles = houghFilter->GetCircles( ncircles );

		typedef vector<ImageType::IndexType> SeedsPositions;
		typedef HoughTransformFilterType::CirclesListType CirclesListType;

		CirclesListType::const_iterator itCircles = circles.begin();
		std::vector<ImageType::IndexType> origins (ncircles); 

		SeedsPositions seeds;
		SeedsPositions::iterator seedsIterator = seeds.begin();
		

		int numCircles = 0;
		int counter = 0;

		for ( outIt.GoToBegin() ; !outIt.IsAtEnd(); ++outIt )
		{
			if(finalMaskImage->GetPixel(outIt.GetIndex())!=0) outIt.Set(image->GetPixel(outIt.GetIndex()));
			//if(resultImage->GetPixel(outIt.GetIndex())!=0) outIt.Set(resultImage->GetPixel(outIt.GetIndex())/255*image->GetPixel(outIt.GetIndex()));
			if(outIt.IsAtEndOfLine()){
				outIt.NextLine();
			}
		}

		while( itCircles != circles.end() )
			{
				
			centerIndex[0] = (short int)((*itCircles)->GetObjectToParentTransform()->GetOffset()[0]);
			centerIndex[1] = (short int)((*itCircles)->GetObjectToParentTransform()->GetOffset()[1]);
			if(image->GetPixel(centerIndex)>= 200){
				outImage->SetPixel(centerIndex, 0);
				seeds.push_back(centerIndex);
				for(double angle = 0;angle <= 2*vnl_math::pi; angle += vnl_math::pi/60.0 )
				{
					localIndex[0] = (long int)((*itCircles)->GetObjectToParentTransform()->GetOffset()[0] + (*itCircles)->GetRadius()[0]*std::cos(angle));
					localIndex[1] = (long int)((*itCircles)->GetObjectToParentTransform()->GetOffset()[1] + (*itCircles)->GetRadius()[0]*std::sin(angle));
					OutputImageType::RegionType outputRegion = outImage->GetLargestPossibleRegion();
					if( outputRegion.IsInside( localIndex ) ){
						outImage->SetPixel( localIndex, 0 );
					}
				}
				numCircles++;
			}
			if(image->GetPixel(centerIndex)>120 && image->GetPixel(centerIndex)<200){
				outImage->SetPixel(centerIndex, 255);
				seeds.push_back(centerIndex);
				for(double angle = 0;angle <= 2*vnl_math::pi; angle += vnl_math::pi/60.0 )
				{
					localIndex[0] = (long int)((*itCircles)->GetObjectToParentTransform()->GetOffset()[0] + (*itCircles)->GetRadius()[0]*std::cos(angle));
					localIndex[1] = (long int)((*itCircles)->GetObjectToParentTransform()->GetOffset()[1] + (*itCircles)->GetRadius()[0]*std::sin(angle));
					OutputImageType::RegionType outputRegion = outImage->GetLargestPossibleRegion();
					if( outputRegion.IsInside( localIndex ) ){
						outImage->SetPixel( localIndex, 255 );
					}
				}			
			}
			
			itCircles++;
		}
		if(numCircles<1){

			for ( outIt.GoToBegin() ; !outIt.IsAtEnd(); ++outIt )
			{
				outIt.Set(0);				
				if(outIt.IsAtEndOfLine()){
					outIt.NextLine();
				}

			}
			
			thresholdFilter->SetLowerThreshold(30);
			thresholdFilter->SetUpperThreshold(80);
			thresholdFilter->SetInsideValue(0);
			thresholdFilter->SetOutsideValue(255);
			thresholdFilter->SetInput(medianFilter->GetOutput());
			finalMaskImage= thresholdFilter->GetOutput();
			thresholdFilter->Update();
			
			numCircles = 0;
			counter = 0;
			vector<bool> shadowy (size[0]);
			

			for ( outIt.GoToBegin() ; !outIt.IsAtEnd(); ++outIt )
			{
				if(finalMaskImage->GetPixel(outIt.GetIndex())==0) outIt.Set(0);
				if(finalMaskImage->GetPixel(outIt.GetIndex())==255) outIt.Set(image->GetPixel(outIt.GetIndex()));
				//outIt.Set(finalMaskImage->GetPixel(outIt.GetIndex()));
				//if(outIt.GetIndex()[1] >= boundarypositions[outIt.GetIndex()[0]]-nIts) outIt.Set(0);
				if(outIt.IsAtEndOfLine() || outIt.GetIndex()[1] == boundarypositions[outIt.GetIndex()[0]] -nIts ){
					outIt.Set(0);
					outIt.NextLine();
				}

			}

			OutputImageType::IndexType outIndex;

			/*for ( outIt.GoToBegin() ; !outIt.IsAtEnd(); --outIt )
			{
				if(outIt.Get()!=0){
					shadowy[outIt.GetIndex()[0]]=false;
					outIt.NextLine();
					outIndex[0]=outIt.GetIndex()[0];
					outIndex[1]= boundarypositions[outIt.GetIndex()[0]]-nIts;
					outIt.SetIndex(outIndex);
					counter=0;
				}
				else{
					counter++;
					if(counter==15){
						shadowy[outIt.GetIndex()[0]]=true;
						outIt.NextLine();
						outIndex[0]=outIt.GetIndex()[0];
						outIndex[1]= boundarypositions[outIt.GetIndex()[0]]-nIts;
						outIt.SetIndex(outIndex);
						counter=0;
					}
				}

			}*/

			bool atShadow;


			for ( outIt.GoToBegin() ; !outIt.IsAtEnd(); --outIt )
			{
				if(!atShadow){
					if(outIt.Get()!=0){
						++counter;
						if(counter==10){
							counter=0;
							shadowy[outIt.GetIndex()[0]]=false;
							outIt.NextLine();
							outIndex[0]=outIt.GetIndex()[0];
							outIndex[1]= boundarypositions[outIt.GetIndex()[0]] - nIts -1;
							outIt.SetIndex(outIndex);
							if(outIt.Get()==0) atShadow= true; //if the first pixel is black we start next round as @shadow
							else{
								atShadow= false;
							}
						}
					}
					if(outIt.Get()==0){
						if(counter<10){
							counter=0;
							atShadow=true;
						}
						else{
							counter=0;
							shadowy[outIt.GetIndex()[0]]=false;
							outIt.NextLine();
							outIndex[0]=outIt.GetIndex()[0];
							outIndex[1]= boundarypositions[outIt.GetIndex()[0]]- nIts -1;
							outIt.SetIndex(outIndex);
							if(outIt.Get()==0) atShadow= true; //if the first pixel is black we start next round as @shadow
							else{
								atShadow= false;
							}
						}
					}
				}
				if(atShadow){
					if(outIt.Get()==0){
						++counter;
						if(counter>=2*nIts){
							counter=0;
							shadowy[outIt.GetIndex()[0]]=true;
							outIt.NextLine();
							outIndex[0]=outIt.GetIndex()[0];
							outIndex[1]= boundarypositions[outIt.GetIndex()[0]]- nIts -1;
							outIt.SetIndex(outIndex);
							if(outIt.Get()==0) atShadow= true; //if the first pixel is black we start next round as @shadow
							else{
								atShadow= false;
							}
						}
					}
					else{
							counter=0;
							shadowy[outIt.GetIndex()[0]]=false;
							outIt.NextLine();
							outIndex[0]=outIt.GetIndex()[0];
							outIndex[1]= boundarypositions[outIt.GetIndex()[0]]- nIts -1;
							outIt.SetIndex(outIndex);
							if(outIt.Get()==0) atShadow= true; //if the first pixel is black we start next round as @shadow
							else{
								atShadow= false;
							}
					}

					
				}
				
			}

			vector<OutputImageType::IndexType> indexFound;


			
			for ( outIt.GoToBegin() ; !outIt.IsAtEnd(); ++outIt ){
				if(shadowy[outIt.GetIndex()[0]] && numCircles< ncircles){
					//outIt.Set(150);
					if(image->GetPixel(outIt.GetIndex())>= 220 && outIt.GetIndex()[1]>3*bY && outIt.GetIndex()[1]< (boundarypositions[outIt.GetIndex()[0]]-nIts - 10) ){
						//indexFound.insert(indexIterator,outIt.GetIndex());
						indexFound.push_back(outIt.GetIndex());
						
						
						outIndex[0]=outIt.GetIndex()[0]+10;
						outIndex[1]= outIt.GetIndex()[1];
						outIt.SetIndex(outIndex);					
						outIt.NextLine();
						++numCircles;

					}
				}
				if(outIt.IsAtEndOfLine()|| outIt.GetIndex()[1]== boundarypositions[outIt.GetIndex()[0]]-nIts ){
					outIt.NextLine();
				}
			}

			if(numCircles>0){

				vector<OutputImageType::IndexType>::iterator indexIterator = indexFound.begin();
				OutputImageType::IndexType *previousIndex = &(*indexIterator);
			
				for(indexIterator = indexFound.begin(); indexIterator < indexFound.end(); indexIterator++ ){
					if(indexIterator != indexFound.begin()&& numCircles>1){
						itk::LineIterator<OutputImageType> it2(outImage, *previousIndex, *indexIterator);
						it2.GoToBegin();
						while (!it2.IsAtEnd())
						{
							it2.Set(255);
							++it2;
						}
						previousIndex = &(*indexIterator);

						
					}
					for(double angle = 0;angle <= 2*vnl_math::pi; angle += vnl_math::pi/60.0 )
							{
								localIndex[0] = (*indexIterator)[0] + 5*std::cos(angle);
								localIndex[1] = (*indexIterator)[1] + 5*std::sin(angle);
								OutputImageType::RegionType outputRegion = outImage->GetLargestPossibleRegion();
								if( outputRegion.IsInside( localIndex ) ){
									outImage->SetPixel( localIndex, 0 );
								}
							}
					}
			}


		}

	}

  




  
  writer->SetInput(outImage);
  writer->SetFileName( outputImage.c_str() );
  
  //  Software Guide : BeginLatex
  //
  //  Image types are defined below.
  //
  //  Software Guide : EndLatex


  
  // Software Guide : BeginCodeSnippet

  try
    {	
		writer->Update();

    }

  catch( itk::ExceptionObject & err )
    {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;

}
// Use an anonymous namespace to keep class types and function names
// from colliding when module is used as shared object module.  Every
// thing should be in an anonymous namespace except for the module
// entry point, e.g. main()
//
}
  int main( int argc, char * argv[] )
{
  PARSE_ARGS;

  itk::ImageIOBase::IOPixelType     pixelType;
  itk::ImageIOBase::IOComponentType componentType;

  try
    {
    itk::GetImageType(inputImage, pixelType, componentType);

    // This filter handles all types on input, but only produces
    // signed types
    switch( componentType )
      {
      case itk::ImageIOBase::UCHAR:
        return DoIt( argc, argv, static_cast<unsigned char>(0) );
        break;
      case itk::ImageIOBase::CHAR:
        return DoIt( argc, argv, static_cast<char>(0) );
        break;
      case itk::ImageIOBase::USHORT:
        return DoIt( argc, argv, static_cast<unsigned short>(0) );
        break;
      case itk::ImageIOBase::SHORT:
        return DoIt( argc, argv, static_cast<short>(0) );
        break;
      case itk::ImageIOBase::UINT:
        return DoIt( argc, argv, static_cast<unsigned int>(0) );
        break;
      case itk::ImageIOBase::INT:
        return DoIt( argc, argv, static_cast<int>(0) );
        break;
      case itk::ImageIOBase::ULONG:
        return DoIt( argc, argv, static_cast<unsigned long>(0) );
        break;
      case itk::ImageIOBase::LONG:
        return DoIt( argc, argv, static_cast<long>(0) );
        break;
      case itk::ImageIOBase::FLOAT:
        return DoIt( argc, argv, static_cast<float>(0) );
        break;
      case itk::ImageIOBase::DOUBLE:
        return DoIt( argc, argv, static_cast<double>(0) );
        break;
      case itk::ImageIOBase::UNKNOWNCOMPONENTTYPE:
      default:
        std::cout << "unknown component type" << std::endl;
        break;
      }
    }

  catch( itk::ExceptionObject & excep )
    {
    std::cerr << argv[0] << ": exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}


namespace lus{

	template <class L>

	int applyMask(int argc, char * argv[], itk::Image<unsigned short,2>::Pointer mImage, itk::Image<unsigned short,2>::Pointer rImage, L){

		PARSE_ARGS;

		typedef          L           InputPixelType;
		typedef          L           OutputPixelType;
  
		typedef itk::ImageFileReader<itk::Image<InputPixelType,2>> ReaderType;
		typedef itk::ImageLinearIteratorWithIndex<itk::Image<unsigned short,2>> contourIterator;
		typedef itk::ImageLinearIteratorWithIndex<itk::Image<InputPixelType,2>> inputIterator;

		ReaderType::Pointer inputReader = ReaderType::New();
		itk::Image<InputPixelType,2>::Pointer readImage;

		inputReader->SetFileName( inputImage.c_str() );
		try
		{
			inputReader->Update();
			readImage = inputReader->GetOutput();
		}
		catch ( itk::ExceptionObject &err)
		{
			 std::cout << "ExceptionObject caught a !" << std::endl;
			std::cout << err << std::endl;
			return -1;
		}

	
		inputIterator iIt(readImage, readImage->GetRequestedRegion());
		iIt.GoToBegin();

		contourIterator mIt(mImage, mImage->GetRequestedRegion());
		mIt.GoToBegin();

		contourIterator rIt(rImage, rImage->GetRequestedRegion());
		rIt.GoToBegin();

		for ( iIt.GoToBegin(),  mIt.GoToBegin(),  rIt.GoToBegin(); ! iIt.IsAtEnd(); ++iIt, ++mIt, ++rIt )
		{
			
			if(mIt.Get()!=0) rIt.Set(iIt.Get());
			
		}

	
		return 0;

	}
	
	template <class J>

	int getImagesRegionOfInterest(int argc, char * argv[], itk::Image<unsigned short,2>::Pointer mask, itk::ImageRegion<2> region, int threshold, J){

		PARSE_ARGS;

		typedef          J           InputPixelType;
		typedef          J           OutputPixelType;
  
		typedef itk::ImageFileReader<itk::Image<InputPixelType,2>> ReaderType;
		typedef itk::ImageLinearIteratorWithIndex<itk::Image<unsigned short,2>> contourIterator;
		typedef itk::ImageLinearIteratorWithIndex<itk::Image<InputPixelType,2>> inputIterator;

		ReaderType::Pointer inputReader = ReaderType::New();
		itk::Image<unsigned short,2>::IndexType actual;
		itk::Image<InputPixelType,2>::Pointer readImage;

		inputReader->SetFileName( inputImage.c_str() );
		try
		{
			inputReader->Update();
			readImage = inputReader->GetOutput();
		}
		catch ( itk::ExceptionObject &err)
		{
			 std::cout << "ExceptionObject caught a !" << std::endl;
			std::cout << err << std::endl;
			return -1;
		}

		inputIterator aIt(readImage,region);
		contourIterator bIt(mask,region);

		aIt.SetDirection(1);
		bIt.SetDirection(1);



		for ( aIt.GoToBegin(),  bIt.GoToBegin(); ! aIt.IsAtEnd(); ++aIt, ++bIt )
		{

			if (aIt.Get() >= threshold){
				actual=bIt.GetIndex();
			}

			if(aIt.IsAtEndOfLine()){
		
				//whenever the limit is detected we still give some space to ensure keeping the complete contour
				//actual[1]= actual[1]+3;
				for(bIt.GoToBeginOfLine(); ! bIt.IsAtEndOfLine() ;++bIt){
					if( actual[1]!= region.GetIndex()[1])bIt.Set(255);
					if( bIt.GetIndex()[1]==actual[1]+3){
						bIt.GoToEndOfLine();
						//bIt.Set(255);
					
					}
				}
				aIt.NextLine();
				bIt.NextLine();
				actual=aIt.GetIndex();
			}
		

		}

		return 0;


	}



	int nonContourDetectedObtainMask(itk::Image<unsigned short,2>::Pointer mask, int length){

		typedef itk::Image<unsigned short,2> maskImageType;
		typedef itk::ImageLinearIteratorWithIndex<maskImageType> maskImageIterator;

		maskImageType::RegionType desiredRegion = mask->GetLargestPossibleRegion();

		maskImageType::IndexType start;
		start[0] = 0 ;
		start[1] = 0 ;

		maskImageType::SizeType size;
		size[0] = desiredRegion.GetSize()[0] ;
		size[1] = length;

		maskImageType::RegionType maskRegion;
		maskRegion.SetSize(  size  );
		maskRegion.SetIndex( start );

		int* limits = new int[desiredRegion.GetSize()[0]];

		maskImageIterator maskImageIt(mask,maskRegion);
		maskImageIt.SetDirection(1);
		
		int towhere=0;
		int count=0;
		int column=0;

		for(maskImageIt.GoToBegin() ;!maskImageIt.IsAtEnd() ; ++maskImageIt ){
			if (maskImageIt.Get()!=0){
				towhere=maskImageIt.GetIndex()[1];
				maskImageIt.Set(0);
			}

			if(maskImageIt.IsAtEndOfLine()){
				limits[column]=towhere;
				++column;
				++count;
				if(count==15){
					towhere=0;
					count=0;
					for(int x=0; x<15; x++){
						if( limits[column - 14 + x] >= towhere) towhere = limits[column - x];
					}
					for(int y=0; y<15; y++){
						limits[column - 14 + y] = towhere;
					}
				}

				maskImageIt.NextLine();
				towhere=0;
			}
		}


		column = 0;
		maskImageIterator maskFillerIt(mask,maskRegion);
		maskFillerIt.SetDirection(1);

		for(maskFillerIt.GoToBegin() ;!maskFillerIt.IsAtEnd() ; ++maskFillerIt ){
			maskFillerIt.Set(255);
			if (maskFillerIt.GetIndex()[1]== limits[column]){
				maskFillerIt.NextLine();
				++column;
			}
			if (maskFillerIt.IsAtEndOfLine()){
				maskFillerIt.NextLine();
				++column;
			}

		}

		return 0;

	}


	/**
	ContourDetection is a method adeccuated to extract the boundaries of the image and return it as a mask to be applied over the original image
	In addition to that, the vertical axis values associated with the contour are updated through the boundaries parameter in order to be able to
	erase that contour if neccesary by substracting a value to the positions supposed to be the width of it.
	Length parameter is referred to the horizontal axis size of the image.

	*/

	vector<int> contourDetection(itk::Image<unsigned short,2>::Pointer mask, int length, vector<int> boundaries){

		typedef itk::Image<unsigned short,2> maskImageType;
		typedef itk::ImageLinearIteratorWithIndex<maskImageType> maskImageIterator;

		maskImageType::RegionType desiredRegion = mask->GetLargestPossibleRegion();
		

		maskImageType::IndexType start;
		start[0] = 0 ;
		start[1] = 0 ;

		maskImageType::SizeType size;
		size[0] = desiredRegion.GetSize()[0] ;
		size[1] = length;

		maskImageType::RegionType maskRegion;
		maskRegion.SetSize(  size  );
		maskRegion.SetIndex( start );

		int* limits = new int[desiredRegion.GetSize()[0]];

		maskImageIterator maskImageIt(mask,maskRegion);
		maskImageIt.SetDirection(1);
		
		int towhere=0;
		int count=0;
		int column=0;
		vector<int> vpos(size[0]);
		bool atBorder = false;
		vector<bool> isBorder(size[0]);

		for(maskImageIt.GoToBegin() ;!maskImageIt.IsAtEnd() ; ++maskImageIt ){
			if(maskImageIt.Get()!=0){
				vpos[maskImageIt.GetIndex()[0]]=maskImageIt.GetIndex()[1];
				maskImageIt.Set(0);

			}

			if(maskImageIt.IsAtEndOfLine()){
				if(vpos[maskImageIt.GetIndex()[0]]==0){
					isBorder[maskImageIt.GetIndex()[0]]= false;
					atBorder=false;

				}

				else{
					if(!atBorder){
						if(abs(vpos[maskImageIt.GetIndex()[0]] - vpos[maskImageIt.GetIndex()[0]-1])<5){
							++count;
							isBorder[maskImageIt.GetIndex()[0]]= false;
						}
						if(count == 30){
							for(int c=0; c<30; c++) {
								isBorder[maskImageIt.GetIndex()[0]-c]= true;
								count--;
							}
							atBorder=true;								
						}

						if(abs(vpos[maskImageIt.GetIndex()[0]] - vpos[maskImageIt.GetIndex()[0]-1])>5){
							isBorder[maskImageIt.GetIndex()[0]]= false;
							count=0;

						}
						
					}
					else{
						if(abs(vpos[maskImageIt.GetIndex()[0]] - vpos[maskImageIt.GetIndex()[0]-1])<5){
							isBorder[maskImageIt.GetIndex()[0]]= true;

						}
						if(abs(vpos[maskImageIt.GetIndex()[0]] - vpos[maskImageIt.GetIndex()[0]-1])>5){
							isBorder[maskImageIt.GetIndex()[0]]= false;
							atBorder= false;
							count=0;
						}						
					}				
				}
				maskImageIt.NextLine();		
			}
		}

		maskImageIterator maskFillerIt(mask,maskRegion);
		maskFillerIt.SetDirection(1);

		

		for(maskFillerIt.GoToBegin() ;!maskFillerIt.IsAtEnd() ; ++maskFillerIt ){
			if(vpos[maskFillerIt.GetIndex()[0]]==0) maskFillerIt.NextLine();
			else{
				maskFillerIt.Set(255);
				if(!isBorder[maskFillerIt.GetIndex()[0]]) maskFillerIt.Set(0);
				if (maskFillerIt.GetIndex()[1]== vpos[maskFillerIt.GetIndex()[0]]){
					maskFillerIt.NextLine();
				}
				if (maskFillerIt.IsAtEndOfLine()){
					maskFillerIt.NextLine();
				}

			}
		}

		maskImageType::IndexType corner1 = desiredRegion.GetIndex();
		maskImageType::IndexType corner2;
		atBorder=false;

		for(int col=0; col<size[0]; col++){
			if(atBorder){
				if(isBorder[col]==false){
					atBorder=false;
					column=col;
				}
			}
			if(!atBorder){
				if( col== size[0]-1){
					corner1[0] = column;
					corner1[1] = vpos[column-1];					
					corner2[0] = col;
					corner2[1] = vpos[column-1];
 
					itk::LineIterator<maskImageType> it2(mask, corner1, corner2);
					it2.GoToBegin();
					while (!it2.IsAtEnd())
					{
						it2.Set(255);
						++it2;
					}
				}
				if(isBorder[col]==true ){
					if(column==0){
						corner1[0]=0;
						corner1[1] = vpos[0];
					}
					else{
						corner1[0] = column-1;
						corner1[1] = vpos[column-1];
					}
					corner2[0] = col;
					corner2[1] = vpos[col];
 
					itk::LineIterator<maskImageType> it1(mask, corner1, corner2);
					it1.GoToBegin();
					while (!it1.IsAtEnd())
					{
						it1.Set(255);
						++it1;
					}
					atBorder=true;

				}
			}			
		}
		maskImageIterator finalMaskIt(mask,maskRegion);
		finalMaskIt.SetDirection(1);

		bool nextLine = false;

		for(finalMaskIt.GoToBegin() ;!finalMaskIt.IsAtEnd() ; ++finalMaskIt ){
			if(nextLine){
				finalMaskIt.Set(255);
				if(finalMaskIt.GetIndex()[1] == vpos[finalMaskIt.GetIndex()[0]]){
					nextLine = false;
					finalMaskIt.NextLine();

				}
			}
			else{
				if(finalMaskIt.Get()==255){
					vpos[finalMaskIt.GetIndex()[0]] = finalMaskIt.GetIndex()[1];
				}
				if(finalMaskIt.IsAtEndOfLine()){
					finalMaskIt.GoToBeginOfLine();
					nextLine=true;
				}
			}


			
		}
		
		return vpos;

		

	}

	int ignoreBoundaries (itk::Image<unsigned short,2>::Pointer mask, int length, vector<int> boundaries, int boundSize){
		typedef itk::Image<unsigned short,2> maskImageType;
		typedef itk::ImageLinearIteratorWithIndex<maskImageType> maskImageIterator;

		maskImageType::RegionType desiredRegion = mask->GetLargestPossibleRegion();
		maskImageType::IndexType start;
		start[0] = 0 ;
		start[1] = 0 ;

		maskImageType::SizeType size;
		size[0] = desiredRegion.GetSize()[0] ;
		size[1] = length;

		maskImageType::RegionType maskRegion;
		maskRegion.SetSize(  size  );
		maskRegion.SetIndex( start );

		maskImageIterator maskImageIt(mask,maskRegion);
		maskImageIt.SetDirection(1);
		for(maskImageIt.GoToBegin() ;!maskImageIt.IsAtEnd() ; ++maskImageIt ){
			if (maskImageIt.IsAtEndOfLine()){
				maskImageIt.NextLine();
			}
			if(maskImageIt.GetIndex()[1]< (boundaries[maskImageIt.GetIndex()[0]] - boundSize) ) maskImageIt.Set(255);
			else{
				maskImageIt.Set(0);
			}

		}

		return 0;



	}

	vector<vector<int>> createDiamondStructuringElement ( int side){

		int i=0, j=0, NUM= side/2; 
		//we design the int matrix that will be used to create de Structuring Element
		vector<vector<int>> diamondStructure(NUM+NUM+1, vector<int>(NUM+NUM+1));
 
		for(i=-NUM; i<=NUM; i++){
			for(j=-NUM; j<=NUM; j++){
				if( abs(i)+abs(j)<=NUM){
					diamondStructure[NUM+i][NUM+j] = 1;
				}
				else { 
					diamondStructure[NUM+i][NUM+j] = 0;
				}
			}
		}

		return diamondStructure;


	}


  }
