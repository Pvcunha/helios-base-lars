#include "learning_models/learning_models.h"

class BPSModel: LearningModels::OnnxModel
{
    public:
        BPSModel(const char *modelPath, Ort::Env *env, int numberofThreads) 
        : LearningModels::OnnxModel(modelPath, env){};
        float getOuput(std::vector<float> &frame){
            float result = this->forward<float>(frame)[0];
            return result;
        };

        static BPSModel &i(){
            Ort::Env* onnxEnv = new Ort::Env(ORT_LOGGING_LEVEL_WARNING, "test");
            static BPSModel instance("models/bps_model.onnx", onnxEnv, 1);
            return instance;
        };
};