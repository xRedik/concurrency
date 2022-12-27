# Concurrency (Farid Guliyev)

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#assignment-description">Assignment Description</a>
      <ul>
        <li><a href="#built-with">Built With</a>
        <ul> 
            <li><a href="#opencv">OpenCV</a></li>
            <li><a href="#openmp">OpenMP</a></li>
        </ul>
        </li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
        <li><a href="#code-description">Code description</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a>
        <ul>
        <li><a href="#compiling-the-program">Compiling the program</a></li>
        <li><a href="#list-of-arguments">List of Arguments</a></li>
        <ul> 
           <li><a href="#filename">Filename</a></li>
           <li><a href="#box-size">Box size</a></li>
           <li><a href="#processing-mode">Processing mode</a></li>
           <ul> 
              <li><a href="#single-thread">Single Thread</a></li>
              <li><a href="#multi-thread">Multi Thread</a></li>
           </ul>
           <li><a href="#visualizing-method-extra-argument">Visualizing method (Extra argument)</a></li>
        </ul>
      </ul>
     </li>
     <li>
      <a href="#test-results">Test Results</a>
      <ul>
        <li><a href="#test-of-different-box-sizes">Test of different box sizes</a></li>
        <li><a href="#test-of-different-processing-mode">Test of different processing mode</a></li>
        <li><a href="#test-of-different-visualizing-method">Test of different visualizing method</a></li>
      </ul>
    </li>
  </ol>
</details>


## Description
The program is for converting the greyscale image to the colored image using multi-threaded programming.  
### Built With

#### OpenCV
For processing on the image, the famous OpenCV library was used which is huge open-source library for the computer vision, machine learning, and image processing, and it gives us lots of functionalities and easy to use tools for manipulating images.
#### OpenMP
For multi threading, I have used OpenMP which is the API that supports physical level multithreading for not only C and C++, for Fortran and many platforms as well. It is easy to use as well, we just need to use some directives to make our loop or function parallel. We can also make the variables shared, private, and change the thread size as well.
## Getting Started
Before going deeper on the code, we will need some prerequisites, so first let's talk about it.
### Prerequisites
As I have said previously, we will use OpenCV for image processing, that is why we need to install it to our computer. We don't need to install the OpenMP API, the reason is that OpenMP is supported since GCC 11. That is why If you have a GCC compiler, then you probably have OpenMP API as well, you just need to activate it with like -fopenmp. We will talk about it detailed in the next steps.
### Installation
First we will need other packages as dependecies of the OpenCV, that is why let's first install them. I want to mention that all these steps is for Linux operating systems.
  ```sh
   sudo apt install -y g++ cmake make git libgtk2.0-dev pkg-config
   ```
Now we are ready to install the OpenCV.
First step is to clone the OpenCV repository from the github that we have got from OpenCV website like that.
  ```sh
  git clone https://github.com/opencv/opencv.git
   ```
We are ready to build source. First we create build directory and go into it like that.
  ```sh
   mkdir -p build && cd build
   ```
Then we generate the build scripts using cmake and build the source using make
  ```sh
   cmake ../opencv
   make -j4
   ```
And as a final step we just install the OpenCV package like that.
  ```sh
  sudo make install
   ```
That is all. We have installed the OpenCV packages. If you want to verify it, you can just check the /usr/local/include/opencv4 file. The header files of the OpenCV library should be here.
### Code description
Our program consists of 5 function. In the main function, we just read the arguments, and check the correctness of the arguments, if anything is wrong, we just print the error message and terminate our program. However, if everyting is correct, we define our boolean variables as if it is single or multi-threaded and F or O visualizing method, then we read the image file, and call our second function which is named ```convert_image_grayscale_to_colored```. This function takes two argument, the Mat image file, and box_size. Our boolean variables are global variables, that is why we don't need to enter them as arguments to the function. In this function, first create window and give name to it then we check our processing_mode boolean variable to see if program is single or multi-threaded. If it is single-threaded we define our thread size as 1, if it is multi-threaded we take our thread size as maximum thread size for our computer. We also starting to measure the time to see execution time of the loop later. Now we are ready to start our loop. Before the loop we define the pragma directive to show our thread size as 1 or maximum thread size. Here we create nested loops for iterating for every box_size row and box_size columns. The we call our third ```get_average_pixel``` function. This function take indexes of the current row and column, and return the mean value for that box (defined by box_size) that these indexes has created. 
Then we just send that value to our third function which is named as ```gray_to_bgr```. Here I have created my own way to get colored image. I have divided the 255 to 3 parts, if it is less and equal to 85, then blue is 255 which is maximum color, and red and green is equal to gray pixel. The reason that I have also defined blue and green is that, I didn't want to get only blue, red and green colors, I have also wanted to get different colors as well. If gray pixel is great or equal to 170, the red color is 255, and other colors are 255-gray_pixel to get distinct color than red. And if value is between 85 and 170, we put a maximum color to green, and other colors are equal to gray pixel. Then we put all of them to the vector and return it. After we got that values in ```convert_image_grayscale_to_colored```, we store the returned vector to new vector and send the address of image object, current indexes of row, column and vector and box_size to the ```set_new_pixel``` function to set the new bgr values to the pixel of every element on that box. Then depending on our visualizing method we visualize our result for every pixel or for every row that completed, and after the completion of the loops we stop our time, and print difference between stop and start time. As a final step we save our image as ```result.jpg```.


## Usage
### Compiling the program
To compile the program, I have wrote Cmake file and compile the program using that file. The reason is that, in default, compiler didn't know about existence of the OpenCV package. That is why we should find and add directories of the OpenCV in cmake file. I have also added OpenMP package and flags for multi-threading. Here is the CMake list.
```
cmake_minimum_required(VERSION 2.8)
project( main )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( main main.cpp )
target_link_libraries( main ${OpenCV_LIBS} )
find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()
```
To compile it we should execute two command. The first command is for building the script using cmake.
```sh
cmake .
```
And the last command for creating the executable file is like that.
```sh
make
```
After that we will see the new executable file as main.
### List of Arguments
The program consists of 4 main, and 1 optional arguments. The sequence of the arguments should be like that.
  ```sh
  ./program_name file_name.jpg box_size processing_mode visualizing_method
   ```
As I said Tte last argument is optional which define the way to display the image as full row or one pixel. The default value is F if you don't enter it or enter another value than F or O.
#### Filename
The first argument is the filename which is the name of the greyscale image.
#### Box size
Box size is the integer to define the way to find average of the pixel, and assign the values. For example, if box size is 3, it will take square box of the 3 pixel, find average of it, calculate new bgr for that value, and assign every pixel on that pixel to that new value. 
#### Processing Mode
Processing mode is the flag to tell the program to execution the processing of the image on single-thread or multiple thread. The argument should be S or M, if any other character is entered, the error message will be displayed.
##### Single Thread 
If processing mode is entered as single-threaded program (S) only 1 thread is used for executing the loop section which is not parallel. You will see the demonstration of it on the Test Results section.
##### Multi Thread
After processing mode is entered as multi-threaded program (M) the maximum number of available threads on physical layer of the computer is used. In my case, it is 8 thread. That is why you will see 8 parallel thread in Test Results section as well.
#### Visualizing method (Extra argument)
For better visualizing method, I have also added the other extra argument as well. You don't have to enter it. The default visualizing method is F which is the Full row. The purpose of that is to show to live processing image for every completed row. The method O is to display to image for every pixel which tooks long time. I will demonstrate both of them in Test Results section.

## Test Results
For the test purpose, we will use the grayscale image of the cat for all tests. Here is the picture of the cat.
<img src="https://user-images.githubusercontent.com/56725845/204272087-4dafb635-de1f-4f6d-9c06-42d41982f848.jpg" width="500">
### Test of different box sizes
First I want to demonstrate the result of the 
grayscale image to the colored image for different box sizes. <br>
For box size as 1: <br>
```./main cat_gs.jpg 1 S``` <br>
<img src="https://user-images.githubusercontent.com/56725845/204273164-707ee810-0215-4c50-99c9-71a4244322ae.jpg" width="500">
<br> For box size as 2: <br>
```./main cat_gs.jpg 2 S``` <br>
<img src="https://user-images.githubusercontent.com/56725845/204273553-04ec0e93-2a69-4983-b065-d2b9f988e25b.jpg" width="500">
<br> For box size as 3: <br>
```./main cat_gs.jpg 3 S``` <br>
<img src="https://user-images.githubusercontent.com/56725845/204274194-b11165aa-be53-42a8-8c2c-630657a3ee55.jpg" width="500">
<br> As you can see if we increase the box size the colored image will be very noisy proportionally, that is why it is better to keep the box size as low value. Now let's start to test the processing method as single-threaded and multi-threaded.

### Test of different processing mode

For the demonstration of the test, I have used box size as 1 and visualizing method as full row. My physical layer of my computer has 8 thread, that is why you will see 8 different parallel region on the image. However for the single thread processing method, you will see only one region that filling from top to the bottom. Here is the results for single thread and multi-thread.
<p float="left">
<img src="https://user-images.githubusercontent.com/56725845/204277992-4084aa0d-69be-4b9b-b806-289fee6ab222.gif" width="400">
<img src="https://user-images.githubusercontent.com/56725845/204278553-9b03fc7a-9856-4b76-826c-4b00ad82aea7.gif" width="400">
</p>

### Test of different visualizing method
For the demonstration I have used box size as 1. I have also hasn't uploaded the full gif for one pixel visualizing method, the reason is that full gif in that case took a lot of time than F method to be completed. <br>
The visualizing methods for single-thread processing mode: <br>

<p float="left">
<img src="https://user-images.githubusercontent.com/56725845/204277992-4084aa0d-69be-4b9b-b806-289fee6ab222.gif" width="400">
<img src="https://user-images.githubusercontent.com/56725845/204280925-5038370a-88a6-4316-a3ad-5d2c930d69f6.gif" width="400">
</p>
You can see the small visualizing of the new pixels in the second picture if you look at the top left of the image. As you can see it took a long time, that is why I have used two visualizing method.

The visualizing methods for multi-thread processing mode: <br>
<p float="left">
<img src="https://user-images.githubusercontent.com/56725845/204278553-9b03fc7a-9856-4b76-826c-4b00ad82aea7.gif" width="400">
<img src="https://user-images.githubusercontent.com/56725845/204281789-6cea0830-1856-44b6-8a65-eacc5d77f2f7.gif" width="400">
</p>
You can see the small visualizing of the new pixels in the 8 different parallel section of the picture. As you can see it took a long time as well.
