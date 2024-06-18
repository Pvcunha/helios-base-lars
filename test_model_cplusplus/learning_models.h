// *************************************************************************
//  This program is implemented by RoboCIn
//  Copyright 2019, Cristiano Oliveira, Mateus Gonçalves, Walber Rodrigues
//  Centro de Informática - Universidade Federal de Pernambuco, Brazil
//  All rights reserved.
// *************************************************************************

#ifndef LEARNINGMODELS_H
#define LEARNINGMODELS_H

// #include "../lib/PY_ENV/include/python3.6m/Python.h"
#include <iostream>
#include <string>
#include <vector>
#include <onnxruntime_cxx_api.h>

namespace LearningModels
{
//!  Class to handle ONNX models and sessions
/*!
OnnxModel is the class that handles every step for ONNX inference models, you can export a model trained in any other
framework (ie. Pytorch, Tensorflow, etc), read specific documentation in each of these frameworks for more details.
The objective of this class is to be used as super class. Your sub class should not override implement
OnnxModel::forward function as it will be used afeter you handling any data processing and frame creation. A frame is an
instance of your data and its size must be the number of features from a sample.
*/
class OnnxModel
{
  public:
    OnnxModel(const char *modelPath, Ort::Env *env, int numberofThreads = 1);
    ~OnnxModel();

  protected:
    template <typename ONNXRETURNTYPE> ONNXRETURNTYPE *forward(std::vector<std::vector<float>> frame)
    {
        // Init the input tensor
        std::vector<Ort::Value> input_tensors;
        for(auto &f : frame){
            size_t inputTensorSize = f.size();
            auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
            Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
                memory_info, 
                f.data(), 
                inputTensorSize, 
                this->inputNodeDims.data(), 
                2
            );
            input_tensors.push_back(input_tensor);
        }
        // size_t inputTensorSize = frame.framesize();
        // auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
        // Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
        //     memory_info, 
        //     frame.data(), 
        //     inputTensorSize, 
        //     this->inputNodeDims.data(), 
        //     2
        // );

        // Get our model output
        std::vector<const char *> output_node_names;
        for (int i = 0; i < this->session->GetOutputCount(); i++)
        {
            output_node_names.push_back(this->session->GetOutputName(i, allocator));
        }

        auto output_tensors = this->session->Run(
            Ort::RunOptions().UnsetTerminate(), 
            this->inputNodeNames.data(), 
            input_tensors, 
            input_tensors.size(), 
            output_node_names.data(), 
            1
        );

        ONNXRETURNTYPE *arr = output_tensors.front().GetTensorMutableData<ONNXRETURNTYPE>();
        return arr;
    }
    template <typename ONNXRETURNTYPE> ONNXRETURNTYPE *forward(std::vector<float> frame)
    {
        // Init the input tensor
        size_t inputTensorSize = frame.size();
        auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
        Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
            memory_info, 
            frame.data(), 
            inputTensorSize, 
            this->inputNodeDims.data(), 
            2
        );

        // Get our model output
        std::vector<const char *> output_node_names;
        for (int i = 0; i < this->session->GetOutputCount(); i++)
        {
            output_node_names.push_back(this->session->GetOutputName(i, allocator));
        }

        auto output_tensors = this->session->Run(
            Ort::RunOptions().UnsetTerminate(), 
            this->inputNodeNames.data(), 
            input_tensors, 
            1, 
            output_node_names.data(), 
            1
        );

        ONNXRETURNTYPE *arr = output_tensors.front().GetTensorMutableData<ONNXRETURNTYPE>();
        return arr;
    }

    Ort::Session* session;
    Ort::Env* env;
    size_t numInputNodes;
    std::vector<int64_t> inputNodeDims;
    Ort::AllocatorWithDefaultOptions allocator;
    std::vector<char*> inputNodeNames;
};

} // namespace LearningModels

#endif