#ifndef JNI_IMPLEMENTATION_UTF8STRINGMANAGER_H
#define JNI_IMPLEMENTATION_UTF8STRINGMANAGER_H

class Utf8StringManager {
  public:
  Utf8StringManager(JNIEnv* env, jbyteArray jutf8String);
  ~Utf8StringManager();
  char * getUtf8String();

  private:
  JNIEnv* env;
  char* utf8String;

};


#endif //JNI_IMPLEMENTATION_UTF8STRINGMANAGER_H
