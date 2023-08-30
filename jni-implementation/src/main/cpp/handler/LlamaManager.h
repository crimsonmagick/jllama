#ifndef JNI_IMPLEMENTATION_LLAMAMANAGER_H
#define JNI_IMPLEMENTATION_LLAMAMANAGER_H
#include <jni.h>
#include "llama.h"
#include "mutex"

class LlamaManager {

    class LlamaSession {
      public:
        void backendInit(bool useNuma);
        void backendFree();
        jobject loadModelFromFile(jbyteArray path, jobject javaParams);
        jint tokenizeWithModel(jobject jModel,
                               jbyteArray jToTokenize,
                               jintArray jTokensOut,
                               jint jmaxTokens,
                               jboolean jBos);

        jobject loadContextWithModel(jobject jModel, jobject jContextParams);
        jint eval(jobject jContext,
                  jintArray jTokens,
                  jint jnTokens,
                  jint jnPast,
                  jint jnThreads);

        jfloatArray getLogits(jobject jContext);

        jint sampleTokenGreedy(jobject jContext, jobject jCandidates);

        jbyteArray tokenToStr(jobject jContext, jint jToken);

        jint tokenBos();
        jint tokenEos();
        jint tokenNl();

      private:
        friend class LlamaManager;
        explicit LlamaSession(JNIEnv* env, LlamaManager* outer) : env(env), manager(outer) {}
        JNIEnv* env;
        LlamaManager* manager;
    };

  public:
    static LlamaManager* getLlamaManager(JNIEnv* env);
    llama_progress_callback getProgressCallback();
    void* generateProgressCallbackContext();
    LlamaManager::LlamaSession newSession(JNIEnv* env);

  private:
    static LlamaManager* singleton;
    static JavaVM* javaVm;
    static inline std::once_flag initFlag;
    explicit LlamaManager();
    static void progressCallback(float progress, void* ctx);

};

#endif //JNI_IMPLEMENTATION_LLAMAMANAGER_H
