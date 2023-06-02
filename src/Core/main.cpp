/**
 * @file main.cpp
 * @author Group 3 (mattes.hinrichsen@rwth-aachen.de)
 * @brief An example program run
 * 
 */
#include"Segmentation.h"
#include"Scan.h"
#include"Timer.cpp"

int main() {
    Timer t;
    
    //Read in a scan
    Scan scan;
    scan.read_from_file("../Example/CT280.hdr");

    //Create a segmentation
    Segmentation auto_seg;
    auto_seg = scan.create_segmentation(1,-250, 225, 1400);

    //Save scan 
    auto_seg.write_to_file<uint8_t>("../Example" , "Seg_CT280", 2);
    cout << "Saved the Segmentation to Example/Seg_CT280.hdr and Example/Seg_CT280.img" << endl;

    //Load a manual segmentation
    Segmentation manual_seg;
    manual_seg.read_from_file("../Example/Organ1.hdr");

    //Compute DICE score between the automatic and manual segmentation
    double dice_score = auto_seg.dice_score(manual_seg, 1, 2);
    cout << "The DICE score of the manual and automatic segmentation is: " << dice_score << endl;

    return 0;
}
