# 使用组件Event Recorder进行STM32调试

## 参考资料
[Keil MDK高级调试——Event Recorder](https://blog.csdn.net/twx11213030422/article/details/114013796)  
[STM32H7的终极调试组件Event Recorder](https://www.cnblogs.com/armfly/p/10757582.html)  

## 实现效果
1. 单片机上电后，蜂鸣器响2次以提示初始化成功，并开始4LED跑马灯。  
2. 在Keil中打开调试工具后，可以通过修改watch窗口中timess（蜂鸣器响的次数）的值来实时改变蜂鸣器响的次数，并通过Event Reacorder工具记下运行时间（如图1所示），除此之外，在Debug(printf) Viewer窗口中可以观察到蜂鸣器响的次数和停止响的文字提示（如图2所示）。  
![图1](.\p1.png)  
![图2](.\p2.png)  


3. 在蜂鸣器响过程中，由于使用了FreeRTOS进行程序编写，LED跑马灯效果不会受其影响。
