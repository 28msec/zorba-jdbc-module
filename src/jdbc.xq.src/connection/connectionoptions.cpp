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
#include "instancemap.h"

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
	static JNIEnv* env;
  zorba::String lStrUUID;
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
    Iterator_t lIter = args[0]->getIterator();
		lIter->open();
		Item item;
		if( lIter->next(item) )
		{
      lStrUUID = item.getStringValue();
    }
		lIter->close();


    DynamicContext* lDctx = const_cast<DynamicContext*>(aDynamincContext);
    InstanceMap* lInstanceMap;
    if (!(lInstanceMap = dynamic_cast<InstanceMap*>(lDctx->getExternalFunctionParameter(JDBC_MODULE_INSTANCE_MAP_CONNECTIONS))))
    {
      JdbcModule::throwError("SQL08003", "Connection does not exist.");
    }
    if(!(oConnection = lInstanceMap->getInstance(lStrUUID)))
    {
      JdbcModule::throwError("SQL08003", "Connection does not exist.");
    }
    std::vector<std::pair<Item, Item>> resultVector;
    env = zorba::jvm::JavaVMSingleton::getInstance(aStaticContext)->getEnv();
    CHECK_EXCEPTION(env);
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
		jclass stringWriterClass = env->FindClass("java/io/StringWriter");
		jclass printWriterClass = env->FindClass("java/io/PrintWriter");
		jclass throwableClass = env->FindClass("java/lang/Throwable");
		jobject stringWriter = env->NewObject(
				stringWriterClass,
				env->GetMethodID(stringWriterClass, "<init>", "()V"));

		jobject printWriter = env->NewObject(
				printWriterClass,
				env->GetMethodID(printWriterClass, "<init>", "(Ljava/io/Writer;)V"),
				stringWriter);

		env->CallObjectMethod(lException,
				env->GetMethodID(throwableClass, "printStackTrace",
						"(Ljava/io/PrintWriter;)V"),
				printWriter);

		//env->CallObjectMethod(printWriter, env->GetMethodID(printWriterClass, "flush", "()V"));
		jmethodID toStringMethod =
			env->GetMethodID(stringWriterClass, "toString", "()Ljava/lang/String;");
		jobject errorMessageObj = env->CallObjectMethod(
				stringWriter, toStringMethod);
		jstring errorMessage = (jstring) errorMessageObj;
		const char *errMsg = env->GetStringUTFChars(errorMessage, 0);
		std::stringstream s;
		s << "A Java Exception was thrown:" << std::endl << errMsg;
		env->ReleaseStringUTFChars(errorMessage, errMsg);
		std::string err("");
		err += s.str();
		env->ExceptionClear();
    JdbcModule::throwError("JAVA-EXCEPTION", err);
	}
  
  return ItemSequence_t(new SingletonItemSequence(result));
}

}}; // namespace zorba, jdbc