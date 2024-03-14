# Video Player

Media player with support for .mp4 files. It also has functions for detecting the movements of objects in the video, especially useful for processing camera recordings. The processing algorithms work thanks to the use of the ``OpenCV`` library.

# Requirements
* OpenCV: ``sudo apt-get install libopencv-dev``

# Building
* Configure ``qmake``, ``git`` and any ``C++`` compiler
* Clone the repository: ``git clone https://github.com/BenQUUU/Video-Player.git``
* Create a build directory: ``mkdir build``
* Change to the build directory: ``cd build``
* Run CMake: ``qmake ../Video-Player``
* Build the project: ``make``
* Run the program: ``./Video-Player``

# How the program works

<p align="center">
  <img src="https://i.imgur.com/bd0t7E7.png" alt="Video playing">
</p>

<p align="center">
  <img src="https://i.imgur.com/vy2U8j6.png" alt="An example of how the motion detection algorithm works">
</p>
