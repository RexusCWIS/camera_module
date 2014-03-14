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

#include "utils/utilities.hpp"

#include "camera.h"
#include "image_buffer.h"
#include "camera_settings.h"
#include "i2c_manager.h"

using namespace std; 

static inline void set_defaults(void); 

typedef struct {
    std::string output_dir; 
} program_options;

static void order_processing(char order);

static image_buffer* images;
static ueye_camera*  camera;
static i2c_manager*  i2c;
static program_options program_opts;

/* Long options definition */
static const struct option long_opts[] = {
    {"output-dir", required_argument, NULL, 'd'},
    {0, 0, 0, 0}
}; 

int main(int argc, char *argv[]) {

    int opt = 0;
    int long_index = 0; 

    set_defaults();
    
    /* Command-line arguments parsing */
    while( (opt = getopt_long(argc, argv, "d:il", long_opts, &long_index)) != -1) {

        switch (opt) {
            case 'd':
                program_opts.output_dir = optarg;
                break; 

            /* List all connected cameras and exit */
            case 'l':
                //exit(listConnectedCameras());
                break;
           
            /* Displays informations about the cameras */
            /** @todo Fix this and mix it with the -l option */
            case 'i':
                //exit(displayCameraInformations()); 
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
    if(!createDirectory(program_opts.output_dir)) {
        exit(EXIT_FAILURE); 
    }
    
	images = new image_buffer(CONFIG_AOI_WIDTH, CONFIG_AOI_HEIGHT, 
	                          CONFIG_BUFFER_SIZE);

	camera = new ueye_camera(CONFIG_CAMERA_ID);

	camera->set_aoi(CONFIG_AOI_H_OFFSET, CONFIG_AOI_V_OFFSET,
					CONFIG_AOI_WIDTH, CONFIG_AOI_HEIGHT);	
	camera->set_auto_exposure();
	camera->set_auto_gain();
	std::cout << "Framerate: " << camera->set_framerate(CONFIG_FRAMERATE) << std::endl;

    i2c = new i2c_manager("/dev/ttyACM0", "i2c.log", &order_processing);
	
	camera->start_acquisition(images->buffer, images->size, 
                                  images->width, images->height);

	std::cout << "Started acquisition" << std::endl;	

	while(!camera->is_over()) {
		usleep(50000);
	}

	std::cout << "Stopped the camera." << std::endl;

	images->save_to_pgm(program_opts.output_dir.c_str());

	std::cout << "Wrote images to " << program_opts.output_dir << std::endl;
	
	delete i2c;	
	delete images;
	delete camera;          

	std::cout << "Ready to exit..." << std::endl;

    exit(EXIT_SUCCESS); 
}

static void order_processing(char order) {

	switch(order) {
        case 'G':
	    std::cout << "The experiment has STARTED." << std::endl;
            
            try {    
                camera->start_acquisition(images->buffer, images->size, 
                                  		  images->width, images->height);
            }

            catch(UEye_Exception const &e) {
        
                cerr << "Camera " << e.camera() << ": " << e.what() <<
                    "\nException ID: " << e.id() << endl;
            }
            break; 
        case 'S':
            camera->stop_acquisition();
            std::cout << "The experiment is OVER." << std::endl;
            std::cout << "Acquired " << camera->get_nb_of_images_acquired() << 
                         "images" << std::endl;
            break;
        
        default:
            break; 
    }
}

static inline void set_defaults(void) {
    program_opts.output_dir = "images";
}

