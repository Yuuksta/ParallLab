#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <malloc.h>
#include <omp.h>
#include <sys/time.h>

using namespace cv;
using namespace std;

int main (int argc, char** argv) {
    int i,j;
    float deltaT;
    struct timeval t1, t2;
    IplImage *img = cvLoadImage(argv[1], 0);
    char *tempData = (char *)malloc(sizeof(img->imageData) * img->width * img->height);

    //make picture binary
    #pragma omp parallel for
    for(i = 0; i < img->height; i++) {
        for(j = 0; j < img->width; j++) {
            if(img->imageData[i * img->width + j] > 0 ) 
                img->imageData[i * img->width + j] = 0; //set black
            else img->imageData[i * img->width + j] = -1; //set white
        }
    }
    cvShowImage("show", img);
    cvWaitKey(0);
    
    //start timer
    gettimeofday(&t1, NULL);
    
    //erosion
    #pragma omp parallel for
    for(i = 1; i < img->height; i++) {
        for(j = 1; j < img->width; j++) {
            //start processing
            //printf("process unit: %d\n", i * img->width + j);
            if(img->imageData[i * img->width + j] == 0 &&
                img->imageData[(i - 1) * img->width + j] == 0 &&
                img->imageData[i * img->width + j - 1] == 0) {
                tempData[i * img->width + j] = 0;        
            }
            else tempData[i * img->width + j] = -1;
        }
    }
    //stop timer
    gettimeofday(&t2, NULL);
    deltaT = (t2.tv_sec-t1.tv_sec) * 1000000 + t2.tv_usec-t1.tv_usec;
    deltaT = deltaT / 1000000;
    printf("time elapsed: %f s\n", deltaT);
    img->imageData = tempData;
    cvShowImage("show", img);
    cvWaitKey(0);
    cvReleaseImage(&img);
    cvDestroyAllWindows();
}