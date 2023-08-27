#include "LlamaSession.h"
#include "../libloader.h"
#include "../LlamaContextParamsManager.h"
#include "../Utf8StringManager.h"
#include "../exceptions/exceptions.h"

const jobject OBJECT_FAILURE = nullptr;

typedef void (* llama_backend_init_pointer)(bool);
void LlamaSession::backendInit(bool useNuma) {
  withJniExceptions(env, [&useNuma] {
    llama_backend_init_pointer func =
        (llama_backend_init_pointer) getFunctionAddress("llama_backend_init");
    func(useNuma);
  });
}

typedef void (* llama_backend_free_pointer)();
void LlamaSession::backendFree() {
  withJniExceptions(env, [] {
    llama_backend_free_pointer func =
        (llama_backend_free_pointer) getFunctionAddress("llama_backend_init");
    func();
  });
}

typedef llama_model* (* llama_load_model_from_file_pointer)
    (const char*, struct llama_context_params);
jobject LlamaSession::loadModelFromFile(jbyteArray path, jobject javaParams) {
  return withJniExceptions(env, [&javaParams, &path, this] {
    llama_load_model_from_file_pointer llamaLoadModel =
        (llama_load_model_from_file_pointer) getFunctionAddress(
            "llama_load_model_from_file");

    auto stringManager = Utf8StringManager(env, path);
    auto paramsManager = LlamaContextParamsManager(env, javaParams);

    llama_model* model =
        llamaLoadModel(stringManager.getUtf8String(), paramsManager.getParams());

    if (model) {
      return jni::constructLlamaOpaqueModel(env, model);
    }
    jni::throwJNIException(env,
                           jni::JNIException(
                               "Unable to load llama model from file"));
    return OBJECT_FAILURE;
  });
}

typedef llama_context* (* llama_new_context_with_model_pointer)
    (llama_model*, llama_context_params);
jobject LlamaSession::loadContextWithModel(jobject jModel, jobject jContextParams) {
  return withJniExceptions(env, [&jModel, &jContextParams, this] {
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
    return OBJECT_FAILURE;
  });
}


typedef int(* llama_tokenize_with_model_pointer)
    (llama_model*, const char*, llama_token*, int, bool);
jint LlamaSession::tokenizeWithModel(jobject jModel,
                                     jbyteArray jToTokenize,
                                     jintArray jTokensOut,
                                     jint jmaxTokens,
                                     jboolean jBos) {
  return withJniExceptions(env, [&jModel, &jToTokenize, &jTokensOut, &jmaxTokens, &jBos, this] {
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
  return withJniExceptions(env, [&jContext, &jTokens, &jnTokens, &jnPast, &jnThreads, this] {

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

typedef int (* llama_n_vocab_pointer)(const struct llama_context*);
typedef float* (* llama_get_logits_pointer)(llama_context*);
jfloatArray LlamaSession::getLogits(jobject jContext) {
  return withJniExceptions(env, [&jContext, this] {
    auto getLogits = (llama_get_logits_pointer) getFunctionAddress(
        "llama_get_logits");
    auto getVocabLength = (llama_n_vocab_pointer) getFunctionAddress("llama_n_vocab");
    auto llamaContext = jni::getLlamaContextPointer(env, jContext);
    float* logits = getLogits(llamaContext);
    int vocabLength = getVocabLength(llamaContext);
    auto jLogits = env->NewFloatArray(vocabLength);
    env->SetFloatArrayRegion(jLogits, 0, vocabLength, logits);
    return jLogits;
  });
}

typedef llama_token (* llama_sample_token_greedy_pointer)
    (struct llama_context*, llama_token_data_array*);
jint LlamaSession::sampleTokenGreedy(jobject jContext, jobject jCandidates) {
  return withJniExceptions(env, [this, &jContext, &jCandidates] {
    auto sampleTokenGreedily = (llama_sample_token_greedy_pointer) getFunctionAddress("llama_sample_token_greedy");
    auto llamaContext = jni::getLlamaContextPointer(env, jContext);
    llama_token_data_array candidates = jni::getTokenDataArray(env, jCandidates);
    jint sampled = sampleTokenGreedily(llamaContext, &candidates);
    delete[] candidates.data;
    return sampled;
  });
}

typedef const char* (* llama_token_to_str_pointer)(llama_context*, llama_token);
jbyteArray LlamaSession::tokenToStr(jobject jContext, jint jToken) {
  return withJniExceptions(env, [&jContext, &jToken, this]{
    auto detokenize = (llama_token_to_str_pointer) getFunctionAddress(
        "llama_token_to_str");

    auto llamaContext = jni::getLlamaContextPointer(env, jContext);

    const char * result = detokenize(llamaContext, jToken);
    auto length = static_cast<jsize>(strlen(result));
    jbyteArray detokenized = env->NewByteArray(length);
    env->SetByteArrayRegion(detokenized, 0, length, reinterpret_cast<const jbyte*>(result));
    return detokenized;
  });
}

typedef llama_token (* get_special_token_pointer)();

jint LlamaSession::tokenBos() {
  return withJniExceptions(env, []{
    auto getBos = (get_special_token_pointer) getFunctionAddress("llama_token_bos");
    return getBos();
  });
}

jint LlamaSession::tokenEos() {
  return withJniExceptions(env, []{
    auto getBos = (get_special_token_pointer) getFunctionAddress("llama_token_eos");
    return getBos();
  });
}

jint LlamaSession::tokenNl() {
  return withJniExceptions(env, []{
    auto getBos = (get_special_token_pointer) getFunctionAddress("llama_token_nl");
    return getBos();
  });
}
