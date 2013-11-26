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

#include "utilities.hpp"
#include "ueye_camera.hpp"
#include "image.hpp"
#include "pipes/rx_pipe.hpp"
#include "exceptions/ueye_exception.hpp"

using namespace std; 

static int displayCameraInformations(void); 
static int listConnectedCameras(void);
static void singleAcquisition(const char *filename); 
static void prepareForAcquisition(void);
static void saveImage(char *buffer); 
static inline void setDefaults(void); 

typedef enum {
    SINGLE, 
    MULTIPLE
}ProgramMode_e;

typedef enum {
    PNG,
    BMP, 
    PGM
}OutputFormat_e; 

typedef struct {
    int nframes; 
    std::string outputFile; 
    std::string fileExtension; 
    std::string outputDir; 
    bool detectExtension;
    OutputFormat_e format; 
}ProgramOptions_s;

typedef struct {
    UEye_Camera *c; 
    RingBuffer *rb;
    RXPipe *rxpipe;
    unsigned int cntr; 
    bool done;
}CameraParameters_s; 

static ProgramOptions_s programOpts;
static ProgramMode_e programMode; 
static CameraParameters_s cp; 


/* Long options definition */
static const struct option longOpts[] = {
    {"format", required_argument, NULL, 'f'}
}; 

int main(int argc, char *argv[]) {

    int opt = 0;
    int longIndex = 0; 

    setDefaults(); 

    cout << "Output file: " << programOpts.outputFile << endl; 

    /* Command-line arguments parsing */
    while( (opt = getopt_long(argc, argv, "a:f:lo:i", longOpts, &longIndex)) != -1) {

        switch (opt) {
            case 'a':
                programMode = MULTIPLE; 
                programOpts.outputDir = optarg; 
                break; 

            /* Specify the output format */
            /** @todo Add validity checks */
            case 'f':
                programOpts.detectExtension = false;
                programOpts.fileExtension   = optarg;
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

            /* Output file specification */
            case 'o':
                programMode = SINGLE; 
                programOpts.outputFile = optarg;
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

    /* Automatic extension detection */ 
    if(programOpts.detectExtension) {
        programOpts.fileExtension = getFileExtension(programOpts.outputFile);
    }

    if(programOpts.fileExtension.empty()) {
        cerr << "No file extension, cannot specify the output format. Specify an extension or use specific command line options." << endl; 
        exit(EXIT_FAILURE); 
    }

    /* Detect the extension provided using the -f command line argument */
    if(programOpts.fileExtension == "png") {
        programOpts.format = PNG; 
    }

    else if(programOpts.fileExtension == "pgm") {
        programOpts.format = PGM; 
    }

    else if(programOpts.fileExtension == "bmp") {
        programOpts.format = BMP; 
    }

    else {
        cerr << "Invalid file extension: " << programOpts.fileExtension << "\ncannot specify the output format. " << endl;
        exit(EXIT_FAILURE); 
    }

    /* Acquisition */
    if(programMode == MULTIPLE) {
        prepareForAcquisition(); 
        while(!cp.done)
            ;
        delete cp.c; 
        delete cp.rxpipe;
        delete cp.rb; 
    }

    else {
        singleAcquisition(programOpts.outputFile.c_str()); 
    }

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

    std::string filename = "images/image";
    string_appendInt(filename, cp.cntr);
    filename += programOpts.fileExtension; 

    Image *i = cp.rb->getImageFromBuffer(buffer);

    /** @todo Propose multiple formats here */
    i->writeToPNG(filename.c_str()); 
    cp.cntr++; 
}

static void prepareForAcquisition(void) {

    cp.c  = new UEye_Camera(1);
    cp.c->setAreaOfInterest(0, 0, 800u, 600u);

    cp.rb = new RingBuffer(800u, 600u, 10); 
    cp.cntr = 0u;

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

static void singleAcquisition(const char *filename) {

    cout << "Frames: " << programOpts.nframes << endl; 

    Image *i = new Image(800u, 600u);
    UEye_Camera *c = new UEye_Camera(1);

    c->setAreaOfInterest(0, 0, 800, 600);
    try {
        c->capture(i); 
    }

    catch(UEye_Exception const &e) {
        
        cerr << "Camera " << e.camera() << ": " << e.what() <<
                "\nException ID: " << e.id() << endl;
    }

    switch(programOpts.format) {
        case PNG: 
            i->writeToPNG(filename);
            break; 
        case PGM: 
            i->writeToPGM(filename); 
            break; 
        default: 
            break; 
    }

    delete i; 
    delete c; 

    return; 
}

static inline void setDefaults(void) {
    programOpts.nframes = 1; 
    programOpts.outputFile = "image.png"; 
    programOpts.fileExtension = "png"; 
    programOpts.outputDir = "images"; 
    programOpts.detectExtension = false; 
    programOpts.format = PNG; 

    programMode = SINGLE; 
}

