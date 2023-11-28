#include <cstring>
#include <jni.h>
#include "LlamaManager.h"
#include "../libloader.h"
#include "../Utf8StringManager.h"
#include "../exceptions/exceptions.h"

const jobject OBJECT_FAILURE = nullptr;

typedef void (* llama_backend_init_pointer)(bool);
void LlamaManager::LlamaSession::backendInit(bool useNuma) {
  withJniExceptions(env, [&useNuma] {
    llama_backend_init_pointer func =
        (llama_backend_init_pointer) getFunctionAddress("llama_backend_init");
    func(useNuma);
  });
}

typedef void (* llama_backend_free_pointer)();
void LlamaManager::LlamaSession::backendFree() {
  withJniExceptions(env, [] {
    llama_backend_free_pointer func =
        (llama_backend_free_pointer) getFunctionAddress("llama_backend_init");
    func();
  });
}

void LlamaManager::progressCallback(float progress, void* ctx) {
  if (ctx) {
    JNIEnv* env;
    jint jniStatus = javaVm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_8);
    jint attachResult;
    if (jniStatus == JNI_EDETACHED) {
      attachResult = javaVm->AttachCurrentThread(reinterpret_cast<void**>(&env), nullptr);
    } else if (jniStatus == JNI_OK) {
      attachResult = JNI_OK;
    } else {
      attachResult = JNI_ERR;
    }
    if (attachResult == JNI_OK) {
      jclass floatClass = env->FindClass("java/lang/Float");
      jmethodID floatConstructor = env->GetMethodID(floatClass, "<init>", "(F)V");
      jobject floatObj = env->NewObject(floatClass, floatConstructor, progress);

      auto progressContext = reinterpret_cast<CallbackContext*>(ctx);
      jclass callbackClass = env->GetObjectClass(progressContext->callback);
      jmethodID acceptMethod = env->GetMethodID(callbackClass, "accept", "(Ljava/lang/Object;)V");
      env->CallVoidMethod(callbackClass, acceptMethod, floatObj);
      env->DeleteLocalRef(floatObj);

      if (jniStatus == JNI_EDETACHED) {
        javaVm->DetachCurrentThread();
      }
    }
  }
}

typedef llama_model* (* llama_load_model_from_file_pointer)
    (const char*, struct llama_model_params);
jobject LlamaManager::LlamaSession::loadModelFromFile(jbyteArray path, jobject javaParams) {
  return withJniExceptions(env, [&javaParams, &path, this] {
    llama_load_model_from_file_pointer llamaLoadModel =
        (llama_load_model_from_file_pointer) getFunctionAddress(
            "llama_load_model_from_file");

    auto stringManager = Utf8StringManager(env, path);
    auto paramsManager = LlamaModelParamsManager(javaParams, this);
    auto params = paramsManager.getParams();

    llama_model* model =
        llamaLoadModel(stringManager.getString(), params);

    if (params.progress_callback_user_data) {
      auto progressContext = reinterpret_cast<CallbackContext*>(params.progress_callback_user_data);
      env->DeleteGlobalRef(progressContext->callback);
      delete progressContext;
    }

    if (model) {
      return jni::constructLlamaModel(env, model);
    }
    jni::throwJNIException(env,
                           jni::JNIException(
                               "Unable to load llama model from file"));
    return OBJECT_FAILURE;
  });
}

typedef llama_context* (* llama_new_context_with_model_pointer)
    (llama_model*, llama_context_params);
jobject LlamaManager::LlamaSession::loadContextWithModel(jobject jModel, jobject jContextParams) {
  return withJniExceptions(env, [&jModel, &jContextParams, this] {
    llama_new_context_with_model_pointer llamaCreateContext =
        (llama_new_context_with_model_pointer) getFunctionAddress(
            "llama_new_context_with_model");

    auto paramsManager = LlamaContextParamsManager(jContextParams, this);
    auto llamaModel = jni::getLlamaModelPointer(env, jModel);
    llama_context
        * context = llamaCreateContext(llamaModel, paramsManager.getParams());

    if (context) {
      return jni::constructLlamaContext(env, context);
    }
    jni::throwJNIException(env,
                           jni::JNIException(
                               "Unable to create context from llama model"));
    return OBJECT_FAILURE;
  });
}


typedef int(* llama_tokenize_pointer)
    (llama_model*, const char*, int, llama_token*, int, bool, bool);
jint LlamaManager::LlamaSession::tokenizeWithModel(jobject jModel,
                                     jbyteArray jToTokenize,
                                     jintArray jTokensOut,
                                     jint jmaxTokens,
                                     jboolean jBos) {
  return withJniExceptions(env, [&jModel, &jToTokenize, &jTokensOut, &jmaxTokens, &jBos, this] {
    llama_tokenize_pointer tokenize =
        (llama_tokenize_pointer) getFunctionAddress("llama_tokenize");


    auto llamaModel = jni::getLlamaModelPointer(env, jModel);

    jbyte* toTokenize = env->GetByteArrayElements(jToTokenize, nullptr);
    jsize length = env->GetArrayLength(jToTokenize);
    jint* tokensOut = env->GetIntArrayElements(jTokensOut, nullptr);
    int result = tokenize(llamaModel,
                          reinterpret_cast<const char*>(toTokenize),
                          length,
                          reinterpret_cast<llama_token*>(tokensOut),
                          jmaxTokens,
                          jBos, false);
    if (result > 0) {
      env->ReleaseIntArrayElements(jTokensOut, tokensOut, 0);
    } else {
      env->ReleaseIntArrayElements(jTokensOut, tokensOut, JNI_ABORT);
    }
    return result;
  });

}


typedef int(* llama_eval_pointer)(llama_context*, llama_token*, int, int);
jint LlamaManager::LlamaSession::eval(jobject jContext,
                        jintArray jTokens,
                        jint jnTokens,
                        jint jnPast) {
  return withJniExceptions(env, [&jContext, &jTokens, &jnTokens, &jnPast, this] {

    llama_eval_pointer eval = (llama_eval_pointer) getFunctionAddress(
        "llama_eval");
    auto llamaContext = jni::getLlamaContextPointer(env, jContext);
    jint* tokens = env->GetIntArrayElements(jTokens, nullptr);
    int result = eval(llamaContext,
                      reinterpret_cast<int*>(tokens),
                      jnTokens,
                      jnPast);
    env->ReleaseIntArrayElements(jTokens, tokens, JNI_ABORT);
    return result;
  });
}

typedef int (* llama_n_vocab_pointer)(const struct llama_model*);
typedef float* (* llama_get_logits_pointer)(llama_context*);
typedef llama_model* (* llama_get_model_pointer)(llama_context* ctx);
jfloatArray LlamaManager::LlamaSession::getLogits(jobject jContext) {
  return withJniExceptions(env, [&jContext, this] {
    auto getLogits = (llama_get_logits_pointer) getFunctionAddress(
        "llama_get_logits");
    auto getModel = (llama_get_model_pointer) getFunctionAddress("llama_get_model");
    auto getVocabLength = (llama_n_vocab_pointer) getFunctionAddress("llama_n_vocab");
    auto llamaContext = jni::getLlamaContextPointer(env, jContext);
    float* logits = getLogits(llamaContext);
    auto llamaModel = getModel(llamaContext);
    int vocabLength = getVocabLength(llamaModel);
    auto jLogits = env->NewFloatArray(vocabLength);
    env->SetFloatArrayRegion(jLogits, 0, vocabLength, logits);
    return jLogits;
  });
}

typedef llama_token (* llama_sample_token_pointer)
    (struct llama_context*, llama_token_data_array*);
jint LlamaManager::LlamaSession::sampleTokenGreedy(jobject jContext, jobject jCandidates) {
  return withJniExceptions(env, [this, jContext, jCandidates] {
    auto sampleTokenGreedily = (llama_sample_token_pointer) getFunctionAddress("llama_sample_token_greedy");
    auto llamaContext = jni::getLlamaContextPointer(env, jContext);
    llama_token_data_array candidates = jni::getTokenDataArray(env, jCandidates);
    jint sampled = sampleTokenGreedily(llamaContext, &candidates);
    delete[] candidates.data;
    return sampled;
  });
}

jint LlamaManager::LlamaSession::sampleToken(jobject jContext, jobject jCandidates) {
  return withJniExceptions(env, [this, jContext, jCandidates] {
    auto sampleToken = (llama_sample_token_pointer) getFunctionAddress("llama_sample_token");
    auto llamaContext = jni::getLlamaContextPointer(env, jContext);
    llama_token_data_array candidates = jni::getTokenDataArray(env, jCandidates);
    jint sampled = sampleToken(llamaContext, &candidates);
    delete[] candidates.data;
    return sampled;
  });
}

typedef int (* llama_token_to_piece_pointer)(llama_model*, llama_token, char*, int);
jint LlamaManager::LlamaSession::tokenToPiece(jobject jModel, jint jToken, jbyteArray joutput) {
  return withJniExceptions(env, [&jModel, jToken, this, joutput]{
    auto detokenize = (llama_token_to_piece_pointer) getFunctionAddress(
        "llama_token_to_piece");

    auto joutputPointer = env->GetByteArrayElements(joutput, nullptr);
    char* outputPointer = reinterpret_cast<char*>(joutputPointer);
    jsize length = env->GetArrayLength(joutput);
    auto llamaModel = jni::getLlamaModelPointer(env, jModel);

    int result = detokenize(llamaModel, jToken, outputPointer, length);
    env->ReleaseByteArrayElements(joutput, joutputPointer, 0);
    return result;
  });
}

typedef llama_token (* get_special_token_pointer)(llama_context*);

jint LlamaManager::LlamaSession::tokenBos(jobject jContext) {
  return withJniExceptions(env, [this, jContext]{
    auto context = jni::getLlamaContextPointer(env, jContext);
    auto getBos = (get_special_token_pointer) getFunctionAddress("llama_token_bos");
    return getBos(context);
  });
}

jint LlamaManager::LlamaSession::tokenEos(jobject jContext) {
  return withJniExceptions(env, [this, jContext]{
    auto context = jni::getLlamaContextPointer(env, jContext);
    auto getEos = (get_special_token_pointer) getFunctionAddress("llama_token_eos");
    return getEos(context);
  });
}

jint LlamaManager::LlamaSession::tokenNl(jobject jContext) {
  return withJniExceptions(env, [this, jContext]{
    auto context = jni::getLlamaContextPointer(env, jContext);
    auto getNl = (get_special_token_pointer) getFunctionAddress("llama_token_nl");
    return getNl(context);
  });
}

void LlamaManager::loggerCallback(enum ggml_log_level level, const char* text, void* user_data) {
  JNIEnv* env;
  jint jniStatus = javaVm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_8);
  jint attachResult;
  if (jniStatus == JNI_EDETACHED) {
    attachResult = javaVm->AttachCurrentThread(reinterpret_cast<void**>(&env), nullptr);
  } else if (jniStatus == JNI_OK) {
    attachResult = JNI_OK;
  } else {
    attachResult = JNI_ERR;
  }
  if (attachResult == JNI_OK) {

    jclass jlogLevelClass = env->FindClass("net/jllama/llama/cpp/java/bindings/LlamaLogLevel");

    jfieldID fieldID;
    switch(level) {
      case 2: fieldID = env->GetStaticFieldID(jlogLevelClass, "ERROR", "Lnet/jllama/llama/cpp/java/bindings/LlamaLogLevel;"); break;
      case 3:
        fieldID = env->GetStaticFieldID(jlogLevelClass, "WARN", "Lnet/jllama/llama/cpp/java/bindings/LlamaLogLevel;");
        break;
      case 4:
        fieldID = env->GetStaticFieldID(jlogLevelClass, "INFO", "Lnet/jllama/llama/cpp/java/bindings/LlamaLogLevel;");
        break;
      default:
        fieldID = env->GetStaticFieldID(jlogLevelClass, "WARN", "Lnet/jllama/llama/cpp/java/bindings/LlamaLogLevel;");
    }
    jobject jLogLevel = env->GetStaticObjectField(jlogLevelClass, fieldID);

    int textLength = static_cast<jint>(strlen(text));
    jbyteArray jtext = env->NewByteArray(textLength);
    jbyte* jtextPointer = env->GetByteArrayElements(jtext, nullptr);
    memcpy(jtextPointer, text, textLength);
    env->ReleaseByteArrayElements(jtext, jtextPointer, 0);

    jclass callbackClass = env->GetObjectClass(jloggerCallback);
    jmethodID acceptMethod = env->GetMethodID(callbackClass, "accept", "(Ljava/lang/Object;Ljava/lang/Object;)V");
    env->CallVoidMethod(callbackClass, acceptMethod, jLogLevel, jtext);

    if (jniStatus == JNI_EDETACHED) {
      javaVm->DetachCurrentThread();
    }

  }
}

typedef void (* llama_log_set_pointer)(ggml_log_callback, void*);
void LlamaManager::LlamaSession::setLogger(jobject logger) {
  withJniExceptions(env, [this, logger] {
    if (jloggerCallback) {
      env->DeleteGlobalRef(jloggerCallback);
    }
    jloggerCallback = env->NewGlobalRef(logger);
    auto logSet = reinterpret_cast<llama_log_set_pointer>(getFunctionAddress("llama_log_set"));
    logSet(loggerCallback, nullptr);
    env->DeleteLocalRef(logger);
  });
}


typedef void (* llama_free_model_pointer)(struct llama_model*);
void LlamaManager::LlamaSession::freeModel(jobject jModel) {
  withJniExceptions(env, [this, jModel] {
    auto freeModel =
        reinterpret_cast<llama_free_model_pointer>(getFunctionAddress(
            "llama_free_model"));

    llama_model* model = jni::getLlamaModelPointer(env, jModel);
    freeModel(model);
  });
}

typedef void (* llama_free_pointer)(struct llama_context*);
void LlamaManager::LlamaSession::freeContext(jobject jContext) {
  withJniExceptions(env, [this, jContext] {
    auto freeContext =
        reinterpret_cast<llama_free_pointer>(getFunctionAddress(
            "llama_free"));

    llama_context* context = jni::getLlamaContextPointer(env, jContext);
    freeContext(context);
  });
}

typedef uint64_t (* llama_time_us_pointer)();
jlong LlamaManager::LlamaSession::getTimestampInMicroseconds(void) {
  return withJniExceptions(env, [] {
    auto getTimestamp =
        reinterpret_cast<llama_time_us_pointer>(getFunctionAddress("llama_time_us"));
    return getTimestamp();
  });
}

typedef llama_context_params (* llama_context_default_params_pointer)(void);
jobject LlamaManager::LlamaSession::defaultContextParams() {
  return withJniExceptions(env, [this] {
    auto getDefaultParams =
        reinterpret_cast<llama_context_default_params_pointer>(getFunctionAddress(
            "llama_context_default_params"));
    llama_context_params params = getDefaultParams();
    auto paramsManager = LlamaContextParamsManager(params, this);
    jobject jParams = paramsManager.getJavaParams();
    return jParams;
  });
}

typedef llama_model_params (* llama_model_default_params_pointer)(void);
jobject LlamaManager::LlamaSession::defaultModelParams() {
  return withJniExceptions(env, [this] {
    auto getDefaultParams =
        reinterpret_cast<llama_model_default_params_pointer>(getFunctionAddress(
            "llama_context_default_params"));
    llama_model_params params = getDefaultParams();
    auto paramsManager = LlamaModelParamsManager(params, this);
    jobject jParams = paramsManager.getJavaParams();
    return jParams;
  });}

typedef void (* llama_sample_repetition_penalty_pointer) (struct llama_context * ctx,
    llama_token_data_array * candidates, const llama_token * last_tokens,
    size_t last_tokens_size, float penalty);

void LlamaManager::LlamaSession::applyRepetitionPenalty(jobject jContext,
  jobject jCandidates, jintArray jPreviousTokens, jfloat jPenalty) {

    withJniExceptions(env, [jContext, this, jCandidates, jPreviousTokens, jPenalty] {
    auto applyPenalty = reinterpret_cast<llama_sample_repetition_penalty_pointer>(
      getFunctionAddress("llama_sample_repetition_penalties"));
    llama_context* context = jni::getLlamaContextPointer(env, jContext);

    llama_token_data_array candidates = jni::getTokenDataArray(env, jCandidates);
    jsize jPreviousTokensLength = env->GetArrayLength(jPreviousTokens);
    jint* jPreviousTokensPointer = env->GetIntArrayElements(jPreviousTokens, nullptr);
    int* previousTokens = reinterpret_cast<int*>(jPreviousTokensPointer);
    if (!jPreviousTokensPointer) {
      throw jni::JNIException("Unable to get jPreviousTokens/jPreviousTokensPointer.");
    }
    applyPenalty(context, &candidates, previousTokens, jPreviousTokensLength, jPenalty);

    env->ReleaseIntArrayElements(jPreviousTokens, jPreviousTokensPointer, JNI_ABORT);
    jni::updateTokenDateArray(env, jCandidates, &candidates);
  });
}

typedef void (* llama_sample_frequency_and_presence_penalties_pointer) (struct llama_context * ctx, llama_token_data_array * candidates, const llama_token * last_tokens, size_t last_tokens_size, float alpha_frequency, float penalty);

void LlamaManager::LlamaSession::applyFrequencyAndPresencePenalties(jobject jContext, jobject jCandidates, jintArray jLastTokens, jfloat jAlphaFrequency, jfloat jPresence) {
    withJniExceptions(env, [jContext, this, jCandidates, jLastTokens, jAlphaFrequency, jPresence] {
        auto applyPenalty = reinterpret_cast<llama_sample_frequency_and_presence_penalties_pointer>(
        getFunctionAddress("llama_sample_frequency_and_presence_penalties"));
        llama_context* context = jni::getLlamaContextPointer(env, jContext);

        llama_token_data_array candidates = jni::getTokenDataArray(env, jCandidates);
        jsize jLastTokensLength = env->GetArrayLength(jLastTokens);
        jint* jLastTokensPointer = env->GetIntArrayElements(jLastTokens, nullptr);
        int* lastTokens = reinterpret_cast<int*>(jLastTokensPointer);
        if (!jLastTokensPointer) {
        throw jni::JNIException("Unable to get jLastTokens/jLastTokensPointer.");
        }
        applyPenalty(context, &candidates, lastTokens, jLastTokensLength, jAlphaFrequency, jPresence);

        env->ReleaseIntArrayElements(jLastTokens, jLastTokensPointer, JNI_ABORT);
        jni::updateTokenDateArray(env, jCandidates, &candidates);
    });
}

typedef void (* llama_sample_softmax_pointer) (struct llama_context * ctx, llama_token_data_array * candidates);
void LlamaManager::LlamaSession::llamaSampleSoftMax(jobject jContext,
                                                    jobject jCandidates) {
  withJniExceptions(env, [jContext, this, jCandidates] {
    auto sampleSoftMax = reinterpret_cast<llama_sample_softmax_pointer>(getFunctionAddress(
        "llama_sample_softmax"));
    llama_context* context = jni::getLlamaContextPointer(env, jContext);
    llama_token_data_array candidates = jni::getTokenDataArray(env, jCandidates);
    sampleSoftMax(context, &candidates);
    jni::updateTokenDateArray(env, jCandidates, &candidates);
  });
}

typedef void (* llama_sample_top_k_pointer)
(struct llama_context * ctx, llama_token_data_array * candidates, int k, size_t min_keep);
void LlamaManager::LlamaSession::llamaSampleTopK(jobject jContext,
                                                 jobject jCandidates,
                                                 jint k,
                                                 jlong minkKeep) {
  withJniExceptions(env, [jContext, this, jCandidates, k, minkKeep] {
    auto sampleTopK = reinterpret_cast<llama_sample_top_k_pointer>(getFunctionAddress(
        "llama_sample_top_k"));
    llama_context* context = jni::getLlamaContextPointer(env, jContext);
    llama_token_data_array candidates = jni::getTokenDataArray(env, jCandidates);
    sampleTopK(context, &candidates, k, minkKeep);
    jni::updateTokenDateArray(env, jCandidates, &candidates);
  });
}

typedef void (* llama_sample_top_p_pointer) (struct llama_context * ctx, llama_token_data_array * candidates, float p, size_t min_keep);
void LlamaManager::LlamaSession::llamaSampleTopP(jobject jContext,
                                                 jobject jCandidates,
                                                 jfloat p,
                                                 jlong minKeep) {
  withJniExceptions(env, [jContext, this, jCandidates, p, minKeep] {
    auto sampleTopP = reinterpret_cast<llama_sample_top_p_pointer>(getFunctionAddress(
        "llama_sample_top_p"));
    llama_context* context = jni::getLlamaContextPointer(env, jContext);
    llama_token_data_array candidates = jni::getTokenDataArray(env, jCandidates);
    sampleTopP(context, &candidates, p, minKeep);
    jni::updateTokenDateArray(env, jCandidates, &candidates);
  });
}

typedef void (* llama_sample_tail_free_pointer) (struct llama_context * ctx, llama_token_data_array * candidates, float z, size_t min_keep);
void LlamaManager::LlamaSession::llamaSampleTailFree(jobject jContext,
                                                     jobject jCandidates,
                                                     jfloat z,
                                                     jlong minkeep) {
  withJniExceptions(env, [jContext, this, jCandidates, z, minkeep] {
    auto sampleTailFree = reinterpret_cast<llama_sample_tail_free_pointer>(getFunctionAddress(
        "llama_sample_tail_free"));
    llama_context* context = jni::getLlamaContextPointer(env, jContext);
    llama_token_data_array candidates = jni::getTokenDataArray(env, jCandidates);
    sampleTailFree(context, &candidates, z, minkeep);
    jni::updateTokenDateArray(env, jCandidates, &candidates);
  });
}

typedef void (* llama_sample_typical_pointer) (struct llama_context * ctx, llama_token_data_array * candidates, float p, size_t min_keep);
void LlamaManager::LlamaSession::llamaSampleTypical(jobject jContext,
                                                    jobject jCandidates,
                                                    jfloat p,
                                                    jint minKeep) {
  withJniExceptions(env, [jContext, this, jCandidates, p, minKeep] {
    auto sampleTypical = reinterpret_cast<llama_sample_typical_pointer>(getFunctionAddress(
        "llama_sample_typical"));
    llama_context* context = jni::getLlamaContextPointer(env, jContext);
    llama_token_data_array candidates = jni::getTokenDataArray(env, jCandidates);
    sampleTypical(context, &candidates, p, minKeep);
    jni::updateTokenDateArray(env, jCandidates, &candidates);
  });
}

typedef void (* llama_sample_temperature_pointer) (struct llama_context * ctx, llama_token_data_array * candidates, float temp);
void LlamaManager::LlamaSession::llamaSampleTemperature(jobject jContext,
                                                        jobject jCandidates,
                                                        jfloat temp) {
  withJniExceptions(env, [jContext, this, jCandidates, temp] {
    auto sampleTemperature = reinterpret_cast<llama_sample_temperature_pointer>(getFunctionAddress(
        "llama_sample_temperature"));
    llama_context* context = jni::getLlamaContextPointer(env, jContext);
    llama_token_data_array candidates = jni::getTokenDataArray(env, jCandidates);
    sampleTemperature(context, &candidates, temp);
    jni::updateTokenDateArray(env, jCandidates, &candidates);
  });
}

typedef llama_batch (* llama_batch_init_pointer) (int32_t n_tokens, int32_t embd, int32_t n_seq_max);
jobject LlamaManager::LlamaSession::llamaBatchInit(jobject jContext,
                                                   jint jMaxTokenCount,
                                                   jint jEmbeddingVectorSize,
                                                   jint jSequenceIdLength) {
  return withJniExceptions(env, [this, jContext, jMaxTokenCount, jEmbeddingVectorSize, jSequenceIdLength] {
    auto batchInit = reinterpret_cast<llama_batch_init_pointer>(getFunctionAddress("llama_batch_init"));
    llama_batch batchStack = batchInit(jMaxTokenCount, jEmbeddingVectorSize, jSequenceIdLength);
    llama_batch* batchHeap = new llama_batch;
    std::memcpy(batchHeap, &batchStack, sizeof(llama_batch));
    return jni::constructBatch(env, jContext, jMaxTokenCount, batchHeap);
  });
}

typedef void (* llama_batch_free_pointer) (llama_batch);
void LlamaManager::LlamaSession::llamaBatchFree(jobject jBatch) {
  return withJniExceptions(env, [this, jBatch] {
    auto batchFree = reinterpret_cast<llama_batch_free_pointer>(getFunctionAddress("llama_batch_free"));
    llama_batch* batch = jni::getLlamaBatchPointer(env, jBatch);
    batchFree(*batch);
    delete batch;
  });
}