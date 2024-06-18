#include <string>
#include <iostream>
#include <filesystem>
#include <bits/stdc++.h>
#include "learning_models.h"

namespace fs = std::filesystem;
/*
        Ort::Value wm_input, raw_wm_input, bps_row_input, ret_bps_input;


        // Init the input tensor
        size_t inputTensorSize = wm[0].size();
        auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

        wm_input  = Ort::Value::CreateTensor<float>(
            memory_info, 
            wm[0].data(), 
            inputTensorSize, 
            this->inputNodeDims.data(), // FIXME: esse cara é um problema
            2
        );
        size_t inputTensorSize = raw_wm[0].size();
        auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

        wm_input  = Ort::Value::CreateTensor<float>(
            memory_info, 
            raw_wm[0].data(), 
            inputTensorSize, 
            this->inputNodeDims.data(), // FIXME: esse cara é um problema
            2
        );


*/
class OnnxTest : LearningModels::OnnxModel
{
    public:
        OnnxTest(const char *modelPath, Ort::Env *env, int numberofThreads) 
        : LearningModels::OnnxModel(modelPath, env){};
        float getOuput(){
            std::vector<std::vector<float>> frame = this->createFrame();
            float result = this->forward<float>(frame)[0];
            return result;
        };

    private:
        std::vector<std::vector<float>> createFrame()
        {
            const int framesize = 167;
            float frame[framesize] = {0};
            auto x = std::vector<float>(frame, frame + framesize);
 
            const int framesize2 = 138;
            float frame2[framesize2] = {0};
            auto y = std::vector<float>(frame2, frame2 + framesize2);

            const int framesize3 = 780;
            float frame3[framesize3] = {0};
            auto z = std::vector<float>(frame3, frame3 + framesize3);

            return std::vector<std::vector<float>>{x, y, z};
        }
};

std::vector<double>* evaluateTime(std::string modelPath)
{
    double timeTaken;
    clock_t start, end;
    std::vector<double>* timeArray = new std::vector<double>();

    start = clock();
    Ort::Env* onnxEnv = new Ort::Env(ORT_LOGGING_LEVEL_WARNING, "test");
    end = clock();
    timeTaken = double(end - start) / double(CLOCKS_PER_SEC);
    std::cout << "ortenv init time " << timeTaken << std::endl;
    timeArray->push_back(timeTaken);

    start = clock();
    OnnxTest net = OnnxTest(modelPath.data(), onnxEnv, 1);
    end = clock();
    timeTaken = double(end - start) / double(CLOCKS_PER_SEC);
    std::cout << "model load time " << timeTaken << std::endl;
    timeArray->push_back(timeTaken);

    start = clock();
    float output = net.getOuput();
    end = clock();
    timeTaken = double(end - start) / double(CLOCKS_PER_SEC);
    std::cout << "model inf time " << timeTaken << std::endl;
    timeArray->push_back(timeTaken);

    std::cout << output << std::endl;

    delete onnxEnv;

    return timeArray;
}

int main() {
    std::string modelPath = "../../models/tpg_fset1.onnx";
    std::vector<std::vector<double>*> timeArrays;

    evaluateTime(modelPath);
    //write to file
    // writeToFile(timeArrays);
}