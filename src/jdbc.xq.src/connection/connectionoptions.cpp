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
	jthrowable lException = 0;
  JNIEnv *env = JdbcModule::getJavaEnv(aStaticContext);
  jobject oConnection;
  static bool isolationLevelsLoaded = false;
  static int TRANSACTION_NONE;
  static int TRANSACTION_READ_UNCOMMITTED;
  static int TRANSACTION_READ_COMMITTED;
  static int TRANSACTION_REPEATABLE_READ;
  static int TRANSACTION_SERIALIZABLE;
  Item result;
  try
  {
		// Local variables
    String lStrUUID = JdbcModule::getStringArg(args, 0);

    InstanceMap* lInstanceMap = JdbcModule::getInstanceMap(aDynamincContext, INSTANCE_MAP_CONNECTIONS);
    if (lInstanceMap = NULL)
    {
      JdbcModule::throwError("SQL08003", "Connection does not exist.");
    }
    jobject oConnection = lInstanceMap->getInstance(lStrUUID);
    if(oConnection==NULL)
    {
      JdbcModule::throwError("SQL08003", "Connection does not exist.");
    }

    std::vector<std::pair<Item, Item>> resultVector;
    jclass cConnection = env->FindClass("java/sql/Connection");
    CHECK_EXCEPTION(env);
    jboolean isAutocommit = env->CallBooleanMethod(oConnection, env->GetMethodID(cConnection, "getAutoCommit", "()Z"));
    resultVector.push_back(std::pair<Item, Item>(theFactory->createString("autocommit"), theFactory->createBoolean(isAutocommit==JNI_TRUE)));
    CHECK_EXCEPTION(env);
    jboolean isReadonly = env->CallBooleanMethod(oConnection, env->GetMethodID(cConnection, "isReadOnly", "()Z"));
    resultVector.push_back(std::pair<Item, Item>(theFactory->createString("readonly"), theFactory->createBoolean(isReadonly==JNI_TRUE)));
    CHECK_EXCEPTION(env);

    if (!isolationLevelsLoaded) {
      TRANSACTION_NONE             = env->CallIntMethod(oConnection, env->GetMethodID(cConnection, "TRANSACTION_NONE", "I"));
      CHECK_EXCEPTION(env);
      TRANSACTION_READ_UNCOMMITTED = env->CallIntMethod(oConnection, env->GetMethodID(cConnection, "TRANSACTION_READ_UNCOMMITTED", "I"));
      CHECK_EXCEPTION(env);
      TRANSACTION_READ_COMMITTED   = env->CallIntMethod(oConnection, env->GetMethodID(cConnection, "TRANSACTION_READ_COMMITTED", "I"));
      CHECK_EXCEPTION(env);
      TRANSACTION_REPEATABLE_READ  = env->CallIntMethod(oConnection, env->GetMethodID(cConnection, "TRANSACTION_REPEATABLE_READ", "I"));
      CHECK_EXCEPTION(env);
      TRANSACTION_SERIALIZABLE     = env->CallIntMethod(oConnection, env->GetMethodID(cConnection, "TRANSACTION_SERIALIZABLE", "I"));
      CHECK_EXCEPTION(env);
      isolationLevelsLoaded=true;
    }

    int isolationLevel = env->CallIntMethod(oConnection, env->GetMethodID(cConnection, "getTransactionIsolation", "()I"));
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

  }
  catch (zorba::jvm::VMOpenException&)
	{
    JdbcModule::throwError("VM001", "Could not start the Java VM (is the classpath set?).");
	}
	catch (JavaException&)
	{
    JdbcModule::throwJavaException(env, lException);
	}
  
  return ItemSequence_t(new SingletonItemSequence(result));
}

}}; // namespace zorba, jdbc
