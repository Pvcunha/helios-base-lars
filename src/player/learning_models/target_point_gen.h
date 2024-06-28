#include "learning_models/learning_models.h"

namespace LearningModels
{
    class TargetPointGenerator : LearningModels::OnnxModel
    {
    public:
        TargetPointGenerator(const char *modelPath, Ort::Env *env, int numberofThreads)
            : LearningModels::OnnxModel(modelPath, env){ };
        
        std::vector<float> getOuput()
        {
            std::vector<std::vector<float>> frame = this->createFrame();
            float *result = this->forward<float>(frame);
            return std::vector<float>{result[0], result[1]};
        };

        std::vector<float> getOuput(std::vector<float> raw_wm, std::vector<float> bps_row)
        {
            std::vector<std::vector<float>> frame = this->createFrame(raw_wm, bps_row);
            float *result = this->forward<float>(frame);
            return std::vector<float>{result[0], result[1]};
        };
        
        static TargetPointGenerator &instance() {
            static TargetPointGenerator instance("models/fset1.onnx", new Ort::Env(ORT_LOGGING_LEVEL_WARNING, "test"), 1);
            return instance;
        }

    private:
        std::vector<std::vector<float>> createFrame()
        {
            const int framesize2 = 138;
            float frame2[framesize2] = {0};
            auto y = std::vector<float>(frame2, frame2 + framesize2);

            const int framesize3 = 780;
            float frame3[framesize3] = {0};
            auto z = std::vector<float>(frame3, frame3 + framesize3);

            return std::vector<std::vector<float>>{y, z};
        }

        std::vector<std::vector<float>> createFrame(std::vector<float> raw_wm, std::vector<float> bps_row)
        {
            return std::vector<std::vector<float>>{raw_wm, bps_row};
        }

    };
}