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

More details are in [HERE](https://blog.csdn.net/BenJamin_Blue/article/details/96892971) 

<div align=center><img width="480" height="640" src="https://i.loli.net/2019/07/22/5d35b933d793d89189.png"/></div>


<div align=center><img width="480" height="640" src="https://i.loli.net/2019/07/22/5d35b93450f1046484.png"/></div>


## Contributors
The project is mainly created by Yihe Wu (BenJamin Blue) in NJUST, June 2019.

Thanks Zehua Zhao, Hao Zhang, Yaobin Mao and bloggers in CSDN for giving helpful supports.

## Citation
Please cite these papers in your publications if it helps your research:

```
@inproceedings{wu2019carriage_number_detection_using_OpneCV,\n
  author = {BenJamin Blue},
  
  title = {carriage_number_detection_using_OpneCV},
  
  year = {2019}
  
}
```

## License
The project is freely available for free non-commercial use, and may be redistributed under these conditions. For commercial queries, please drop an e-mail at 750166816@qq.com. We will send the detail agreement to you.
