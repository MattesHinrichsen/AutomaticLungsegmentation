/**
 * @file Scan.h
 * @author Group 3 (mattes.hinrichsen@rwth-aachen.de)
 * @brief Implementation of the scan object and its unique functions
 */
#pragma once

class Segmentation;

#include "Nifti.h"
#include "Segmentation.h"

class Scan: public Nifti {
public:
    /**
     * @brief Creates a segmentation of the scan
     * 
     * @param N_CORES Number of cores the segmentation is run on
     * @param threshold The threshold brightness value
     * @param smallest_group  The smallest volume still considered to feasibly be a lung
     * @param biggest_group The biggest volume still considered to feasibly be a lung
     * @param num_found Return parameter set to the number of lungs found in the scan
     * @return Segmentation 
     */
    Segmentation create_segmentation(int N_CORES, int threshold, int smallest_group, int biggest_group, int* num_found = nullptr);
};