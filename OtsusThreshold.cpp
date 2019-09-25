/** Implementation file for finding Otsu's threshold
 *
 *  \file ipcv/otsus_threshold/OtsusThreshold.cpp
 *  \author <Frank Rossi> (ftr7394@rit.edu)
 *  \date 09/17/19
 */

#include <cmath>

#include "OtsusThreshold.h"

#include <iostream>

#include "imgs/ipcv/utils/Utils.h"

using namespace std;

namespace ipcv {

/** Find Otsu's threshold for each channel of a 3-channel (color) image
 *
 *  \param[in] src          source cv::Mat of CV_8UC3
 *  \param[out] threshold   threshold values for each channel of a 3-channel
 *                          color image in cv::Vec3b
 */
bool OtsusThreshold(const cv::Mat& src, cv::Vec3b& threshold) {
  threshold = cv::Vec3b();
  /** I started by using the functions from the past project to make
   * the histogram of the src, and then turn that histogram into a CDF
   * and PDF, which are used in otsu's formula.
   */
  cv::Mat_<int> hist;
  ipcv::Histogram(src, hist);

  cv::Mat_<double> cdf;
  ipcv::HistogramToCdf(hist, cdf);

  cv::Mat_<double> pdf;
  ipcv::HistogramToPdf(hist, pdf);

  /** The vector mu is the mu(k) = Summation from 1-dc of i pi
   *  The vector thresh is used to hold the threshold values
   *
   *  The variable muk is used to hold the values which are then put
   *  into the vector mu
   *
   *  The variable numer and denom are used to calculate the main
   *  formula. I just thought that breaking up the equation into two
   *  pieces would be easier on the eyes and easier to work through.
   *
   *  The variable means is equation (18) from otsu's paper
   */
  vector<double> mu;
  vector<double> thresh;

  double muk;
  double numer;
  double denom;
  double means;
  for (int r = 0; r < pdf.rows; r++) {
    for (int dc = 0; dc < pdf.cols; dc++) {
      // I iterated through the rows and columns of the pdf, then
      // I used muk to calculate the values that are pushed into
      // vector mu or line (7) in otsu's paper. This column loop
      // is seperate from the other because we need to create the
      // whole vector before we do any math.
      muk = muk + (dc * pdf.at<double>(r, dc));
      mu.push_back(muk);
    }
    for (int dc = 0; dc < pdf.cols; dc++) {
      // In this loop is formula (18) from otsu's paper. Then the if
      // statement check if any of our values are not avaiable and sets
      // the value to 0 if it is.
      numer = pow(((mu[255] * cdf.at<double>(r, dc))) - (mu[dc]), 2);
      denom = (cdf.at<double>(r, dc) * (1 - cdf.at<double>(r, dc)));
      means = numer / denom;
      if (isnan(means) == true) {
        means = 0;
      }
      thresh.push_back(means);
    }
    // Used to find the max index of the columns, which would be our goodness
    // values. Then we take the max our those goodness values to find the right
    // threshold. It is then set as the threshold.
    int idx[1];
    cv::minMaxIdx(thresh, NULL, NULL, NULL, idx);
    threshold[r] = idx[1];
  }
  return true;
}
}
