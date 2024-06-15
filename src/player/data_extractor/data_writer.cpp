#include "data_writer.h"
#include <string>

DataWriter::DataWriter() {
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    sprintf(buffer, "%d%d%d%d%d", timeinfo->tm_year, timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min);
    
    std::string stime(buffer);
    timeHash = stime;
}

void DataWriter::initFileAction(const rcsc::WorldModel &wm) {
    if(!this->actionFile.is_open()) {
        this->actionFile.open("player_"+std::to_string(wm.self().unum())+"_action_dframe.csv", std::ios::out | std::ios::app);
    }
}

void DataWriter::initFileTime(const rcsc::WorldModel &wm) {
    if(!this->timeFile.is_open()) {
        this->timeFile.open("time_"+std::to_string(wm.self().unum())+".csv", std::ios::out | std::ios::app);
    }
}

// std::string DataWriter::joinDoubleArray(std::vector<pair::double> data, char separator=',')
// {
//     std::ostringstream o;
//     for(auto i=data.begin(); i != data.end(); i++)
//     {
//         if(i != data.end()-1){
//             o << *i << separator;
//         }
//         else{
//             o << *i;
//         }
//     }
//     o << '\n';
//     return o.str();
// }

void DataWriter::writeLineTime(const rcsc::WorldModel& wm, std::vector<std::pair<int,double>> timeArray) {
    if(!this->timeFile.is_open()) {
        initFileTime(wm);
    }

    // timeFile << timeHash << <<  "," << line << std::endl;
    
    for(auto d : timeArray){
        int cycle = d.first;
        double timediff = d.second;   
        timeFile << timeHash << ","<< timediff <<  "," << cycle <<  '\n';
    }

    this->timeFile.close();

    std::cout << "Time written" << std::endl;

}

void DataWriter::initFileWorldModel(const rcsc::WorldModel &wm) {
    if(!this->worldModelFile.is_open())
        this->worldModelFile.open("player_"+std::to_string(wm.self().unum())+"_worldmodel_dframe.csv", std::ios::out | std::ios::app);
}

void DataWriter::writeLineAction(const rcsc::WorldModel& wm, std::string line) {
    if(!this->actionFile.is_open()) {
        initFileAction(wm);
    }

    actionFile << timeHash << "," << line << std::endl;
}

void DataWriter::writeLineWorldModel(const rcsc::WorldModel& wm, std::string line) {
    if(!this->actionFile.is_open()) {
        initFileWorldModel(wm);
    }

    worldModelFile << timeHash << "," << line << std::endl;
}

