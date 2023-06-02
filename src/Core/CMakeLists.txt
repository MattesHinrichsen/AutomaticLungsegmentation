cmake_minimum_required(VERSION 3.16)

project(Lung_Segmentation)

add_compile_options(-D QT_FREE -O3 -fopenmp -std=c++17 -march=native)

add_link_options(-fopenmp)

add_executable(main main.cpp
                batch_mode.cpp
                Disjoint_Set.cpp
                Nifti.cpp
                Scan.cpp
                Segmentation.cpp
                scale.cpp
                gaussian_blur.cpp)