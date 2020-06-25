#include <jni.h>
#include <string>

//jint JNI_OnLoad(JavaVM *vm, void *reserved) {
//    JNIEnv *env = NULL;
//    vm->AttachCurrentThread(&env, NULL);
//    vm->DetachCurrentThread();
//    return JNI_VERSION_1_6;
//}


extern "C"
JNIEXPORT jstring JNICALL
Java_com_linda_android_core_MainActivity_getAndroidId(JNIEnv *env, jobject instance) {

    jclass activity = env->GetObjectClass(instance);
    jclass context_class = env->FindClass( "android/content/Context");

    jmethodID  systemservice_method=env->GetMethodID(activity,"getSystemService","(Ljava/lang/String;)Ljava/lang/Object;");
    jfieldID  teleservice_field=env->GetStaticFieldID(activity,"TELEPHONY_SERVICE","Ljava/lang/String;");
    jobject  str=env->GetStaticObjectField(context_class,teleservice_field);
    jobject  telephony=env->CallObjectMethod(instance,systemservice_method,str);

    jclass  telemanager_class=env->FindClass("android/telephony/TelephonyManager");
    jmethodID deviceid_method=env->GetMethodID(telemanager_class,"getDeviceId","()Ljava/lang/String;");
   // jobject  jstring1=env->CallObjectMethod(telephony,systemService);

    // get contentResolver
    jmethodID method = env->GetMethodID(activity, "getContentResolver",
                                        "()Landroid/content/ContentResolver;");
    jobject resolverInstance = env->CallObjectMethod(instance, method);

    // get android_id from android Settings$Secure
    jclass setting_class = env->FindClass("android/provider/Settings$Secure");
    jmethodID contentresolver_methodId = env->GetStaticMethodID(setting_class, "getString",
                                                "(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;");
    jstring param_android_id = env->NewStringUTF("android_id");
    jstring android_id = (jstring) env->CallStaticObjectMethod(setting_class, contentresolver_methodId,
                                                               resolverInstance,
                                                               param_android_id);

    // get serial num from android Build
    jclass build_class = env->FindClass("android/os/Build");
    jfieldID serial_field = env->GetStaticFieldID(build_class, "SERIAL", "Ljava/lang/String;");
    jstring serial_string = (jstring) env->GetStaticObjectField(build_class, serial_field);


    // concat strings
    jclass string_class = env->FindClass("java/lang/String");
    jmethodID concat_method = env->GetMethodID(string_class, "concat",
                                                 "(Ljava/lang/String;)Ljava/lang/String;");
    jstring result = (jstring) env->CallObjectMethod(android_id, concat_method, serial_string);

    // release
    env->DeleteLocalRef(param_android_id);
    env->DeleteLocalRef(serial_string);

    return result;
}

