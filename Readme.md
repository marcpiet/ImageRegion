
## Motivation
    It is an exercise/test which I have to do to be allowed to the next step of reqruitment proccess - I am willing to change job.

## Synopsis
    After compilation you get a console application  which can be used to display graphical files (jpg, bmp, png, etc.) in a window.
    You can also use this app for determination of areas around pointed pixel where color is very similar. The color variation is customizable.
    Another feature allows to draw a perimeter of determined area. The area can be sometimes very jagged so there is also option 
    to smooth the region and to smoth the contour. Every time you can display results of operation and save it within a graphical file.
    This is also example of using openCV to load and view images.

## Methods
    Within the source code you can find the set of generic methods, which can be potentially useful in another project. Here is the API fot them:

 	.
	. 
	.
	.
	.
	.
	.
	.
	.
	.
	.
	.
	.




## Requirements
    - OpenCV 2.X    (tested with 2.4.9)
    - CMake > 2.8   (however, I have compiled it with using Visual Studio, under Windows)


## Installation
    Under Ubuntu you can do as follows:
    cmake .
    make

## Usage
    ImageRegion [press ENTER]
    ... and choose one of the available options (context menu will be shown on the screen),
    ... follow instructions, enter parameters, filenames etc.
    ... the user interface is very intuitive and 'interactive'.

## Tests
    You can check working of methods and algorithms on the base of some examples:
    Every time upload the mentioned image (from the test case directory), perform operation 'find region'
    with using given parameters and show results (display ...)
    1) 

## License
    Copying of this source code is forbidden unless you give reference to this webpage.