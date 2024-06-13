#include <string>
#include <iostream>
#include <filesystem>
#include <bits/stdc++.h>
#include "learning_models.h"

namespace fs = std::filesystem;

class OnnxTest : LearningModels::OnnxModel
{
    public:
        OnnxTest(const char *modelPath, Ort::Env *env, int numberofThreads) 
        : LearningModels::OnnxModel(modelPath, env){};
        float getOuput(){
            std::vector<float> frame = this->createFrame();
            float result = this->forward<float>(frame)[0];
            return result;
        };

    private:
        std::vector<float> createFrame()
        {
            return std::vector<float>(3120, 0.0f);

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
    std::string modelPath = "../../models/cyrus_model.onnx";
    std::vector<std::vector<double>*> timeArrays;

    evaluateTime(modelPath);
    //write to file
    // writeToFile(timeArrays);
}