#!/bin/bash
# This script handles the OnBoard Data Handling of the CWIS experiment. 
# It realizes various checkups at boot and manages the data acquired during the experiment. 

# Cleanup
echo "System cleanup" > /home/cwis/experiment.log
rm -vf /tmp/camera_pipe.p >> /home/cwis/experiment.log
# rm -rf /home/cwis/images/* >> /home/cwis/experiment.log

sleep 5

# Experiment
echo "Starting the I2C communication..." >> /home/cwis/experiment.log
/home/cwis/code/i2c_com/i2c-com.out > /home/cwis/i2c.log &
#/home/cwis/code/simulator/cwis_sim.out &
sleep 1
echo "Starting the camera acquisition software..." >> /home/cwis/experiment.log
/home/cwis/code/camera/cwis_camera.out -d /home/cwis/images > /home/cwis/camera.log
#./cam-sim/cam-sim.out
killall -v i2c-com.out >> /home/cwis/experiment.log

echo "Mounting the safety box drive" >> /home/cwis/experiment.log
mount /mnt/usb >> /home/cwis/experiment.log
mount /mnt/drive >> /home/cwis/experiment.log
echo "Copying files" >> /home/cwis/experiment.log
cp -vR /home/cwis/images/ /mnt/usb/ >> /home/cwis/experiment.log
cp -vR /home/cwis/images/ /mnt/drive/cwis_dcwis_d/ >> /home/cwis/experiment.log
cp -v /home/cwis/experiment.log /mnt/usb/ >> /home/cwis/experiment.log
cp -v /home/cwis/experiment.log /mnt/drive/ >> /home/cwis/experiment.log
echo "Unmounting the safety box drive" >> /home/cwis/experiment.log
umount /mnt/usb >> /home/cwis/experiment.log
umount /mnt/drive >> /home/cwis/experiment.log
echo "END OF THE EXPERIMENT" >> /home/cwis/experiment.log
halt

