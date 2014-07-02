/* 
 * SafeJNI is licensed under MIT licensed. See LICENSE.md file for more information.
 * Copyright (c) 2014 MortimerGoro
*/

#pragma once

#include <jni.h>
#include <memory>
#include <string>
#include <vector>
#include <exception>
#include <stdint.h>


namespace safejni {


#pragma mark Compile Time Strings

//Compile Time String
template <char... Cs> struct CompileTimeString {
    static const char * value() {
        static const char a[sizeof...(Cs)] = {Cs...};
        return a;
    }
};
 
//Concatenate 2 Strings
template <class L, class R> struct Concatenate2;
 
template <char... LC, char... RC>
struct Concatenate2<CompileTimeString<LC...>, CompileTimeString<RC...>> {
    using Result = CompileTimeString<LC..., RC...>;
};
 
//Concatenate N strings
template <typename ...C> struct Concatenate;
 
template <typename C1> struct Concatenate<C1>{
    using Result = C1;
};
 
template <typename C1, typename C2> struct Concatenate<C1,C2>{
    using Result = typename Concatenate2<C1,C2>::Result ;
};
 
template <typename C1, typename ...C> struct Concatenate<C1,C...>{
    using Result = typename Concatenate2<C1,typename Concatenate<C...>::Result>::Result;
};

#pragma mark Utility functions
    
    class JNIException: public std::exception
    {
    public:
        std::string message;
        JNIException(const std::string & message);
        virtual const char* what() const throw();
    };

    class JNIMethodInfo
    {
    public:
        jclass classId;
        jmethodID methodId;
        JNIMethodInfo(jclass classId, jmethodID methodId);
        ~JNIMethodInfo();
    };

    typedef std::shared_ptr<JNIMethodInfo> SPJNIMethodInfo;

    class Utils 
    {
    private:
        static JNIEnv * env;
        static JavaVM * javaVM;
    public:
        static void init(JavaVM * vm, JNIEnv * env);
        static inline JNIEnv * getJNIEnv() { return env;}
        static JNIEnv* getJNIEnvAttach();
        static jstring toJString(const char * str);
        inline static jstring toJString(const std::string & str) {
            return toJString(str.c_str());
        }
        static jobjectArray toJObjectArray(const std::vector<std::string> & data);
        static jbyteArray toJObjectArray(const std::vector<uint8_t> & data);

        static std::string toString(jstring str);
        static std::vector<std::string> toVectorString(jobjectArray array);
        static std::vector<uint8_t> toVectorByte(jbyteArray);
        
        static SPJNIMethodInfo getStaticMethodInfo(const std::string& className, const std::string& methodName, const char * signature);
        static SPJNIMethodInfo getMethodInfo(const std::string& className, const std::string& methodName, const char * signature);
        static void checkException();
    };

    void init(JavaVM * javaVM, JNIEnv * env);

#define JNI_EXCEPTION_CHECK safejni::Utils::checkException();
    
#pragma mark C++ To JNI conversion templates
    
    //default template
    template <typename T>
    struct CPPToJNIConversor {
        inline static void convert(T obj);
    };
    
    //generic pointer implementation (using jlong type)
    template<typename T>
    struct CPPToJNIConversor<T*> {
        using JNIType = CompileTimeString<'J'>;
        inline static jlong convert(T* obj) { return reinterpret_cast<jlong>(obj);}
    };
    
    //object implementations
    template<>
    struct CPPToJNIConversor<std::string> {
        using JNIType = CompileTimeString<'L','j','a','v','a','/','l','a','n','g','/','S','t','r','i','n','g',';'>;
        inline static jstring convert(const std::string & obj) { return Utils::toJString(obj);}
    };
    
    template<>
    struct CPPToJNIConversor<const char *> {
        using JNIType = CompileTimeString<'L','j','a','v','a','/','l','a','n','g','/','S','t','r','i','n','g',';'>;
        inline static jstring convert(const char * obj) { return Utils::toJString(obj);}
    };
    
    template<>
    struct CPPToJNIConversor<std::vector<std::string>> {
        using JNIType = CompileTimeString<'[','L','j','a','v','a','/','l','a','n','g','/','S','t','r','i','n','g',';'>;
        inline static jobjectArray convert(const std::vector<std::string> & obj) { return Utils::toJObjectArray(obj);}
    };

    template<>
    struct CPPToJNIConversor<std::vector<uint8_t>> {
        using JNIType = CompileTimeString<'[','B'>;
        inline static jbyteArray convert(const std::vector<uint8_t> & obj) { return Utils::toJObjectArray(obj);}
    };
    
    //Add more types here
    
    //void conversor
    
    template<>
    struct CPPToJNIConversor<void> {
        using JNIType = CompileTimeString<'V'>;
    };
    
    //primivite objects implementation
    
    template<>
    struct CPPToJNIConversor<int32_t> {
        using JNIType = CompileTimeString<'I'>;
        inline static jint convert(int value) { return static_cast<jint>(value);}
    };
    
    template<>
    struct CPPToJNIConversor<int64_t> {
        using JNIType = CompileTimeString<'J'>;
        inline static jlong convert(int64_t value) { return static_cast<jlong>(value);}
    };
    
    template<>
    struct CPPToJNIConversor<float> {
        using JNIType = CompileTimeString<'F'>;
        inline static jfloat convert(float value) { return static_cast<jfloat>(value);}
    };
    
    template<>
    struct CPPToJNIConversor<double> {
        using JNIType = CompileTimeString<'D'>;
        inline static jdouble convert(double value) { return static_cast<jdouble>(value);}
    };
    
    template<>
    struct CPPToJNIConversor<bool> {
        using JNIType = CompileTimeString<'Z'>;
        inline static jboolean convert(bool value) { return static_cast<jboolean>(value);}
    };
    
    template<>
    struct CPPToJNIConversor<int8_t> {
        using JNIType = CompileTimeString<'B'>;
        inline static jbyte convert(int8_t value) { return static_cast<jbyte>(value);}
    };
    
    template<>
    struct CPPToJNIConversor<uint8_t> {
        using JNIType = CompileTimeString<'C'>;
        inline static jchar convert(uint8_t value) { return static_cast<uint8_t>(value);}
    };
    
    template<>
    struct CPPToJNIConversor<int16_t> {
        using JNIType = CompileTimeString<'S'>;
        inline static jshort convert(int16_t value) { return static_cast<jshort>(value);}
    };
    
    
    
#pragma mark JNI To C++ conversion templates
    
    template <typename T>
    struct JNIToCPPConversor {
        inline static void convert(T);
        inline static const char * jniTypeName();
    };
    
    template<>
    struct JNIToCPPConversor<std::string> {
        inline static std::string convert(jobject obj) { return Utils::toString((jstring)obj); }
    };
    
    template<>
    struct JNIToCPPConversor<std::vector<std::string>> {
        inline static std::vector<std::string> convert(jobject obj) { return Utils::toVectorString((jobjectArray)obj); }
    };
    
    template<>
    struct JNIToCPPConversor<std::vector<uint8_t>> {
        inline static std::vector<uint8_t> convert(jobject obj) { return Utils::toVectorByte((jbyteArray)obj);}
    };
    
    
#pragma mark JNI Call Template Specializations
    
    //default implementation (for jobject types)
    template <typename T, typename... Args>
    struct JNICaller {
        static T callStatic(JNIEnv *env, jclass cls, jmethodID method, Args... v) {
            auto obj = env->CallStaticObjectMethod(cls,method,v...);
            T result = JNIToCPPConversor<T>::convert(obj);
            if (obj)
                env->DeleteLocalRef(obj);
            return result;
        }
        static T callInstance(JNIEnv *env, jobject instance,jmethodID method, Args... v){
            auto obj = env->CallObjectMethod(instance,method,v...);
            T result = JNIToCPPConversor<T>::convert(obj);
            if (obj)
                env->DeleteLocalRef(obj);
            return result;
        }
    };
    
    //generic pointer implementation (using jlong types)
    template <typename T, typename... Args>
    struct JNICaller<T*,Args...> {
        static T* callStatic(JNIEnv *env, jclass cls, jmethodID method, Args... v) {
            return (T*)env->CallLongMethod(cls, method, v...);
        }
        static T* callInstance(JNIEnv *env, jobject instance, jmethodID method, Args... v) {
            return (T*)env->CallLongMethod(instance, method, v...);
        }
    };
    
    //void implementation
    template <typename... Args>
    struct JNICaller<void,Args...> {
        static void callStatic(JNIEnv *env, jclass cls, jmethodID method, Args... v) {
            env->CallStaticVoidMethod(cls, method, v...);
        }
        static void callInstance(JNIEnv *env, jobject instance, jmethodID method, Args... v) {
            env->CallVoidMethod(instance, method, v...);
        }
    };
    
    //primitive types implementations
    template <typename... Args>
    struct JNICaller<bool,Args...> {
        static bool callStatic(JNIEnv *env, jclass cls, jmethodID method, Args... v) {
            return env->CallStaticBooleanMethod(cls, method, v...);
        }
        static bool callInstance(JNIEnv *env, jobject instance, jmethodID method, Args... v) {
            return env->CallBooleanMethod(instance, method, v...);
        }
    };
    
    template <typename... Args>
    struct JNICaller<int8_t,Args...> {
        static int8_t callStatic(JNIEnv *env, jclass cls, jmethodID method, Args... v) {
            return env->CallStaticByteMethod(cls, method, v...);
        }
        static int8_t callInstance(JNIEnv *env, jobject instance, jmethodID method, Args... v) {
            return env->CallByteMethod(instance, method, v...);
        }
    };
    
    template <typename... Args>
    struct JNICaller<uint8_t,Args...> {
        static uint8_t callStatic(JNIEnv *env, jclass cls, jmethodID method, Args... v) {
            return env->CallStaticCharMethod(cls, method, v...);
        }
        static uint8_t callInstance(JNIEnv *env, jobject instance, jmethodID method, Args... v) {
            return env->CallCharMethod(instance, method, v...);
        }
    };
    
    template <typename... Args>
    struct JNICaller<int16_t,Args...> {
        static int16_t callStatic(JNIEnv *env, jclass cls, jmethodID method, Args... v) {
            return env->CallStaticShortMethod(cls, method, v...);
        }
        static int16_t callInstance(JNIEnv *env, jobject instance, jmethodID method, Args... v) {
            return env->CallShortMethod(instance, method, v...);
        }
    };
    
    template <typename... Args>
    struct JNICaller<int32_t,Args...> {
        static int32_t callStatic(JNIEnv *env, jclass cls, jmethodID method, Args... v) {
            return env->CallStaticIntMethod(cls, method, v...);
        }
        static int32_t callInstance(JNIEnv *env, jobject instance, jmethodID method, Args... v) {
            return env->CallIntMethod(instance, method, v...);
        }
    };
    
    template <typename... Args>
    struct JNICaller<int64_t,Args...> {
        static int64_t callStatic(JNIEnv *env, jclass cls, jmethodID method, Args... v) {
            return env->CallStaticLongMethod(cls, method, v...);
        }
        static int64_t callInstance(JNIEnv *env, jobject instance, jmethodID method, Args... v) {
            return env->CallLongMethod(instance, method, v...);
        }
    };
    
    template <typename... Args>
    struct JNICaller<float,Args...> {
        static float callStatic(JNIEnv *env, jclass cls, jmethodID method, Args... v) {
            return env->CallStaticFloatMethod(cls, method, v...);
        }
        static float callInstance(JNIEnv *env, jobject instance, jmethodID method, Args... v) {
            return env->CallFloatMethod(instance, method, v...);
        }
    };
    
    template <typename... Args>
    struct JNICaller<double,Args...> {
        static double callStatic(JNIEnv *env, jclass cls, jmethodID method, Args... v) {
            return env->CallStaticDoubleMethod(cls, method, v...);
        }
        static double callInstance(JNIEnv *env, jobject instance, jmethodID method, Args... v) {
            return env->CallDoubleMethod(instance, method, v...);
        }
    };
    
#pragma mark JNI Signature Utilities
    
    //helper method to append a JNI parameter signature to a buffer
    template <typename T> char appendJNIParamSignature(std::string & buffer) {
        buffer+= CPPToJNIConversor<T>::jniTypeName();
    }
    
    //deduces the signature of a JNI method according to the variadic params and the return type
    template <typename T, typename... Args>
    inline const char * getJNISignature(Args...) {
        return Concatenate<CompileTimeString<'('>, //left parenthesis
                            typename CPPToJNIConversor<Args>::JNIType..., //params signature
                            CompileTimeString<')'>, //right parenthesis
                            typename CPPToJNIConversor<T>::JNIType,
                            CompileTimeString<'\0'>> //return type signature
                            ::Result::value();
    }
    
#pragma mark JNI Param Destructor Templates
    
    //Helper object to destroy parameters converter to JNI
    template <uint8_t NUM_PARAMS>
    struct JNIParamDestructor {
        JNIEnv* jniEnv;
        jobject jniParams[NUM_PARAMS] = {0};
        int currentIndex;
        
        JNIParamDestructor(JNIEnv * env): jniEnv(env), currentIndex(0) {
            
        }
        
        void add(jobject jniObject) {
            jniParams[currentIndex++] = jniObject;
        }
        
        ~JNIParamDestructor() {
            for (int i = 0; i< NUM_PARAMS; ++i) {
                if (jniParams[i])
                    jniEnv->DeleteLocalRef(jniParams[i]);
            }
            JNI_EXCEPTION_CHECK
        }
    };
    
    //optimized base case for the destructor
    template<>
    struct JNIParamDestructor<0> {
        JNIParamDestructor(JNIEnv * env) {}
        ~JNIParamDestructor() {
            JNI_EXCEPTION_CHECK
        }
    };
    
    
    //helper template to decide when a jni type must be destroyed (by adding the ref to the JNIParamDestructor struct);
    template<typename T, typename D>
    struct JNIDestructorDecider {
        //by default the template ignores destruction (for primivitve types)
        inline static void decide(T jniParam, D & destructor) {}
    };
    
    template<typename D>
    struct JNIDestructorDecider<jobject,D> {
        inline static void decide(jobject obj, D & destructor) {destructor.add(obj);}
    };
    
    template<typename D>
    struct JNIDestructorDecider<jbyteArray,D> {
        inline static void decide(jbyteArray obj, D & destructor) {destructor.add((jobject)obj);}
    };
    
    template<typename D>
    struct JNIDestructorDecider<jobjectArray,D> {
        inline static void decide(jobjectArray obj, D & destructor) {destructor.add((jobject)obj);}
    };
    
    template<typename D>
    struct JNIDestructorDecider<jstring,D> {
        inline static void decide(jstring obj, D & destructor) {destructor.add((jobject)obj);}
    };
    
#pragma mark JNI Param Conversor Utility Template
    
    //JNI param conversor helper: Converts the parameter to JNI and adds it to the destructor if needed
    template <typename T, typename D>
    auto JNIParamConversor(const T & arg, D & destructor) -> decltype(CPPToJNIConversor<T>::convert(arg))
    {
        auto result = CPPToJNIConversor<T>::convert(arg);
        JNIDestructorDecider<decltype(CPPToJNIConversor<T>::convert(arg)),D>::decide(result, destructor);
        return result;
    }
    
    
#pragma mark Public API
    
    //generic call to static method
    template<typename T = void, typename... Args> T callStatic(const std::string & className, const std::string & methodName, Args... v)
    {
        static constexpr uint8_t nargs = sizeof...(Args);
        JNIEnv* jniEnv = Utils::getJNIEnvAttach();
        SPJNIMethodInfo methodInfo = Utils::getStaticMethodInfo(className, methodName, getJNISignature<T,Args...>(v...));
        JNIParamDestructor<nargs> paramDestructor(jniEnv);
        return JNICaller<T,decltype(CPPToJNIConversor<Args>::convert(v))...>::callStatic(jniEnv, methodInfo->classId, methodInfo->methodId, JNIParamConversor<Args>(v, paramDestructor)...);
    }
    
    //generic call to instance method
    template<typename T = void, typename... Args> T call(jobject instance, const std::string & className, const std::string & methodName, Args... v)
    {
        static constexpr uint8_t nargs = sizeof...(Args);
        JNIEnv* jniEnv = Utils::getJNIEnvAttach();
        SPJNIMethodInfo methodInfo = Utils::getMethodInfo(className, methodName, getJNISignature<T,Args...>(v...));
        JNIParamDestructor<nargs> paramDestructor(jniEnv);
        return JNICaller<T,decltype(CPPToJNIConversor<Args>::convert(v))...>::callInstance(jniEnv, instance, methodInfo->methodId, JNIParamConversor<Args>(v, paramDestructor)...);
    }
    
    //jobject global ref wrapper class
    struct JNIObject {
        std::string jniClassName;
        jobject instance = nullptr;
        
        ~JNIObject() {
            if (instance) {
                JNIEnv* jniEnv = Utils::getJNIEnv();
                jniEnv->DeleteGlobalRef(instance);
            }
        }
        
        template<typename... Args> static std::shared_ptr<JNIObject> create(const std::string & className, Args ...v)
        {
            static constexpr uint8_t nargs = sizeof...(Args);
            JNIObject * result = new JNIObject();
            JNIEnv* jniEnv = Utils::getJNIEnvAttach();
            SPJNIMethodInfo methodInfo = Utils::getMethodInfo(className, "<init>", getJNISignature<void,Args...>(v...));
            JNIParamDestructor<nargs> paramDestructor(jniEnv);
            result->instance = jniEnv->NewObject(methodInfo->classId, methodInfo->methodId, JNIParamConversor<Args>(v, paramDestructor)...);
            result->instance = jniEnv->NewGlobalRef(result->instance);
            result->jniClassName = className;
            return std::shared_ptr<JNIObject>(result);
        }
        
        template<typename T = void,typename... Args> inline T call(const std::string & methodName, Args... v)
        {
            return safejni::call<T, Args...>(instance, jniClassName, methodName, v...);
        }
        
    };
    
    typedef std::shared_ptr<JNIObject> SPJNIObject;
}
