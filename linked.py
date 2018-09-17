import PCDataLoad as dl
import PCAbstractor as ab
import resizer as rs
import cv2
import numpy as np
def loadin_exist():
    ret=np.load('ret.npy')
    lbl=np.load('lbl.npy')
    ret2=np.load('ret2.npy')
    lbl2=np.load('lbl2.npy')
    return ret,lbl,ret2,lbl2
    
def loadin():
    x=['sqr','sphr']
    ret=[]
    lbl=[]
    ret2=[]
    lbl2=[]
    ret_labels=[]
    cnt=0
    print('start')
    
    for j in range(110):
        cnt=0
        for i in x:
            if j%30==0:
                print('x')
            if j<=2:
                cloud=dl.DataLoader('../data_pcd/'+x[cnt]+str(j)+'.pcd')
                
                for i in range(6):
                    cloud[i]=rs.resize(cloud[i],250)
                ret.append(cloud)
                
                if x[cnt]=='sqr':
                    lbl.append(0)
                else:
                    lbl.append(1)
            else:
                cloud=dl.DataLoader('../data_pcd/'+x[cnt]+str(j)+'.pcd')
                for i in range(6):
                    cloud[i]=rs.resize(cloud[i],250)
                    
                ret2.append(cloud)
                if x[cnt]=='sqr':
                    lbl2.append(0)
                else:
                    lbl2.append(1)
        
            cnt+=1
    #print(ret)
    try:
        np.save('ret',ret)
        np.save('lbl',lbl)
        np.save('ret2',ret2)
        np.save('lbl2',lbl2)
    except:
        print('saving unsuccesful')
        
    return ret,lbl,ret2,lbl2
def loadin_pred(file):
    ret=[]
    cloud=dl.DataLoader(file)
    for i in range(6):
        cloud[i]=rs.resize(ab.fill(cloud[i]),250)
        cv2.imwrite('as'+str(i)+'.jpg',cloud[i])
    ret.append(cloud)
    return ret
    
##_,x,_,y=loadin()
##print(x)
##print(y)
