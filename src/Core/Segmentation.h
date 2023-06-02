/**
 * @file Segmentation.h
 * @author Group 3 (mattes.hinrichsen@rwth-aachen.de)
 * @brief Implementation of the segmentation object and its unique functions
 * 
 */
#pragma once

#include"CLS_File.h"
#include"Scan.h"
#include"Nifti.h"
#include<iostream>
#include<unordered_map>

class Scan;
using namespace std;

class Segmentation: public Nifti {
    CLS_File cls;
    
public:
    /**
     * @brief Sets the header in accordance to scans header
     * 
     * @param scan Pointer to the scans whose header is copied 
     */
    void set_header_in_accordance_to_scan(Scan* scan);

    /**
     * @brief Returns the cls file
     */
    CLS_File& get_cls();

    /**
     * @brief Calculates the dice score between this and the specified segmentation
     * 
     * @param seg Second segmentation 
     * @param value1 Value of this segmentation that is used for the comparison
     * @param value2 Value of the second segmentation that is used for the comparison
     * @return double The Dice score
     */
    double dice_score(Segmentation& seg, float value1, float value2) const;
}; 