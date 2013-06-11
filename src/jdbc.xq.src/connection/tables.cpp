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

#include "tables.h"
#include "jdbc.h"
#include "jsonitemsequence.h"

namespace zorba
{
namespace jdbc
{

ItemSequence_t
TablesFunction::evaluate(const ExternalFunction::Arguments_t& args,
                           const zorba::StaticContext* aStaticContext,
                           const zorba::DynamicContext* aDynamincContext) const
{
  CHECK_CONNECTION
  jobject result=NULL;

  JDBC_MODULE_TRY

    String lStrUUID = JdbcModule::getStringArg(args, 0);

    jobject oConnection = JdbcModule::getObject(aDynamincContext, lStrUUID, INSTANCE_MAP_CONNECTIONS);

    jobject oDatabaseMetadata = env->CallObjectMethod(oConnection, jConnection.getMetadata);

    String sTemp;
    jstring jCatalog = NULL;
    if (JdbcModule::getOptionalStringArg(args, 1, sTemp))
    {
      jCatalog = env->NewStringUTF(sTemp.c_str());
    }
    jstring jSchema = NULL;
    if (JdbcModule::getOptionalStringArg(args, 2, sTemp))
    {
      jSchema = env->NewStringUTF(sTemp.c_str());
    }
    jstring jTable = NULL;
    if (JdbcModule::getOptionalStringArg(args, 3, sTemp))
    {
      jTable = env->NewStringUTF(sTemp.c_str());
    }
    jobjectArray jTypes = env->NewObjectArray(1, env->FindClass("java/lang/String"), env->NewStringUTF("TABLE"));
    
    result = env->CallObjectMethod(oDatabaseMetadata, jDatabaseMetadata.getTables, jCatalog, jSchema, jTable, jTypes);
    CHECK_EXCEPTION

  JDBC_MODULE_CATCH
  
  return ItemSequence_t(new JSONItemSequence(result));
}

}}; // namespace zorba, jdbc
