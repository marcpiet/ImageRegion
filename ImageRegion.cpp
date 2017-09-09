

#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//void show_mat(const cv::Mat &image, std::string const &win_name);
//cv::Mat &invert_mat(cv::Mat &mat);
//cv::Mat& invert_mat_pointer(cv::Mat &mat);


void show_mat(const cv::Mat &image, std::string const &win_name) {
    namedWindow(win_name, WINDOW_AUTOSIZE);
    imshow(win_name, image);
    waitKey(0);
	destroyWindow(win_name);
}


int main(int argc, char **argv) {
//    if (argc != 3) {
//        printf("usage: Opencv_Test <Image_Path> <Output_Path>\n");
//        return -1;
//    }

	int iChoice = 0;
	int isInputFileLoaded = 0,
		isRegionDetermined = 0,
		isPerimeterDetermined = 0;
	Mat image;

	//do  // the main loop of the consol application interface
	//{


	//}
	//while( 1 );

	//-----------------

	std::string inFileName;
	cout << "Enter the input image file name (using quotation marks): ";
	cin >> inFileName;

    image = imread(inFileName, 1);

    if (!image.data) {
        printf("No image data (warning: OpenCV recognize files by extensions)\n");
        return -1;
    }

	do
	{
		cout << "What do you want to do now? Choose from options:" << endl 
			 << "1 = show loaded image," << endl
			 << "2 = find region with similar color," << endl
			 << "0 = exit application," << endl
			 << "Your choice is... ";
		cin >> iChoice;
		if( iChoice < 0 || iChoice > 2 )
		{
			cout << "The value outside of allowed range. Try again." << endl;
			continue;
		}
		break;
	}
	while(1);

	if( iChoice == 0 )
		return 0;

	if( iChoice == 1 )
	{
		show_mat(image, "Input");
		//waitKey(0);

	}
	else if( iChoice == 2 )
	{
		goto FIND_REGION;
	}

	do
	{
		cout << "What do you want to do now? Choose from options:" << endl 
			 << "2 = find region with similar color," << endl
			 << "0 = exit application," << endl
			 << "Your choice is... ";
		cin >> iChoice;
		if( iChoice != 0 && iChoice != 2 )
		{
			cout << "The value outside of allowed range. Try again." << endl;
			continue;
		}
		break;
	}
	while(1);

	if( iChoice == 0 )
		return 0;

	//else iChoice == 2

	//------------- FIND REGION
FIND_REGION:

	int channels = image.channels();
	int nRows    = image.rows;
    int nCols    = image.cols;

	int pX = nCols, pY = nRows;  // coordinates of origination point

	do
	{
		cout << "Enter X coordinate of pixel (to begin searching a whole similar region)" << endl << " [0-" << (nCols-1) << "]: ";
		cin >> pX;
		if( pX < 0 || pX > nCols-1 )
		{
			cout << "The value outside of allowed range. Try again." << endl;
			continue;
		}
		break;
	} while(1);

	do
	{
		cout << "Enter Y coordinate of pixel (to begin searching a whole similar region)" << endl << " [0-" << (nRows-1) << "]: ";
		cin >> pY;
		if( pY < 0 || pY > nRows-1 )
		{
			cout << "The value outside of allowed range. Try again." << endl;
			continue;
		}
		break;
	} while(1);

	int tf=0;
	do
	{
		cout << "Enter the color tolerance factor" << endl << " [1-100]: ";
		cin >> tf;
		if( tf < 1 || tf > 100 )
		{
			cout << "The value outside of allowed range. Try again." << endl;
			continue;
		}
		break;
	} while(1);

	cout << "DBG: x=" << pX << ", y=" << pY << ", tf=" << tf << endl;

//========================================================

	Mat region;
	region = Mat::zeros( nRows,nCols,CV_8UC1 );

	/*
	TBD:
	Searching for similar area as it is needed every time

	region = searchRegion( image, pX, pY, tf );

	*/

	cout << endl << "The region has been found!" << endl << endl;

//=======================================

	do
	{
		cout << "What do you want to do now? Choose from options:" << endl 
			 << "3 = show region," << endl
			 << "4 = find perimeter," << endl
			 << "5 = save result to the file," << endl
			 << "0 = exit application," << endl
			 << "Your choice is... ";
		cin >> iChoice;
		if( iChoice != 0 && iChoice != 3 && iChoice != 4 && iChoice != 5 )
		{
			cout << "The value outside of allowed range. Try again." << endl;
			continue;
		}
		break;
	}
	while(1);

	if( iChoice == 0 )
		return 0;

	if( iChoice == 3 )
	{
		show_mat(region, "Similar region");
	}
	else if( iChoice == 5 ) 
	{
		std::string outFileName;
		cout << "Enter the file name for output (using quotation marks):";
		cin >> outFileName;
		imwrite( outFileName, region );
	}
	//else iChoice == 4 // find perimeter

//========================================================

	Mat perimeter;
	perimeter = Mat::zeros( nRows,nCols,CV_8UC1 );

	/*
	TBD:
	Searching for perimeter of area with similar color

	perimeter = searchPerimeter( region, value ); //, pX, pY, tf );

	*/

	cout << endl << "The perimeter has been determined!" << endl << endl;

//=======================================
MENU:
	do
	{
		cout << "What do you want to do now? Choose from options:" << endl 
			 << "6 = show perimeter," << endl
			 << "7 = save perimeter to the file," << endl
			 << "0 = exit application," << endl
			 << "Your choice is... ";
		cin >> iChoice;
		if( iChoice != 0 && iChoice != 6 && iChoice != 7 )
		{
			cout << "The value outside of allowed range. Try again." << endl;
			continue;
		}
		break;
	}
	while(1);

	if( iChoice == 0 )
		return 0;

	if( iChoice == 6 )
	{
		show_mat(perimeter, "Area perimeter");
	}
	else if( iChoice == 7 ) 
	{
		std::string outFileName;
		cout << "Enter the file name for the output (using quotation marks):";
		cin >> outFileName;
		imwrite( outFileName, perimeter );
	}


	goto MENU;






	/*


	show_mat(image, "Input");

    //image = invert_mat_pointer(image);
	//image = invert_mat(image);

    show_mat(region, "Output");

	std::string outFileName;
	cout << "Enter the file name for output (using quotation marks):";
	cin >> outFileName;

	imwrite(outFileName, region);
	//imwrite("out.png", image);
	//cvSaveImage(argv[2], &image);
	*/

    return 0;
}

/*
cv::Mat& invert_mat(cv::Mat &mat) {
    // accept only char type matrices
    CV_Assert(mat.depth() == CV_8U);

    const int channels = mat.channels();
    switch (channels) {
        case 1: {
//            gray scale image
            MatIterator_<uchar> it, end;
            for (it = mat.begin<uchar>(), end = mat.end<uchar>(); it != end; ++it)
                *it = ~*it;
            break;
        }
        case 3: {
//            RGB image
            MatIterator_<Vec3b> it, end;
            for (it = mat.begin<Vec3b>(), end = mat.end<Vec3b>(); it != end; ++it) {
                (*it)[0] = ~(*it)[0];
                (*it)[1] = ~(*it)[1];
                (*it)[2] = ~(*it)[2];
            }
        }
    }

    return mat;
}

cv::Mat& invert_mat_pointer(cv::Mat &mat) {

	int channels = mat.channels();
	int nRows = mat.rows;
    int nCols = mat.cols;

    int i, j;
    switch (channels) {
        case 1: {
//            gray scale image
            uchar *p;
            for (i = 0; i < nRows; ++i) {
                p = mat.ptr<uchar>(i);
                for (j = 0; j < nCols; ++j) {
                    p[j] = ~p[j];
                }
            }
        }
        case 3: {
//            RGB image
            Vec3b *p;
            for (i = 0; i < nRows; ++i) {
                p = mat.ptr<Vec3b>(i);
                for (j = 0; j < nCols; ++j) {
                    p[j][0] = ~p[j][0];
                    p[j][1] = ~p[j][1];
                    p[j][2] = ~p[j][2];
                }
            }
        }
    }

    return mat;
}

*/
