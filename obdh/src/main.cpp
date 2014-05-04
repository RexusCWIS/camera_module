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

unsigned int system_time = 0;

static void order_processing(char order);
static inline void set_defaults(void);
static inline void update_status(void); 
static void set_camera_parameters(void);
static void print_camera_parameters(void);

typedef struct {
    std::string output_dir;
    std::string image_log;
} program_options;

#define STATUS_ON	    (1 << 0)
#define STATUS_CAMERA_ON    (1 << 1)
#define STATUS_ACQUIRING    (1 << 2)
#define STATUS_COPYING	    (1 << 3)


typedef struct {
	char nb_of_images[2];
	char framerate;
	char flags;
} module_status;

static image_buffer* images;
static ueye_camera*  camera;
static i2c_manager*  i2c;
static program_options program_opts;
static module_status status = {{0, 0}, 0, 0};
static unsigned int start_time   = 0;
static unsigned int end_time     = 0;
static bool start_of_experiment = false;
static bool end_of_experiment   = false;


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
    while( (opt = getopt_long(argc, argv, "d:il:", long_opts, &long_index)) != -1) {

        switch (opt) {
            case 'd':
                program_opts.output_dir = optarg;
                break; 

            /* List all connected cameras and exit */
            case 'l':
		program_opts.image_log = optarg;
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
    
    std::cout << "Starting I2C communication..." << std::endl;
    i2c = new i2c_manager("/dev/ttyACM0", "/home/cwis/i2c.log", &order_processing);

    status.flags |= STATUS_ON;
    update_status();

    /* Create output directory */
    if(!createDirectory(program_opts.output_dir)) {
        exit(EXIT_FAILURE);
    }
    
    std::cout << "Creating image buffer..." << std::endl;
    images = new image_buffer(CONFIG_AOI_WIDTH, CONFIG_AOI_HEIGHT, 
	                          CONFIG_BUFFER_SIZE);

    std::cout << "Setting camera parameters..." << std::endl;
    set_camera_parameters();

    status.flags |= STATUS_CAMERA_ON;
    update_status();

    while(!start_of_experiment)
        ;

    status.flags |= STATUS_ACQUIRING;

    while(!end_of_experiment) {
        usleep(150000);
	unsigned int nb_of_images = camera->get_nb_of_images_acquired();
	status.nb_of_images[0] = (char) ((nb_of_images & 0xFF00u) >> 8u);
	status.nb_of_images[1] = (char) (nb_of_images & 0xFFu);
	
	update_status();
    }

    end_time = system_time;
    status.flags &= ~STATUS_ACQUIRING;
    status.flags |= STATUS_COPYING;
    update_status();

    std::cout << "Acquired " << camera->get_nb_of_images_acquired() << " images in "
              << (end_time - start_time) / 1000.0 << " seconds." << std::endl;

    std::cout << "[" << system_time << "] " << "Writing images to " << program_opts.output_dir << std::endl;
    images->save_to_png(program_opts.output_dir.c_str());
    images->save_log(program_opts.image_log.c_str());

	std::cout << "[" << system_time << "]" << " Saved images in " 
	          << (system_time - end_time) / 1000.0 << " seconds" << std::endl;

    std::cout << "Stopping I2C communication..." << std::endl;	
    delete i2c;	
    
    std::cout << "Freeing allocated memory..." << std::endl;
    delete images;

    std::cout << "Putting the camera in standby mode..." << std::endl;
    delete camera;          

    std::cout << "Ready to exit..." << std::endl;

    exit(EXIT_SUCCESS); 
}

static void order_processing(char order) {

	switch(order) {
        case 'G':
	    start_time = system_time;
	    start_of_experiment = true;
	    std::cout << "[" << start_time << "] The experiment has STARTED." << std::endl;
            
            try {    
                camera->start_acquisition(images);
            }

            catch(UEye_Exception const &e) {
        
                cerr << "Camera " << e.camera() << ": " << e.what() <<
                    "\nException ID: " << e.id() << endl;
            }
            break; 
        case 'S':
            camera->stop_acquisition();
            std::cout << "[" << system_time << "] The experiment is OVER." << std::endl;
	    end_of_experiment = true;
            break;
        
        default:
            break; 
    }
}

static inline void set_defaults(void) {
    program_opts.output_dir = "images";
    program_opts.image_log  = "images.log";
}

static inline void update_status(void) {
    i2c->write((char *) &status, 4u, 0u);
}

static void set_camera_parameters(void) {
	
	camera = new ueye_camera(CONFIG_CAMERA_ID);
	
	camera->set_aoi(CONFIG_AOI_H_OFFSET, CONFIG_AOI_V_OFFSET,
					CONFIG_AOI_WIDTH, CONFIG_AOI_HEIGHT);	
	
	double framerate = camera->set_framerate(CONFIG_FRAMERATE);
	std::cout << "Framerate: " << framerate << " fps" << std::endl;
	
	camera->set_exposure(CONFIG_EXPOSURE);
	camera->set_gain(CONFIG_GAIN);
	
	status.framerate = (char) framerate;
}

static void print_camera_parameters(void) {

    std::cout << "Camera parameters" << std::endl << "======================" << std::endl;
    std::cout << "Pixel clock: " << camera->get_pixel_clock() << " MHz" << std::endl;
    std::cout << "Exposure: " << camera->get_exposure() << std::endl;
    std::cout << "Gain: " << camera->get_gain() << " (min: " << IS_MIN_GAIN <<
              ", max: " << IS_MAX_GAIN << ")" << std::endl;
    
    std::cout << std::endl;
}

