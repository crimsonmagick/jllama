#ifndef JNI_IMPLEMENTATION_LLAMAMANAGER_H
#define JNI_IMPLEMENTATION_LLAMAMANAGER_H
#include <jni.h>
#include "llama.h"
#include "mutex"

class LlamaManager {

  private:
    struct CallbackContext {
      jobject callback;
    };
    class LlamaSession {
      public:
        void backendInit(bool useNuma);
        void backendFree();
        jobject loadModelFromFile(jbyteArray path, jobject javaParams);
        void freeModel(jobject jModel);
        jint tokenizeWithModel(jobject jModel,
                               jbyteArray jToTokenize,
                               jintArray jTokensOut,
                               jint jmaxTokens,
                               jboolean jBos);

        jobject loadContextWithModel(jobject jModel, jobject jContextParams);
        void freeContext(jobject pJobject);
        jint eval(jobject jContext,
                  jintArray jTokens,
                  jint jnTokens,
                  jint jnPast);

        jfloatArray getLogits(jobject jContext);
        jfloatArray getLogitsIth(jobject jContext, jint i);

        jint sampleTokenGreedy(jobject jContext, jobject jCandidates);

        jint tokenToPiece(jobject jModel, jint jToken, jbyteArray output);

        jint tokenBos(jobject jContext);
        jint tokenEos(jobject jModel);
        jint tokenNl(jobject jModel);

        void setLogger(jobject logger);

        jlong getTimestampInMicroseconds();
        jobject defaultContextParams();
        jint sampleToken(jobject jContext, jobject jCandidates);
        void applyRepetitionPenalty(jobject jContext,
                                    jobject jCandidates,
                                    jintArray jPreviousTokens,
                                    jfloat penalty);

        void applyFrequencyAndPresencePenalties(jobject jContext,
                                                jobject jCandidates,
                                                jintArray jLastTokens,
                                                jfloat jAlphaFrequency,
                                                jfloat jPresence);
        void llamaSampleSoftMax(jobject jContext, jobject jCandidates);
        void
        llamaSampleTopK(jobject jContext,
                        jobject jCandidates,
                        jint k,
                        jlong minkKeep);
        void
        llamaSampleTopP(jobject jContext,
                        jobject jCandidates,
                        jfloat p,
                        jlong minKeep);
        void llamaSampleTailFree(jobject jContext,
                                 jobject jCandidates,
                                 jfloat z,
                                 jlong minkeep);
        void llamaSampleTypical(jobject jContext,
                                jobject jCandidates,
                                jfloat p,
                                jint minKeep);
        void llamaSampleTemperature(jobject jContext,
                                    jobject jCandidates,
                                    jfloat temp);
      jobject defaultModelParams();
      jobject llamaBatchInitOld(jobject jContext, jint jMaxTokenCount,
                                jint jEmbeddingVectorSize, jint jSequenceIdLength);
      void llamaBatchFree(jobject jBatch);
        void
        submitSequence(jobject jBatch, jintArray jTokens, jint jSequenceId, jint sequenceTokenIndex);
        jint evaluate(jobject jContext, jobject jBatch);
        void setCurrentTokenCount(jobject jBatch, jint currentTokenCount);
      jint decodeNative(jobject jContext, jobject jBatch);
      void submitSequencePiece(jobject jBatch,
                               jintArray jToken,
                               jfloatArray jEmbd,
                               jintArray jPos,
                               jobjectArray jSeqId,
                               jbooleanArray jLogits);
      jobject
      llamaBatchInit(jobject jContext, jint nTokens, jint jEmbd, jint nSeqId);
     private:
        friend class LlamaManager;
        explicit LlamaSession(JNIEnv* env, LlamaManager* outer)
            : env(env), manager(outer) {}
        JNIEnv* env;
        LlamaManager* manager;
        class LlamaContextParamsManager {
          public:
            LlamaContextParamsManager(llama_context_params contextParams, LlamaSession* session);
            LlamaContextParamsManager(jobject javaContextParams, LlamaSession* session);
            llama_context_params getParams();
            jobject getJavaParams();

          private:
            llama_context_params llamaContextParams{};
            jobject jLlamaContextParams;
            LlamaSession* session;
        };

      class LlamaModelParamsManager {
       public:
        LlamaModelParamsManager(llama_model_params modelParams, LlamaSession* session);
        LlamaModelParamsManager(jobject javaModelParams, LlamaSession* session);
        ~LlamaModelParamsManager();
        llama_model_params getParams();
        jobject getJavaParams();

       private:
        llama_model_params llamaModelParams{};
        jobject jLlamaModelParams;
        jfloatArray tensorSplitFloatArray;
        const float* tensorSplit;
        LlamaSession* session;
      };

      void submitSequenceNative(jobject jBatch,
                                jintArray jTokens,
                                jint jSequenceId,
                                jint sequenceTokenIndex);
    };
    static LlamaManager* singleton;
    static JavaVM* javaVm;
    static jobject jloggerCallback;
    static inline std::once_flag initFlag;
    explicit LlamaManager();
    static bool progressCallback(float progress, void* ctx);
    static void loggerCallback(enum ggml_log_level level, const char * text, void * user_data);

  public:
    static LlamaManager* getLlamaManager(JNIEnv* env);
    LlamaManager::LlamaSession newSession(JNIEnv* env);

};

#endif //JNI_IMPLEMENTATION_LLAMAMANAGER_H
