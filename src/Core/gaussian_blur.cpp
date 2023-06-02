#include <vector>
#include <omp.h>
#include <math.h>
#include <cmath>
using namespace std;

#define KERNEL_SIZE 5
const short KERNEL_RADIUS = (KERNEL_SIZE-1)/2;
/**
 * @brief generic Gaussian Blur function on a 3-dimensional grid
 * 
 * @tparam T input datatype
 * @param data input data grid
 * @param std_dev standard deviation used to generate the Gauss Kernel
 */
template<class T>
void gaussian_blur(vector<vector<vector<T>>> &data, const float std_dev){

    int dim_x = data.size(), dim_y = data[0].size(), dim_z = data[0][0].size();
    float kernel[KERNEL_SIZE];
    float variance = pow(std_dev,2);
    float sum = 0;

    //filling the kernel
    for(int i=0; i<KERNEL_SIZE; i++) {
        kernel[i] = exp(-pow(i-KERNEL_RADIUS, 2)/2/variance); //equation for gaussian distribution
        sum += kernel[i];
    }
    for(int i=0; i<KERNEL_SIZE; i++) {
        kernel[i] /= sum; //normalizing, the sum of kernel should be 1
    }

    
    int coordinate;

    //blurring along the x axis
    #pragma omp parallel for
    for(int y=0; y<dim_y; y++){
        vector<T> output_data(dim_x, 0); //saving a single row of output entries to save memory.
        for(int z=0; z<dim_z; z++){
            for(int x=0; x<dim_x; x++){
                for(int i=0; i<KERNEL_SIZE; i++){
                    coordinate = x-KERNEL_RADIUS+i;
                    if(coordinate<0)coordinate=0; //rounding if coordinate is out of bounds
                    if(coordinate>=dim_x)coordinate=dim_x-1;

                    output_data[x] += kernel[i]*data[coordinate][y][z];
                }                
            }
            for(int x=0; x<dim_x; x++){
                data[x][y][z]=output_data[x]; //writing data
                output_data[x] = 0; //resetting output vector
            }
        }
    }

    //blurring along the y axis
    #pragma omp parallel for
    for(int x=0; x<dim_x; x++){
        vector<T> output_data(dim_y, 0);
        for(int z=0; z<dim_z; z++){
            for(int y=0; y<dim_y; y++){
                for(int i=0; i<KERNEL_SIZE; i++){
                    coordinate = y-KERNEL_RADIUS+i;
                    if(coordinate<0)coordinate=0;
                    if(coordinate>=dim_y)coordinate=dim_y-1;

                    output_data[y] += kernel[i]*data[x][coordinate][z];
                }
            }
            for(int y=0; y<dim_y; y++){
                data[x][y][z]=output_data[y];
                output_data[y] = 0;
            }
        }
    }

    //blurring along the z axis
    #pragma omp parallel for
    for(int x=0; x<dim_x; x++){
        vector<T> output_data(dim_z, 0);
        for(int y=0; y<dim_y; y++){
            for(int z=0; z<dim_z; z++){
                for(int i=0; i<KERNEL_SIZE; i++){
                    coordinate = z-KERNEL_RADIUS+i;
                    if(coordinate<0)coordinate=0;
                    if(coordinate>=dim_z)coordinate=dim_z-1;

                    output_data[z] += kernel[i]*data[x][y][coordinate];
                }
            }
            for(int z=0; z<dim_z; z++){
                data[x][y][z]=output_data[z];
                output_data[z] = 0;
            }
        }
    }
}