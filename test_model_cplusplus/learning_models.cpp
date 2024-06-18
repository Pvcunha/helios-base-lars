#include "learning_models.h"

LearningModels::OnnxModel::OnnxModel(const char *modelPath, Ort::Env *env, int numberOfThreads)
{
    // Set environment for our session
    Ort::SessionOptions session_options;
    session_options.SetIntraOpNumThreads(numberOfThreads); // number of threads should be a define, we may experience issues to change that in the future
    session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);

    // Load session
    this->session = new Ort::Session(*env, modelPath, session_options);

    // Get the properties of our model
    this->numInputNodes = session->GetInputCount();
    this->inputNodeNames = std::vector<char *>(this->numInputNodes);

    for (int i = 0; i < numInputNodes; i++)
    {
        char *input_name = session->GetInputName(i, allocator);
        this->inputNodeNames[i] = input_name;
        Ort::TypeInfo typeInfo = session->GetInputTypeInfo(i);
        auto tensorInfo = typeInfo.GetTensorTypeAndShapeInfo();
        ONNXTensorElementDataType type = tensorInfo.GetElementType();
        this->inputNodeDims = tensorInfo.GetShape();

        for(auto x : tensorInfo.GetShape()){
            std::cout << x << " ";
        }
        std::cout << std::endl;
    }
}

//! ONNXModel destructor.
/*!
    This destructor must destroy every dynamic created array/object during any step of the class life cycle.
*/
LearningModels::OnnxModel::~OnnxModel()
{
    for (size_t i = 0; i < this->numInputNodes; i++)
        allocator.Free(this->inputNodeNames[i]);
}