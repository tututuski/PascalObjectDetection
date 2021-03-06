#ifndef FEATURE_H
#define FEATURE_H

#include "Common.h"
#include "PascalImageDatabase.h"
#include "ParametersMap.h"

using namespace cv;

typedef std::vector<float> Feature;
typedef std::vector<Feature> FeatureCollection;

//! Feature Extraction Class
/*!
    This class implements an abstract feature extractor, 
    it receives a database of images and return a vector of features obtained after the image processing.
*/

class FeatureExtractor
{
public:
    //! Constructor
    FeatureExtractor(const ParametersMap &params = ParametersMap()) {};

    //! Destructor
    virtual ~FeatureExtractor() {};
    virtual ParametersMap getParameters() const = 0;
    virtual std::string getFeatureType() const = 0;

    // Extract feature vector for image image. Decending classes must implement this method
    //void operator()(const Mat &image, Feature &feat) const;

    // Extract feature vector for image image. Decending classes must implement this method
    virtual void operator()(Mat &image, Feature &feat) const = 0;

    // Extracts descriptor for each image in the database, stores result in FeatureCollection,
    // this is used for training the support vector machine.
    void operator()(const PascalImageDatabase &db, FeatureCollection &featureCollection) const;

    void scale(FeatureCollection &featureCollection,  FeatureCollection &scaledFeatureCollection);

    // Extracts descriptor for each level of imPyr and stores the results in featPyr
    void operator()(const std::vector<Mat> &imPyr, FeatureCollection &featPyr) const;

    // Ratio of input image size to output response size (necessary when computing location of detections)
    virtual double scaleFactor() const = 0;

    // Factory method that allocates the correct feature vector extractor given
    // the name of the extractor (caller is responsible for deallocating
    // extractor). To extend the code with other feature extractors or
    // other configurations for feature extractors add appropriate constructor
    // calls in implementation of this function.
    static FeatureExtractor *create(ParametersMap params = ParametersMap());
    static FeatureExtractor *create(const std::string &featureType, const ParametersMap &params = ParametersMap());

    static void save(FILE *f, const FeatureExtractor *feat);
    static FeatureExtractor *load(FILE *f);

    // TODO document this
    static ParametersMap getDefaultParameters(const std::string &featureType);
};

//! HOG Feature Extraction Class
/*!
    This class implements a HOG feature extractor, 
    it receives a database of images and return a vector of HOG features obtained after the image processing.
    This is an inherited class from the FeatureExtractor class.
*/

class HOGFeatureExtractor : public FeatureExtractor
{
private:
    int _nAngularBins;                    // Number of angular bins
    bool _unsignedGradients;              // If true then we only consider the orientation modulo 180 degrees (i.e., 190
    // degrees is considered the same as 10 degrees)
    int _cellSize;                        // Support size of a cell, in pixels

    //HOGDescriptor _hog;

public:
    std::string getFeatureType() const { return "hog"; };

    static ParametersMap getDefaultParameters();
    ParametersMap getParameters() const;

    //HOGFeatureExtractor(int nAngularBins = 18, bool unsignedGradients = true, int cellSize = 6);
    //! Constructor
    HOGFeatureExtractor(const ParametersMap &params = ParametersMap());

    void operator()(Mat &image, Feature &feat) const;

    Mat renderHOG(Mat& img, Mat& out, vector<float>& descriptorValues, Size winSize, Size cellSize, int scaleFactor, double viz_factor) const;

    double scaleFactor() const { return 1.0 / double(_cellSize); }

};

#endif // FEATURE_H
