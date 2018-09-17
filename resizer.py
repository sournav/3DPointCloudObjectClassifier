import cv2
import numpy as np
##img=cv2.imread('bobeth.jog.png',0)
def resize(img,tgt,col=0):
    y,x=img.shape
    if x>=tgt and y>=tgt:
        
        return cv2.resize(img, (tgt,tgt))
    elif x>=tgt and y<tgt:
        cv2.resize(img, (x,tgt))
    elif x<tgt and y>=tgt:
        cv2.resize(img,(tgt,y))
    else:
        rando=2
        #left blank on purpose
        
    y,x=img.shape
    x=round((tgt-x)/2)
    y=round((tgt-y)/2)
    
    img = cv2.copyMakeBorder(img,y,y,x,x,cv2.BORDER_CONSTANT,value=col)
    return cv2.resize(img, (tgt,tgt))

##cv2.imshow('asa0s',img)
##
##imgr=resize(img,39)
##            
##                       
##
##        
##        
##        
##
##cv2.imshow('asas',imgr)
##
