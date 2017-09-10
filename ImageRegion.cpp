
#include <stdio.h>
#include <stdlib.h>  // for abs()
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class CPoint
{
	public:
        int x, y;
        CPoint() : x(0), y(0) {}
        CPoint(int x, int y) : x(x), y(y) {}
};

/**
Input:
imIn	- reference to the matrix with input image data
pX, pY	- coordinates of the point around which the area with similar color will be searched
simf	- similarity factor; says how much every color component (R,G,B) can differ from origination point
		  to by clasified to the similarity region
Output:
imOut	- reference to the matrix where output data can be stored
Returned values:
0       - operation succedded,
-1      - operation failed.
*/
int find_region( Mat& imIn, const int pX, const int pY, const int simf, Mat& imOut )
{
	int channels = imIn.channels();

	/*  we assume that image is RGB */
	if( channels != 3 )
	{
		cout << "Only RGB images can be processed" << endl;
		return -1;   // we assume that program serves only RGB images
	}

	int nRows    = imIn.rows;
    int nCols    = imIn.cols;
	imOut = Mat::zeros( nRows, nCols, CV_8UC1 );

	Vec3b* rin;   // input row
	uchar* rout;  // output row

	rin = imIn.ptr<Vec3b>(pY);  // row in

	int refB = rin[pX][0];
	int	refG = rin[pX][1];
	int	refR = rin[pX][2];

	list<CPoint> tListOld;
	list<CPoint> tListNew;

	CPoint cp(pX,pY);
	tListOld.push_back( cp );

	int x = 0, y=0;

	int iter_counter = 0;
	int point_counter;

	while(1)
	{
		point_counter = 0;   // for debug 

		list<CPoint>::iterator iter = tListOld.begin();
		for( ; iter!=tListOld.end(); iter++ ) // the whole list to be analized
		{
			x = iter->x;
			y = iter->y;

			rout = imOut.ptr<uchar>(y); // row out

			// modify output
			rout[x] = 255;

			// check neighbours
			if( y > 0 )
			{
				rin = imIn.ptr<Vec3b>(y-1);  // previous
				rout = imOut.ptr<uchar>(y-1); // 
				if( rout[x] != 0 )
				{
					;  // already served
				}
				else if( abs( rin[x][0]-refB) <= simf &&  abs( rin[x][1]-refG) <= simf && abs( rin[x][2]-refR) <= simf ) 
				{
					cp.x = x; 
					cp.y = y-1;
					tListNew.push_back( cp );
					rout[x] = 255;  // 
				}
				else
					rout[x] = 1;  // means, the point was checked
			}

			if( y < nRows-1 )
			{
				rin = imIn.ptr<Vec3b>(y+1);  // next
				rout = imOut.ptr<uchar>(y+1); // 
				if( rout[x] != 0 )
				{
					;  // already served
				}
				else if( abs(rin[x][0]-refB) <= simf &&  abs(rin[x][1]-refG) <= simf && abs(rin[x][2]-refR) <= simf ) 
				{
					cp.x = x; 
					cp.y = y+1;
					tListNew.push_back( cp );
					rout[x] = 255;  // 
				}
				else
					rout[x] = 1;  // means, the point was checked
			}

			if( x > 0 )
			{
				rin = imIn.ptr<Vec3b>(y);  // 
				rout = imOut.ptr<uchar>(y); // 
				if( rout[x-1] != 0 )
				{
					;  // already served
				}
				else if( abs(rin[x-1][0]-refB) <= simf &&  abs(rin[x-1][1]-refG) <= simf && abs(rin[x-1][2]-refR) <= simf ) 
				{
					cp.x = x-1; 
					cp.y = y;
					tListNew.push_back( cp );
					rout[x-1] = 255;  // 
				}
				else
					rout[x-1] = 1;  // means, the point was checked
			}

			if( x < nCols-1 )
			{
				rin = imIn.ptr<Vec3b>(y);  // 
				rout = imOut.ptr<uchar>(y); // 
				if( rout[x+1] != 0 )
				{
					;  // already served
				}
				else if( abs(rin[x+1][0]-refB) <= simf &&  abs(rin[x+1][1]-refG) <= simf && abs(rin[x+1][2]-refR) <= simf ) 
				{
					cp.x = x+1; 
					cp.y = y;
					tListNew.push_back( cp );
					rout[x+1] = 255;  // 
				}
				else
					rout[x+1] = 1;  // means, the point was checked
			}
			point_counter++;
		}

		iter_counter++;
		//cout << "DBG: iter no. " << iter_counter << ", number of added points: " << point_counter << endl;

		if( tListNew.size() == 0 )
			break;

		tListOld = tListNew;
		tListNew.clear();
	}

	int i,j;
	// erasing of markers that pixel has been already checked
	for( i=0; i<nRows; i++ )
	{
		rout = imOut.ptr<uchar>(i);
		for( j=0; j<nCols; j++ )
			if( rout[j] == 1 )
				rout[j] = 0;
	}

	return 0;
}

//--------------------------------------------

/**
Input:
imIn	- reference to the matrix with input image data
          (2-dimensional region determined within method find_region)
Output:
imOut	- reference to the matrix where output data can be stored
Returned values:
0       - operation succedded,
-1      - operation failed.
*/
int find_perimeter( Mat& imIn, Mat& imOut )
{
	int channels = imIn.channels();

	/*  we assume that image is gray scale */
	if( channels != 1 )
	{
		cout << "Only GRAY SCALE images can be processed" << endl;
		return -1;   // we assume that this method serves only gray scale objects
	}

	int nRows = imIn.rows;
    int nCols = imIn.cols;
	imOut = Mat::zeros( nRows, nCols, CV_8UC1 );

	uchar* rinp;  // input row (previous)
	uchar* rin;   // input row
	uchar* rinn;  // input row (next)
	uchar* rout;  // output row

	int i, j;

	for( i=0; i<nRows; i++ )
	{
		rin = imIn.ptr<uchar>(i);

		for( j=0; j<nCols; j++ )
		{
			if( rin[j] == 255 )
			{
				if( i == 0 || i == nRows-1 || j == 0 || j == nCols-1 )  // the edge of image with pixel within region
				{
					rout = imOut.ptr<uchar>(i);
					rout[j] = 255;
				}
				else
				{
					rinp = imIn.ptr<uchar>(i-1);
					rinn = imIn.ptr<uchar>(i+1);

					if( rinp[j] == 0 || rinp[j+1] == 0 || rin[j+1] == 0 || rinn[j+1] == 0 || rinn[j] == 0 || rinn[j-1] == 0 || rin[j-1] == 0 || rinp[j-1] == 0 )
					{
						rout = imOut.ptr<uchar>(i);
						rout[j] = 255;
					}
				}
			}
		}
	}

	return 0;
}

//-----------------------------

/**
Input:
image	 - reference to the matrix (3-dimensional) with input image data
win_name - the name for the output window frame
*/
void display_image( const Mat& image, string const& win_name )
{
    namedWindow( win_name, WINDOW_AUTOSIZE );
    imshow( win_name, image );
    waitKey( 0 );
	destroyWindow( win_name );
	return;
}

/**
Input:
image	 - reference to the matrix with input image data
          (2-dimensional region or perimeter determined earlier)
win_name - the name for the output window frame
*/
void display_pixels( const Mat& image, string const& win_name )
{
    namedWindow( win_name, WINDOW_AUTOSIZE );
    imshow( win_name, image );
    waitKey( 0 );
	destroyWindow( win_name );
	return;
}

/**
Input:
image	 - reference to the matrix with input image data
          (2-dimensional region or perimeter determined earlier)
filename - the name for the output image file (with extension)
*/
void save_pixels( const Mat& image, string const& filename )
{
	imwrite( filename, image );
	return;
}

#include <iostream>
#include <fstream>

/**
Input:
image	 - reference to the matrix with input image data
          (2-dimensional region or perimeter determined earlier)
filename - the name for the output text file
*/
void save_to_text_file( Mat& image, string const& filename )
{
	int channels = image.channels();
	int nRows = image.rows;
    int nCols = image.cols;

	ofstream out_file;
	out_file.open( filename );

	//if( image.depth() == CV_8U )
	if( channels == 1 )   // GRAY_SCALE
	{
		out_file << "GRAY_SCALE;\n";
	}
	else  // RGB
	{
		out_file << "RGB_IMAGE;\n";
	}

	out_file << nCols << ";" << nRows << ";\n";

	int iWrapCnt = 0;
	int i, j;

	if( channels == 1 )
	{
		uchar* rin;   // input row
		//int cl;

		for( i=0; i<nRows; i++ )
		{
			rin = image.ptr<uchar>(i);
			for( j=0; j<nCols; j++ )
			{
				//cl = rin[j];
				if( rin[j] != 0 )
				{
					//out_file << i << "," << j << "," << cl << ";";
					// we can ommit a color of pixel as we know that image (here in our application) has only two colors (black and white),
					// thanks to it the output file will have smaller size
					out_file << i << "," << j << ";";
					iWrapCnt++;

					if( iWrapCnt > 15 )
					{
						out_file << "\n";
						iWrapCnt = 0;
					}
				}
			}
		}
	}
	else  // RGB  // just in case, for the future use maybe
	{
		Vec3b* rin;   // input row
		int clr, clg, clb;

		for( i=0; i<nRows; i++ )
		{
			rin = image.ptr<Vec3b>(i);
			for( j=0; j<nCols; j++ )
			{
				clb = rin[j][0];
				clg = rin[j][1];
				clr = rin[j][2];
				if( clb != 0 || clg != 0 || clr != 0 )  // other than black
				{
					out_file << i << "," << j << "," << clr << "," << clg << "," << clb << ";";
					iWrapCnt++;

					if( iWrapCnt > 10 )
					{
						out_file << "\n";
						iWrapCnt = 0;
					}
				}
			}
		}
	}

	out_file.close();

	return;
}

//----------------------------------------

/**
Input:
imIn	- reference to the matrix with input image data
          (2-dimensional region determined within method find_region)
smF     - smoothing factor,
Output:
imOut	- reference to the matrix where output data can be stored
Returned values:
0       - operation succedded,
-1      - operation failed.
*/
int find_smooth_perimeter( Mat& imIn, int smF, Mat& imOut )
{
	int channels = imIn.channels();

	/*  we assume that image is gray scale */
	if( channels != 1 )
	{
		cout << "Only GRAY SCALE images can be processed" << endl;
		return -1;   // we assume that this method serves only gray scale objects
	}

	int nRows = imIn.rows;
    int nCols = imIn.cols;
	imOut = Mat::zeros( nRows, nCols, CV_8UC1 );

	Mat imWork = Mat::zeros( nRows, nCols, CV_8UC1 );

	uchar* rin;   // input row
	uchar* rout;  // output row

	int i, j, k, l;              // counters
	int smU, smR, smD, smL;      // limits for counters for local smoothing
	int limU, limR, limD, limL;  // limits for counters for global smoothing
	//int inc = smF/6 + 1;       // for optimisation of computing time for bigger smF value
	int inc = 1;

	// for optimisation purpose we limit area where computing will be performed
	limU = nRows-1;
	limR = 0;
	limD = 0;
	limL = nCols-1;

	for( i=0; i<nRows; i++ )
	{
		rin = imIn.ptr<uchar>(i);
		for( j=0; j<nCols; j++ )
		{
			// determine limits
			if( rin[j] == 255 )
			{
				if( i > limD )
					limD = i;
				if( i < limU )
					limU = i;
				if( j < limL )
					limL = j;
				if( j > limR )
					limR = j;
			}
		}
	}

	if( limL-smF <= 0 )			limL = 0;
	else						limL -= smF;

	if( limR+smF >= nCols-1 )	limR = nCols-1;
	else						limR += smF;

	if( limU-smF <= 0 )			limU = 0;
	else						limU -= smF;

	if( limD+smF >= nRows-1 )	limD = nRows-1;
	else						limD += smF;

	// for optimisation purpose - end

	for( i=limU; i<=limD; i++ )
	{
		// determine local smoothing area
		if( i-smF > 0 )
			smU = i-smF;
		else
			smU = 0;

		if( i+smF < nRows-1 )
			smD = i+smF;
		else
			smD = nRows-1;

		rout = imWork.ptr<uchar>(i);
		for( j=limL; j<=limR; j++ )
		{
			// determine limits
			if( j-smF > 0 )
				smL = j-smF;
			else
				smL = 0;

			if( j+smF < nCols-1 )
				smR = j+smF;
			else
				smR = nCols-1;

			float sum = 0;
			int count = 0;
			for( k=smU; k<=smD; k+=inc )
			{
				rin = imIn.ptr<uchar>(k);
				for( l=smL; l<=smR; l+=inc )
				{
					sum += rin[l];
					count++;
				}
			}
			rout[j] = (uchar)(sum/count);
		}
	}
	
	// tresholding now
	for( i=0; i<nRows; i++ )
	{
		rin = imWork.ptr<uchar>(i);
		for( j=0; j<nCols; j++ )
		{
			if( rin[j] <= 128 )
				rin[j] = 0;
			else
				rin[j] = 255;
		}
	}

	//alternatively with using OpenCV methods:
	//cv::blur( imIn, imOut, cv::Size( 2*smF,2*smF) );
	//cv::threshold( imOut, imWork, 128, 255, THRESH_BINARY );

	find_perimeter( imWork, imOut );

	return 0;
}


//----------------------------------------

/**
Input:
iFileLoaded      - control variable, it decides how the menu should look like, allows to show appropriate menus options
iRegDetermined   -                   -||-
iPerimDetermined -                   -||-
Returned values:
iChoice          - the option, user choice from menu
*/
int showMenu( const int iFileLoaded, const int iRegDetermined, const int iPerimDetermined )
{
	int iChoice = 0;
	int iAllowTab[20];

	do
	{
		memset( iAllowTab, 0, sizeof(iAllowTab) );

		cout << endl << "What do you want to do now? Choose from options:" << endl;

		cout << "0 = exit application," << endl;
		iAllowTab[0] = 1;  // exit possible always

		cout << "1 = load an image," << endl;
		iAllowTab[1] = 1;  // loading possible always

		if( iFileLoaded )
		{
		cout << "2 = display loaded image," << endl;
		iAllowTab[2] = 1;  // 

		// for debug only
		//cout << "15 = save RGB image to the text file," << endl;
		//iAllowTab[15] = 1;  // 

		cout << "3 = find region of similar color," << endl;
		iAllowTab[3] = 1;  // 

		if( iRegDetermined )
		{
		cout << "4 = display region (pixels)," << endl;
		iAllowTab[4] = 1;  // 

		cout << "5 = save region to the file," << endl;
		iAllowTab[5] = 1;  // 

		cout << "6 = find perimeter," << endl;
		iAllowTab[6] = 1;  // 

		cout << "7 = find smooth perimeter," << endl;
		iAllowTab[7] = 1;  // 

		if( iPerimDetermined )
		{
		cout << "8 = display perimeter (pixels)," << endl;
		iAllowTab[8] = 1;  // 

		cout << "9 = save perimeter to the file," << endl;
		iAllowTab[9] = 1;  // 
		}

		cout << "10 = save region to text file," << endl;
		iAllowTab[10] = 1;  // 

		if( iPerimDetermined )
		{
		cout << "11 = save perimeter to text file," << endl;
		iAllowTab[11] = 1;  // 
		}
		}
		}

		cout << "Your choice is... ";

		cin >> iChoice;
		if( iChoice < 0 || iChoice > 11 || iAllowTab[iChoice] == 0 )
		//if( iChoice < 0 || iChoice > 15 || iAllowTab[iChoice] == 0 )  // for debug only
		{
			cout << "The value outside of allowed range. Try again." << endl;
			continue;
		}
		break;
	}
	while(1);

	return iChoice;
}

//============================================================================================

int main( int argc, char **argv )
{
    if( argc != 1 ) 
	{
        cout << "Usage: just launch ImageRegion app and follow with using available options (menu will be shown)." << endl;
        return -1;
    }

	Mat image;
	Mat region;
	Mat perimeter;

	string inFileName;
	string outFileName;

	int iChoice = 0;

	int isInputFileLoaded = 0,
		isRegionDetermined = 0,
		isPerimeterDetermined = 0;

	int nRows = 0;
	int nCols = 0;
	int pX = nCols, pY = nRows;  // coordinates of origination point
	int simf = 0;   // similarity factor
	int smF = 0;    // smoothing factor

	do  // the main loop of the console application interface
	{
		iChoice = showMenu( isInputFileLoaded, isRegionDetermined, isPerimeterDetermined );

		switch( iChoice )
		{
		case 0:		// exit application
			return 0;

		case 1:		// load an image
			cout << "Enter the input image filename (with extension): ";
			cin >> inFileName;

			image = imread( inFileName, 1 );

			if( !image.data )
			{
				printf( "No image data (warning: OpenCV recognize files by extensions)\n" );
				break;
			}
			isInputFileLoaded = 1;
			isRegionDetermined = 0;
			isPerimeterDetermined = 0;
			break;

		case 2:		// show loaded image
			display_image( image, "Input image" );
			break;

		case 3:		// find region with similar color
			nRows    = image.rows;
			nCols    = image.cols;

			cout << "Enter starting point for determining homogeneous region (in regards to pixels color)." << endl;

			do
			{
				cout << "Enter X coordinate of pixel [0-" << (nCols-1) << "]: ";
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
				cout << "Enter Y coordinate of pixel [0-" << (nRows-1) << "]: ";
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
				cout << "Enter the similarity factor [1-255]: ";
				cin >> simf;
				if( simf < 1 || simf > 100 )
				{
					cout << "The value outside of allowed range. Try again." << endl;
					continue;
				}
				break;
			} while(1);

			//-----------------

			if( find_region( image, pX, pY, simf, region ) != 0 )
				break;

			cout << endl << "The region has been marked!" << endl << endl;

			isRegionDetermined = 1;
			isPerimeterDetermined = 0;
			break;

		case 4:		// show region
			display_pixels( region, "The region of similar pixels" );
			break;
		
		case 5:		// save region to the file
			cout << "Enter the output filename (with extension): ";
			cin >> outFileName;

			save_pixels( region, outFileName );
			break;

		case 6:		// find perimeter
			// Drawing perimeter of area with similar color
			if( find_perimeter( region, perimeter ) != 0 )
				break;

			cout << endl << "The perimeter has been determined!" << endl << endl;

			isPerimeterDetermined = 1;
			break;

		case 7:		// find smooth perimeter
			do
			{
				cout << "Enter the smoothing factor [1-20]: ";
				cin >> smF;
				if( smF < 1 || smF > 20 )
				{
					cout << "The value outside of allowed range. Try again." << endl;
					continue;
				}
				break;
			} while(1);

			// Drawing smooth perimeter of area with similar color
			if( find_smooth_perimeter( region, smF, perimeter ) != 0 )
				break;

			cout << endl << "The perimeter has been determined!" << endl << endl;

			isPerimeterDetermined = 1;
			break;

		case 8:		// show perimeter
			display_pixels( perimeter, "The perimeter of region" );
			break;

		case 9:		// save perimeter to the file
			cout << "Enter the output filename (with extension): ";
			cin >> outFileName;

			save_pixels( perimeter, outFileName );
			break;

		case 10:		// save region to the text file
			cout << "Enter the output filename: ";
			cin >> outFileName;

			save_to_text_file( region, outFileName );
			break;

		case 11:	// save perimeter to the text file
			cout << "Enter the output filename: ";
			cin >> outFileName;

			save_to_text_file( perimeter, outFileName );
			break;

		// for debug only
		/*
		case 15:	// save RGB image to the text file
			cout << "Enter the output filename (with extension): ";
			cin >> outFileName;

			save_to_text_file( image, outFileName );
			break;
			*/

		default:
			;
		}
	}
	while( 1 );

    return 0;
}

//void show_mat(const cv::Mat &image, std::string const &win_name);
//cv::Mat &invert_mat(cv::Mat &mat);
//cv::Mat& invert_mat_pointer(cv::Mat &mat);

/*
    //image = invert_mat_pointer(image);
	//image = invert_mat(image);

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
*/
