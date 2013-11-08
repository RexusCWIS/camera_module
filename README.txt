camera_module
=============

Camera acquisition software for the CWIS experiment.

I. INTRODUCTION

II. DEPENDENCIES

This software requires the use of some non-standard libraries:
-libueye_api:   uEye camera library. It can be installed by downloading the uEye SDK 
                from the manufacturer (IDS) website. 

-libpng:        Library used to write PNG files. On Ubuntu, you can install it by 
                downloading the 'libueye-dev' package.

-gtest:         Google C++ Unit Test Framework. This framework is automatically downloaded
                and compiled by the unit tests Makefile. 


