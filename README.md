#include <list>

#include <opencv2/opencv.hpp>

#include "utilities.h"
#include "bow.h"
#include "Classifier.h"

using namespace cv;

#define train1 "c:/Temp/Zhorin/data/train/indoor-dwelling-train"
#define train2 "c:/Temp/Zhorin/data/train/indoor-msu-train"
#define train3 "c:/Temp/Zhorin/data/train/outdoor-blossoms-train"
#define train4 "c:/Temp/Zhorin/data/train/outdoor-urban-train"
#define test1 "c:/Temp/Zhorin/data/test/indoor-dwelling-test" 
#define test2 "c:/Temp/Zhorin/data/test/indoor-msu-test" 
#define test3 "c:/Temp/Zhorin/data/test/outdoor-blossoms-test"
#define test4 "c:/Temp/Zhorin/data/test/outdoor-urban-test"
#define vocsize 100


int main(int argc, char **argv)
{
  int te1 = 0, te2 = 0, te3 = 0, te4 = 0, tr1 = 0, tr2 = 0, tr3 = 0, tr4 = 0;
  std::vector<string> filesList;
  GetFilesInFolder(train1, filesList, tr1);
  GetFilesInFolder(train2, filesList, tr2);
  GetFilesInFolder(train3, filesList, tr3);
  GetFilesInFolder(train4, filesList, tr4);
  GetFilesInFolder(test1, filesList, te1);
  GetFilesInFolder(test2, filesList, te2);
  GetFilesInFolder(test3, filesList, te3);
  GetFilesInFolder(test4, filesList, te4);
  
  int sum = tr1 + te1 + te2 + tr2 + te3 + tr3 + te4 + tr4;
  int trsum = tr1 + tr2  + tr3 + tr4;
  int tesum = te1 + te2 +  te3 + te4;
  std::vector<Mat> descriptors;
  std::vector<Mat> imgDesc;
  Mat testSamples(tesum, vocsize, CV_32F);
  Mat trainSamples(trsum, vocsize, CV_32F);
 /* Mat trainSampleMask(1, trsum + tesum, CV_8U);
  for (int i = 0; i < trsum; ++i) {
    trainSampleMask.at<int>(i) = 1;
  }
  for (int i = trsum; i < tesum + trsum; ++i) {
    trainSampleMask.at<int>(i) = 0;
  }*/
  std::vector<std::vector<KeyPoint>> keypoints;
  keypoints.resize(sum);
  descriptors.resize(sum);
  int i = 0;
  Mat voc;
  while (i < sum)
  {
    DetectKeypointsOnImage(filesList[i], keypoints[i], descriptors[i]);
    i++;
  }
  printf("DetectKeypointsOnImage\n");
  i = 0;
  voc = BuildVocabulary(descriptors, vocsize, tr1 + tr2 + tr3 + tr4);
  printf("BuildVocabulary\n");
  while (i < trsum)
  {
    ComputeImgDescriptor(filesList[i], voc, trainSamples.row(i));
    i++;
  }
  int j = 0;
    while (i < sum)
  {
    ComputeImgDescriptor(filesList[i], voc, testSamples.row(j));
    i++;
	j++;
  }
  //samples.copyTo(trainSamples,trainSampleMask);
   printf("ComputeImgDescriptor\n");
  Mat trainLabels(trsum, 1, CV_32S);
  Mat testLabels(tesum, 1, CV_32S);
  int ind = 0;
  for (int j = 0 ; j < tr1; j++, ind++)
    trainLabels.at<int>(ind) = 1;
  for (int j = 0; j < tr2; j++, ind++)
    trainLabels.at<int>(ind) = 2;
  for (int j = 0; j < tr3; j++, ind++)
    trainLabels.at<int>(ind) = 3;
  for (int j = 0; j < tr4; j++, ind++)
    trainLabels.at<int>(ind) = 4;
  ind = 0;
  for (int j = 0; j < te1; j++, ind++)
    testLabels.at<int>(ind) = 1;
  for (int j = 0; j < te2; j++, ind++)
    testLabels.at<int>(ind) = 2;
  for (int j = 0; j < te3; j++, ind++)
    testLabels.at<int>(ind) = 3;
  for (int j = 0; j < te4; j++, ind++)
    testLabels.at<int>(ind) = 4;
   printf("labels\n");
  CvRTrees rf;
  
  /*Mat trainSampleMask(1, trsum + tesum, CV_8U);
  for (int i = 0; i < trsum; ++i) {
    trainSampleMask.at<int>(i) = 1;
  }
  for (int i = trsum; i < tesum + trsum; ++i) {
    trainSampleMask.at<int>(i) = 0;
  }*/
/*   Mat trainSampleMask(1, trsum, CV_32S);
  for (int i = 0; i < trsum; ++i) {
    trainSampleMask.at<int>(i) = i;
  }*/
  CvRTParams params = CvRTParams(); 
  //params.max_depth = 5;
 // params.min_sample_count = 0; 
  //params.calc_var_importance = false; 
  params.term_crit.type = CV_TERMCRIT_ITER;
  params.term_crit.max_iter = 10;
  //Mat varIdx(1, vocsize + 1, CV_8U, Scalar(1));
  //varIdx.at<int>(vocsize) = 0;
  Mat varTypes(1, vocsize + 1, CV_8UC1);
  for (int i = 0; i < vocsize; i++) {    
    varTypes.at<uchar>(i) = CV_VAR_ORDERED;
  }
  varTypes.at<uchar>(vocsize) = CV_VAR_CATEGORICAL;
  rf.train(trainSamples, CV_ROW_SAMPLE, trainLabels, Mat(), Mat(), varTypes, Mat(), params);
  printf("TrainClassifier\n");
  //rf = TrainClassifier(samples, labels, trsum, tesum, vocsize);
  rf.save("model-rf.yml", "simpleRTreesModel");
   printf("save\n");
  float trainError = 0.0f;
  for (int i = 0; i < trsum; ++i) {
    int prediction = (int)(rf.predict(trainSamples.row(i))); 
	printf("%i ",prediction);
    trainError += (trainLabels.at<int>(i) != prediction); 
  } 
  trainError /= float(trsum);
  printf("\n");
  float testError = 0.0f; 
  for (int i = 0; i <tesum; ++i) {
    int prediction = (int)(rf.predict(testSamples.row(i)));
	printf("%i ",prediction);
    testError += (testLabels.at<int>(i) != prediction);
  }   
  printf("\n");
  testError /= float(sum - trsum);
    printf("train error = %.4f\ntest error = %.4f\n", trainError, testError);
  return 0;
