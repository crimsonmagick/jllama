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

bool LlamaManager::progressCallback(float progress, void* ctx) {
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
  return false;
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
                                     jboolean jBos,
                                     jboolean jSpecial) {
  return withJniExceptions(env, [jModel, jToTokenize, jTokensOut, jmaxTokens, jBos, jSpecial, this] {
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
                          jBos, jSpecial);
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

jfloatArray LlamaManager::LlamaSession::getLogitsIth(jobject jContext, jint i) {
  return withJniExceptions(env, [&jContext, this, i] {
    auto getLogits = (llama_get_logits_pointer) getFunctionAddress(
        "llama_get_logits");
    auto getModel = (llama_get_model_pointer) getFunctionAddress("llama_get_model");
    auto getVocabLength = (llama_n_vocab_pointer) getFunctionAddress("llama_n_vocab");
    auto llamaContext = jni::getLlamaContextPointer(env, jContext);
    float* logits = getLogits(llamaContext);
    auto llamaModel = getModel(llamaContext);
    int vocabLength = getVocabLength(llamaModel);
    auto jLogits = env->NewFloatArray(vocabLength);
    env->SetFloatArrayRegion(jLogits, 0, vocabLength, logits + i * vocabLength);
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

typedef llama_token (* get_special_token_pointer)(llama_model*);

jint LlamaManager::LlamaSession::tokenBos(jobject jModel) {
  return withJniExceptions(env, [this, jModel]{
    auto model = jni::getLlamaModelPointer(env, jModel);
    auto getBos = (get_special_token_pointer) getFunctionAddress("llama_token_bos");
    return getBos(model);
  });
}

jint LlamaManager::LlamaSession::tokenEos(jobject jModel) {
  return withJniExceptions(env, [this, jModel]{
    auto model = jni::getLlamaModelPointer(env, jModel);
    auto getEos = (get_special_token_pointer) getFunctionAddress("llama_token_eos");
    return getEos(model);
  });
}

jint LlamaManager::LlamaSession::tokenNl(jobject jModel) {
  return withJniExceptions(env, [this, jModel]{
    auto model = jni::getLlamaModelPointer(env, jModel);
    auto getNl = (get_special_token_pointer) getFunctionAddress("llama_token_nl");
    return getNl(model);
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

    jclass jlogLevelClass = env->FindClass("net/jllama/core/LlamaLogLevel");

    jfieldID fieldID;
    switch(level) {
      case 2: fieldID = env->GetStaticFieldID(jlogLevelClass, "ERROR", "Lnet/jllama/core/LlamaLogLevel;"); break;
      case 3:
        fieldID = env->GetStaticFieldID(jlogLevelClass, "WARN", "Lnet/jllama/core/LlamaLogLevel;");
        break;
      case 4:
        fieldID = env->GetStaticFieldID(jlogLevelClass, "INFO", "Lnet/jllama/core/LlamaLogLevel;");
        break;
      default:
        fieldID = env->GetStaticFieldID(jlogLevelClass, "WARN", "Lnet/jllama/core/LlamaLogLevel;");
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
            "llama_model_default_params"));
    llama_model_params params = getDefaultParams();
    auto paramsManager = LlamaModelParamsManager(params, this);
    jobject jParams = paramsManager.getJavaParams();
    return jParams;
  });
}

typedef void (* llama_sample_softmax_pointer) (struct llama_context * ctx, llama_token_data_array * candidates);
void LlamaManager::LlamaSession::llamaSampleSoftmax(jobject jContext, jobject jCandidates) {

  withJniExceptions(env, [jContext, this, jCandidates] {
    auto sampleSoftmax = reinterpret_cast<llama_sample_softmax_pointer>(getFunctionAddress("llama_sample_softmax"));
    llama_context* context = jni::getLlamaContextPointer(env, jContext);
    llama_token_data_array candidates = jni::getTokenDataArray(env, jCandidates);
    sampleSoftmax(context, &candidates);
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
    auto sampleTopK = reinterpret_cast<llama_sample_top_k_pointer>(getFunctionAddress("llama_sample_top_k"));
    llama_context* context = jni::getLlamaContextPointer(env, jContext);
    llama_token_data_array candidates = jni::getTokenDataArray(env, jCandidates);
    sampleTopK(context, &candidates, k, minkKeep);
    jni::updateTokenDateArray(env, jCandidates, &candidates);
  });
}

typedef void (* llama_sample_p_pointer)(struct llama_context * ctx, llama_token_data_array * candidates, float p, size_t min_keep);
void LlamaManager::LlamaSession::llamaSampleTopP(jobject jContext,
                                                 jobject jCandidates,
                                                 jfloat p,
                                                 jlong minKeep) {
  withJniExceptions(env, [jContext, this, jCandidates, p, minKeep] {
    auto sampleTopP = reinterpret_cast<llama_sample_p_pointer>(getFunctionAddress("llama_sample_top_p"));
    llama_context* context = jni::getLlamaContextPointer(env, jContext);
    llama_token_data_array candidates = jni::getTokenDataArray(env, jCandidates);
    sampleTopP(context, &candidates, p, minKeep);
    jni::updateTokenDateArray(env, jCandidates, &candidates);
  });
}
void LlamaManager::LlamaSession::llamaSampleMinP(jobject jContext,
                                                 jobject jCandidates,
                                                 jfloat jP,
                                                 jlong jMinKeep) {
  withJniExceptions(env, [jContext, this, jCandidates, jP, jMinKeep] {
    auto sampleMinP = reinterpret_cast<llama_sample_p_pointer>(getFunctionAddress("llama_sample_min_p"));
    llama_context* context = jni::getLlamaContextPointer(env, jContext);
    llama_token_data_array candidates = jni::getTokenDataArray(env, jCandidates);
    sampleMinP(context, &candidates, jP, jMinKeep);
    jni::updateTokenDateArray(env, jCandidates, &candidates);
  });
}

JNIEXPORT void JNICALL Java_net_jllama_core_LlamaContext_llamaSampleMinPNative
  (JNIEnv *, jobject, jobject, jfloat, jlong);


typedef void (* llama_sample_tail_free_pointer)(struct llama_context * ctx, llama_token_data_array * candidates, float z, size_t min_keep);
void LlamaManager::LlamaSession::llamaSampleTailFree(jobject jContext,
                                                     jobject jCandidates,
                                                     jfloat z,
                                                     jlong minkeep) {
  withJniExceptions(env, [jContext, this, jCandidates, z, minkeep] {
    auto sampleTailFree = reinterpret_cast<llama_sample_tail_free_pointer>(getFunctionAddress("llama_sample_tail_free"));
    llama_context* context = jni::getLlamaContextPointer(env, jContext);
    llama_token_data_array candidates = jni::getTokenDataArray(env, jCandidates);
    sampleTailFree(context, &candidates, z, minkeep);
    jni::updateTokenDateArray(env, jCandidates, &candidates);
  });
}

typedef void (* llama_sample_typical_pointer)(struct llama_context * ctx, llama_token_data_array * candidates, float p, size_t min_keep);
void LlamaManager::LlamaSession::llamaSampleTypical(jobject jContext,
                                                    jobject jCandidates,
                                                    jfloat p,
                                                    jint minKeep) {
  withJniExceptions(env, [jContext, this, jCandidates, p, minKeep] {
    auto sampleTypical = reinterpret_cast<llama_sample_typical_pointer>(getFunctionAddress("llama_sample_typical"));
    llama_context* context = jni::getLlamaContextPointer(env, jContext);
    llama_token_data_array candidates = jni::getTokenDataArray(env, jCandidates);
    sampleTypical(context, &candidates, p, minKeep);
    jni::updateTokenDateArray(env, jCandidates, &candidates);
  });
}

typedef void (* llama_sample_temp_pointer)(struct llama_context * ctx, llama_token_data_array * candidates, float temp);
void LlamaManager::LlamaSession::llamaSampleTemperature(jobject jContext,
                                                        jobject jCandidates,
                                                        jfloat temp) {
  withJniExceptions(env, [jContext, this, jCandidates, temp] {
    auto sampleTemperature = reinterpret_cast<llama_sample_temp_pointer>(getFunctionAddress("llama_sample_temp"));
    llama_context* context = jni::getLlamaContextPointer(env, jContext);
    llama_token_data_array candidates = jni::getTokenDataArray(env, jCandidates);
    sampleTemperature(context, &candidates, temp);
    jni::updateTokenDateArray(env, jCandidates, &candidates);
  });
}

typedef llama_batch (* llama_batch_init_pointer)(int32_t n_tokens, int32_t embd, int32_t n_seq_max);
jobject LlamaManager::LlamaSession::llamaBatchInit(jobject jContext,
                                                   jint jNTokens,
                                                   jint jEmbd,
                                                   jint nSeqId) {
  return withJniExceptions(env, [this, jContext, jNTokens, jEmbd, nSeqId] {
    auto batchInit = reinterpret_cast<llama_batch_init_pointer>(getFunctionAddress("llama_batch_init"));
    llama_batch batchStack = batchInit(jNTokens, jEmbd, nSeqId);
    llama_batch* batchHeap = new llama_batch;
    std::memcpy(batchHeap, &batchStack, sizeof(llama_batch));
    return jni::constructBatch(env, batchHeap, jNTokens, jEmbd, nSeqId);
  });
}

typedef void (* llama_batch_free_pointer)(llama_batch);
void LlamaManager::LlamaSession::llamaBatchFree(jobject jBatch) {
  return withJniExceptions(env, [this, jBatch] {
    auto batchFree = reinterpret_cast<llama_batch_free_pointer>(getFunctionAddress("llama_batch_free"));
    llama_batch* batch = jni::getLlamaBatchPointer(env, jBatch);
    batchFree(*batch);
    delete batch;
  });
}

typedef int (* llama_decode_pointer)(llama_context*, llama_batch);
jint LlamaManager::LlamaSession::decodeNative(jobject jContext, jobject jBatch) {
  return withJniExceptions(env, [this, jContext, jBatch] {
    auto decode = reinterpret_cast<llama_decode_pointer>(getFunctionAddress("llama_decode"));

    llama_context* context = jni::getLlamaContextPointer(env, jContext);
    llama_batch* batch = jni::getLlamaBatchPointer(env, jBatch);
    jclass jBatchClass = env->GetObjectClass(jBatch);

    int32_t nTokens = jni::getInt32(env, jBatchClass, jBatch, "nTokens");
    batch->n_tokens = nTokens;

    jintArray jTokenArray = jni::getInt32Array(env, jBatchClass,jBatch, "token");
    jint* jTokens = jTokenArray != nullptr ? env->GetIntArrayElements(jTokenArray, nullptr) : nullptr;

    jfloatArray jEmbdArray = jni::getFloatArray(env, jBatchClass,jBatch, "embd");
    jfloat* jEmbd = jEmbdArray != nullptr ? env->GetFloatArrayElements(jEmbdArray, nullptr) : nullptr;

    jintArray jPosArray = jni::getInt32Array(env, jBatchClass,jBatch, "pos");
    jint* jPos = env->GetIntArrayElements(jPosArray, nullptr);

    jintArray jNSeqIdArray = jni::getInt32Array(env, jBatchClass,jBatch, "nSeqId");
    jint* jNSeqId = env->GetIntArrayElements(jNSeqIdArray, nullptr);

    jobjectArray jSeqIdArray = jni::get2dInt32Array(env, jBatchClass,jBatch, "seqId");

    jbyteArray jLogitsArray = jni::getByteArray(env, jBatchClass,jBatch, "logits");
    jbyte* jLogits = env->GetByteArrayElements(jLogitsArray, nullptr);

    for (int i = 0; i < nTokens; i++) {
      if (jTokens) {
        batch->token[i] = jTokens[i];
      }
      if (jEmbd) {
        batch->embd[i] = jEmbd[i];
      }
      batch->pos[i] = jPos[i];
      batch->n_seq_id[i] = jNSeqId[i];
      auto jSeqIdPiecesArray = reinterpret_cast<jintArray>(env->GetObjectArrayElement(jSeqIdArray, i));
      jint* jSeqIdPieces = env->GetIntArrayElements(jSeqIdPiecesArray, nullptr);
      for (int j = 0; j < jNSeqId[i]; j++) {
        batch->seq_id[i][j] = jSeqIdPieces[j];
      }
      env->ReleaseIntArrayElements(jSeqIdPiecesArray, jSeqIdPieces, JNI_ABORT);
      batch->logits[i] = jLogits[i];
    }
    int decodeStatus = decode(context, *batch);
    if (jTokens) {
      env->ReleaseIntArrayElements(jTokenArray, jTokens, JNI_ABORT);
    }
    if (jEmbd) {
        env->ReleaseFloatArrayElements(jEmbdArray, jEmbd, JNI_ABORT);
    }
    env->ReleaseIntArrayElements(jPosArray, jPos, JNI_ABORT);
    env->ReleaseIntArrayElements(jNSeqIdArray, jNSeqId, JNI_ABORT);
    env->ReleaseByteArrayElements(jLogitsArray, jLogits, JNI_ABORT);
    return decodeStatus;
  });
}

typedef int (* llama_get_kv_cache_used_cells_pointer)(llama_context*);
jint LlamaManager::LlamaSession::getKvCacheUsedCells(jobject jContext) {
  return withJniExceptions(env, [this, jContext] {
    auto getUsedCells =
        reinterpret_cast<llama_get_kv_cache_used_cells_pointer>(getFunctionAddress(
            "llama_get_kv_cache_used_cells"));
    return getUsedCells(jni::getLlamaContextPointer(env, jContext));
  });
}

typedef void (* llama_kv_cache_clear_pointer)(llama_context*);
void LlamaManager::LlamaSession::kvCacheClear(jobject jContext) {
  withJniExceptions(env, [this, jContext] {
    auto clearCache =
        reinterpret_cast<llama_kv_cache_clear_pointer >(getFunctionAddress("llama_kv_cache_clear"));
    clearCache(jni::getLlamaContextPointer(env, jContext));
  });
}

typedef void (* llama_kv_cache_seq_rm_pointer)(llama_context*, int, int, int);
void LlamaManager::LlamaSession::kvCacheSeqRm(jobject jContext, jint jSeqId, jint p0, jint p1) {
  withJniExceptions(env, [this, jContext, jSeqId, p0, p1] {
    auto removeSeq =
        reinterpret_cast<llama_kv_cache_seq_rm_pointer>(getFunctionAddress("llama_kv_cache_seq_rm"));
    removeSeq(jni::getLlamaContextPointer(env, jContext), jSeqId, p0, p1);
  });
}

typedef void (* llama_kv_cache_seq_cp_pointer)(llama_context*, int, int, int, int);
void LlamaManager::LlamaSession::kvCacheSeqCp(jobject jContext, jint jSeqId, jint jSeqIdDst, jint p0, jint p1) {
  withJniExceptions(env, [this, jContext, jSeqId, jSeqIdDst, p0, p1] {

    auto copySeq = reinterpret_cast<llama_kv_cache_seq_cp_pointer>(getFunctionAddress("llama_kv_cache_seq_cp"));
    copySeq(jni::getLlamaContextPointer(env, jContext), jSeqId, jSeqIdDst, p0,
            p1);
  });
}

typedef void (* llama_kv_cache_seq_keep_pointer)(llama_context*, int);
void LlamaManager::LlamaSession::kvCacheSeqKeep(jobject jContext, jint seqId) {
    withJniExceptions(env, [this, jContext, seqId] {
        auto keepSeq = reinterpret_cast<llama_kv_cache_seq_keep_pointer>(getFunctionAddress("llama_kv_cache_seq_keep"));
        keepSeq(jni::getLlamaContextPointer(env, jContext), seqId);
    });
}

typedef void (* llama_kv_cache_seq_shift_pointer)(llama_context*, int, int, int, int);
void LlamaManager::LlamaSession::kvCacheSeqShift(jobject jContext, jint seqId, jint p0, jint p1, jint delta) {
    withJniExceptions(env, [this, jContext, seqId, p0, p1, delta] {
        auto shiftSeq = reinterpret_cast<llama_kv_cache_seq_shift_pointer>(getFunctionAddress("llama_kv_cache_seq_shift"));
        shiftSeq(jni::getLlamaContextPointer(env, jContext), seqId, p0, p1, delta);
    });
}

typedef void (* llama_sample_repetition_penalties_pointer)(llama_context*, llama_token_data_array*, llama_token*, size_t, float, float, float);
void LlamaManager::LlamaSession::llamaSampleRepetitionPenalties(jobject jContext, jobject jCandidates, jintArray jLastTokensArray, jlong jPenaltyLastN, jfloat jPenaltyRepeat, jfloat jPenaltyFreq, jfloat jPenaltyPresent) {
  withJniExceptions(env, [jContext, jCandidates, jLastTokensArray, jPenaltyLastN, jPenaltyRepeat, jPenaltyFreq, jPenaltyPresent, this] {
    auto sampleRepetitionPenalties = reinterpret_cast<llama_sample_repetition_penalties_pointer>(getFunctionAddress("llama_sample_repetition_penalties"));
    llama_context* context = jni::getLlamaContextPointer(env, jContext);
    llama_token_data_array candidates = jni::getTokenDataArray(env, jCandidates);
    jint* jLastTokens = env->GetIntArrayElements(jLastTokensArray, nullptr);
    if (!jLastTokens) {
      jni::throwJNIException(env, jni::JNIException("Unable to get lastTokens array elements"));
    }
    sampleRepetitionPenalties(context, &candidates, reinterpret_cast<llama_token*>(jLastTokens), jPenaltyLastN, jPenaltyRepeat, jPenaltyFreq, jPenaltyPresent);
    env->ReleaseIntArrayElements(jLastTokensArray, jLastTokens, JNI_ABORT);
    jni::updateTokenDateArray(env, jCandidates, &candidates);
  });
}
