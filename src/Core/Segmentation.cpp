#include"Segmentation.h"
#include<cassert>

void Segmentation::set_header_in_accordance_to_scan(Scan* scan) {
    hdr = scan->get_hdr();
    data = vector<vector<vector<float>>>(hdr.dim[3], vector<vector<float>>(hdr.dim[2], vector<float>(hdr.dim[1])));
}

double Segmentation::dice_score(Segmentation& seg, float value1, float value2) const {
    assert(hdr.dim[1] == seg.get_hdr().dim[1] && hdr.dim[2] == seg.get_hdr().dim[2] && hdr.dim[3] == seg.get_hdr().dim[3]);
    double X = 0;
    double Y = 0;
    double XandY = 0;
    for (int i = 0; i<hdr.dim[3]; i++) {
        for (int j = 0; j<hdr.dim[2]; j++) {
            for (int k = 0; k<hdr.dim[1]; k++) {
                if (data[i][j][k] == value1) X++;
                if (seg.get_data()[i][j][k] == value2) {
                    Y++;
                    if (data[i][j][k] == value1) XandY++;
                }
            }
        }
    }
    return 2*XandY/(X+Y);
}

CLS_File& Segmentation::get_cls() {
    return cls;
}