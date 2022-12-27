#include<bits/stdc++.h>
#include <opencv2/opencv.hpp>
#include <omp.h>

using namespace cv;
using namespace std;
using namespace std::chrono;

bool processing_mode = false;
bool visualizing_method = true;

vector <int>  gray_to_bgr_pixel(int gray_pixel){
	int red_color, blue_color, green_color;
	if(gray_pixel >= 170){
		red_color = 255; 
		green_color = blue_color = 255 -gray_pixel;
	}
	else if(gray_pixel <=85){
		red_color = green_color = gray_pixel;
		blue_color = 255;
	}
	else{
		red_color = blue_color = gray_pixel;
		green_color = 255;
	}

	vector <int> bgr_color {blue_color, green_color, red_color};

	return bgr_color;
}

int get_average_pixel(Mat &img, int index_i, int index_j, int box_size){
	int sum = 0;
	for(int q = index_i;q<index_i+box_size && q<img.rows;q++){
		for(int t=index_j;t<index_j+box_size && t<img.cols;t++){
			Vec3b &pixel = img.at<Vec3b>(q,t);
			sum+=(pixel.val[0] + pixel.val[1] + pixel.val[2])/3;
		}
	}
	return sum/box_size;
}

void set_new_pixel(Mat &img, int index_i, int index_j, int box_size, vector <int> new_values){
	for(int q = index_i;q<index_i+box_size && q<img.rows;q++){
		for(int t=index_j;t<index_j+box_size && t<img.cols;t++){
			Vec3b &pixel = img.at<Vec3b>(q,t);
			for(int k=0;k<img.channels();k++){
				pixel.val[k] = new_values[k];
			}
		}
	}
}

void convert_image_grayscale_to_colored(Mat img, int box_size){

	namedWindow("Image", WINDOW_NORMAL);
    	setWindowProperty ("Image", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);

	int num_thread_pr_mode = 1; //default is Single Mode

	if(processing_mode)
		num_thread_pr_mode = omp_get_max_threads(); //Multi mode is activated so we took maximum number of thread

	auto start = high_resolution_clock::now();

	#pragma omp parallel for num_threads(num_thread_pr_mode)
	for(int i=0;i<img.rows;i+=box_size){
		for(int j=0;j<img.cols;j+=box_size){
			vector <int> new_values;
			new_values = gray_to_bgr_pixel(get_average_pixel(img,i,j,box_size));
			set_new_pixel(img, i,j, box_size, new_values);

			if(!visualizing_method){
				imshow("Image",img);
				waitKey(1);
			}
		}
		if(visualizing_method){
			imshow("Image",img);
			waitKey(1);
		}
	}

	auto stop = high_resolution_clock::now();
    	auto duration = duration_cast<seconds>(stop - start);

	cout<<"Execution time: "<<duration.count()<<" seconds"<<endl;

	waitKey(3000);

	imwrite("result.jpg", img);
}


int main(int argc, char** argv)
{
	if(argc < 4){
		cout<<"Please add arguments in the format of ./program_name file_name.jpg box_size processing_mode"<<endl<<endl;
		cout<<"You can also add extra last argument for method of visualizing like O or F"<<endl;
		cout<<"The meaning of it visualization for one pixel (O) or full row (F)"<<endl;
		return -1;
	}
	
	vector <string> arg_list(argv+1,argv+argc);
	
	Mat img;
	img = imread(arg_list[0], 1);

	int box_size = stoi(arg_list[1]);
	if (!img.data) {
		printf("No image data \n");
		return -1;
	}


	if(!arg_list[2].compare("S"))
		processing_mode = false;
	else if(!arg_list[2].compare("M"))
		processing_mode = true;
	else{
		cout<<"PLease enter either S or M"<<endl;
		return -1;
	}

	if(arg_list.size() >= 4){
		if(!arg_list[3].compare("O"))
			visualizing_method = true;
		else if(!arg_list[3].compare("F"))
			visualizing_method = false;
		else
			cout<<"You should have entered either O or F, default F visualizing method is chosen"<<endl;
	}
	else{
		cout<<"No visualizing method was added, default F visualizing method is chosen"<<endl;
	}

	convert_image_grayscale_to_colored(img,box_size);

	return 0;
}
