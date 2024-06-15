#include "learning_models/learning_models.h"

class BPSModel: LearningModels::OnnxModel
{
    public:
        BPSModel(const char *modelPath, Ort::Env *env, int numberofThreads) 
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