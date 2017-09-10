

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

int find_region( cv::Mat& imIn, int pX, int pY, int tf, cv::Mat& imOut )
{
	int channels = imIn.channels();
	int nRows    = imIn.rows;
    int nCols    = imIn.cols;

	/*  we assume that image is RGB */
	if( channels != 3 )
	{
		cout << "Only RGB images can be processed" << endl;
		return -1;   // we assume that program serves only RGB images
	}

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
			// ...
			if( y > 0 )
			{
				rin = imIn.ptr<Vec3b>(y-1);  // previous
				rout = imOut.ptr<uchar>(y-1); // 
				if( rout[x] != 0 )
				{
					;  // already served
				}
				else if( abs( rin[x][0]-refB) <= tf &&  abs( rin[x][1]-refG) <= tf && abs( rin[x][2]-refR) <= tf ) 
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
				else if( abs(rin[x][0]-refB) <= tf &&  abs(rin[x][1]-refG) <= tf && abs(rin[x][2]-refR) <= tf ) 
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
				else if( abs(rin[x-1][0]-refB) <= tf &&  abs(rin[x-1][1]-refG) <= tf && abs(rin[x-1][2]-refR) <= tf ) 
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
				else if( abs(rin[x+1][0]-refB) <= tf &&  abs(rin[x+1][1]-refG) <= tf && abs(rin[x+1][2]-refR) <= tf ) 
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

		cout << "DBG: iter no. " << iter_counter << ", number of added points: " << point_counter << endl;

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

	/*
	for( i=0; i<nRows; i++ )
	{
		p = imIn.ptr<Vec3b>(i);
		rout = imOut.ptr<uchar>(i);
		for( j=0; j<nCols; j++ )
		{
			//if( i < pY && j < pX || i > pY && j > pX )
			if( p[j][0] + p[j][1] + p[j][2] < 370 )
			{
				//o[j][0] = ~p[j][0];
				//o[j][1] = ~p[j][1];
				//o[j][2] = ~p[j][2];
				o[j] = 255;
			}
			else
			{
				//p[j][0] = ~p[j][0];
				//p[j][1] = ~p[j][1];
				//p[j][2] = ~p[j][2];
				o[j] = 128;
			}
		}
	}
	*/

	return 0;
}

//--------------------------------------------

int find_perimeter( cv::Mat& imIn, cv::Mat& imOut )
{
	int channels = imIn.channels();
	int nRows    = imIn.rows;
    int nCols    = imIn.cols;

	/*  we assume that image is gray scale */
	if( channels != 1 )
	{
		cout << "Only GRAY SCALE images can be processed" << endl;
		return -1;   // we assume that this method serves only gray scale objects
	}

	uchar* rinp;  // input row (previous)
	uchar* rin;   // input row
	uchar* rinn;  // input row (next)
	uchar* rout;  // output row

	/*
	int cU = 0,
		cUR = 0,
		cR = 0,
		cDR = 0,
		cD = 0,
		cDL = 0,
		cL = 0,
		cUL = 0;
		*/

	int i, j;

	for( i=0; i<nRows; i++ )
	{
		/*
		if( i == 0 )
			rinp = NULL;
		else
			rinp = imIn.ptr<uchar>(i-1);
		*/
		rin = imIn.ptr<uchar>(i);
		/*
		if( i == nRows-1 )
			rinn = NULL;
		else
			rinn = imIn.ptr<uchar>(i+1);
		*/

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

					/*
					if( i == 0 )
						cUL = cU = cUR = 0;
					if( j == 0 )
						cUL = cL = cDL = 0;
					if( i == nRows-1 )
						cDL = cD = cDR = 0;
					if( j == nCols-1 )
						cUR = cR = cDR = 0;
					*/
					/*
					cU = rinp[j];
					cUR = rinp[j+1];
					cR = rin[j+1];
					cDR = rinn[j+1];
					cD = rinn[j];
					cDL = rinn[j-1];
					cL = rin[j-1];
					cUL = rinp[j-1];
					*/

					//if( cU == 0 || cUR == 0 || cR == 0 || cDR == 0 || cD == 0 || cDL == 0 || cL == 0 || cUL == 0 )
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

		cout << endl << "What do you want to do now? Choose from options:" << endl;

		cout << "0 = exit application," << endl;
		iAllowTab[0] = 1;  // exit possible always

		cout << "1 = load an image," << endl;
		iAllowTab[1] = 1;  // loading possible always

		if( iFileLoaded )
		{
		cout << "2 = display loaded image," << endl;
		iAllowTab[2] = 1;  // 

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

		if( iPerimDetermined )
		{
		cout << "7 = display perimeter (pixels)," << endl;
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

/*
class CImageRGB
{
private:
	;
protected:
	;
public:
	;
}
*/

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
	int tf = 0;   // tolerance factor

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
				//return -1;
				break;
			}
			isInputFileLoaded = 1;
			isRegionDetermined = 0;
			isPerimeterDetermined = 0;
			break;

		case 2:		// show loaded image
			show_mat( image, "Input image" );
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
				cout << "Enter the color tolerance factor [1-255]: ";
				cin >> tf;
				if( tf < 1 || tf > 100 )
				{
					cout << "The value outside of allowed range. Try again." << endl;
					continue;
				}
				break;
			} while(1);

			//cout << "DBG: x=" << pX << ", y=" << pY << ", tf=" << tf << endl;

			//========================================================

			region = Mat::zeros( nRows,nCols,CV_8UC1 );

			if( find_region( image, pX, pY, tf, region ) != 0 )
				break;

			cout << endl << "The region has been marked!" << endl << endl;

			isRegionDetermined = 1;
			isPerimeterDetermined = 0;

			//=======================================
			break;

		case 4:		// show region
			show_mat( region, "Similar region" );
			break;
		
		case 5:		// save region to the file
			cout << "Enter the output filename (with extension): ";
			cin >> outFileName;
			imwrite( outFileName, region );
			break;

		case 6:		// find perimeter
			perimeter = Mat::zeros( nRows,nCols,CV_8UC1 );

			// Drawing perimeter of area with similar color
			find_perimeter( region, perimeter ); //, pX, pY, tf );

			cout << endl << "The perimeter has been determined!" << endl << endl;

			isPerimeterDetermined = 1;
			break;

		case 7:		// show perimeter
			show_mat( perimeter, "Area perimeter" );
			break;

		case 8:		// save perimeter to the file
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

    //image = invert_mat_pointer(image);
	//image = invert_mat(image);

    return 0;
}

//void show_mat(const cv::Mat &image, std::string const &win_name);
//cv::Mat &invert_mat(cv::Mat &mat);
//cv::Mat& invert_mat_pointer(cv::Mat &mat);

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
