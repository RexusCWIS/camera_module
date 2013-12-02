/**
 * @file main.cpp
 * @author Olivier Desenfans
 * @brief CWIS camera acquisition software. 
 */

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include <pthread.h>
#include "shared_object.hpp"

#include <uEye.h>

#include "utils/utilities.hpp"
#include "camera/ueye_camera.hpp"
#include "camera/image.hpp"
#include "pipes/rx_pipe.hpp"
#include "camera/exceptions/ueye_exception.hpp"

#include "camera_settings.hpp"

using namespace std; 

static int displayCameraInformations(void); 
static int listConnectedCameras(void);
static void prepareForAcquisition(void);
static void saveImage(char *buffer);
static void * storage(void *arg); 
static inline void setDefaults(void); 

typedef struct {
    std::string outputDir; 
}ProgramOptions_s;

typedef struct {
    UEye_Camera *c; 
    RingBuffer *rb;
    RXPipe *rxpipe;
}CameraParameters_s; 

static ProgramOptions_s programOpts;
static CameraParameters_s cp; 

static SharedObject<unsigned int> nbOfBufferedImages(0);
static bool endOfAcquisition;  
static thread_t storageThread; 

/* Long options definition */
static const struct option longOpts[] = {
    {"output-dir", required_argument, NULL, 'd'}
}; 

int main(int argc, char *argv[]) {

    int opt = 0;
    int longIndex = 0; 

    setDefaults();
    
    /* Command-line arguments parsing */
    while( (opt = getopt_long(argc, argv, "d:il", longOpts, &longIndex)) != -1) {

        switch (opt) {
            case 'd':
                programOpts.outputDir = optarg;
                break; 

            /* List all connected cameras and exit */
            case 'l':
                exit(listConnectedCameras()); 
                break;
           
            /* Displays informations about the cameras */
            /** @todo Fix this and mix it with the -l option */
            case 'i':
                exit(displayCameraInformations()); 
                break;

            /* Missing argument to an option */
            case ':':
                cerr << "Invalid option." << endl; 
                exit(EXIT_FAILURE);
                break; 

            /* Unknown argument, not specified in the optstring */
            case '?':
                break;

            /* Long option with no short option counterpart */
            case 0:
                break; 
        }
    }

    /* Acquisition */
    if(!createDirectory(programOpts.outputDir)) {
        exit(EXIT_FAILURE); 
    }

    prepareForAcquisition(); 
        
    while(!endOfAcquisition)
        ;
        
    delete cp.c; 
    delete cp.rxpipe;
    delete cp.rb; 

    exit(EXIT_SUCCESS); 
}

static void orderProcessing(char orders[], int size) {
    
    switch(orders[size-1]) {
        case 'G':
            std::cout << "The experiment has STARTED." << std::endl;
            pthread_create(&storageThread, NULL, &storage, NULL); 
            
            try {    
                cp.c->start(cp.rb, &saveImage);
            }

            catch(UEye_Exception const &e) {
        
                cerr << "Camera " << e.camera() << ": " << e.what() <<
                    "\nException ID: " << e.id() << endl;
            }
            break; 
        case 'S':
            cp.c->stop(); 
            std::cout << "The experiment is OVER." << std::endl; 
            endOfAcquisition = true; 
            break;
        
        default:
            break; 
    }
}

static void saveImage(char *buffer) {

    /** @todo Reprocess the entire callback functionality */
    (void) buffer; 

    nbOfBufferedImages.lock(); 
    nbOfBufferedImages.m_value++;  
    nbOfBufferedImages.unlock(); 

}

static void *storage(void *arg) {

    unsigned int currentImage = 0, 
                 imageCounter = 0, 
                 bufferedImages = 1;

    std::string filename;

    while((!endOfAcquisition) || bufferedImages) {

        nbOfBufferedImages.lock(); 
        bufferedImages = nbOfBufferedImages.m_value; 
        nbOfBufferedImages.unlock(); 

        /* While there are images in the ring buffer, save them */
        if(bufferedImages != 0) {
            filename = programOpts.outputDir + "/image";
            string_appendInt(filename, imageCounter);
 
            filename += ".pgm"; 
            
            cp.rb->at(currentImage)->writeToPGM(filename.c_str());

            imageCounter++;
            currentImage = imageCounter % RING_BUFFER_SIZE; 
            nbOfBufferedImages.lock(); 
            nbOfBufferedImages.m_value--; 
            nbOfBufferedImages.unlock(); 
        }
    }
}

static void prepareForAcquisition(void) {

    endOfAcquisition = false;

    
    cp.c  = new UEye_Camera(1);
    cp.c->setAreaOfInterest(AOI_H_OFFSET, AOI_V_OFFSET, AOI_WIDTH, AOI_HEIGHT);
    cp.c->setAutoExposure(); 
    cp.c->setAutoGain(); 
    double fr = cp.c->setFramerate(5.0);
    
    std::cout << "Framerate: " << fr << " frames per second" << std::endl; 

    cp.rb = new RingBuffer(AOI_WIDTH, AOI_HEIGHT, RING_BUFFER_SIZE); 

    cp.rxpipe = new RXPipe("/tmp/camera_pipe.p", &orderProcessing);
    cp.rxpipe->start(); 
}

static int displayCameraInformations(void) {

    UEye_Camera *c = new UEye_Camera(1); 
    c->displayInfo(); 
    delete c; 
    return EXIT_SUCCESS; 
}

static int listConnectedCameras(void) {

    UEYE_CAMERA_LIST *camList = NULL;
    INT numCams = 0; 

    if (is_GetNumberOfCameras(&numCams) == IS_INVALID_PARAMETER) {
        return EXIT_FAILURE; 
    }

    if (numCams == 0) {
        
        cout << "No camera found." << endl; 
    }

    else {

        /* Camera list allocation */
        camList = (UEYE_CAMERA_LIST *) new unsigned char[sizeof(ULONG) + 
                                                   numCams * sizeof(UEYE_CAMERA_INFO)];

        
        camList->dwCount = numCams;

        if (is_GetCameraList(camList) != IS_SUCCESS) {
            
            return EXIT_FAILURE; 
        }

        for (int cam = 0; cam < (int) camList->dwCount; cam++) {
            
            cout << "Camera " << cam << ":\n" <<
                 "ID:\t" << camList->uci[cam].dwCameraID << "\n" <<
                 "Model:\t" << camList->uci[cam].Model << "\n" <<
                 "Serial number: \t" << camList->uci[cam].SerNo << "\n" << endl; 
        }

        /* Free the camera list memory */
        delete [] camList; 
    }
    
    return EXIT_SUCCESS;  
}

static inline void setDefaults(void) {
    programOpts.outputDir = "images";
}

