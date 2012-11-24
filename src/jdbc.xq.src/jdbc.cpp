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

#include <zorba/base64.h>
#include <zorba/empty_sequence.h>
#include <zorba/diagnostic_list.h>
#include <zorba/function.h>
#include <zorba/external_module.h>
#include <zorba/user_exception.h>
#include <zorba/file.h>
#include <zorba/item_factory.h>
#include <zorba/serializer.h>
#include <zorba/singleton_item_sequence.h>
#include <zorba/vector_item_sequence.h>
#include <zorba/zorba.h>

#include "jdbc.h"

#include "connect.h"
#include "connectionoptions.h"
#include "disconnect.h"
#include "isconnected.h"
#include "commit.h"
#include "rollback.h"
#include "execute.h"
#include "executequery.h"
#include "executequerydeterm.h"
#include "executeupdate.h"
#include "clearparams.h"
#include "executeprepared.h"
#include "executequeryprepared.h"
#include "executeupdateprepared.h"
#include "parametermetadata.h"
#include "preparestatement.h"
#include "setboolean.h"
#include "setnull.h"
#include "setnumeric.h"
#include "setstring.h"
#include "affectedrows.h"
#include "metadata.h"
#include "resultset.h"


namespace zorba
{
namespace jdbc
{


zorba::ExternalFunction* 
JdbcModule::getExternalFunction(const zorba::String& localName)
{
  FuncMap_t::iterator lIte = theFunctions.find(localName);

  ExternalFunction*& lFunc = theFunctions[localName];

  if (lIte == theFunctions.end())
  {
    // 2 CONNECTION HANDLING
    if (localName == "connect")
    {
      lFunc = new ConnectFunction(this);
    }
    else if (localName == "disconnect")
    {
      lFunc = new DisconnectFunction(this);
    }
    else if (localName == "is-connected")
    {
      lFunc = new IsConnectedFunction(this);
    }
    else if (localName == "connection-options")
    {
      lFunc = new ConnectionOptionsFunction(this);
    }
    // 3 TRANSACTIONS
    else if (localName == "commit")
    {
      lFunc = new CommitFunction(this);
    }
    else if (localName == "rollback")
    {
      lFunc = new RollbackFunction(this);
    }
    // 4 SIMPLE STATEMENTS
    else if (localName == "execute")
    {
      lFunc = new ExecuteFunction(this);
    }
    else if (localName == "execute-query")
    {
      lFunc = new ExecuteQueryFunction(this);
    }
    else if (localName == "execute-query-deterministic")
    {
      lFunc = new ExecuteQueryDeterministicFunction(this);
    }
    else if (localName == "execute-update")
    {
      lFunc = new ExecuteUpdateFunction(this);
    }
    // 5 PREPARED STATEMENTS
    else if (localName == "prepare-statement")
    {
      lFunc = new PrepareStatementFunction(this);
    }
    else if (localName == "set-numeric")
    {
      lFunc = new SetNumericFunction(this);
    }
    else if (localName == "set-string")
    {
      lFunc = new SetStringFunction(this);
    }
    else if (localName == "set-boolean")
    {
      lFunc = new SetBooleanFunction(this);
    }
    else if (localName == "set-null")
    {
      lFunc = new SetNullFunction(this);
    }
    else if (localName == "clear-params")
    {
      lFunc = new ClearParamsFunction(this);
    }
    else if (localName == "parameter-metadata")
    {
      lFunc = new ParameterMetadataFunction(this);
    }
    else if (localName == "execute-prepared")
    {
      lFunc = new ExecutePreparedFunction(this);
    }
    else if (localName == "execute-query-prepared")
    {
      lFunc = new ExecuteQueryPreparedFunction(this);
    }
    else if (localName == "execute-update-prepared")
    {
      lFunc = new ExecuteUpdatePreparedFunction(this);
    }
    // 6 DATASETS
    else if (localName == "result-set")
    {
      lFunc = new ResultSetFunction(this);
    }
    else if (localName == "metadata")
    {
      lFunc = new MetadataFunction(this);
    }
    else if (localName == "affected-rows")
    {
      lFunc = new AffectedRowsFunction(this);
    }
  }
  return lFunc;
}

void
JdbcModule::throwError(const char *aLocalName, const char* aErrorMessage)
{
  String errDescription(aErrorMessage);
  JdbcModule::throwError (aLocalName, errDescription);
}

void
JdbcModule::throwError(const char *aLocalName, String aErrorMessage)
{
  String errNS(JDBC_MODULE_NAMESPACE);
  String errName(aLocalName);
  Item errQName = Zorba::getInstance(0)->getItemFactory()->createQName(errNS, errName);
  throw USER_EXCEPTION(errQName, aErrorMessage);
}

void
JdbcModule::throwJavaException(JNIEnv *env, jthrowable& lException)
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


JNIEnv* 
JdbcModule::getJavaEnv(const zorba::StaticContext* aStaticContext) {
  static JNIEnv* env;
  if (env==NULL) {
  	jthrowable lException = 0;
    try {
        env = zorba::jvm::JavaVMSingleton::getInstance(aStaticContext)->getEnv();
        CHECK_EXCEPTION(env);
    }
    catch (zorba::jvm::VMOpenException&)
	  {
      JdbcModule::throwError("VM001", "Could not start the Java VM (is the classpath set?).");
	  }
	  catch (JavaException&)
	  {
      JdbcModule::throwJavaException(env, lException);
	  }

  }
  return env;
}

String
JdbcModule::getStringArg(const ExternalFunction::Arguments_t& args, int index) {
  String result;  
  Iterator_t lIter = args[index]->getIterator();
	lIter->open();
	Item item;
	if( lIter->next(item) )
	{
    result = item.getStringValue();
  }
	lIter->close();
  return result;
}

Item
JdbcModule::getItemArg(const ExternalFunction::Arguments_t& args, int index) {
  Iterator_t lIter = args[index]->getIterator();
	lIter->open();
	Item item;
	lIter->next(item);
	lIter->close();
  return item;
}

String
JdbcModule::getUUID() {
  uuid lUUID;
  uuid::create(&lUUID);
  std::stringstream lStream;
  lStream << lUUID;
  return lStream.str();
}

InstanceMap* 
  JdbcModule::getCreateInstanceMap(const zorba::DynamicContext* aDynamincContext, String mapName) {
  InstanceMap* result;
  DynamicContext* lDctx = const_cast<DynamicContext*>(aDynamincContext);
  if (!(result = dynamic_cast<InstanceMap*>(lDctx->getExternalFunctionParameter(mapName))))
  {
    result = new InstanceMap();
    lDctx->addExternalFunctionParameter(mapName, result);
  }
  return result;
}

InstanceMap* 
  JdbcModule::getInstanceMap(const zorba::DynamicContext* aDynamincContext, String mapName) {
  DynamicContext* lDctx = const_cast<DynamicContext*>(aDynamincContext);
  InstanceMap* result = dynamic_cast<InstanceMap*>(lDctx->getExternalFunctionParameter(mapName));
  return result;
}


}}; // namespace zorba, jdbc

#ifdef WIN32
#  define DLL_EXPORT __declspec(dllexport)
#else
#  define DLL_EXPORT __attribute__ ((visibility("default")))
#endif

extern "C" DLL_EXPORT zorba::ExternalModule* createModule()
{
  return new zorba::jdbc::JdbcModule();
}
