#!/bin/bash

while true
do
	# preparation work: remove existing files
	rm -f /home/pi/OpenCV-red-circle-detection/images/image.jpg
	rm -f /home/pi/OpenCV-red-circle-detection/positions.txt
	rm -f /home/pi/OpenCV-red-circle-detection/finished.txt
	
	echo "--------------Taking picture---------------!"
	# take an image of the current platform
	raspistill -o /home/pi/OpenCV-red-circle-detection/images/image.jpg
	
	echo "-----------Identifying the center----------!"
	# identify the center and redirect result to position.txt
	/home/pi/OpenCV-red-circle-detection/circle_detect /home/pi/OpenCV-red-circle-detection/images/image.jpg >> /home/pi/OpenCV-red-circle-detection/positions.txt
	
	echo "--------------Moving the robot-------------!"
	# move robotic arm to desired position
	python /home/pi/final_project/final_code.py
	
	echo "finished" >> /home/pi/OpenCV-red-circle-detection/finished.txt
done