/**
 * @file Nifti.h
 * @author Group 3 (mattes.hinrichsen@rwth-aachen.de)
 * @brief The main NIFTI class handling all functions and data used in both the Segmentation and Scan class.
 * 
 */
#pragma once

#include<vector>
#include<string>
#include "nifti1.h"
#include "nifti2.h"
#include<fstream>
#include"scale.cpp"
#include"gaussian_blur.cpp"

using namespace std;

class Nifti {
protected:
    vector<vector<vector<float>>> data; 
    nifti_2_header hdr;

public:
    /**
     * @brief Reads the specified file into the object
     * 
     * @param file_location Location and name of the file
     * @param run_scale Wether or not to scale the scan
     * @param desired_voxeldimension The dimension to which is scaled if wanted
     * @param run_blur Wether or not to blur the image
     * @param blur_stddev The strength of the blur
     */
    void read_from_file(const string file_location, bool run_scale=false, float desired_voxeldimension=0.14, bool run_blur=false, float blur_stddev=2.0);

    /**
     * @brief Get the data
     * 
     */
    vector<vector<vector<float>>>& get_data();

    /**
     * @brief Get the header
     * 
     */
    const nifti_2_header get_hdr();

    /**
     * @brief Trilinear Scaling function to resize the scan data to a uniform scale
     * 
     * @param scale desired voxel dimension in mm
     */
    void standardize_to_scale(float scale=0.14);

    /**
     * @brief Gaussian Blur function on scan data
     * 
     * @param std_dev standard deviation used to generate the Gauss Kernel
     */
    void blur_data(float std_dev=2);
    

    // 
    /**
     * @brief Writes the object to the drive
     * 
     * @tparam T Datatype of used for the output file
     * @param file_location The location to which it is written
     * @param name Name of the file
     * @param data_type_code The code of the template type. Used to complete the header.
     * @param file_extension File extension of the output file. true means .hdr, false .nii
     */
    template<typename T>
    void write_to_file(string file_location, string name, short data_type_code, bool file_extension=true) {

        // Write Header
        std::ofstream file;
        hdr.datatype = data_type_code;        

        if (file_extension) {     //.hdr case
            //Setting the correct parameters of the header
            hdr.vox_offset = 0;
            char tmp[8] = {0x6E, 0x69, 0x32, 0x00, 0x0D, 0x0A, 0x1A, 0x0A} ;
            std::copy(tmp, tmp+8, hdr.magic);

            file.open(file_location + "/" + name + ".hdr", std::ios::binary);

            if(file.is_open()) file.write(reinterpret_cast<char *>(&hdr), sizeof(hdr));
            else throw std::invalid_argument("Couldn't open output file.");

            //Close header file and open img file for the data
            file.close();
            file.open(file_location + "/" + name + ".img", std::ios::binary);
        } else {        //.nii case

            //Setting the correct parameters of the header
            hdr.vox_offset = 544; 
            char tmp[8] = {0x6E, 0x2B, 0x32, 0x00, 0x0D, 0x0A, 0x1A, 0x0A};
            std::copy(tmp, tmp+8, hdr.magic);

            file.open(file_location + "/" + name + ".nii", std::ios::binary);
            if(file.is_open()) {
                file.write(reinterpret_cast<char *>(&hdr), sizeof(hdr));

                // Find the correct point in the file for the data to start
                file.seekp((long)(hdr.vox_offset));

            }  else throw std::invalid_argument("Couldn't open output file.");
        }


        //Write data
        if (file.is_open()) {
            vector<T> tmp(hdr.dim[1],0);
            for (int i = 0; i<hdr.dim[3]; i++) {
                for (int j = 0; j<hdr.dim[2]; j++) {
                    //Temporary casting to the output datatype
                    tmp = std::vector<T>(data[i][j].begin(), data[i][j].end());
                    file.write(reinterpret_cast<char *>(&tmp[0]), sizeof(T)*hdr.dim[1]);
                }
            }
            file.close();
        } else throw std::invalid_argument("Couldn't open output file.");
    }  

private:
    /**
     * @brief Helper function that reads the header into the hdr field of the object
     * 
     * @param file The open filestream from which is read
     * @return nifti_2_header Returns and sets the header
     */
    nifti_2_header read_header(ifstream& file);
    
    /**
     * @brief Helper function that reads the data into the data field of the object
     * 
     * @tparam T Datatype that was used to store the binary data of the scan
     * @param file Filestream from which is read
     */
    template<typename T>
    void read_data(ifstream& file) {
        vector<T> tmp(hdr.dim[1]);
        for (int i = 0; i<hdr.dim[3]; i++) {
            for (int j = 0; j<hdr.dim[2]; j++) {
                file.read (reinterpret_cast<char *>(&tmp[0]), sizeof(T)*hdr.dim[1]);  
                // Cast to float
                data[i][j] = std::vector<float>(tmp.begin(), tmp.end());
            }
        }
    }

    
};