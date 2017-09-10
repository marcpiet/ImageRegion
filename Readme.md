
## Motivation
    It is an exercise/test which I have to do to be allowed to the next step of recruitment process.
    I am willing to change job.

## Synopsis
    After compilation you get a console application  which can be used to display graphical files
    (jpg, bmp, png, etc.) in a window. You can also use this app for determination of areas around
    pointed pixel where color is very similar. The color variation is customizable. Another feature
    allows to draw a perimeter of determined area. The area can be sometimes very jagged so there is
    also option to smooth the contour.
    Every time you can display results of operation and save it into a graphical file or even into
    text file. This is also example of using openCV to load and view images.

## Methods
    Within the source code you can find the set of generic methods, which can be potentially useful
    in another project. Here is the API for them:

        int find_region( Mat& imIn, const int pX, const int pY, const int simf, Mat& imOut ),
                where:
                imIn   - reference to input image (matrix representation of data),
                imOut  - reference to output image (matrix representation of data),
                pX, pY - coordinates of the point around which the area with similar color will be searched;
                         the range of allowed values depends on size of input image.
                simf   - similarity factor, how the color of pixel may differ in regards to origination
                         point; allowed values [1-255], values between 10 and 20 give most expected results
                         (for real photos, e.g. test4.jpg).

        int find_perimeter( Mat& imIn, Mat& imOut ),
                where:
                imIn, imOut - description as above,

        int find_smooth_perimeter( Mat& imIn, int smF, Mat& imOut ),
                where:
                imIn, imOut - description as above,
                smF         - smoothing factor decides, how the region (determined earlier) is smoothed
                              before the find_perimeter() procedure will be launched on it.
                              This parameter accepts values [1-20]. Values between 10 and 20 usualy give
                              satisfying results.

        void display_image( const Mat& image, string const& win_name ),
                where:
                image	 - reference to the matrix (3-dimensional) with input image data,
                win_name - the name for the output window frame,

        void display_pixels( const Mat& image, string const& win_name ),
                where:
                image	 - reference to the matrix with input image data
                           (2-dimensional region or perimeter determined earlier),
                win_name - the name for the output window frame,

        void save_pixels( const Mat& image, string const& filename ),
                image	 - reference to the matrix with input image data
                           (2-dimensional region or perimeter determined earlier),
                filename - the name for the output image file (with extension),

        void save_to_text_file( Mat& image, string const& filename ),
                image	 - reference to the matrix with input image data
                           (2-dimensional region or perimeter determined earlier),
                filename - the name for the output text file.


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
    If you are checking this program, every time upload one of the mentioned images (from the test case
    directory), perform operation 'find region' with using given parameters and then choose
    'find_perimeter'. Look at the the result (use the option: display region/display perimeter).
    These examples show how exactly the algorithm
    works and how much details during searching of continuous region with similar color can be achieved.
    1) File test4.jpg, X=330, Y=210, similarity factor=15,
    2) File test4.jpg, X=2900, Y=800, similarity factor=10,
    3) File test4.jpg, X=3000, Y=1300, similarity factor=20.
    I have also attached the file created by me (5.bmp), which can be used to checking of algorithm
    efficiency and performance.
    After watching results for plain 'find_perimeter' method you can try launch modified method
    'find_smooth_perimeter' with entering a smoothing factor about 15.

## License
    Copying of this source code is forbidden unless you give reference to this web page.