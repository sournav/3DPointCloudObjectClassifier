/*
*Developed by Sournav Sekhar Bhattacharya

*SpaceCRAFT/ASTRO Lab

*Texas A&M University College Station

*GIG THEM AGRICULTURALISTS!!
*/

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <Eigen>
using Eigen::ArrayXXd;
using namespace std;

int main(){
	//initialize input stream
	//note this only takes in a modified pcd file with the header replaced 
	//by the number of points
	ifstream inf("specialsphr.pcd");
	int i = 1;
	int x=0;
	int y=0;
	//load in the number of elements
	std::string no_elements;
	inf >> no_elements;
	int n=atoi(no_elements.c_str());
	
	//initialize meta data variables
	double sums [3]={0,0,0};
	double cloud [3][n];
	double temp;
	double max_x=-200,max_y=-200,max_z=-200;
	double min_x=200,min_y=200,min_z=200;
	double maxvals[3] = {0,0,0};
	
	//loading in the points into cloud via input stream from file
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
	//radjust data so there are no negative maxvals
	maxvals[0]=max_x+abs(min_x);
	maxvals[1]=max_y+abs(min_y);
	maxvals[2]=max_z+abs(min_z);
	
	//change the output size of the image here
	int size = 250;
	
	//averages note if the numbe rof points on the PCD file is innacurate this will not be correct
	double avg_x=sums[0]/(double)n;
    double avg_y=sums[1]/(double)n;
    double avg_z=sums[2]/(double)n;
	
	//Using eigin arrays initialized to zero
	ArrayXXd yz_min = ArrayXXd::Zero(size,size);
	ArrayXXd yz_max = ArrayXXd::Zero(size,size);
	ArrayXXd xz_min = ArrayXXd::Zero(size,size);
	ArrayXXd xz_max = ArrayXXd::Zero(size,size);
	ArrayXXd xy_min = ArrayXXd::Zero(size,size);
	ArrayXXd xy_max = ArrayXXd::Zero(size,size);
	
	//initialize temporary x,y,z values
	int temp_x;
	int temp_y;
	int temp_z;
	
	//This makes sure that none of the temp values are out of bounds
	n-=1;
	for(int i=0;i<=n;i++){
		
		//First round normalization to the rest of the point cloud (and making everything positive)
		temp_x=(int)(round(((cloud[0][i]+abs(min_x))/maxvals[0])*n));
		temp_y=(int)(round(((cloud[1][i]+abs(min_y))/maxvals[1])*n));
		temp_z=(int)(round(((cloud[2][i]+abs(min_z))/maxvals[2])*n));
		
		
		//Second round normalization for the rest of the dataset to the size of the image
		temp_x=(int)(round(((double)(temp_x)/(double)(n))*(size-1)));
		temp_y=(int)(round(((double)(temp_y)/(double)(n))*(size-1)));
		temp_z=(int)(round(((double)(temp_z)/(double)(n))*(size-1)));
	
		//assignment of temp values to specific clouds 
		
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
