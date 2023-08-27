#ifndef JNI_IMPLEMENTATION_LLAMASESSION_H
#define JNI_IMPLEMENTATION_LLAMASESSION_H

#include <jni.h>
#include "../exceptions/DynamicLibraryException.h"
#include "../jni.h"

class LlamaSession {
  public:
  explicit LlamaSession(JNIEnv* env) : env(env) {}

  void backendInit(bool useNuma);
  void backendFree();

  jobject loadModelFromFile(jbyteArray path, jobject javaParams);
  jint tokenizeWithModel(jobject jModel, jbyteArray jToTokenize, jintArray jTokensOut, jint jmaxTokens, jboolean jBos);

  jobject loadContextWithModel(jobject jModel, jobject jContextParams);
  jint eval(jobject jContext, jintArray jTokens, jint jnTokens, jint jnPast, jint jnThreads);

  jfloatArray getLogits(jobject jContext);

  jint sampleTokenGreedy(jobject jContext, jobject jCandidates);

  jbyteArray tokenToStr(jobject jContext, jint jToken);

  jint tokenBos();
  jint tokenEos();
  jint tokenNl();

  private:
  JNIEnv* env;
};

#endif //JNI_IMPLEMENTATION_LLAMASESSION_H