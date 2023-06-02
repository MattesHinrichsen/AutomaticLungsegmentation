#include <vector>
#include <omp.h>
using namespace std;

/**
 * @brief generic Trilinear Scaling function on a 3-dimensional grid
 * 
 * @tparam T input datatype
 * @param new_size_x output dimensions
 * @param new_size_y output dimensions
 * @param new_size_z output dimensions
 * @param data input data grid 
 */
template<class T>
void bilinear_resize(int new_size_x, int new_size_y, int new_size_z, vector<vector<vector<T>>> &data){ 
    vector<vector<vector<T>>> resized_data = vector<vector<vector<T>>>(new_size_x, vector<vector<T>>(new_size_y, vector<T>(new_size_z)));

    int old_size_x = data.size();
    int old_size_y = data[0].size();
    int old_size_z = data[0][0].size();

    float x_ratio = ((float)(old_size_x-1))/(new_size_x-1);
    float y_ratio = ((float)(old_size_y-1))/(new_size_y-1);
    float z_ratio = ((float)(old_size_z-1))/(new_size_z-1);

    #pragma omp parallel for
    for(int x=0; x<new_size_x; x++){
        for(int y=0; y<new_size_y; y++){
            for(int z=0; z<new_size_z; z++){
                int x_floor = (int)(x_ratio*x);
                int y_floor = (int)(y_ratio*y);
                int z_floor = (int)(z_ratio*z);
                int x_ceil  = (int)(x_ratio*x) +1;
                int y_ceil  = (int)(y_ratio*y) +1;
                int z_ceil  = (int)(z_ratio*z) +1;

                float offset_x = x_ratio*x - (float) x_floor;
                float offset_y = y_ratio*y - (float) y_floor;
                float offset_z = z_ratio*z - (float) z_floor;

                x_ceil -= (x_ceil == old_size_x);
                y_ceil -= (y_ceil == old_size_y);
                z_ceil -= (z_ceil == old_size_z);

                //a whole bunch of interpolation
                T d_000 = data[x_floor][y_floor][z_floor];
                T d_001 = data[x_ceil] [y_floor][z_floor];
                T d_010 = data[x_floor][y_ceil] [z_floor];                
                T d_011 = data[x_ceil] [y_ceil] [z_floor];
                T d_100 = data[x_floor][y_floor][z_ceil] ;
                T d_101 = data[x_ceil] [y_floor][z_ceil] ;
                T d_110 = data[x_floor][y_ceil] [z_ceil] ;
                T d_111 = data[x_ceil] [y_ceil] [z_ceil] ;

                T d_00i = (1-offset_x)*d_000 + offset_x*d_001;
                T d_01i = (1-offset_x)*d_010 + offset_x*d_011;
                T d_10i = (1-offset_x)*d_100 + offset_x*d_101;
                T d_11i = (1-offset_x)*d_110 + offset_x*d_111;

                T d_0ii = (1-offset_y)*d_00i + offset_y*d_01i;
                T d_1ii = (1-offset_y)*d_10i + offset_y*d_11i;

                T d_iii = (1-offset_z)*d_0ii + offset_z*d_1ii;

                //writing results
                resized_data[x][y][z] = d_iii;
            }
        }
    }

    data = resized_data;
}