"""
ALL CODE WRITTEN BY SOURNAV SEKHAR BHATTACAHRYA UNLESS OTHERWISE
SPECIFIED.

Verion 0.1
PC_DataLoad
"""
import pcl
import numpy as np
import cv2
import PCAbstractor as ab

def DataLoader(file):
    #loading in the raw pcd file
    file =pcl.io.file_io.loadpcd(file)
    
    cloud=file.xyz
    
    sum_x=0
    sum_y=0
    sum_z=0
    cnt=0
    max_x=0
    max_y=0
    max_z=0
    min_x=0
    min_y=0
    min_z=0
    maxvals=[0,0,0]
    #finds sums,maxs,mins
    for i in cloud:
        sum_x+=i[0]
        sum_y+=i[1]
        sum_z+=i[2]
        cnt+=1
        if min_x>i[0]:
            min_x=i[0]
        if min_y>i[1]:
            min_y=i[1]
        if min_z>i[2]:
            min_z=i[2]
        #cnt+=1
        if max_x<i[0]:
            max_x=i[0]
        if max_y<i[1]:
            max_y=i[1]
        if max_z<i[2]:
            max_z=i[2]
    #process to create maxvals for data normalization
    maxvals[0]+=(max_x+abs(min_x))
    maxvals[1]+=(max_y+abs(min_y))
    maxvals[2]+=(max_z+abs(min_z))
    #figuring out dimensions
    max_x=int(round(cnt))
    max_y=int(round(cnt))
    max_z=int(round(cnt))
    
    
    #creating channels
    xy_max=np.zeros((max_x,max_y))
    xy_min=np.zeros((max_x,max_y))
    yz_max=np.zeros((max_y,max_z))
    yz_min=np.zeros((max_y,max_z))
    xz_max=np.zeros((max_x,max_z))
    xz_min=np.zeros((max_x,max_z))

    #finding averages
    avg_x=sum_x/cnt
    avg_y=sum_y/cnt
    avg_z=sum_z/cnt
    maxs=[0,0,0]

    #heavy work (comments to come in later)
    for i in cloud:
        x=int(round(((i[0]+abs(min_x))/maxvals[0])*max_x))-1
        y=int(round(((i[1]+abs(min_y))/maxvals[1])*max_y))-1
        z=int(round(((i[2]+abs(min_z))/maxvals[2])*max_z))-1
        ax=avg_x*max_x
        ay=avg_y*max_y
        az=avg_z*max_z
        x=int(round((x/max_x)*255))
        y=int(round((y/max_y)*255))
        z=int(round((z/max_z)*255))

        
        if i[0]<avg_x:
            
            yz_min[y][z]=x          
        else:
             yz_max[y][z]=x
        if i[1]<avg_y:
            xz_min[x][z]=y
        else:
            xz_max[x][z]=y
        if i[2]<avg_z:
            xy_min[x][y]=z
        else:
            xy_max[x][y]=z
        
        med=[yz_min,yz_max,xz_min,xz_max,xy_min,xy_max]
        cnt=0
        for i in med:
            cv2.imwrite('../scrnsht/img_ip'+str(cnt)+'.jpg',i)
            cnt+=1
            

    return [yz_min,yz_max,xz_min,xz_max,xy_min,xy_max]

           
        
