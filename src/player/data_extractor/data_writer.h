#ifndef DATA_WRITER_H
#define DATA_WRITER_H

#include <iostream>
#include <fstream>
#include <rcsc/player/world_model.h>

class DataWriter {
    private:
        std::string timeHash;
        std::fstream actionFile;
        std::fstream timeFile;
        std::fstream worldModelFile;
    
    public:
        DataWriter();
        
        static DataWriter& instance() {
            static DataWriter instance;
            return instance;
        }
        
        void initFileAction(const rcsc::WorldModel &wm);

        void initFileWorldModel(const rcsc::WorldModel &wm);

        void initFileTime(const rcsc::WorldModel &wm);
        
        void writeLineAction(const rcsc::WorldModel& wm, std::string line);

        void writeLineWorldModel(const rcsc::WorldModel& wm, std::string line);

        void writeLineTime(const rcsc::WorldModel& wm, std::vector<std::pair<int,double>> timeArray);
        
        // std::string joinDoubleArray(std::vector<double> data, char separator=',');



};

#endif