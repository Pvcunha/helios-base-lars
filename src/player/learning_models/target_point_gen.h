#include "learning_models/learning_models.h"

namespace LearningModels
{
    class TargetPointGenerator : LearningModels::OnnxModel
    {
    public:
        TargetPointGenerator(const char *modelPath, Ort::Env *env, int numberofThreads, int wm_size, int raw_wm_size, int bps_row_size)
            : LearningModels::OnnxModel(modelPath, env){
                this->wm_size = wm_size;
                this->raw_wm_size = raw_wm_size;
                this->bps_row_size = bps_row_size;
            };
        
        std::vector<float> getOuput()
        {
            std::vector<std::vector<float>> frame = this->createFrame();
            float *result = this->forward<float>(frame);
            return std::vector<float>{result[0], result[1]};
        };

        std::vector<float> getOuput(std::vector<float> wm, std::vector<float> raw_wm, std::vector<float> bps_row)
        {
            std::vector<std::vector<float>> frame = this->createFrame(wm, raw_wm, bps_row);
            float *result = this->forward<float>(frame);
            return std::vector<float>{result[0], result[1]};
        };
        
        static TargetPointGenerator &instance() {
            static TargetPointGenerator instance("models/tpg_fset1.onnx", new Ort::Env(ORT_LOGGING_LEVEL_WARNING, "test"), 1, 167, 138, 780);
            return instance;
        }

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

        std::vector<std::vector<float>> createFrame(std::vector<float> wm, std::vector<float> raw_wm, std::vector<float> bps_row)
        {
            return std::vector<std::vector<float>>{wm, raw_wm, bps_row};
        }

        int raw_wm_size;
        int wm_size;
        int bps_row_size;
    };
}