/**
 * @file batch_mode.h
 * @author Group 3 (mattes.hinrichsen@rwth-aachen.de)
 * @brief The implementation of the batch mode without the use of QT, allowing users to run the segmentation process on all scans in a directory.
 * 
 */

#pragma once
#include<filesystem>
#include<string>
#include<iostream>
#include<sstream>
#include<vector>
#include"Scan.h"
#include"Segmentation.h"
using namespace std;


/**
 * @class Class containing all functions and variable needed to run the segmentation on all files in a directory.
 * 
 */
class Batch_Mode {
    int num_found = 0;

public:
    /**
     * @brief Function to run the segmentation on all specified files.
     * 
     * @tparam T Datatype used for the segmentation output file
     * @param input_dir The base directory from where the search for files begins
     * @param names The list of file names the user wants to segment
     * @param search_subdirs Truthvalue to decide wether or not to recursively search subdirectories of input_dir
     * @param save_type The integer encoding of the above specified datatype. Also needs to be consistent with it
     * @param output_type Parameter deciding wether to store as .nii or .hdr. True means .hdr
     * @param N_CORES Number of cores to run the segmentation with
     * @param threshold The threshold value used in the segmentation
     * @param smallest_group The biggest volume still considered to possibly be a lung
     * @param biggest_group The smallest volume still considered to possibly be a lung
     */
    template<typename T>
    void run_batch_mode(string input_dir, vector<string> names, bool search_subdirs, int save_type, bool output_type, int N_CORES, int threshold, int smallest_group, int biggest_group) {   //All files with this name. To find type use ".hdr" as a name
        
        count_entries(input_dir, names, search_subdirs);
        for(string name : names) {
            if(search_subdirs) {
                for(const auto& dirEntry : std::filesystem::recursive_directory_iterator(input_dir)) {
                    std::string str = dirEntry.path().string();
                    if(dirEntry.path().filename().string() == name) {
                        Scan n;
                        n.read_from_file(str);
                        Segmentation s;
                        s = n.create_segmentation(N_CORES, threshold, smallest_group, biggest_group);
                        string out_name = "Seg_" + name.substr(0, name.size()-4);
                        s.write_to_file<T>(dirEntry.path().parent_path().string(), out_name, save_type, output_type);
                    } 
                }
            } else {
                for(const auto& dirEntry : std::filesystem::directory_iterator(input_dir)) {
                    std::string str = dirEntry.path().string();
                    if(dirEntry.path().filename().string() == name) {
                        Scan n;
                        n.read_from_file(str);
                        Segmentation s = n.create_segmentation(N_CORES, threshold, smallest_group, biggest_group);
                        string out_name = "Seg_" + name.substr(0, name.size()-4);
                        s.write_to_file<T>(dirEntry.path().parent_path().string(), out_name, save_type, output_type);
                    }
                }
            }
        }
    };

    /**
     * @brief Batch scaling. Inaccessible to the user.
     * 
     * @tparam T 
     * @param input_dir The base directory from where the search for files begins
     * @param names The list of file names the user wants to segment
     * @param search_subdirs Truthvalue to decide wether or not to recursively search subdirectories of input_dir
     * @param save_type The integer encoding of the above specified datatype. Also needs to be consistent with it
     * @param output_type Parameter deciding wether to store as .nii or .hdr. True means .hdr
     */
    template<typename T>
    void scale_batch_mode(string input_dir, vector<string> names, bool search_subdirs, int save_type, bool output_type, float scale=0.140, int N_CORES=1) {   //All files with this name. To find type use ".hdr" as a name
        
        count_entries(input_dir, names, search_subdirs);
        int count = 0;
        for(string name : names) {
            if(search_subdirs) {
                for(const auto& dirEntry : std::filesystem::recursive_directory_iterator(input_dir)) {
                    std::string str = dirEntry.path().string();

                    if(dirEntry.path().filename().string() == name) {
                        Scan n;
                        n.read_from_file(str);
                        n.standardize_to_scale(scale);
                        stringstream scale_sstream;
                        scale_sstream << fixed << setprecision(0) << 1000*scale; //fancy string coversion vor allem um den Dezimalpunkt zu vermeiden
                        string out_name = name.substr(0, name.size()-4) + "_scaled_" + scale_sstream.str() + "μm";
                        n.write_to_file<T>(dirEntry.path().parent_path().string(), out_name, save_type, output_type);
                    }   
                }
            } else {
                for(const auto& dirEntry : std::filesystem::directory_iterator(input_dir)) {
                    std::string str = dirEntry.path().string();
                    if(dirEntry.path().filename().string() == name) {
                        Scan n;
                        n.read_from_file(str);
                        n.standardize_to_scale(scale);
                        stringstream scale_sstream;
                        scale_sstream << fixed << setprecision(0) << 1000*scale; //fancy string conversion vor allem um den Dezimalpunkt zu vermeiden
                        string out_name = name.substr(0, name.size()-4) + "_scaled_" + scale_sstream.str() + "μm";

                        n.write_to_file<T>(dirEntry.path().parent_path().string(), out_name, save_type, output_type);
                    }
                }
            }
        }
    };

    /**
     * @brief Counts the number of files matching the description of the user in order to assess correctness of parameters before running the batch mode 
     * 
     * @param input_dir The base directory from where the search for files begins
     * @param names The list of file names the user wants to segment
     * @param search_subdirs Truthvalue to decide wether or not to recursively search subdirectories of input_dir
     * @return int Both returns and sets the internal num_found parameter to the number of files matching the description
     */
    int count_entries(string input_dir, vector<string> names, bool search_subdirs);

    /**
     * @brief Deletes all files matching the description. Inaccessible to the user. Was used for cleanup after testing the batch mode.
     * 
     * @param input_dir The base directory from where the search for files begins
     * @param names The list of file names the user wants to segment
     * @param search_subdirs Truthvalue to decide wether or not to recursively search subdirectories of input_dir
     * @param type Type of file to delete. Used to also delete .img file in .hdr type
     */
    void delete_all(string input_dir, vector<string> names, bool search_subdirs, bool type);
};
