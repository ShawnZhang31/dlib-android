# 在Android Studio工程中集成Dlib
在Android Studio工程中集成Dlib的方法流程，由于本人是Android小白，有错误之处还请指正。
## 集成前需要了解的概念
 1. Dlib是什么？   
     [Dlib](http://dlib.net/)是一个使用C++开发的包含机器学习的图形算法工具。Dlib可以跨平台应用于各种操作系统：windows、MacOS、Linux、iOS、Android等几乎所有平台。而且效率非常高，这一点需要注意。Dlib只有在Release下才能最大限度的发挥效率，在Debug下的效率非常低下。以Android为例，Dlib在Debug下的帧率只有不到2FPS,但是在Release下未做任何优化的情况下可以轻松达到15FPS+
2. Dlib和OpenCV的关系  
    Dlib是一个算法工具，Dlib自身并不能直接使用，必须先准备好OpenCV，才能使用Dlib，所以在集成Dlib之前，我们必须先集成OpenCV。关于在Android Studio中集成OpenCV的方法请移步:https://github.com/ShawnZhang31/opencv-android-studio 查阅。
3. 在Android工程中Dlib和OpenCV的差异   
   OpenCV采用C/C++进行开发，为了保证运行效率使用了很多的C代码，但是最新的OpenCV中C++代码的比重越来越大。Dlib完全是实用C++开发，主要是实用C++11。OpenCV团队专门为Android设备编译的Android版本的OpenCV，不过OpenCV团队在编译Android版本的时候使用的STL是gnustl_static。而Dlib的作者并没有为任何平台编译二进制库，而是给出了CMakeLists.txt让用户自己编译，而Dlib的作者也比较推崇大家将Dlib的源码合并到工程中使用。所以我们使用Dlib必须自己编译。不过Dlib因为使用的是更现代化的C++，Dlib在编译的时候默认使用的STL是c++_static或者c++_shared。再加上Dlic使用了C++11，所以使用默认配置编译出来的dlib.so文件在和opencv连接的时候总是报出非常多的错误，甚至经常出现文件找不到的情况。   
   Android版本的OpenCV的编译相当复杂，有兴趣的可以参看OpenCV官方给出的方法自行编译:https://github.com/opencv/opencv/wiki/Building_OpenCV4Android_from_trunk 。最后的方法是我们采用OpenCV官方编译好的OpenCV库文件，而将Dlib编译的STL修改为gnustl_static，配合OpenCV使用。  
   **注意:2018年10月16日公布的OpenCV 4.0 Beta版本宣称使用了C++编译器。也就意味着与Dlib的默认编译不再存在冲突。因为是Beta版本，我还没有测试，感兴趣的可以自行测试。不过目前OpenCV 4.0 Beta还没有发布Android版本的SDK**
## 集成步骤
### 集成OpenCV  
 1. 创建一个支持C++ 11的Android Studio工程集成OpenCV并实现使用OpenCV打开摄像头  
    具体集成方法请参阅:https://github.com/ShawnZhang31/opencv-android-studio
### 编译Dlib
  1. 编译环境准备(以我个人电脑为例)
     - 操作系统: MacOS 10.14 
     - NDK版本:R16C
     - Dlib版本:19.15
  2. 创建一个dlib4android文件夹并在该文件夹下创建一个jni文件件   
    文件夹结构如下:   
       ```tree
        dlib4android.
                    └── jni
       ```
  3. 将解压的dlib-19.15文件夹拷贝到jni文件夹下,此时文件结构如下:   
      ```tree
        dlib4android.
                    └── jni
                        └── dlib-19.15
                            ├── CMakeLists.txt
                            ├── dlib
                            ├── documentation.html
                            ├── examples
                            ├── ISSUE_TEMPLATE.md
                            ├── MANIFEST.in
                            ├── python_examples
                            ├── README.md
                            ├── setup.py
                            └── tools
       ```  
     虽然我们编译的时候只需要dlib-19.15下的dlib文件夹即可，但是为了保险方便期间都可以放进去。
  4. 在jni文件夹下创建Android.mk和Application.mk文件，此时文件夹结构如下: 
     ```tree
        dlib4android.
                    └── jni
                        ├── Android.mk
                        ├── Application.mk
                        └── dlib-19.15
                            ├── CMakeLists.txt
                            ├── dlib
                            ├── documentation.html
                            ├── examples
                            ├── ISSUE_TEMPLATE.md
                            ├── MANIFEST.in
                            ├── python_examples
                            ├── README.md
                            ├── setup.py
                            └── tools
       ```  
  5. 编辑Android.mk文件内容如下:   
     ```CMake
        LOCAL_PATH := $(call my-dir)
        include $(CLEAR_VARS)
        LOCAL_MODULE := dlib #定义最终输出的.so文件的名称
        LOCAL_SRC_FILES := $(LOCAL_PATH)/dlib-19.15/dlib/all/source.cpp  # 编译的源文件
        include $(BUILD_SHARED_LIBRARY)
     ```
  6. 编辑Application.mk文件内容如下:   
     ```CMake
        APP_OPTIM:=release #设置Dlib采用Release模式编译，虽然Dlib默认就是使用Release模式编译的，但是在这里强调一下省心
        APP_STL := gnustl_static #为了和OpenCV兼容，STL设置为与OpenCV一直，至于OpenCV 4.0等到发布Android版本的时候再来验证
        APP_CPPFLAGS += -std=c++11 #指定采用c++11编译器
        APP_CPPFLAGS += -frtti #支持c++运行时定义，关闭可以节约内存，不过Android手机的内存都辣么大
        APP_CPPFLAGS += -fexceptions #支持C++异常处理
        APP_CPPFLAGS += -DDLIB_NO_GUI_SUPPORT=1 #交叉编译项 dlib提供了基于xquart的图形界面，该界面不支持Android，关闭该界面的支持
        APP_CPPFLAGS += -DLIB_PNG_SUPPORT  #交叉编译项 支持读取PNG图像
        APP_CPPFLAGS += -DLIB_JPEG_SUPPORT #交叉编译项 支持读取JPG图像
        APP_ABI := x86_64, x86, armeabi-v7a, arm64-v8a #指定需要编译的ABI版本，如果设置为all，则编译所有的ABI平台
        APP_PLATFORM := android-16 #编译的SDK版本
        NDK_TOOLCHAIN_VERSION := clang #工具链
     ```
   7. 编译  
      使用终端(Mac)或者CMD(Windows)cd进入jni文件夹，运行你的NDK安装目录下的ndk-build，此时ndk开始编译。  
      不过你很快就会遇到错误而终止，错误提示可能如下:  
      ```c++
        In file included from ../../../../src/main/cpp/dlib-19.15/dlib/opencv.h:10:
        In file included from ../../../../src/main/cpp/dlib-19.15/dlib/opencv/cv_image.h:10:
        In file included from ../../../../src/main/cpp/dlib-19.15/dlib/opencv/../pixel.h:7:
        ../../../../src/main/cpp/dlib-19.15/dlib/serialize.h:1635:30: error: no member named 'to_string' in namespace 'std'
                        std::to_string(objects_read+1) + "th object from the file " + filename +
                        ~~~~~^
        1 error generated.
        ninja: build stopped: subcommand failed.
      ```
      这时候不用担心，这是因为dlib默认使用的是的STL是c++_static，而我们为了配合opencv官方发布的Android SDK在Application.mk文件中将其指定为gnustl_static。而在gnustl_static中std::string是没有实现to_string的方法和round的方法。这个时候我们就需要在dlib-19.15/dlib下面创建一个.h文件来自己实现这个方法了。我在dlib-19.15/dlib下创建了一个**opencv_dlib_bridge.h**文件，并实现这两个方法。**opencv_dlib_bridge.h**的内容如下:
      ```c++
        /**
         * @file opencv_dlib_bridge.h
         * @author SeventyThree
         * @brief OpenCV使用的是gnustl_static模板编译的，Dlib使用的是c++_static，使用该文件将双方结合在一起
         * @version 0.1
         * @date 2018-10-30
         * 
         * @copyright Copyright (c) 2018
         * 
         */

            #include <string>
            #include <sstream>

            using namespace std;
            namespace std{
                template <typename T> std::string to_string(const T& n)
                {
                    std::ostringstream stm;
                    stm << n;
                    return stm.str();
                }

                template <typename T> T round(T v)
                {
                    return (v>0)?(v+0.5):(v-0.5);
                }
            }
      ```
      然后将opencv_dlib_bridge.h文件include到serialize.h文件中即可。这样重新编译，就可以正常通过编译了。ndk会在dlib4android文件夹下面创建libs文件夹存放对应abi的libdlib.so文件.而obj只是临时文件不用理会。
      ```tree
        dlib4android.
                    ├── jni
                    |   ├── Android.mk
                    |   ├── Application.mk
                    |   └── dlib-19.15
                    |       ├── CMakeLists.txt
                    |       ├── dlib
                    |       ├── documentation.html
                    |       ├── examples
                    |       ├── ISSUE_TEMPLATE.md
                    |       ├── MANIFEST.in
                    |       ├── python_examples
                    |       ├── README.md
                    |       ├── setup.py
                    |       └── tools
                    ├── libs
                    |   └── arm64-v8a
                    |       └── libdlib.so
                    |   └── armeabi-v7a
                    |       └── libdlib.so
                    |   └── x86
                    |       └── libdlib.so
                    |   └── x86_64
                    |       └── libdlib.so
                    └── obj
       ```
    8. APP_CPPFLAGS += -DDLIB_JPEG_STATIC带来错误  
       如果在Application.mk文件中添加了这句，在ndk编译的时候还会出现如下所示的找不到文件的错误:  
       ```c++
        undefined reference to 'jpeg_set_quality(jpeg_compress_struct*, int, int)'
        jni/./dlib/all/../image_saver/save_jpeg.cpp:153: error: undefined reference to 'jpeg_start_compress(jpeg_compress_struct*, int)'
        jni/./dlib/all/../image_saver/save_jpeg.cpp:158: error: undefined reference to 'jpeg_write_scanlines(jpeg_compress_struct*, unsigned char**, unsigned int)'
        jni/./dlib/all/../image_saver/save_jpeg.cpp:161: error: undefined reference to 'jpeg_finish_compress(jpeg_compress_struct*)'
       ```
       这也是gnustl_stactic造成的，我们需要在jpeg_loader.cpp文件中加上**extern "C"**  
       ```c++
        #ifdef DLIB_JPEG_SUPPORT

        #include "../array2d.h"
        #include "../pixel.h"
        #include "../dir_nav.h"
        #include "jpeg_loader.h"
        #include <stdio.h>
        #ifdef DLIB_JPEG_STATIC
        extern "C"{
        #   include "../external/libjpeg/jpeglib.h"
        }
        #else
        #   include <jpeglib.h>
        #endif
        #include <sstream>
        #include <setjmp.h>
       ```
 
