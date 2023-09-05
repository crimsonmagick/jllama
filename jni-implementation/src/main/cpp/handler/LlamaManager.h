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
                  jint jnPast,
                  jint jnThreads);

        jfloatArray getLogits(jobject jContext);

        jint sampleTokenGreedy(jobject jContext, jobject jCandidates);

        jint tokenToPiece(jobject jContext, jint jToken, jbyteArray output);

        jint tokenBos(jobject jContext);
        jint tokenEos(jobject jContext);
        jint tokenNl(jobject jContext);

        void setLogger(jobject logger);

        jlong getTimestampInMicroseconds();
        jobject defaultContextParams();
        jint sampleToken(jobject jContext, jobject jCandidates);
        void applyRepetitionPenalty(jobject jContext,
                                    jobject jCandidates,
                                    jintArray jPreviousTokens,
                                    jfloat penalty);

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
            ~LlamaContextParamsManager();
            llama_context_params getParams();
            jobject getJavaPrams();

          private:
            llama_context_params llamaContextParams{};
            jobject jLlamaContextParams;
            jfloatArray tensorSplitFloatArray;
            const float* tensorSplit;
            LlamaSession* session;
        };
    };
    static LlamaManager* singleton;
    static JavaVM* javaVm;
    static jobject jloggerCallback;
    static inline std::once_flag initFlag;
    explicit LlamaManager();
    static void progressCallback(float progress, void* ctx);
    static void loggerCallback(enum llama_log_level level, const char * text, void * user_data);

  public:
    static LlamaManager* getLlamaManager(JNIEnv* env);
    LlamaManager::LlamaSession newSession(JNIEnv* env);

};

#endif //JNI_IMPLEMENTATION_LLAMAMANAGER_H
