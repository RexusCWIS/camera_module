#!/bin/bash
# This script handles the OnBoard Data Handling of the CWIS experiment. 
# It realizes various checkups at boot and manages the data acquired during the experiment. 

# Experiment
echo "Starting the camera acquisition software..." >> /home/cwis/experiment.log
/home/cwis/code/obdh/cwis-camera.out -d /home/cwis/images -l /home/cwis/image_files.log > /home/cwis/camera.log 2>&1

echo "Mounting the safety box drive" >> /home/cwis/experiment.log
mount /mnt/usb >> /home/cwis/experiment.log
#mount /mnt/drive >> /home/cwis/experiment.log
echo "Copying files" >> /home/cwis/experiment.log
cp -vR /home/cwis/images/ /mnt/usb/ >> /home/cwis/experiment.log
cp -v /home/cwis/camera.log /mnt/usb/ >> /home/cwis/experiment.log
cp -v /home/cwis/image_files.log /mnt/usb/ >> /home/cwis/experiment.log
# cp -vR /home/cwis/images/ /mnt/drive/ >> /home/cwis/experiment.log
# cp -v /home/cwis/camera.log /mnt/drive/ >> /home/cwis/experiment.log
# cp -v /home/cwis/image_files.log /mnt/drive/ >> /home/cwis/experiment.log

echo "Unmounting the safety box drive" >> /home/cwis/experiment.log
umount /mnt/usb >> /home/cwis/experiment.log
#umount /mnt/drive >> /home/cwis/experiment.log
echo "END OF THE EXPERIMENT" >> /home/cwis/experiment.log
halt
