#include <jni.h>
#include <android/log.h>
#include <cstdlib>
#include "safejni.h"

using std::string;
using std::vector;
using namespace safejni;

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO  , "SafeJNITest",__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "SafeJNITest",__VA_ARGS__)

#define TEST_STATIC_CLASS "com/safejni/test/TestActivity"

namespace {

    void test1() 
    {
        string a = "Hello ";
        string b = "World!";
        string result = safejni::callStatic<string>(TEST_STATIC_CLASS, "concat", a, b);
        LOGI("Test1: %s", result.c_str());

        result = safejni::callStatic<string>(TEST_STATIC_CLASS, "concat", "also works with", "char pointers!");
        LOGI("Test1: %s", result.c_str());
    }

    void test2() 
    {
        vector<string> values = {"Nesta", "Maldini", "Cannavaro", "Baresi"};
        vector<string> result;
        result = safejni::callStatic<vector<string>>(TEST_STATIC_CLASS, "toUpper", values);

        for (string & str: result) {
            LOGI("Test2: %s", str.c_str());
        }
    }

    void test3() 
    {
        vector<uint8_t> bytes = {100,200,155,100,224,5,100,20};
        int32_t sum = safejni::callStatic<int32_t>(TEST_STATIC_CLASS, "sum", bytes);

        LOGI("Test3: SUM %d", sum);

        int valueToAdd = 1;
        bytes = safejni::callStatic<vector<uint8_t>>(TEST_STATIC_CLASS, "add", bytes, valueToAdd);

        for (uint8_t byte: bytes) {
            LOGI("Test3: Byte %d", byte);
        }
    }

    void test4()
    {
        //create instance
        SPJNIObject javaObject = JNIObject::create("com/safejni/test/Ninja", "Snake");

        string name = javaObject->call<string>("getName");
        LOGI("Test4: name %s", name.c_str());
    }
    


}

extern "C"
{   
    jint JNI_OnLoad(JavaVM* vm, void* reserved)
    {
        JNIEnv* env;
        if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
            return -1;
        }
        // Initialize JNIUtils
        safejni::init(vm, env);

        return JNI_VERSION_1_6;
    }  

    void Java_com_safejni_test_TestActivity_runTests(JNIEnv * env, jobject thiz)
    {
        void (*tests[])() = {test1, test2, test3, test4};

        for (int i = 0; i < 4; ++i) {
            LOGI("About to run Test%d", i + 1);
            tests[i]();
        }
    }


}