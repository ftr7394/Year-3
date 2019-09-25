/** Implementation file for image quantization
 *
 *  \file ipcv/quantize/quantize.cpp
 *  \author <Frank Rossi> ftr7394@rit.edu
 *  \date 9/4/19
 */

#include "Quantize.h"

#include <iostream>

using namespace std;

/** Perform uniform grey-level quantization on a color image
 *
 *  \param[in] src                 source cv::Mat of CV_8UC3
 *  \param[in] quantization_levels the number of levels to which to quantize
 *                                 the image
 *  \param[out] dst                destination cv:Mat of CV_8UC3
 */
void Uniform(const cv::Mat& src, const int quantization_levels, cv::Mat& dst) {
/** I intially set two variables
 *
 * beg which holds the values from the image
 *
 * fin which becomes the value that I put in my destination matrix or the final 
 * values after uniform quantization
 *
 *Additionally, I set denom as a double which is used to find the size of the
 *quantization level
 */
	int beg;
	int fin;
	double denom = 256/quantization_levels;
	//The first for loop is used to loop through the channels of the image
	for (int chan = 0; chan < src.channels(); chan++){
		//The for loops below loop through each row and column of the img
		for (int rs = 0; rs < src.rows; rs++){
			for(int cl = 0; cl < src.cols; cl++){
				//The code below is the bulk of the quantizing
				beg = src.at<cv::Vec3b>(rs,cl)[chan];
			 	fin = beg/denom;
				dst.at<cv::Vec3b>(rs,cl)[chan] = fin;
		 }
}
}
}
/** Perform improved grey scale quantization on a color image
 *
 *  \param[in] src                 source cv::Mat of CV_8UC3
 *  \param[in] quantization_levels the number of levels to which to quantize
 *                                 the image
 *  \param[out] dst                destination cv:Mat of CV_8UC3
 */
void Igs(const cv::Mat& src, const int quantization_levels, cv::Mat& dst) {
	//The beg, fin, and denom variables have the same purpose in this function
	//
	//maxdig is used to make sure that after adding the remainder to the initial
	//values, the values do not pass the max digital count
	//
	//remainder is used to calulate the remainder
	int beg;
	int fin;
	int denom = 256/quantization_levels;
	int maxdig;
	int remainder = 0;
	//The for loops below are the same from the last function
	for (int chan = 0; chan < src.channels(); chan++){
		for (int rs = 0; rs < src.rows; rs++){
			for(int cl = 0; cl < src.cols; cl++){
				//Once again we use beg to hold our values from the image
				//we also check to make sure no values go past the max 
				//digital count and if it does,the code sets the digital
				//count to 255
				beg = src.at<cv::Vec3b>(rs,cl)[chan];
				maxdig = beg + remainder;
				remainder = beg % denom;
				if (maxdig > 255){
					maxdig = 255;
				}
			 	fin = maxdig/denom;
				dst.at<cv::Vec3b>(rs,cl)[chan] = fin;
		 }
}
}
}

namespace ipcv {

bool Quantize(const cv::Mat& src, const int quantization_levels,
              const QuantizationType quantization_type, cv::Mat& dst) {
  dst.create(src.size(), src.type());

  switch (quantization_type) {
    case QuantizationType::uniform:
      Uniform(src, quantization_levels, dst);
      break;
    case QuantizationType::igs:
      Igs(src, quantization_levels, dst);
      break;
    default:
      cerr << "Specified quantization type is unsupported" << endl;
      return false;
  }

  return true;
}
}
