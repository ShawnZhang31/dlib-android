
// The MIT License (MIT)

// Copyright (c) SevetyThree

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.

/**
 * @file opencv_dlib_bridge.h
 * @author SeventyThree(shawnzhang31@gmail.com)
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