#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
//#include <opencv2/opencv.hpp>
#include <Eigen>
using Eigen::ArrayXXd;
using namespace std;
int main(){
	
	ifstream inf("specialsphr.pcd");
	int i = 1;
	int x=0;
	int y=0;
	std::string no_elements;
	inf >> no_elements;
	int n=atoi(no_elements.c_str());
	double sums [3]={0,0,0};
	double cloud [3][n];
	double temp;
	double max_x=-200,max_y=-200,max_z=-200;
	double min_x=200,min_y=200,min_z=200;
	double maxvals[3] = {0,0,0};
	while(inf){
		
		std:string str;
		inf>>str;
		temp=atof(str.c_str());
		sums[0]+=temp;
		cloud[0][y]=temp;
		if(min_x>temp){
			min_x=temp;
		}
		if(max_x<temp){
			max_x=temp;
		}
		inf>>str;
		temp=atof(str.c_str());
		sums[1]+=temp;
		cloud[1][y]=temp;
		if(min_y>temp){
			min_y=temp;
		}
		if(max_y<temp){
			max_y=temp;
		}
		inf>>str;
		temp=atof(str.c_str());
		sums[2]+=temp;
		cloud[2][y]=temp;
		if(min_z>temp){
			min_z=temp;
		}
		if(max_z<temp){
			max_z=temp;
		}
		y+=1;
		
		
		
	}
	maxvals[0]=max_x+abs(min_x);
	maxvals[1]=max_y+abs(min_y);
	maxvals[2]=max_z+abs(min_z);
	int size = 250;
	double avg_x=sums[0]/(double)n;
    double avg_y=sums[1]/(double)n;
    double avg_z=sums[2]/(double)n;
	cout<<"max: "<<max_x<<","<<max_y<<","<<max_z<<endl;
	cout<<"min: "<<min_x<<","<<min_y<<","<<min_z<<endl;
	ArrayXXd yz_min = ArrayXXd::Zero(size,size);
	ArrayXXd yz_max = ArrayXXd::Zero(size,size);
	ArrayXXd xz_min = ArrayXXd::Zero(size,size);
	ArrayXXd xz_max = ArrayXXd::Zero(size,size);
	ArrayXXd xy_min = ArrayXXd::Zero(size,size);
	ArrayXXd xy_max = ArrayXXd::Zero(size,size);
	
	int temp_x;
	int temp_y;
	int temp_z;
	n-=1;
	for(int i=0;i<=n;i++){
		temp_x=(int)(round(((cloud[0][i]+abs(min_x))/maxvals[0])*n));
		temp_y=(int)(round(((cloud[1][i]+abs(min_y))/maxvals[1])*n));
		temp_z=(int)(round(((cloud[2][i]+abs(min_z))/maxvals[2])*n));
		int temp_x2=temp_x;
		temp_x=(int)(round(((double)(temp_x)/(double)(n))*(size-1)));
		temp_y=(int)(round(((double)(temp_y)/(double)(n))*(size-1)));
		temp_z=(int)(round(((double)(temp_z)/(double)(n))*(size-1)));
	
		
	
		if(cloud[0][i]<avg_x){
		yz_min(temp_y,temp_z)=temp_x;}
		else{
		yz_max(temp_y,temp_z)=temp_x;}
		if(cloud[1][i]<avg_y){
		xz_min(temp_x,temp_z)=temp_y;}
		else{
		xz_max(temp_x,temp_z)=temp_y;}
		if(cloud[2][i]<avg_z){
		yz_min(temp_x,temp_y)=temp_z;}
		else{
		yz_max(temp_x,temp_y)=temp_z;}
		
		
	}
	
}
