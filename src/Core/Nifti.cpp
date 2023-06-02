#include"Nifti.h"
#include<iostream>

#define SIZE_LIMIT 6.0 //GigaByte
/*
    This file size limit is a bit arbitrary and might create with the GUI version where the user would
    be allowed to open multiple 6GB files. 
    We should keep a running total of used memory.
*/

void Nifti::read_from_file(const string file_location, bool run_scale, float desired_voxeldimension, bool run_blur, float blur_stddev) {    
    std::ifstream file;
        
    //Read Header. 
    file.open(file_location, ios::binary);
    if (file.is_open()) hdr = read_header(file);
    else throw std::invalid_argument("Couldn't open file.");

    //Enforcing size Limit
    if(hdr.dim[1]*hdr.dim[2]*hdr.dim[3]*4/1e9 > SIZE_LIMIT) {
        throw std::invalid_argument("File too big to read.");
    } 


    
    /* 
        Check for correct file format
        We only loosly enforce this. While the type should be specified in the magic field of the header, in practice
        it isn't. Therefore we also allow the decison to be made of the file extension.
    */
    string data_location = "";      //.nii case
    if(hdr.magic[1] == 'i' || file_location.substr(file_location.size() - 3, file_location.size()) == "hdr") {       //.hdr case
        data_location = file_location.substr(0, file_location.size() - 3) + "img";
    } else if(hdr.magic[1] != '+' && file_location.substr(file_location.size() - 3, file_location.size()) != "nii") { //Neither
        throw std::invalid_argument("No valid NIFTI file found.");
    }   


    //Open data file
    if (data_location != "") {          //.hdr case
        file.close();                                 
        file.open(data_location, std::ios::in | std::ios::binary);
    } else {                            //.nii case
        file.seekg((long)(hdr.vox_offset));
    }

    //Initalizes data vector to the correct dimensions
    data = vector<vector<vector<float>>>(hdr.dim[3], vector<vector<float>>(hdr.dim[2], vector<float>(hdr.dim[1])));

    //Reading Data. 
    if (file.is_open())
    { 
        switch(hdr.datatype) {
            case 2: 
                read_data<uint8_t>(file);
                break;
            case 4:
                read_data<short>(file);
                break;
            case 8:
                read_data<int>(file);
                break;
            case 16:
                read_data<float>(file);
                break;
            case 64:
                read_data<double>(file);
                break;
            case 256:
                read_data<int8_t>(file);
                break;
            case 512:
                read_data<unsigned short>(file);
                break;
            case 768:
                read_data<unsigned int>(file);
                break;
            case 1024:
                read_data<long>(file);
                break;
            default:
                throw std::invalid_argument("The stored datatype is currently not supported. Datatype Code: " + to_string(hdr.datatype));
                break;
        }
        
    } else throw std::invalid_argument("Couldn't open file.");

    file.close();

    //Scale the scan if desired 
    if(run_scale) {
        standardize_to_scale(desired_voxeldimension);
    }

    //Blur the scan if desired 
    if(run_blur) {
        blur_data(blur_stddev/hdr.pixdim[1]);
    }
}

vector<vector<vector<float>>>& Nifti::get_data() {
    return data;
}

const nifti_2_header Nifti::get_hdr() {
    return hdr;
}

nifti_2_header Nifti::read_header(ifstream& file) {
    /* 
        The first paramter of the header is the size of it in bits. This also uniquely identifies NIFTI1 and NIFTI2.
    */
    int size_of_header;
    file.read(reinterpret_cast<char *>(&size_of_header), 4);
    file.seekg(0);

    if(size_of_header == 348) {             //Nifti1
        nifti_1_header h;
        file.read(reinterpret_cast<char *>(&h), sizeof(nifti_1_header));
        return h;
    } else if(size_of_header == 540) {      //Nifti2
        nifti_2_header h;
        file.read(reinterpret_cast<char *>(&h), sizeof(nifti_2_header));
        return h;
    } else {
        throw std::invalid_argument("File format doesnt match NIFTI1 nor NIFTI2.");
    }
}

void Nifti::standardize_to_scale(float scale) {
    int new_size_z = hdr.dim[1] *hdr.pixdim[1]/scale;
    int new_size_y = hdr.dim[2] *hdr.pixdim[2]/scale;
    int new_size_x = hdr.dim[3] *hdr.pixdim[3]/scale;
    bilinear_resize<float>(new_size_x, new_size_y, new_size_z, data);

    hdr.srow_x[0] = scale/hdr.pixdim[1] *hdr.srow_x[0];
    hdr.srow_x[1] = scale/hdr.pixdim[1] *hdr.srow_x[1];
    hdr.srow_x[2] = scale/hdr.pixdim[1] *hdr.srow_x[2];
    hdr.srow_y[0] = scale/hdr.pixdim[2] *hdr.srow_y[0];
    hdr.srow_y[1] = scale/hdr.pixdim[2] *hdr.srow_y[1];
    hdr.srow_y[2] = scale/hdr.pixdim[2] *hdr.srow_y[2];
    hdr.srow_z[0] = scale/hdr.pixdim[3] *hdr.srow_z[0];
    hdr.srow_z[1] = scale/hdr.pixdim[3] *hdr.srow_z[1];
    hdr.srow_z[2] = scale/hdr.pixdim[3] *hdr.srow_z[2];

    hdr.dim[1] = new_size_z;
    hdr.dim[2] = new_size_y;
    hdr.dim[3] = new_size_x;
    hdr.pixdim[1] = scale;
    hdr.pixdim[2] = scale;
    hdr.pixdim[3] = scale;
}

void Nifti::blur_data(float std_dev){
    gaussian_blur<float>(data, std_dev);
}