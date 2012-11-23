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
