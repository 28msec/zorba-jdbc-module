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

#include "connectionoptions.h"
#include "jdbc.h"

namespace zorba
{
namespace jdbc
{


ItemSequence_t
ConnectionOptionsFunction::evaluate(const ExternalFunction::Arguments_t& args,
                           const zorba::StaticContext* aStaticContext,
                           const zorba::DynamicContext* aDynamincContext) const
{
  JNIEnv *env = JdbcModule::getJavaEnv(aStaticContext);
  static bool isolationLevelsLoaded = false;
  static int TRANSACTION_NONE;
  static int TRANSACTION_READ_UNCOMMITTED;
  static int TRANSACTION_READ_COMMITTED;
  static int TRANSACTION_REPEATABLE_READ;
  static int TRANSACTION_SERIALIZABLE;
  Item result;

  JDBC_MODULE_TRY
    String lStrUUID = JdbcModule::getStringArg(args, 0);

    jobject oConnection = JdbcModule::getObject(aDynamincContext, lStrUUID, INSTANCE_MAP_CONNECTIONS);

    std::vector<std::pair<Item, Item> > resultVector;

    if (cConnection == NULL) {
      cConnection = JdbcModule::getJavaClass(JC_CONNECTION, env);
    }

    jboolean isAutocommit = env->CallBooleanMethod(oConnection, getAutoCommit(env));
    resultVector.push_back(std::pair<Item, Item>(theFactory->createString("autocommit"), theFactory->createBoolean(isAutocommit==JNI_TRUE)));
    CHECK_EXCEPTION(env);
    jboolean isReadonly = env->CallBooleanMethod(oConnection, isReadOnly(env));
    resultVector.push_back(std::pair<Item, Item>(theFactory->createString("readonly"), theFactory->createBoolean(isReadonly==JNI_TRUE)));
    CHECK_EXCEPTION(env);

    if (!isolationLevelsLoaded) {
      TRANSACTION_NONE = env->GetStaticIntField(cConnection, T_NONE(env));
      CHECK_EXCEPTION(env);
      TRANSACTION_READ_UNCOMMITTED = env->GetStaticIntField(cConnection, T_UNCOMMITTED(env));
      CHECK_EXCEPTION(env);
      TRANSACTION_READ_COMMITTED = env->GetStaticIntField(cConnection, T_COMMITTED(env));
      CHECK_EXCEPTION(env);
      TRANSACTION_REPEATABLE_READ = env->GetStaticIntField(cConnection, T_REPEATABLE(env));
      CHECK_EXCEPTION(env);
      TRANSACTION_SERIALIZABLE = env->GetStaticIntField(cConnection, T_SERIALIZABLE(env));
      CHECK_EXCEPTION(env);
      isolationLevelsLoaded=true;
    }

    int isolationLevel = env->CallIntMethod(oConnection, getTransactionIsolation(env));
    CHECK_EXCEPTION(env);
    String isolLevel;
    if (isolationLevel==TRANSACTION_NONE) {
      isolLevel = "NOT-SUPPORTED";
    } else if (isolationLevel==TRANSACTION_READ_UNCOMMITTED) {
      isolLevel = "READ-UNCOMMITTED";
    } else if (isolationLevel==TRANSACTION_READ_COMMITTED) {
      isolLevel = "READ-COMMITTED";
    } else if (isolationLevel==TRANSACTION_REPEATABLE_READ) {
      isolLevel = "REPEATABLE-READ";
    } else if (isolationLevel==TRANSACTION_SERIALIZABLE) {
      isolLevel = "SERIALIZABLE";
    }
    resultVector.push_back(std::pair<Item, Item>(theFactory->createString("isolation-level"), theFactory->createString(isolLevel)));
    result = theFactory->createJSONObject(resultVector);

  JDBC_MODULE_CATCH
  
  return ItemSequence_t(new SingletonItemSequence(result));
}

jclass ConnectionOptionsFunction::cConnection = NULL;
jmethodID ConnectionOptionsFunction::getAutoCommit(JNIEnv *env) {
  static jmethodID mGetAutoCommit=NULL;
  if (mGetAutoCommit==NULL) {
    mGetAutoCommit=env->GetMethodID(cConnection, "getAutoCommit", "()Z");
  }
  return mGetAutoCommit;
}
jmethodID ConnectionOptionsFunction::isReadOnly(JNIEnv *env) {
  static jmethodID mIsReadOnly=NULL;
  if (mIsReadOnly==NULL) {
    mIsReadOnly=env->GetMethodID(cConnection, "isReadOnly", "()Z");
  }
  return mIsReadOnly;
}
jfieldID ConnectionOptionsFunction::T_NONE(JNIEnv *env) {
  static jfieldID fT_NONE=NULL;
  if (fT_NONE==NULL) {
    fT_NONE=env->GetStaticFieldID(cConnection, "TRANSACTION_NONE", "I");
  }
  return fT_NONE;
}
jfieldID ConnectionOptionsFunction::T_UNCOMMITTED(JNIEnv *env) {
  static jfieldID fT_UNCOMMITTED=NULL;
  if (fT_UNCOMMITTED==NULL) {
    fT_UNCOMMITTED=env->GetStaticFieldID(cConnection, "TRANSACTION_READ_UNCOMMITTED", "I");
  }
  return fT_UNCOMMITTED;
}
jfieldID ConnectionOptionsFunction::T_COMMITTED(JNIEnv *env) {
  static jfieldID fT_COMMITTED=NULL;
  if (fT_COMMITTED==NULL) {
    fT_COMMITTED=env->GetStaticFieldID(cConnection, "TRANSACTION_READ_COMMITTED", "I");
  }
  return fT_COMMITTED;
}
jfieldID ConnectionOptionsFunction::T_REPEATABLE(JNIEnv *env) {
  static jfieldID fT_REPEATABLE=NULL;
  if (fT_REPEATABLE==NULL) {
    fT_REPEATABLE=env->GetStaticFieldID(cConnection, "TRANSACTION_REPEATABLE_READ", "I");
  }
  return fT_REPEATABLE;
}
jfieldID ConnectionOptionsFunction::T_SERIALIZABLE(JNIEnv *env) {
  static jfieldID fT_SERIALIZABLE=NULL;
  if (fT_SERIALIZABLE==NULL) {
    fT_SERIALIZABLE=env->GetStaticFieldID(cConnection, "TRANSACTION_SERIALIZABLE", "I");
  }
  return fT_SERIALIZABLE;
}
jmethodID ConnectionOptionsFunction::getTransactionIsolation(JNIEnv *env) {
  static jmethodID mGetTransactionIsolation=NULL;
  if (mGetTransactionIsolation==NULL) {
    mGetTransactionIsolation=env->GetMethodID(cConnection, "getTransactionIsolation", "()I");
  }
  return mGetTransactionIsolation;
}



}}; // namespace zorba, jdbc
