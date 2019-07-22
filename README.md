# carriage_number_detection_using_OpneCV

## Environment
Win10+VS2017+OpenCV3.4.6

## Results
In this work, we complete the task of carriage number detection in a set of images. 
The binary images can be obtained by performing grayscale processing and thresholding on the input images. 
Then we detect the edges using Canny operator and make two groups of closing operation and opening operation with them. 
Finally, only the character area with more texture edge information can be retained after such operation, and the character detection process is completed. 
The cut characters are obtained by the x-y projection method.
After template matching, the recognized characters are printed on the original image to complete the character recognition process at last.

![detection and recognization result](https://i.loli.net/2019/07/22/5d35b93450f1046484.png) 

![detection and recognization result](https://i.loli.net/2019/07/22/5d35b933d793d89189.png) 

More details are in [here](https://blog.csdn.net/BenJamin_Blue/article/details/96892971) 
