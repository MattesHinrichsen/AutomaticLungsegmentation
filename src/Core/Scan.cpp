#include "Scan.h"
#include "Segmentation.h"
#include "Disjoint_Set.h"
#include <iostream>
#include <omp.h>
using namespace std;

Segmentation Scan::create_segmentation(int N_CORES, int threshold, int smallest_group, int biggest_group, int* num_found) {

    //This can't actually be changed anymore
    const int search_range = 1; 

    //Creating and setting up the segmentation
    Segmentation seg;
    seg.set_header_in_accordance_to_scan(this);

    Disjoint_Set sets;
    
    //Arrays for the label before it gets written into the segmentation aswell as for the thresholding
    vector<vector<vector<bool>>> thresh = vector<vector<vector<bool>>>(hdr.dim[3], vector<vector<bool>>(hdr.dim[2], vector<bool>(hdr.dim[1], false)));
    vector<vector<vector<unsigned int>>> pre_label = vector<vector<vector<unsigned int>>>(hdr.dim[3], vector<vector<unsigned int>>(hdr.dim[2], vector<unsigned int>(hdr.dim[1], 0)));


    //Pre thresholding
    for(int i=0;i<hdr.dim[3];i++) {
        for(int j=0;j<hdr.dim[2];j++) {
            for(int k=0;k<hdr.dim[1];k++) {
                if (data[i][j][k] <= threshold) thresh[i][j][k] = true;
            }
        }
    }

    //Pass one of HK
    int current_label = 1;
    #pragma omp parallel num_threads(N_CORES)
    {
        // Store the first i for every thread. Needed to rerun the first layer once the "above" thread is done. 
        int first_i;
        bool passed_first = false;

        #pragma omp for schedule(static)
        for(int i=0;i<hdr.dim[3];i++) {
            
            if(!passed_first) {
                first_i = i;
                passed_first = true; 
            }    

            for(int j=0;j<hdr.dim[2];j++) {
                for(int k=0;k<hdr.dim[1];k++) {
                    if (thresh[i][j][k]) {
                        
                        //List of labels found around current voxels
                        int labels[search_range+1][search_range+1][search_range+1] = {0};
                        //Minimum label among the surrounding ones
                        int min_label = 2100000000;

                        bool is_empty = true;
                        for(int x = -search_range; x<=0;x++) {
                            for(int y = -search_range; y<=0;y++) {
                                for(int z = -search_range; z<=0;z++) {
                                    if (!(x==0 && y==0 && z==0) && (i+x)>=0 && (j+y)>=0 && (k+z)>=0 && thresh[i+x][j+y][k+z] && (i+x)>=first_i) { 
                                        
                                        //Populating surrounding label array
                                        labels[-x][-y][-z] = pre_label[i+x][j+y][k+z];
                                        is_empty = false;
                                        if(pre_label[i+x][j+y][k+z] < min_label) min_label = pre_label[i+x][j+y][k+z];
                                    }
                                }
                            }
                        }

                        
                        if (!is_empty) {
                            //Unioning the surrounding values with the min value
                            pre_label[i][j][k] = min_label;
                            for(int x = 0; x<=search_range;x++) {
                                for(int y = 0; y<=search_range;y++) {
                                    for(int z = 0; z<=search_range;z++) { 
                                        if (labels[x][y][z] != 0 && min_label != labels[x][y][z]) {
                                            #pragma omp critical 
                                            {
                                                sets.union_sets(min_label, labels[x][y][z]);
                                            }
                                        }
                                    }
                                }
                            }
                        } else {
                            //No surrounding labels where found. Creating new label.
                            #pragma omp critical
                            {
                                pre_label[i][j][k] = current_label;
                                sets.make_set(current_label);
                                current_label++;
                            }
                        }
                    }
                }
            }
        }

        //Joining all threads to ensure that all are finished when they redo the first layer.
        #pragma omp barrier

        //Same as above but only on the first layer of every thread
        if (first_i > 0) {
            for(int j=0; j<hdr.dim[2]; j++) {
                for(int k=0; k<hdr.dim[1]; k++) {
                    if(thresh[first_i][j][k]) {
                        unsigned short labels[4] = {0,0,0,0};
                        bool is_empty = true;
                        int prev_counter = 0;
                        for(int y = -1; y<1;y++) {
                            for(int z= -1; z<1;z++) {
                                if ((j+y)>=0 && (k+z)>=0 && thresh[first_i - 1][j+y][k+z]) {                                                                         //Allready checked check
                                    labels[prev_counter] = pre_label[first_i - 1][j+y][k+z];
                                    is_empty = false;
                                } else {
                                    labels[prev_counter] = 0;
                                }
                                prev_counter++;
                            }
                        }

                        if (!is_empty) {
                            for(int x = 0; x<4; x++) {
                                if (labels[x] != 0 && pre_label[first_i][j][k] != labels[x]) {
                                    #pragma omp critical 
                                    {
                                        sets.union_sets(pre_label[first_i][j][k], labels[x]);
                                    }
                                } 
                            }
                        }
                    } 
                }
            }
        }
    }

    //Pass two of HK
    vector<int> vol(current_label);
    for(int i=0;i<hdr.dim[3];i++) {
        for(int j=0;j<hdr.dim[2];j++) {
            for(int k=0;k<hdr.dim[1];k++) {
                if (thresh[i][j][k]) {
                    //Replacing all labels by their "representitive".
                    pre_label[i][j][k] = sets.find_set(pre_label[i][j][k]);
                    //Counting the volume for each label.
                    vol[pre_label[i][j][k]]++;
                }
            }
        }
    }
    

    double voxel_volume = hdr.pixdim[1]*hdr.pixdim[2]*hdr.pixdim[3];
    int count = 0;
    unordered_map<unsigned int, unsigned int> map;
    int current_index = 1;
    //Delete small components
    for(int i=0;i<hdr.dim[3];i++) {
        for(int j=0;j<hdr.dim[2];j++) {
            for(int k=0;k<hdr.dim[1];k++) {
                if (thresh[i][j][k] && (vol[pre_label[i][j][k]]*voxel_volume >= smallest_group && vol[pre_label[i][j][k]]*voxel_volume <= biggest_group)) {
                    if(map.count(pre_label[i][j][k])) {
                        seg.get_data()[i][j][k] = map[pre_label[i][j][k]];
                    } else {
                        map.insert({pre_label[i][j][k], current_index});
                        seg.get_data()[i][j][k] = current_index;
                        seg.get_cls().add_to_cls("Lung" + std::to_string(current_index), current_index);
                        current_index++;
                    }
                }
            }
        }
    }

    for(int i = 0;i<vol.size();i++) {
        if(vol[i]*voxel_volume >= smallest_group && vol[i]*voxel_volume <= biggest_group) count++;
    }

    if(num_found != nullptr) *num_found = count;


    return seg;
}




/* 
    Unused serial version of the above alogrithm.


Segmentation Scan::create_segmentation(int N_CORES, int threshold, int smallest_group, int biggest_group, int* num_found) {

    const int search_range = 1; //ceil((pow(hdr.dim[1]*hdr.dim[2]*hdr.dim[3]/69551, 1/3.)-1)/2.);  //Dynamicly scale search range with image dimensions 

    Segmentation seg;
    seg.set_header_in_accorddance_to_scan(this);

    Disjoint_Set sets;
    
    vector<vector<vector<bool>>> thresh = vector<vector<vector<bool>>>(hdr.dim[3], vector<vector<bool>>(hdr.dim[2], vector<bool>(hdr.dim[1], false)));
    vector<vector<vector<unsigned int>>> pre_label = vector<vector<vector<unsigned int>>>(hdr.dim[3], vector<vector<unsigned int>>(hdr.dim[2], vector<unsigned int>(hdr.dim[1], 0)));


    //Pre thresholding
    for(int i=0;i<hdr.dim[3];i++) {
        for(int j=0;j<hdr.dim[2];j++) {
            for(int k=0;k<hdr.dim[1];k++) {
                if (data[i][j][k] <= threshold) thresh[i][j][k] = true;
            }
        }
    }

    //Pass one
    int current_label = 1;
    for(int i=0;i<hdr.dim[3];i++) {
        //cout << "i: " << i << endl;
        for(int j=0;j<hdr.dim[2];j++) {
            for(int k=0;k<hdr.dim[1];k++) {
                if (thresh[i][j][k]) {
                    //Replace with array on stack
                    int labels[search_range+1][search_range+1][search_range+1];
                    int min_label = 2100000000;
                    bool is_empty = true;
                    for(int x = -search_range; x<=0;x++) {
                        for(int y = -search_range; y<=0;y++) {
                            for(int z = -search_range; z<=0;z++) {
                                if (!(x==0 && y==0 && z==0) && (i+x)>=0 && (j+y)>=0 && (k+z)>=0 && thresh[i+x][j+y][k+z]) {   //&& (i+x)<hdr.dim[3] && (j+y)<hdr.dim[2] && (k+z)<hdr.dim[1]
                                    // if(seg.get_data()[i+x][j+y][k+z] == 0) {
                                    //     cout << seg.get_data()[i+x][j+y][k+z] << endl;
                                    // }
                                        
                                    
                                    labels[-x][-y][-z] = pre_label[i+x][j+y][k+z];
                                    //labels.push_back(seg.get_data()[i+x][j+y][k+z]);
                                    is_empty = false;
                                    if(pre_label[i+x][j+y][k+z] < min_label) min_label = pre_label[i+x][j+y][k+z];
                                } else {
                                    labels[-x][-y][-z] = 0;
                                }
                            }
                        }
                    }

                    if (!is_empty) {
                        pre_label[i][j][k] = min_label;
                        for(int x = 0; x<=search_range;x++) {
                            for(int y = 0; y<=search_range;y++) {
                                for(int z = 0; z<=search_range;z++) { 
                                    if (labels[x][y][z] != 0) sets.union_sets(min_label, labels[x][y][z]);
                                }
                            }
                        }
                    } else {
                        pre_label[i][j][k] = current_label;
                        sets.make_set(current_label);
                        current_label++;
                    }
                }
            }
        }
    }

    cout << current_label << endl;
    //Pass two
    vector<int> vol(current_label);
    for(int i=0;i<hdr.dim[3];i++) {
        for(int j=0;j<hdr.dim[2];j++) {
            for(int k=0;k<hdr.dim[1];k++) {
                if (thresh[i][j][k]) {
                    pre_label[i][j][k] = sets.find_set(pre_label[i][j][k]);
                    vol[pre_label[i][j][k]]++;
                }
            }
        }
    }



    
    double voxel_volume = hdr.pixdim[1]*hdr.pixdim[2]*hdr.pixdim[3];
    int count = 0;
    unordered_map<unsigned int, unsigned int> map;
    int current_index = 1;

    //Delete small components
    for(int i=0;i<hdr.dim[3];i++) {
        for(int j=0;j<hdr.dim[2];j++) {
            for(int k=0;k<hdr.dim[1];k++) {
                if (thresh[i][j][k] && (vol[pre_label[i][j][k]]*voxel_volume >= smallest_group && vol[pre_label[i][j][k]]*voxel_volume <= biggest_group)) {
                    if(map.count(pre_label[i][j][k])) {
                        seg.get_data()[i][j][k] = map[pre_label[i][j][k]];
                    } else {
                        map.insert({pre_label[i][j][k], current_index});
                        seg.get_data()[i][j][k] = current_index;
                        current_index++;
                    }
                }
            }
        }
    }

    


    // ofstream file;
    // file.open("group_size.txt", std::fstream::app);
    for(int i = 0;i<vol.size();i++) {
        // if(vol[i] > 2500) file << "i: " << i << ", vol[i]: " << vol[i] << endl;
        if(vol[i]*voxel_volume >= smallest_group && vol[i]*voxel_volume <= biggest_group) count++;//file << vol[i] << ", ";
    }
    // cout << "Found " << count << " groups." << endl;
    if(num_found != nullptr) *num_found = count;
    // file << endl << endl;
    // file.close();
    
    seg.create_cls_file();
    return seg;
} */