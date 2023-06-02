#if QT_FREE
    #include"batch_mode.h"
#else
    #include"batch_mode_qt.h"
#endif



int Batch_Mode::count_entries(string input_dir, vector<string> names, bool search_subdirs) {
    int count = 0;
    for(string name : names) {
        if(search_subdirs) {
            for(const auto& dirEntry : std::filesystem::recursive_directory_iterator(input_dir)) {
                if(dirEntry.path().filename().string() == name) {
                    count++;
                }   
            }
        } else {
            for(const auto& dirEntry : std::filesystem::directory_iterator(input_dir)) {
                if(dirEntry.path().filename().string() == name) {
                    count++;
                }   
            }
        }
    }
    num_found = count;
    return count;
}  


void Batch_Mode::delete_all(string input_dir, vector<string> names, bool search_subdirs, bool type) {
    int c = count_entries(input_dir, names, search_subdirs);
    bool b;
    cout << "Are you sure you want to delete " << c << " fies [0 / 1]: ";
    cin >> b;
    for(string name : names) {
        if(search_subdirs && b) {
            for(const auto& dirEntry : std::filesystem::recursive_directory_iterator(input_dir)) {
                if(dirEntry.path().filename().string() == name) {
                    std::filesystem::remove(dirEntry);
                    if(type) {
                        string str = dirEntry.path().string();
                        std::filesystem::remove(str.substr(0, str.size()-3) + "img");
                    } 
                }   
            }
        } else if(b){
            for(const auto& dirEntry : std::filesystem::directory_iterator(input_dir)) {
                if(dirEntry.path().filename().string() == name) {
                    std::filesystem::remove(dirEntry);
                    if(type) {
                        string str = dirEntry.path().string();
                        std::filesystem::remove(str.substr(0, str.size()-3) + "img");
                    } 
                }   
            }
        }
    }
} 
