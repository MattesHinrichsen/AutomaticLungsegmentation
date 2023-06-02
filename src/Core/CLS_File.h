/**
 * @file CLS_File.h
 * @author Group 3 (mattes.hinrichsen@rwth-aachen.de)
 * @brief The implementation of the CLS file used to store information on the encoding used for different classes in a segmentation.
 * 
 */

#pragma once
#include<iostream>
#include<fstream>
#include<filesystem>
#include<string>
#include<vector>
#include"Scan.h"
#include<map>

using namespace std;

/**
 * @class CLS file class
 * 
 */
class CLS_File {

private:

    vector<vector<int>> pre_colors = {{0,0,0,255}, {255,0,0,255}, {0,255,0,255}, {0,0,255,0}, {255,0,255,255}}; //We cheat by only specifying four colors because we know there will never be more lungs than this.
    vector<vector<int>> colors = {{0,0,0,255}};  /** List of all colors*/
    vector<string> names = {"unclassified"}; /** List of all names*/
    vector<float> indices = {0}; /** List of all indices*/

public:
    /**
     * @brief Returns the colors in a map with their index as key for quick lockup
     * 
     */
    map<float, vector<int>> get_color_map() {
        map<float, vector<int>> m;
        for(int i=0;i<indices.size();i++) {
            m.insert({indices[i], colors[i]});
        }
        return m;
    }

    /**
     * @brief Finds the name of an index
     * 
     * @param idx The index
     * @return string The name belonging to the index
     */
    string find_name(float idx) {
        for(int i=0;i<names.size();i++) {
            if(indices[i] == idx) {
                return names[i];
            }
        }
        return "";
    }

    /**
     * @brief Reads a CLS file into the object
     * 
     * @param path The path of the file
     */
    void read(const string path) {
        // If there is no CLS file found at the specified this function does nothing
        if(std::filesystem::exists(path)) {
            std::ifstream file;
            file.open(path);
            if (file.is_open()) {
                colors.clear();
                names.clear();
                indices.clear();
                //Colors
                {
                    string str_color;
                    getline(file,str_color, '=');
                    getline(file,str_color);

                    stringstream ss(str_color);
                    string tmp;
                    int idx;
                    while (!ss.eof()) {
                        getline(ss, tmp, '|');
                        stringstream ss2(tmp);
                        string tmp2;
                        vector<int> tmp_vec;
                        while (!ss2.eof()) {
                            getline(ss2, tmp2, ' ');
                            stringstream(tmp2) >> idx;
                            tmp_vec.push_back(idx);
                        }
                        colors.push_back(tmp_vec);                  
                    }
                }

                //Indices
                {
                    string str_index;
                    getline(file,str_index, '=');
                    getline(file,str_index);

                    stringstream ss(str_index);
                    string tmp;
                    float idx;
                    while (!ss.eof()) {
                        getline(ss, tmp, '|');
                        stringstream(tmp) >> idx;
                        indices.push_back(idx);
                    }
                }

                //Names
                {
                    string str_name;
                    getline(file,str_name, '=');
                    getline(file,str_name);

                    stringstream ss(str_name);
                    string tmp;
                    while (!ss.eof()) {
                        getline(ss, tmp, '|');
                        names.push_back(tmp);
                    }
                }
            } else throw std::invalid_argument("Couldn't read specified cls file.");
        }
    }

    /**
     * @brief Writes a cls to the drive
     * 
     * @tparam T Datatype used in the output of the segmentation
     * @param path Path to which the file is written
     * @param name name of file
     */
    template<typename T>
    void write(const string path ,const string name)
    {
        std::ofstream file;
        if(std::filesystem::exists(path)) {
            file.open(path + "/" + name);
            if (file.is_open()) {

                //Colors
                file << "ClassColors=";
                for(int i=0;i<names.size()-1;i++) {
                    file << colors[i][0] << " " << colors[i][1] << " " << colors[i][2] << " " << colors[i][3] << "|";
                }
                file << colors[names.size()-1][0] << " " << colors[names.size()-1][1] << " " << colors[names.size()-1][2] << " " << colors[names.size()-1][3] << endl;

                //Indices
                file << "ClassIndices=";
                for(int i=0;i<names.size()-1;i++) {
                    file << (T)indices[i] <<  "|";
                }
                file << (T)indices[names.size()-1] <<  endl;

                //Names
                file << "ClassNames=";
                for(int i=0;i<names.size()-1;i++) {
                    file << names[i] <<  "|";
                }
                file << names[names.size()-1] <<  endl;
            } else throw std::invalid_argument("Couldn't write specified cls file.");
        } else throw std::invalid_argument("Specified path doesn't exist: " + path);
    }

    /**
     * @brief Adds a new class to the object
     * 
     * @param name The name of the new class
     * @param index The number value/index of the new class
     */
    void add_to_cls(string name, float index) {
        names.push_back(name);
        indices.push_back(index);
        if (names.size()-1 < 5) {
            colors.push_back(pre_colors[names.size()-1]);
        } else {
            colors.push_back(pre_colors[4]);
        }
        
    }

};
