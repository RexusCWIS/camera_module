/**
 * @file main.c
 * @author Olivier Desenfans
 * @brief CWIS camera acquisition software. 
 */

#include <iostream>
#include <stdlib.h>
#include "unistd.h"

#include <uEye.h>

#include "types.hpp"
#include "utilities.hpp"
#include "ueye_camera.hpp"
#include "image.hpp"
#include "exceptions/ueye_exception.hpp"

using namespace std; 

static int listConnectedCameras(void);
static void singleAcquisition(const char *filename); 

static bool saveAsPNG = false, saveAsBMP = false, saveAsPGM = false; 

int main(int argc, char *argv[]) {

    int opt = 0;
    std::string outputFile;

    /* Command-line arguments parsing */
    while( (opt = getopt(argc, argv, "lo:")) != -1) {

        switch (opt) {
            /* List all connected cameras and exit */
            case 'l':
                exit(listConnectedCameras()); 

            /* Output file specification */
            case 'o':
                outputFile = optarg;
                break; 
               
            /* Missing argument to an option */
            case ':':
                cerr << "Invalid option." << endl; 
                exit(EXIT_FAILURE);
                break; 

            /* Unknown argument, not specified in the optstring */
            case '?':
                break;
        }
    }

    std::string ext = getFileExtension(outputFile);
    if(ext.empty()) {
        cerr << "No file extension, cannot specify the output format. Specify an extension or use specific command line options." << endl; 
        exit(EXIT_FAILURE); 
    }

    if(ext == "png") {
        saveAsPNG = true; 
    }

    else if(ext == "pgm") {
        saveAsPGM = true; 
    }

    else if(ext == "bmp") {
        saveAsBMP = true; 
    }

    else {
        cerr << "Invalid file extension: " << ext << "\ncannot specify the output format. " << endl;
        exit(EXIT_FAILURE); 
    }

    /* Acquisition */
    singleAcquisition(outputFile.c_str()); 

    exit(EXIT_SUCCESS); 
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
        camList = (UEYE_CAMERA_LIST *) new UInt8_t[sizeof(ULONG) + 
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

    if(saveAsPNG) { 
        i->writeToPNG(filename);
    }

    if(saveAsPGM) {
        i->writeToPGM(filename); 
    }

    delete i; 
    delete c; 

    return; 
}

