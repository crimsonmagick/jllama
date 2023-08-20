#include "LlamaSession.h"
#include "../libloader.h"
#include "../LlamaContextParamsManager.h"
#include "../Utf8StringManager.h"

const int FAILURE = 1;

typedef void (* llama_backend_init_pointer)(bool);
void LlamaSession::backendInit(bool useNuma) {
  withJniExceptions([&useNuma] {
    llama_backend_init_pointer func =
        (llama_backend_init_pointer) getFunctionAddress("llama_backend_init");
    func(useNuma);
  });
}

typedef void (* llama_backend_free_pointer)();
void LlamaSession::backendFree() {
  withJniExceptions([] {
    llama_backend_free_pointer func =
        (llama_backend_free_pointer) getFunctionAddress("llama_backend_init");
    func();
  });
}

typedef llama_model* (* llama_load_model_from_file_pointer)
    (const char*, struct llama_context_params);
jobject LlamaSession::loadModelFromFile(jbyteArray path, jobject javaParams) {
  return withJniExceptions([&javaParams, &path, this] {
    llama_load_model_from_file_pointer llamaLoadModel =
        (llama_load_model_from_file_pointer) getFunctionAddress(
            "llama_load_model_from_file");

    auto paramsManager = LlamaContextParamsManager(env, path, javaParams);

    llama_model* model =
        llamaLoadModel(paramsManager.getPath(), paramsManager.getParams());

    if (model) {
      return jni::constructLlamaOpaqueModel(env, model);
    }
    jni::throwJNIException(env,
                           jni::JNIException(
                               "Unable to load llama model from file"));
  });
}

typedef llama_context* (* llama_new_context_with_model_pointer)
    (llama_model*, llama_context_params);
jobject
LlamaSession::loadContextWithModel(jobject jModel, jobject jContextParams) {
  return withJniExceptions([&jModel, &jContextParams, this] {
    llama_new_context_with_model_pointer llamaCreateContext =
        (llama_new_context_with_model_pointer) getFunctionAddress(
            "llama_new_context_with_model");

    auto paramsManager = LlamaContextParamsManager(env, jContextParams);
    auto llamaModel = jni::getLlamaModelPointer(env, jModel);
    llama_context
        * context = llamaCreateContext(llamaModel, paramsManager.getParams());

    if (context) {
      return jni::constructLlamaOpaqueContext(env, context);
    }
    jni::throwJNIException(env,
                           jni::JNIException(
                               "Unable to create context from llama model"));
  });
}


typedef int(* llama_tokenize_with_model_pointer)
    (llama_model*, const char*, llama_token*, int, bool);
jint LlamaSession::tokenizeWithModel(jobject jModel,
                                     jbyteArray jToTokenize,
                                     jintArray jTokensOut,
                                     jint jmaxTokens,
                                     jboolean jBos) {
  return withJniExceptions([&jModel, &jToTokenize, &jTokensOut, &jmaxTokens, &jBos, this] {
    llama_tokenize_with_model_pointer tokenize =
        (llama_tokenize_with_model_pointer) getFunctionAddress(
            "llama_tokenize_with_model");

    auto llamaModel = jni::getLlamaModelPointer(env, jModel);

    auto stringManager = Utf8StringManager(env, jToTokenize);
    auto toTokenize = stringManager.getUtf8String();
    jint* tokensOut = env->GetIntArrayElements(jTokensOut, nullptr);
    int result = tokenize(llamaModel,
                          toTokenize,
                          reinterpret_cast<llama_token*>(tokensOut),
                          jmaxTokens,
                          jBos);
    if (result > 0) {
      env->ReleaseIntArrayElements(jTokensOut, tokensOut, 0);
    } else {
      env->ReleaseIntArrayElements(jTokensOut, tokensOut, JNI_ABORT);
    }
    return result;
  });

}


typedef int(* llama_eval_pointer)(llama_context*, llama_token*, int, int, int);
jint LlamaSession::eval(jobject jContext,
                        jintArray jTokens,
                        jint jnTokens,
                        jint jnPast,
                        jint jnThreads) {
  return withJniExceptions([&jContext, &jTokens, &jnTokens, &jnPast, &jnThreads, this] {

    llama_eval_pointer eval = (llama_eval_pointer) getFunctionAddress(
        "llama_eval");
    auto llamaContext = jni::getLlamaContextPointer(env, jContext);
    jint* tokens = env->GetIntArrayElements(jTokens, nullptr);
    int result = eval(llamaContext,
                      reinterpret_cast<int*>(tokens),
                      jnTokens,
                      jnPast,
                      jnThreads);
    env->ReleaseIntArrayElements(jTokens, tokens, JNI_ABORT);
    return result;
  });
}
