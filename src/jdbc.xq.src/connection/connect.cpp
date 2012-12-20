/*
 * Copyright 2006-2012 The FLWOR Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "connect.h"
#include "jdbc.h"

namespace zorba
{
namespace jdbc
{


ItemSequence_t
ConnectFunction::evaluate(const ExternalFunction::Arguments_t& args,
                           const zorba::StaticContext* aStaticContext,
                           const zorba::DynamicContext* aDynamincContext) const
{
  JNIEnv *env = JdbcModule::getJavaEnv(aStaticContext);
  Item result;
  JDBC_MODULE_TRY
    jstring url(NULL), username(NULL), password(NULL);
    Item item = JdbcModule::getItemArg(args, 0);
    bool hasUsername=false;
    if (item.isJSONItem()) 
    {
      Iterator_t lKeys = item.getObjectKeys();
      lKeys->open();
      Item lKey;
      while (lKeys->next(lKey))
      {
        zorba::String keystring = lKey.getStringValue();
        zorba::String value = item.getObjectValue(keystring).getStringValue();
        if (keystring=="url") {
          url =  env->NewStringUTF(value.c_str());
          CHECK_EXCEPTION(env);
        } else if (keystring=="user") {
          username =  env->NewStringUTF(value.c_str());
          CHECK_EXCEPTION(env);
          hasUsername = true;
        } else if (keystring=="password") {
          password =  env->NewStringUTF(value.c_str());
          CHECK_EXCEPTION(env);
        } else if (keystring.compare("type")) {
        } else if (keystring.compare("driver")) {
        }
      }
      lKeys->close();
    }

    if (cDriverManager==NULL) {
      cDriverManager = JdbcModule::getJavaClass(JC_DRIVER_MANAGER, env);
    }

    jobject oConnection;
    if (hasUsername) {
      oConnection = env->CallStaticObjectMethod(cDriverManager, getConnectionWithCredentialsMethod(env), url, username, password);
      CHECK_EXCEPTION(env);
    } else {
      oConnection = env->CallStaticObjectMethod(cDriverManager, getConnectionMethod(env), url);
      CHECK_EXCEPTION(env);
    }

    if (cConnection==NULL) {
      cConnection = JdbcModule::getJavaClass(JC_CONNECTION, env);
    }
    item = JdbcModule::getItemArg(args, 1);
    if ((!item.isNull()) && (item.isJSONItem()))
    {
      Iterator_t lKeys = item.getObjectKeys();
      lKeys->open();
      Item lKey;
      while (lKeys->next(lKey))
      {
        zorba::String keystring = lKey.getStringValue();
        if (keystring=="autocommit") {
          jboolean value = JNI_FALSE;
          if (item.getObjectValue(keystring).getBooleanValue()) {
            value = JNI_TRUE;
          }
          env->CallVoidMethod(oConnection, getSetAutoCommitMethod(env), value);
          CHECK_EXCEPTION(env);
        } else if (keystring=="readonly") {
          jboolean value = JNI_FALSE;
          if (item.getObjectValue(keystring).getBooleanValue()) {
            value = JNI_TRUE;
          }
          env->CallVoidMethod(oConnection, getSetReadOnlyMethod(env), value);
          CHECK_EXCEPTION(env);
        } else if (keystring=="isolation-level") {
          jint isolationLevel = (int) item.getObjectValue(keystring).getLongValue();
          env->CallVoidMethod(oConnection, getSetTransactionIsolationMethod(env), isolationLevel);
          CHECK_EXCEPTION(env);
        }
      }
      lKeys->close();
    }

    InstanceMap* lInstanceMap = JdbcModule::getCreateInstanceMap(aDynamincContext, INSTANCE_MAP_CONNECTIONS);
    String lStrUUID = JdbcModule::getUUID();
    lInstanceMap->storeInstance(lStrUUID, oConnection);

    result = theFactory->createAnyURI(lStrUUID);
  JDBC_MODULE_CATCH
  return ItemSequence_t(new SingletonItemSequence(result));
}

jclass ConnectFunction::cDriverManager = NULL;
jmethodID ConnectFunction::getConnectionMethod(JNIEnv *env) {
  static jmethodID mConnection = NULL;
  if (mConnection == NULL) {
    mConnection = env->GetStaticMethodID(cDriverManager, "getConnection", "(Ljava/lang/String;)Ljava/sql/Connection;");
  }
  return mConnection;
}
jmethodID ConnectFunction::getConnectionWithCredentialsMethod(JNIEnv *env) {
  static jmethodID mConnection = NULL;
  if (mConnection == NULL) {
    mConnection = env->GetStaticMethodID(cDriverManager, "getConnection", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/sql/Connection;");
  }
  return mConnection;
}


jclass ConnectFunction::cConnection = NULL;
jmethodID ConnectFunction::getSetAutoCommitMethod(JNIEnv *env) {
  static jmethodID mSetAutoCommit = NULL;
  if (mSetAutoCommit == NULL) {
    mSetAutoCommit = env->GetMethodID(cConnection, "setAutoCommit", "(Z)V");
  }
  return mSetAutoCommit;
}
jmethodID ConnectFunction::getSetReadOnlyMethod(JNIEnv *env) {
  static jmethodID mSetReadOnly = NULL;
  if (mSetReadOnly == NULL) {
    mSetReadOnly = env->GetMethodID(cConnection, "setReadOnly", "(Z)V");
  }
  return mSetReadOnly;
}
jmethodID ConnectFunction::getSetTransactionIsolationMethod(JNIEnv *env) {
  static jmethodID mSetTransactionIsolation = NULL;
  if (mSetTransactionIsolation == NULL) {
    mSetTransactionIsolation = env->GetMethodID(cConnection, "setTransactionIsolation", "(I)V");
  }
  return mSetTransactionIsolation;
}



}}; // namespace zorba, jdbc
