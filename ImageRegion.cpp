

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

int showMenu( int iFileLoaded, int iRegDetermined, int iPerimDetermined )
{
	int iChoice = 0;
	int iAllowTab[8];// = {0};

	do
	{
		memset( iAllowTab, 0, sizeof(iAllowTab) );

		cout << "What do you want to do now? Choose from options:" << endl;

		cout << "0 = exit application," << endl;
		iAllowTab[0] = 1;  // exit possible always

		cout << "1 = load an image," << endl;
		iAllowTab[1] = 1;  // loading possible always

		if( iFileLoaded )
		{
		cout << "2 = show loaded image," << endl;
		iAllowTab[2] = 1;  // 

		cout << "3 = find region with similar color," << endl;
		iAllowTab[3] = 1;  // 

		if( iRegDetermined )
		{
		cout << "4 = show region," << endl;
		iAllowTab[4] = 1;  // 

		cout << "5 = save region to the file," << endl;
		iAllowTab[5] = 1;  // 

		cout << "6 = find perimeter," << endl;
		iAllowTab[6] = 1;  // 

		if( iPerimDetermined )
		{
		cout << "7 = show perimeter," << endl;
		iAllowTab[7] = 1;  // 

		cout << "8 = save perimeter to the file," << endl;
		iAllowTab[8] = 1;  // 
		}
		}
		}

		cout << "Your choice is... ";

		cin >> iChoice;
		if( iChoice < 0 || iChoice > 8 || iAllowTab[iChoice] == 0 )
		{
			cout << "The value outside of allowed range. Try again." << endl;
			continue;
		}
		break;
	}
	while(1);

	return iChoice;
}

int main( int argc, char **argv ) {
//    if (argc != 3) {
//        printf("usage: Opencv_Test <Image_Path> <Output_Path>\n");
//        return -1;
//    }

	int iChoice = 0;
	int isInputFileLoaded = 0,
		isRegionDetermined = 0,
		isPerimeterDetermined = 0;

	Mat image;
	Mat region;
	Mat perimeter;

	std::string inFileName;
	std::string outFileName;

	int tf=0;
	int channels = 0;
	int nRows    = 0;
	int nCols    = 0;
	int pX = nCols, pY = nRows;  // coordinates of origination point

	do  // the main loop of the console application interface
	{
		iChoice = showMenu( isInputFileLoaded, isRegionDetermined, isPerimeterDetermined );

		switch( iChoice )
		{
		case 0:
			//..."0 = exit application,"
			return 0;

		case 1:
			//..."1 = load an image,"
			cout << "Enter the input image file name: ";
			cin >> inFileName;

			image = imread( inFileName, 1 );

			if( !image.data )
			{
				printf( "No image data (warning: OpenCV recognize files by extensions)\n" );
				//return -1;
				break;
			}
			isInputFileLoaded = 1;
			isRegionDetermined = 0;
			isPerimeterDetermined = 0;
			break;

		case 2:
			//..."2 = show loaded image,"
			show_mat( image, "Input image" );
			break;

		case 3:
			//..."3 = find region with similar color,"
			channels = image.channels();
			nRows    = image.rows;
			nCols    = image.cols;

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

			region = Mat::zeros( nRows,nCols,CV_8UC1 );

			/*
			TBD:
			Searching for similar area as it is needed every time

			region = searchRegion( image, pX, pY, tf );

			*/

			cout << endl << "The region has been found!" << endl << endl;
			isRegionDetermined = 1;

			//=======================================
			break;

		case 4:
			//..."4 = show region,"
			show_mat( region, "Similar region" );
			break;
		
		case 5:
			//..."5 = save region to the file,"
			cout << "Enter the file name for output:";
			cin >> outFileName;
			imwrite( outFileName, region );
			break;

		case 6:
			//..."6 = find perimeter,"

			//========================================================

			perimeter = Mat::zeros( nRows,nCols,CV_8UC1 );

			/*
			TBD:
			Searching for perimeter of area with similar color
			perimeter = searchPerimeter( region, value ); //, pX, pY, tf );
			*/

			cout << endl << "The perimeter has been determined!" << endl << endl;

			//=======================================
			isPerimeterDetermined = 1;
			break;

		case 7:
			//..."7 = show perimeter,"
			show_mat( perimeter, "Area perimeter" );
			break;

		case 8:
			//..."8 = save perimeter to the file,"
			cout << "Enter the file name for the output:";
			cin >> outFileName;
			imwrite( outFileName, perimeter );
			break;

		default:
			;
		}

	}
	while( 1 );

	//-----------------


	/*


	show_mat(image, "Input");

    //image = invert_mat_pointer(image);
	//image = invert_mat(image);

    show_mat(region, "Output");

	std::string outFileName;
	cout << "Enter the file name for output:";
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
