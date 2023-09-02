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
    (const char*, struct llama_context_params);
jobject LlamaManager::LlamaSession::loadModelFromFile(jbyteArray path, jobject javaParams) {
  return withJniExceptions(env, [&javaParams, &path, this] {
    llama_load_model_from_file_pointer llamaLoadModel =
        (llama_load_model_from_file_pointer) getFunctionAddress(
            "llama_load_model_from_file");

    auto stringManager = Utf8StringManager(env, path);
    auto paramsManager = LlamaContextParamsManager(javaParams, this);
    auto params = paramsManager.getParams();

    llama_model* model =
        llamaLoadModel(stringManager.getUtf8String(), params);

    if (params.progress_callback_user_data) {
      auto progressContext = reinterpret_cast<CallbackContext*>(params.progress_callback_user_data);
      env->DeleteGlobalRef(progressContext->callback);
      delete progressContext;
    }

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
jobject LlamaManager::LlamaSession::loadContextWithModel(jobject jModel, jobject jContextParams) {
  return withJniExceptions(env, [&jModel, &jContextParams, this] {
    llama_new_context_with_model_pointer llamaCreateContext =
        (llama_new_context_with_model_pointer) getFunctionAddress(
            "llama_new_context_with_model");

    auto paramsManager = LlamaContextParamsManager(jContextParams, this);
    auto params = paramsManager.getParams();
    auto llamaModel = jni::getLlamaModelPointer(env, jModel);
    llama_context
        * context = llamaCreateContext(llamaModel, paramsManager.getParams());

    if (params.progress_callback_user_data) {
      auto progressContext = reinterpret_cast<CallbackContext*>(params.progress_callback_user_data);
      env->DeleteGlobalRef(progressContext->callback);
      delete progressContext;
    }

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
jint LlamaManager::LlamaSession::tokenizeWithModel(jobject jModel,
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
jint LlamaManager::LlamaSession::eval(jobject jContext,
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
jfloatArray LlamaManager::LlamaSession::getLogits(jobject jContext) {
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
jint LlamaManager::LlamaSession::sampleTokenGreedy(jobject jContext, jobject jCandidates) {
  return withJniExceptions(env, [this, &jContext, &jCandidates] {
    auto sampleTokenGreedily = (llama_sample_token_greedy_pointer) getFunctionAddress("llama_sample_token_greedy");
    auto llamaContext = jni::getLlamaContextPointer(env, jContext);
    llama_token_data_array candidates = jni::getTokenDataArray(env, jCandidates);
    jint sampled = sampleTokenGreedily(llamaContext, &candidates);
    delete[] candidates.data;
    return sampled;
  });
}

typedef int (* llama_token_to_piece_pointer)(llama_context*, llama_token, char*, int);
jint LlamaManager::LlamaSession::tokenToPiece(jobject jContext, jint jToken, jbyteArray joutput) {
  return withJniExceptions(env, [&jContext, jToken, this, joutput]{
    auto detokenize = (llama_token_to_piece_pointer) getFunctionAddress(
        "llama_token_to_piece");

    auto joutputPointer = env->GetByteArrayElements(joutput, nullptr);
    char* outputPointer = reinterpret_cast<char*>(joutputPointer);
    jsize length = env->GetArrayLength(joutput);
    auto llamaContext = jni::getLlamaContextPointer(env, jContext);

    int result = detokenize(llamaContext, jToken, outputPointer, length);
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

void LlamaManager::loggerCallback(enum llama_log_level level, const char* text, void* user_data) {
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

    jclass jlogLevelClass = env->FindClass("com/mangomelancholy/llama/cpp/java/bindings/LlamaLogLevel");

    jfieldID fieldID;
    switch(level) {
      case 2: fieldID = env->GetStaticFieldID(jlogLevelClass, "ERROR", "Lcom/mangomelancholy/llama/cpp/java/bindings/LlamaLogLevel;"); break;
      case 3:
        fieldID = env->GetStaticFieldID(jlogLevelClass, "WARN", "Lcom/mangomelancholy/llama/cpp/java/bindings/LlamaLogLevel;");
        break;
      case 4:
        fieldID = env->GetStaticFieldID(jlogLevelClass, "INFO", "Lcom/mangomelancholy/llama/cpp/java/bindings/LlamaLogLevel;");
        break;
      default:
        fieldID = env->GetStaticFieldID(jlogLevelClass, "WARN", "Lcom/mangomelancholy/llama/cpp/java/bindings/LlamaLogLevel;");
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

typedef void (* llama_log_set_pointer)(llama_log_callback, void*);
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

typedef uint64_t (* llama_time_us_pointer)(void);
jlong LlamaManager::LlamaSession::getTimestampInMicroseconds() {
  return withJniExceptions(env, [] {
    auto getTimestamp =
        reinterpret_cast<llama_time_us_pointer>(getFunctionAddress("llama_time_us"));
    return getTimestamp();
  });
}
