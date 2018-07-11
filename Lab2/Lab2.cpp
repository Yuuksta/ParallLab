#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <malloc.h>
#include <pthread.h>
#include <sys/time.h>

using namespace cv;
using namespace std;

typedef struct THREAD_HANDLE_T {
    char *originData;
    char *destData;
    int height;
    int width;
} THREAD_HANDLE_T;

void *process1(void *arg);
void *process2(void *arg);
void *process3(void *arg);
void *process4(void *arg);

int main (int argc, char** argv) {
    int i,j;
    float deltaT;
    struct timeval t1, t2;
    //load image
    IplImage *img = cvLoadImage(argv[1], 0);
    char *destData = (char *)malloc(sizeof(img->imageData) * img->width * img->height);
    THREAD_HANDLE_T *arg = (THREAD_HANDLE_T *)malloc(sizeof(THREAD_HANDLE_T));
    pthread_t tid1, tid2, tid3, tid4;
    //make picture binary
    for(i = 0; i < img->height; i++) {
        for(j = 0; j < img->width; j++) {
            if(img->imageData[i * img->width + j] > 0 ) 
                img->imageData[i * img->width + j] = 0; //set black
            else img->imageData[i * img->width + j] = -1; //set white
        }
    }
    //show origin binary image
    cvShowImage("show", img);
    cvWaitKey(0);

    //start timer
    gettimeofday(&t1, NULL);

    //initialize args
    arg->originData = img->imageData;
    arg->destData = destData;
    arg->height = img->height;
    arg->width = img->width;
    printf("image height: %d\n", img->height);
    printf("image width: %d\n", img->width);

    //create threads
    if(pthread_create(&tid1, NULL, process1, arg)) {
        printf("create pthread error!\n");
        return -1;
    }
    if(pthread_create(&tid2, NULL, process2, arg)) {
        printf("create pthread error!\n");
        return -1;
    }
    if(pthread_create(&tid3, NULL, process3, arg)) {
        printf("create pthread error!\n");
        return -1;
    }
    if(pthread_create(&tid4, NULL, process4, arg)) {
        printf("create pthread error!\n");
        return -1;
    }
    
    //pthread join
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);

    img->imageData = destData;

    //stop timer
    gettimeofday(&t2, NULL);
    deltaT = (t2.tv_sec-t1.tv_sec) * 1000000 + t2.tv_usec-t1.tv_usec;
    deltaT = deltaT / 1000000;
    printf("time elapsed: %f s\n", deltaT);

    //show erosioned image
    cvShowImage("show", img);
    cvWaitKey(0);
    cvReleaseImage(&img);
    cvDestroyAllWindows();
    return 0;
}

void *process1(void *arg) {
    THREAD_HANDLE_T *args = (THREAD_HANDLE_T *)arg;
    int i,j;
    for(i = 1; i < args->height/4; i++) {
        for(j = 1; j < args->width; j++) {
            //start processing
            //printf("process1 unit: %d\n", i * args->width + j);
            if(args->originData[i * args->width + j] == 0 &&
                args->originData[(i - 1) * args->width + j] == 0 &&
                args->originData[i * args->width + j - 1] == 0) {
                args->destData[i * args->width + j] = 0;        
            }
            else args->destData[i * args->width + j] = -1;
        }
    }
}

void *process2(void *arg) {
    THREAD_HANDLE_T *args = (THREAD_HANDLE_T *)arg;
    int i,j;
    for(i = args->height / 4; i < args->height / 2; i++) {
        for(j = 1; j < args->width; j++) {
            //start processing
            //printf("process2 unit: %d\n", i * args->width + j);
            if(args->originData[i * args->width + j] == 0 &&
                args->originData[(i - 1) * args->width + j] == 0 &&
                args->originData[i * args->width + j - 1] == 0) {
                args->destData[i * args->width + j] = 0;        
            }
            else args->destData[i * args->width + j] = -1;
        }
    }
}

void *process3(void *arg) {
    THREAD_HANDLE_T *args = (THREAD_HANDLE_T *)arg;
    int i,j;
    for(i = args->height / 2; i < (args->height * 3) / 4; i++) {
        for(j = 1; j < args->width; j++) {
            //start processing
            //printf("process3 unit: %d\n", i * args->width + j);
            if(args->originData[i * args->width + j] == 0 &&
                args->originData[(i - 1) * args->width + j] == 0 &&
                args->originData[i * args->width + j - 1] == 0) {
                args->destData[i * args->width + j] = 0;        
            }
            else args->destData[i * args->width + j] = -1;
        }
    }
}

void *process4(void *arg) {
    THREAD_HANDLE_T *args = (THREAD_HANDLE_T *)arg;
    int i,j;
    for(i = (args->height * 3) / 4; i < args->height; i++) {
        for(j = 1; j < args->width; j++) {
            //start processing
            //printf("process4 unit: %d\n", i * args->width + j);
            if(args->originData[i * args->width + j] == 0 &&
                args->originData[(i - 1) * args->width + j] == 0 &&
                args->originData[i * args->width + j - 1] == 0) {
                args->destData[i * args->width + j] = 0;        
            }
            else args->destData[i * args->width + j] = -1;
        }
    }
}