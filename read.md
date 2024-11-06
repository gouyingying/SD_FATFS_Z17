# SD卡框架使用流程

## CUBEMX部分

### 开启外部低速时钟，开启RTC，并配置时钟树使SD卡端分频为48MHz

![image-20241106201914390](C:/Users/ZONE7/AppData/Roaming/Typora/typora-user-images/image-20241106201914390.png)



### 由于硬件封装的需要将SD卡设置为4线模式

![image-20241106133355333](E:\工作记录\SD\image-20241106133355333.png)



### 将分频因数改为4，使得SD卡读写频率设置为8MHz

![{30073609-2F96-4E82-8942-63A5CE752D0C}]({30073609-2F96-4E82-8942-63A5CE752D0C}.png)

![image-20241106133524729](E:\工作记录\SD\image-20241106133524729.png)

### 开启SDIO中断且优先级务必高于其DMA中断的优先级

![image-20241106133821518](E:\工作记录\SD\image-20241106133821518.png)

![image-20241106134035423](C:\Users\ZONE7\AppData\Roaming\Typora\typora-user-images\image-20241106134035423.png)

### 第二个警告是最新版的CUBE会在FATFS配置时多一项引脚测试，但硬件上并不存在，只需使用一路GPIO_INPUT，即可消除警告

### 第三个只需按路径勾选即可

![image-20241106134215095](E:\工作记录\SD\image-20241106134215095.png)

### 阻止cube自动进行sdio的初始化，原因代码部分给出

![image-20241106202107253](C:/Users/ZONE7/AppData/Roaming/Typora/typora-user-images/image-20241106202107253.png)

## 代码部分

![image-20241106202349488](C:/Users/ZONE7/AppData/Roaming/Typora/typora-user-images/image-20241106202349488.png)

### 这是CUBE生成的SDIO初始化，这行代码中会将SD卡总线宽度设置为4，这会导致后续sd卡的BSP初始化出现问题，导致SD卡不能正常使用，所以我们将其复制更改为一个新的初始化函数使用。

![image-20241106203029696](C:/Users/ZONE7/AppData/Roaming/Typora/typora-user-images/image-20241106203029696.png)

### 随后，需要复制四个文件，如图

![image-20241106203126714](image-20241106203126714.png)

### 这里主要写的是SD卡操作的用户接口和文件管理系统的用户接口

![{4A7B8809-0E81-4734-B7CD-535DADE977A3}](C:/Users/ZONE7/AppData/Local/Packages/MicrosoftWindows.Client.CBS_cw5n1h2txyewy/TempState/ScreenClip/{4A7B8809-0E81-4734-B7CD-535DADE977A3}.png)

### 进行初始化，请务必注意SDIO初始化和FATFS初始化的顺序，否则会导致挂载不成功

![{01999A17-FE18-4A5F-87B3-A0E2720C86F8}](C:/Users/ZONE7/AppData/Local/Packages/MicrosoftWindows.Client.CBS_cw5n1h2txyewy/TempState/ScreenClip/{01999A17-FE18-4A5F-87B3-A0E2720C86F8}.png)

![{0EDE0FDE-C643-4638-BB0D-9F7DF96DE302}](C:/Users/ZONE7/AppData/Local/Packages/MicrosoftWindows.Client.CBS_cw5n1h2txyewy/TempState/ScreenClip/{0EDE0FDE-C643-4638-BB0D-9F7DF96DE302}.png)

### 这一部分是挂载以及初始化文件，初始化文件的意义是每次上电接受的数据能够被容易被分离出来，更容易处理，大致效果如下

![image-20241106203905253](C:/Users/ZONE7/AppData/Roaming/Typora/typora-user-images/image-20241106203905253.png)

### 主要的存储函数是这样。函数内部已经写好了每一步的注释，思路就是，打开文件、跳到末尾、写数据、关闭保存。

### ![{44355D60-B793-4BDF-94C5-88572F3EC195}](C:/Users/ZONE7/AppData/Local/Packages/MicrosoftWindows.Client.CBS_cw5n1h2txyewy/TempState/ScreenClip/{44355D60-B793-4BDF-94C5-88572F3EC195}.png)



### 需要注意的是目前SD卡写不了float类型数据，为了方便和准确度我将所有数据都乘了100，后续在上位机端处理分析即可，由于函数内部用的是uint_32类型的数据操作，在传入数据尤其是float类型数据时最好进行数据类型转化，如果32位不够（虽然很扯，就只需要在函数内改变就好）

![image-20241106210634458](C:/Users/ZONE7/AppData/Roaming/Typora/typora-user-images/image-20241106210634458.png)

### 该函数是不定长收发的，调用时只需要传入，文件名，数据数量，和数据即可，也可多次调用，每次调用换行一次，可以按需使用

## 最后，有一些坑

## 在使用时如果已经开启过FreeRTOS千万记得配置CMake，不然你可能疯掉，不知道是不是bug，两个同时开的时候他就不自动改cMake了

## 切记，切记

### 对，还有就是我没有封装创建（会导致覆盖丢东西，或不连续存入），所以最好先用你想用的文件名生成一个文件在SD卡里，再使用。或者使用f_open函数，最后一个参数为FA_Create

