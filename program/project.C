#include "opencv2/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<string>
#include<fstream>
#include<math.h>
#include<malloc.h>

using namespace cv;
using namespace std;

int per_num;
int i_per_person ;
void get_per_name(char **per_name){
	string str;
	int i = 0;
	ifstream file("name.txt");
	while (getline(file,str)){
		strcpy(per_name[i++],str.c_str());
	}
	file.close();
	
}
void get_name(String test_per_name,char *name){
     int i = 0,k=0;
     int count = 0;
     while(test_per_name[i]!='\0')
		i++;
     while(count!=2){
		if(test_per_name[i]=='/')
			count++;
		if(count==2)
			i=i+2;
		i--;
     }
     while(test_per_name[i]!='/')
		name[k++] = test_per_name[i++];
     name[k] = '\0';
}

/********************************************collect LBP features********************************************/

void lbp(unsigned char *ar,string str,int i,int v,int row,int col){
	int k = 0;
	Mat img = imread(str,CV_LOAD_IMAGE_GRAYSCALE);
	for ( ;i<row;i++) {
		for(int j = v ;j<col;j++) {
			int t = 0;
			t = img.at<uchar>(i-1,j-1) >=  img.at<uchar>(i,j)   ? (t<<1)|1:t<<1;
			t = img.at<uchar>(i-1,j)   >=  img.at<uchar>(i,j)   ? (t<<1)|1:t<<1;
			t = img.at<uchar>(i-1,j+1) >=  img.at<uchar>(i,j)   ? (t<<1)|1:t<<1;
			t = img.at<uchar>(i,j+1)   >=  img.at<uchar>(i,j)   ? (t<<1)|1:t<<1;
			t = img.at<uchar>(i+1,j+1) >=  img.at<uchar>(i,j)   ? (t<<1)|1:t<<1;
			t = img.at<uchar>(i+1,j)   >=  img.at<uchar>(i,j)   ? (t<<1)|1:t<<1;
			t = img.at<uchar>(i+1,j-1) >=  img.at<uchar>(i,j)   ? (t<<1)|1:t<<1;
			t = img.at<uchar>(i,j-1)   >=  img.at<uchar>(i,j)   ? (t<<1)|1:t<<1;
			ar[k++] = t; 
		}
	}	
}

 /******************************************Euclidean Distance****************************************************/
 double euclidean_distance(unsigned char a1[],unsigned char a2[],int size){
	double result = 0;
	for (int i=0;i<size;i++)
		result+=pow(a1[i]-a2[i],2);
	return sqrt(result);
 }
 double g_his_euclidean_distance(unsigned char a1[],unsigned char a2[],int size){
	double result = 0;
	for (int i=0;i<size;i++)
		result+=pow(a1[i]-a2[i],2);
	return sqrt(result);
 }
double d_euclidean_distance(double a1[],double a2[],int size){
	double result = 0;
	for (int i=0;i<size;i++)
		result+=pow(a1[i]-a2[i],2);
	return sqrt(result);
 }



 /******************************************PARTITION OF IMAGAES****************************************************/
void partition(int block_size,int pixels,string str,unsigned char ***new_ar){
	int new_row = 0;
	int col = 0;
        for (int i = 0; i<=pixels;i+=block_size) {
		    for (int j = 0;j<=pixels;j+=block_size )  {
	        		lbp(new_ar[new_row][col++],str,i+1,j+1,i+block_size-1,j+block_size-1);
				//cout<<i+1<<"-"<<i+block_size-1<<"X"<<j+1<<"-"<<j+block_size-1<<" ";
				}
			//cout<<endl;
			col = 0;
			new_row++;
		   }
}
/******************************************HISTOGRAM****************************************************/
void histogram(unsigned char *ar,unsigned char *his,int pixels){
	 for (int i = 0;i<pixels;i++)
		his[ar[i]/5]++;
}

void g_histogram(int *ar,int *his,int pixels){
	 for (int i = 0;i<pixels;i++)
		his[ar[i]/5]++;
}





/******************************************MAIN PROGRAM****************************************************/
int main(){	
/**********************retrieving how many person and image**********************/
   int no_of_person = 0,no_of_image = 0;
    ifstream filer("test.txt");
        string strr="empty";
	char prev_per_name[10];
	char new_per_name[10];
	getline(filer,strr);
	if(strcmp(strr.c_str(),"empty")!=0){
		get_name(strr,prev_per_name);
		no_of_person++;
		no_of_image++;
	}	
    while (getline(filer,strr)) {
	get_name(strr,new_per_name);
	if(strcmp(new_per_name,prev_per_name)!=0){
		strcpy(prev_per_name,new_per_name);
		no_of_person++;
	}
	no_of_image++;
		
    }
     filer.close();
    cout<<"no of person = "<<no_of_person;
    per_num = no_of_person;
    cout<<" image per person = "<<no_of_image/no_of_person<<endl;
    i_per_person = no_of_image/no_of_person;
/*******************************************************************************/

  double result_eu[per_num];
  int result_per[per_num];

ifstream test_file("test_images.txt");
string str1;
char name[10];
char **per_name = new char*[per_num];
for(int i=0;i<per_num;i++)
	per_name[i] = new char[10];
get_per_name(per_name);

// unsigned char ar[per_num][i_per_person][10][10][64];
   unsigned char *****ar = new unsigned char ****[per_num];
   for (int i=0;i<per_num;i++) {
	ar[i] = new unsigned char ***[i_per_person];
	for (int j = 0;j<i_per_person;j++) {
		ar[i][j] = new unsigned char **[10];
		for (int k=0;k<10;k++) {
			ar[i][j][k] = new unsigned char *[10];
			for (int l=0;l<10;l++) {
				ar[i][j][k][l] = new unsigned char [64];
			}
		}
	}
  }

    string loc_of_img[per_num];
    int per = 0, ino = 0;
    ifstream file("test.txt");
    int part_no = 10;
    int block_size = 100/part_no;
        string str;
    while (getline(file,str)) {
        partition(block_size,99,str,ar[per][ino]);
	ino++;
	if (ino==i_per_person) {
		loc_of_img[per] = str;
	        //cout<<loc_of_img[per]<<endl;
		ino = 0;
		per++;
	}
    }
     file.close();
		

  // unsigned char his[per_num][i_per_person][10][10][52];
    unsigned char *****his = new unsigned char ****[per_num];
   for (int i=0;i<per_num;i++) {
	his[i] = new unsigned char ***[i_per_person];
	for (int j = 0;j<i_per_person;j++) {
		his[i][j] = new unsigned char **[10];
		for (int k=0;k<10;k++) {
			his[i][j][k] = new unsigned char *[10];
			for (int l=0;l<10;l++) {
				his[i][j][k][l] = new unsigned char [52];
			}
		}
	}
  }
    

    for (int i = 0;i<per_num;i++)
	for(int j = 0;j<i_per_person;j++)
	      for (int m = 0;m<10;m++)
		for(int k=0;k<10;k++) {
			 for (int l = 0;l<52;l++)
				his[i][j][m][k][l]=0;
   			 histogram(ar[i][j][m][k],his[i][j][m][k],64);
		}



while(getline(test_file,str1)){
    get_name(str1,name);
    // unsigned char test[10][10][64];
       unsigned char ***test = new unsigned char **[10];
   for (int i=0;i<10;i++) {
	test[i] = new unsigned char *[10];
	for (int j = 0;j<10;j++) {
		test[i][j] = new unsigned char [64];
	}
    }
    
     partition(block_size,99,str1,test);
     unsigned char test_his[10][10][52];
    for(int i=0;i<10;i++){
	for (int j = 0;j<10;j++){
        	 for (int l = 0;l<52;l++)
			test_his[i][j][l] = 0;
	 histogram(test[i][j],test_his[i][j],64);
	}
    }
    
     double eu=0;
     double min_eu  = 999999999;
     int res = 0;
     for (int i = 0;i<per_num;i++) {
	for (int j=0;j<i_per_person;j++){
	    for (int m = 0;m<10;m++)
		for (int k=0;k<10;k++)
			eu+=g_his_euclidean_distance(his[i][j][m][k],test_his[m][k],52);
		//cout<<eu<<endl;
		///min_eu>eu ? min_eu = eu, res = i : min_eu, i;
		///eu = 0;
	}
	//eu = eu/4;
	result_per[i] = i;
	result_eu[i] = eu;
	min_eu>eu ? min_eu = eu, res = i : min_eu, i;
	//cout<<eu<<endl;
	eu = 0;
	//cout<<endl;
     }
	/******************ascending order of eu****************/
	for(int i=0;i<per_num-1;i++)
		for (int j=i+1;j<per_num;j++){
			if(result_eu[i]>result_eu[j]){
				double temp_eu = result_eu[i];
				result_eu[i] = result_eu[j];
				result_eu[j] = temp_eu;
				int temp_per = result_per[i];
				result_per[i]=result_per[j];
				result_per[j] = temp_per;
			}
		}
	/*******************************************************/
	
	/********************displaying result******************/
	Mat result_image(400,600,CV_8UC3);
	Mat newimg1 = imread(str1,CV_LOAD_IMAGE_COLOR);
	Mat resimg1 = imread(loc_of_img[result_per[0]],CV_LOAD_IMAGE_COLOR);
	Mat resimg2 = imread(loc_of_img[result_per[1]],CV_LOAD_IMAGE_COLOR);
	Mat resimg3 = imread(loc_of_img[result_per[2]],CV_LOAD_IMAGE_COLOR);
	for (int i=0;i<400;i++)
		for (int j=0;j<600;j++)
			result_image.at<Vec3b>(i,j) = 37;
	for (int i=50;i<150;i++) {
		for(int j=250;j<350;j++) {
			result_image.at<Vec3b>(i,j) = newimg1.at<Vec3b>(i-50,j-250);
		}
	}
	for (int i=250;i<350;i++){
		if(per_num>=1)
		for(int j=50;j<150;j++)
			result_image.at<Vec3b>(i,j) = resimg1.at<Vec3b>(i-250,j-50);
		if(per_num>=2)
		for(int j=250;j<350;j++)
			result_image.at<Vec3b>(i,j) = resimg2.at<Vec3b>(i-250,j-250);
		if(per_num>=3)
		for(int j=450;j<550;j++)
			result_image.at<Vec3b>(i,j) = resimg3.at<Vec3b>(i-250,j-450);
	}
	putText(result_image,"test image", Point2f(200,45), FONT_HERSHEY_PLAIN, 2,  Scalar(0,0,255,255));
	putText(result_image,per_name[result_per[0]], Point2f(50,245), FONT_HERSHEY_PLAIN, 2,  Scalar(0,255,0,255));
	if(per_num>=1){
	putText(result_image,to_string(result_eu[0]), Point2f(50,365), FONT_HERSHEY_PLAIN, 1,  Scalar(0,0,255,255));
	putText(result_image,"best result", Point2f(50,395), FONT_HERSHEY_PLAIN, 1,  Scalar(0,255,0,255));
	}
	if(per_num>=2){
	putText(result_image,per_name[result_per[1]], Point2f(250,245), FONT_HERSHEY_PLAIN, 2,  Scalar(0,255,0,255));
	putText(result_image,to_string(result_eu[1]), Point2f(250,365), FONT_HERSHEY_PLAIN, 1,  Scalar(0,0,255,255));
	putText(result_image,"possible", Point2f(250,395), FONT_HERSHEY_PLAIN, 1,  Scalar(0,255,0,255));
	}
	if(per_num>=3){
	putText(result_image,per_name[result_per[2]], Point2f(450,245), FONT_HERSHEY_PLAIN, 2,  Scalar(0,255,0,255));
	putText(result_image,to_string(result_eu[2]), Point2f(450,365), FONT_HERSHEY_PLAIN, 1,  Scalar(0,0,255,255));	
	putText(result_image,"possible", Point2f(450,395), FONT_HERSHEY_PLAIN, 1,  Scalar(0,255,0,255));
	}

	putText(result_image,"DATABASE", Point2f(430,50), FONT_HERSHEY_PLAIN, 1,  Scalar(0,255,0,255));
	putText(result_image,"number of person: "+to_string(per_num), Point2f(370,70), FONT_HERSHEY_PLAIN, 1,  Scalar(0,255,0,255));
	putText(result_image,"image per person: "+to_string(i_per_person), Point2f(370,85), FONT_HERSHEY_PLAIN, 1,  Scalar(0,255,0,255));
	imshow("face recognition",result_image);
	waitKey(0);

}
	test_file.close();
}
