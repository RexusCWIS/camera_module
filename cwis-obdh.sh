#!/bin/bash
# This script handles the OnBoard Data Handling of the CWIS experiment. 
# It realizes various checkups at boot and manages the data acquired during the experiment. 

# Cleanup
rm -f /tmp/camera_pipe.p

# Experiment
./simulator/cwis_sim.out &
./camera/cwis_camera.out -a images

