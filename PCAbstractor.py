import cv2
#img = cv2.imread('bob.png',0)



def pc_dataLoad(img):
    res=[]
    x=0
    y=0
    for i in img:
        y=0
        for j in i:
            if j!=0:
                res.append([x,y,j])
                
            y+=1
        x+=1
    
    return res
            
def closest(pt,pts):
    min_dist=1000000
    fpt=[]
    for j in pts:
        x_comp=j[0]-pt[0]
        y_comp=j[1]-pt[1]
        dist=x_comp^2+y_comp^2
        if dist!=0 and dist<=min_dist:
                min_dist=dist
                fpt=[j[0],j[1],j[2]]
                
    #print(pt,fpt)                
    return min_dist,fpt
#x=pc_dataLoad(img)
#print(closest(x[1],x))

def rect(img,pt1,pt2,col):
    
    x1=max([pt1[0],pt2[0]])
    y1=max([pt1[1],pt2[1]])
    x2=min([pt1[0],pt2[0]])
    y2=min([pt1[1],pt2[1]])
    for i in range(x2,x1+1):
        for j in range(y2,y1+1):
            img[i][j]=col
    return img

def fill(img):
    res=pc_dataLoad(img)
    #print(len(res))
    for i in res:
        
        _,pt=closest(i,res)

        #print(i,pt)
        #print(i[2])
        img=rect(img, (i[0],i[1]), (pt[0],pt[1]), int(i[2]))
    return img
##img=fill(img)
###img=rect(img, [7,10], [15,5],100)
##cv2.imwrite('imgr.jpg',img)
