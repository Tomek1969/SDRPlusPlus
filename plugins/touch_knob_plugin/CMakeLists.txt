cmake
cmakecmake_minimum_required(VERSION 3.13)
project(touch_knob_plugin)
add_library(touch_knob_plugin MODULE main.cpp)
target_link_libraries(touch_knob_plugin PRIVATE sdrpp_core)
target_compile_features(touch_knob_plugin PRIVATE cxx_std_17)
