# 功能

播放音乐，显示波形图的测试代码

# 实现

* miniaudio 播放音乐
* sdl2 显示波形图

# 编译

* 下载sdl
    - 下载页面：<https://www.libsdl.org/download-2.0.php>  
    - mingw: <https://www.libsdl.org/release/SDL2-devel-2.0.22-mingw.zip>

* 修改cmake/FindSDL2.cmake 中 sdl2 所在路径(注意路径分隔符使用**/**)

* 修改main.c 中音乐的路径

* 编译命令

```shell
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```

# 运行程序

bin目录下为编译好的程序，自动播放同目录下的 1.mp3

![波形图效果](./bin/1.png)