# CAMERA CALIBRATION
Camera calibration is a key skill in computer vision and robotics. It is essential for tasks like taking great photos, helping self-driving cars navigate safely, guiding surgical tools, and allowing humanoid robots to interact well with their surroundings. These distortions, if left unaddressed, can lead to inaccurate spatial mapping and flawed interpretations of the world—a critical failure point in any vision-driven system. Therefore, the ability to meticulously calibrate a camera, effectively eliminating these distortions, is not just a good practice but an absolute necessity for building robust and trustworthy perception systems.

# TOOLS
- C++
- OpenCV (C++ API)
- Ubuntu 22.04 (WSL2)

# PRE-REQUISITES AND INSTALLATION (UBUNTU 22.04)
- sudo apt-get update
- sudo apt-get install libopencv-dev

# RESULTS
- # CORNER DETECTION
![Corner Detection (4)](https://github.com/user-attachments/assets/823cd323-47df-4b19-810d-f0616c6fbac4)
![Corner Detection (2)](https://github.com/user-attachments/assets/62099b1d-40c2-450b-956e-04fc2e2b9110)

- # CAMERA CALIBRATION
![Original vs Undistorted](https://github.com/user-attachments/assets/2a85272d-2bf8-4731-a243-706260f9863f)
![Camera Parameters](https://github.com/user-attachments/assets/64e9dab6-42cf-48a7-86cf-3f6205514df3)

# NEXT STEPS
Prepare my own dataset and apply camera calibration for 3D reconstruction of an environment using Python or C++.

# REFERENCES
[Camera Calibration using OpenCV](https://docs.opencv.org/4.x/d9/d0c/group__calib3d.html#ga3207604e4b1a1758aa66acb6ed5aa65d)
