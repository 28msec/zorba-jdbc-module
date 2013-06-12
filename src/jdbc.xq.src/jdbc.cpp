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

#include <zorba/diagnostic_list.h>
#include <zorba/empty_sequence.h>
#include <zorba/external_module.h>
#include <zorba/function.h>
#include <zorba/item_factory.h>
#include <zorba/serializer.h>
#include <zorba/singleton_item_sequence.h>
#include <zorba/user_exception.h>
#include <zorba/util/base64_util.h>
#include <zorba/vector_item_sequence.h>
#include <zorba/zorba.h>

#include "jdbc.h"

#include "connect.h"
#include "connectionoptions.h"
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
#include "closeprepared.h"
#include "parametermetadata.h"
#include "preparestatement.h"
#include "setboolean.h"
#include "setnull.h"
#include "setnumeric.h"
#include "setstring.h"
#include "setvalue.h"
#include "affectedrows.h"
#include "metadata.h"
#include "resultset.h"
#include "closedataset.h"
#include "tables.h"


namespace zorba
{
namespace jdbc
{

JNIEnv*               env;
JavaDriverManager     jDriverManager;
JavaConnection        jConnection;
JavaStatement         jStatement;
JavaResultSet         jResultSet;
JavaResultSetMetadata jResultSetMetadata;
JavaDatabaseMetadata  jDatabaseMetadata;
JavaPreparedStatement jPreparedStatement;
JavaParameterMetadata jParameterMetadata;
JavaBlob              jBlob;
bool isOutputJSON = true;

zorba::ExternalFunction* 
JdbcModule::getExternalFunction(const zorba::String& localName)
{
  FuncMap_t::iterator lIte = lFunctions.find(localName);

  ExternalFunction*& lFunc = lFunctions[localName];

  if (lIte == lFunctions.end())
  {
    // 2 CONNECTION HANDLING
    if (localName == "connect")
    {
      lFunc = new ConnectFunction(this);
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
    else if (localName == "set-value")
    {
      lFunc = new SetValueFunction(this);
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
    else if (localName == "close-prepared")
    {
      lFunc = new ClosePreparedFunction(this);
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
    else if (localName == "close-dataset")
    {
      lFunc = new CloseDataSetFunction(this);
    }
#if 0
    else if (localName == "tables")
    {
      lFunc = new TablesFunction(this);
    }
#endif
  }
  return lFunc;
}

void
JdbcModule::throwMapError(String aMap)
{
      if (aMap == INSTANCE_MAP_CONNECTIONS) {
        throwError("SQL08003", "Connection does not exist.");
      } else if (aMap == INSTANCE_MAP_STATEMENTS) {
        throwError("SQL003", "Statement does not exist.");
      } else if (aMap == INSTANCE_MAP_PREPAREDSTATEMENTS) {
        throwError("SQL008", "Prepared statement does not exist.");
      }
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

bool
JdbcModule::getOptionalStringArg(const ExternalFunction::Arguments_t& args, int index, String& aRes) {
  Iterator_t lIter = args[index]->getIterator();
  lIter->open();
  Item item;
  if( lIter->next(item) )
  {
    aRes = item.getStringValue();
    lIter->close();
    return true;
  }
  lIter->close();
  return false;
}

Item
JdbcModule::getItemArg(const ExternalFunction::Arguments_t& args, int index) {
  Item item;
  if (index < (int)args.size()) {
    Iterator_t lIter = args[index]->getIterator();
    lIter->open();
    lIter->next(item);
    lIter->close();
  }
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
    result->id = mapName;
    lDctx->addExternalFunctionParameter(mapName, result);
  }
  return result;
}

jobject 
  JdbcModule::getObject(const zorba::DynamicContext* aDynamincContext, String aObjectUUID, String aMap) {
    InstanceMap* lInstanceMap = getCreateInstanceMap(aDynamincContext, aMap);
    if (lInstanceMap==NULL)
      throwMapError(aMap);
    jobject oResult = lInstanceMap->getInstance(aObjectUUID);
    if(oResult==NULL)
      throwMapError(aMap);
    return oResult;
}

void 
  JdbcModule::deleteObject(const zorba::DynamicContext* aDynamincContext, String aObjectUUID, String aMap) {
    InstanceMap* lInstanceMap = getCreateInstanceMap(aDynamincContext, aMap);
    if (lInstanceMap==NULL)
      throwMapError(aMap);
    jobject oResult = lInstanceMap->getInstance(aObjectUUID);
    if(oResult==NULL)
      throwMapError(aMap);
    lInstanceMap->deleteInstance(aObjectUUID);
}

void JdbcModule::initGlobals(const zorba::StaticContext* aStaticContext) {
  if (env!=NULL) 
    return;
  JDBC_MODULE_TRY
    env = zorba::jvm::JavaVMSingleton::getInstance(aStaticContext)->getEnv();
    CHECK_EXCEPTION
    jDriverManager.init();
    jConnection.init();
    jStatement.init();
    jResultSet.init();
    jResultSetMetadata.init();
    jDatabaseMetadata.init();
    jPreparedStatement.init();
    jParameterMetadata.init();
    jBlob.init();
    SQLTypes::init();
  JDBC_MODULE_CATCH
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
/* vim:set et sw=2 ts=2: */
