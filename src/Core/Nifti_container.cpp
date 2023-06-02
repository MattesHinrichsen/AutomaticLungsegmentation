/**
 * @file Nifti_container.cpp
 * @author Group 3 (mattes.hinrichsen@rwth-aachen.de)
 * @brief Implementation of a collection of data needed to specify the content of the user interface
 * 
 */
#pragma once
#include"Segmentation.h"
#include"Scan.h"
#include<string>
#include<QString>
#include<vector>
#include<set>

struct Seg_Tuple {
    Segmentation seg; /** The segmentation*/
    QString name; /** Name of the segmentation in the UI*/
    set<int> classes; /** Classes that will be displayed in the UI*/
};

struct Nifti_container {
    QString name; /** Name of the scan*/
    Scan scan; /** The scan object*/
    int slice = 0; /** Current index of the slice that is displayed in the UI*/
    int seg_index = -1; /**Index into the segmentation list that is currently selected*/
    vector<Seg_Tuple> segmentations; /** All segmentations belonging to the scan*/
};