/* 
 * SafeJNI is licensed under MIT licensed. See LICENSE.md file for more information.
 * Copyright (c) 2014 MortimerGoro
*/

#include <jni.h>
#include <android/log.h>
#include <cstdlib>
#include "safejni.h"

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "SafeJNI",__VA_ARGS__)

using std::string;
using std::vector;

namespace safejni {
    
	JNIEnv* Utils::env = 0;
    JavaVM* Utils::javaVM = 0;


    void init(JavaVM * vm, JNIEnv * env)
    {
        Utils::init(vm, env);
    }
    
    JNIMethodInfo::JNIMethodInfo(jclass classId, jmethodID methodId): classId(classId), methodId(methodId)
    {
        
    }
    
    JNIMethodInfo::~JNIMethodInfo()
    {
     	if (classId) {
     		Utils::getJNIEnv()->DeleteLocalRef(classId);
     	}
    }
    

    JNIException::JNIException(const std::string & message): message(message)
    {
        LOGE("JNI Exception: %s", message.c_str());
    }

    const char* JNIException::what() const throw()
    {
        return message.c_str();
    }

    void Utils::init(JavaVM * vm, JNIEnv * jniEnv)
    {
        Utils::javaVM = vm;
        Utils::env = jniEnv;
    }

    JNIEnv * Utils::getJNIEnvAttach() 
    {
        if (javaVM) {
            int status = javaVM->AttachCurrentThread(&env, NULL);
            if (status < 0) {
                throw JNIException("Could not attach the JNI environment to the current thread.");
            }
        }

        return env;
    }

	jstring Utils::toJString(const char * str)
	{
	 	return env->NewStringUTF(str);
	}
    
    jobjectArray Utils::toJObjectArray(const std::vector<std::string> & data)
    {
        jclass classId = env->FindClass("java/lang/String");
        jint size = data.size();
        jobjectArray joa = env->NewObjectArray(size, classId, 0);
        
        for (int i = 0; i < size; i++)
        {
            jstring jstr = toJString(data[i]);
            env->SetObjectArrayElement(joa, i, jstr);
            
        }
        env->DeleteLocalRef(classId);
        
        JNI_EXCEPTION_CHECK
        return joa;
    }
    
    jbyteArray Utils::toJObjectArray(const std::vector<uint8_t> & data)
    {
        jbyteArray jba = env->NewByteArray(data.size());
        env->SetByteArrayRegion(jba, 0, data.size(), (const jbyte*)&data[0]);
        JNI_EXCEPTION_CHECK
        return jba;
    }

    jobject Utils::toHashMap(const std::map<std::string, std::string> & data)
    {
        jclass classId = env->FindClass("java/util/HashMap");
        jmethodID methodId = env->GetMethodID(classId, "<init>", "()V");
        jobject hashmap = env->NewObject(classId, methodId);
        
        methodId = env->GetMethodID(classId, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
        for (auto & item : data)
        {
            jstring key = env->NewStringUTF(item.first.c_str());
            jstring value = env->NewStringUTF(item.second.c_str());
            env->CallObjectMethod(hashmap, methodId, key, value);
            
            env->DeleteLocalRef(key);
            env->DeleteLocalRef(value);
        }
        env->DeleteLocalRef(classId);
        JNI_EXCEPTION_CHECK
        return hashmap;
    }
    
    std::string Utils::toString(jstring str)
    {
        if (!str) {
            return std::string();
        }
        jboolean isCopy;
        const char* chars = env->GetStringUTFChars(str, &isCopy);
        std::string s;
        if (chars) {
            s = chars;
            env->ReleaseStringUTFChars(str, chars);
        }
        JNI_EXCEPTION_CHECK
        return s;
    }
    
    std::vector<std::string> Utils::toVectorString(jobjectArray array)
    {
        std::vector<std::string> result;
        if (array) {
            jint length = env->GetArrayLength(array);
            
            for (int i = 0; i < length; i++) {
                jobject valueJObject = env->GetObjectArrayElement(array, i);
                    result.push_back(toString((jstring)valueJObject));
                    env->DeleteLocalRef(valueJObject);
                }
        }
        JNI_EXCEPTION_CHECK
        return result;
    }
    
    std::vector<uint8_t> Utils::toVectorByte(jbyteArray array)
    {
        if (!array) {
            return std::vector<uint8_t>();
        }
        jsize size = env->GetArrayLength(array);
        std::vector<uint8_t> result(size);
        env->GetByteArrayRegion(array, 0, size, (jbyte*)&result[0]);
        JNI_EXCEPTION_CHECK
        return result;
    }

    std::vector<jobject> Utils::toVectorJObject(jobjectArray array)
    {
        std::vector<jobject> result;
        if (array) {
            jint length = env->GetArrayLength(array);
            
            for (int i = 0; i < length; i++) {
                jobject valueJObject = env->GetObjectArrayElement(array, i);
                result.push_back(valueJObject);
            }
        }
        return result;
    }
    
	SPJNIMethodInfo Utils::getStaticMethodInfo(const string& className, const string& methodName, const char * signature)
    {
        jclass classId = 0;
        jmethodID methodId = 0;
        classId = env->FindClass(className.c_str());
        JNI_EXCEPTION_CHECK

        if (!classId){
            throw JNIException(string("Could not find the given class: ") + className);
        }
        
        methodId = env->GetStaticMethodID(classId, methodName.c_str(), signature);
        JNI_EXCEPTION_CHECK
        
        if (!methodId){
            throw JNIException(string("Could not find the given '") + methodName + string("' static method in the given '") + className + string("' class using the '") + signature + string("' signature."));
        }
        
        return SPJNIMethodInfo(new JNIMethodInfo(classId, methodId));
    }
    
    SPJNIMethodInfo Utils::getMethodInfo(const string& className, const string& methodName, const char * signature)
    {
        jclass classId = 0;
        jmethodID methodId = 0;
        classId = env->FindClass(className.c_str());

        JNI_EXCEPTION_CHECK
        if (!classId){
            throw JNIException(string("Could not find the given class: ") + className);
        }
        
        methodId = env->GetMethodID(classId, methodName.c_str(), signature);
        JNI_EXCEPTION_CHECK
        
        if (!methodId){
            throw JNIException(string("Could not find the given '") + methodName + string("' static method in the given '") + className + string("' class using the '") + signature + string("' signature."));
        }
        
        return SPJNIMethodInfo(new JNIMethodInfo(classId, methodId));
    }   

    void Utils::checkException()
    {
        if (env->ExceptionCheck())
        {
            jthrowable jthrowable = env->ExceptionOccurred();
            env->ExceptionDescribe();
            env->ExceptionClear();
            SPJNIMethodInfo methodInfo = getMethodInfo("java/lang/Throwable", "getMessage", "()Ljava/lang/String;");
            string exceptionMessage= toString(reinterpret_cast<jstring>(env->CallObjectMethod(jthrowable, methodInfo->methodId)));
            throw new JNIException(exceptionMessage);
        }
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
}