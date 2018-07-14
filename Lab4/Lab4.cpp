#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <malloc.h>
#include <sys/time.h>
#include <mpich/mpi.h>

using namespace cv;
using namespace std;

int main (int argc, char** argv) {
    int i, j, rank, size, receiveTag, rankCount;
    float deltaT;
    IplImage *img = cvLoadImage(argv[1], 0);
    char *tempData = (char *)malloc(sizeof(img->imageData) * img->width * img->height);
    char *destData = (char *)malloc(sizeof(img->imageData) * img->width * img->height);
    struct timeval t1, t2;

    //make picture binary
    for(i = 0; i < img->height; i++) {
        for(j = 0; j < img->width; j++) {
            if(img->imageData[i * img->width + j] > 0 ) img->imageData[i * img->width + j] = 0; //set black
            else img->imageData[i * img->width + j] = -1; //set white
        }
    }
    cvShowImage("show", img);
    //cvWaitKey(0);
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //erosion
    if(rank == size - 1) { //final process
        //start timer
        gettimeofday(&t1, NULL);
        //receive
        rankCount = 0;
        while(1) {
            MPI_Recv(tempData, img->height * img->width, MPI_CHAR, MPI_ANY_SOURCE   , receiveTag, MPI_COMM_WORLD,);
            if(receiveTag  >= 0 && receiveTag <= 8) {
                for(i = img->height * receiveTag ? 1 : (img->height * receiveTag) / (size - 1); i < (img->height * (receiveTag + 1)) / (size - 1); i++) {
                    for(j = 1; j < img->width; j++) {    
                        destData[i * img->width + j] = tempData[i * img->width + j];
                    }    
                }
                rankCount++;
            }
            if(rankCount == size - 1) break;
        }
    }
    else { //process different part of the picture by rank
        for(i = img->height * rank ? 1 : (img->height * rank) / (size - 1); i < (img->height * (rank + 1)) / (size - 1); i++) {
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
        //send processed data
        MPI_Send(tempData,img->height * img->width, MPI_CHAR, size - 1, rank, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    
    if(rank == size - 1) {
        //stop timer
        gettimeofday(&t2, NULL);
        deltaT = (t2.tv_sec-t1.tv_sec) * 1000000 + t2.tv_usec-t1.tv_usec;
        deltaT = deltaT / 1000000;
        printf("time elapsed: %f s\n", deltaT);
        img->imageData = destData;
        cvShowImage("show", img);
        cvWaitKey(0);
        cvReleaseImage(&img);
        cvDestroyAllWindows();
    }    
}

