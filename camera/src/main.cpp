/**
 * @file main.cpp
 * @author Olivier Desenfans
 * @brief CWIS camera acquisition software. 
 */

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

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
static inline void setDefaults(void); 

typedef struct {
    std::string outputDir; 
}ProgramOptions_s;

typedef struct {
    UEye_Camera *c; 
    RingBuffer *rb;
    RXPipe *rxpipe;
    bool done;
}CameraParameters_s; 

static ProgramOptions_s programOpts;
static CameraParameters_s cp; 


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
        
    while(!cp.done)
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
            cp.done = true;
            break;
        
        default:
            break; 
    }
}

static void saveImage(char *buffer) {

    std::string filename = programOpts.outputDir + "/image";
 
    filename += ".pgm"; 

    Image *i = cp.rb->getImageFromBuffer(buffer);
    if(i->isBeingWritten()) {
        std::cout << "Ring buffer overflow!" << std::endl; 
    }
    i->writeToPGM(filename.c_str()); 
}

static void prepareForAcquisition(void) {

    cp.c  = new UEye_Camera(1);
    cp.c->setAreaOfInterest(AOI_H_OFFSET, AOI_V_OFFSET, AOI_WIDTH, AOI_HEIGHT);
    cp.c->setAutoExposure(); 
    cp.c->setAutoGain(); 
    double fr = cp.c->setFramerate(5.0);
    
    std::cout << "Framerate: " << fr << " frames per second" << std::endl; 

    cp.rb = new RingBuffer(AOI_WIDTH, AOI_HEIGHT, 10); 

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

