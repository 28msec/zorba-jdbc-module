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

#include "resultset.h"
#include "jdbc.h"
#include "jsonitemsequence.h"

namespace zorba
{
namespace jdbc
{


ItemSequence_t
ResultSetFunction::evaluate(const ExternalFunction::Arguments_t& args,
                           const zorba::StaticContext* aStaticContext,
                           const zorba::DynamicContext* aDynamincContext) const
{
  JNIEnv *env = JdbcModule::getJavaEnv(aStaticContext);
  jobject result=NULL;

  JDBC_MODULE_TRY
    String lStatementUUID = JdbcModule::getStringArg(args, 0);

    InstanceMap* lInstanceMap = JdbcModule::getCreateInstanceMap(aDynamincContext, INSTANCE_MAP_STATEMENTS);
    if (lInstanceMap==NULL)
    {
      JdbcModule::throwError("SQL003", "Statement does not exist.");
    }
    jobject oPreparedStatement = lInstanceMap->getInstance(lStatementUUID);
    if(oPreparedStatement==NULL)
    {
      JdbcModule::throwError("SQL003", "Statement does not exist.");
    }

    jclass cPreparedStatement = env->FindClass("java/sql/Statement");
    CHECK_EXCEPTION(env);

    int iUpdateCount = env->CallIntMethod(oPreparedStatement, env->GetMethodID(cPreparedStatement, "getUpdateCount", "()I"));
    CHECK_EXCEPTION(env);
    if (iUpdateCount != -1) {
       JdbcModule::throwError("SQL005", "Query must be a non-updating query.");
    }

    result = env->CallObjectMethod(oPreparedStatement, env->GetMethodID(cPreparedStatement, "getResultSet", "()Ljava/sql/ResultSet;"));
    CHECK_EXCEPTION(env);
    
  JDBC_MODULE_CATCH
  
  return ItemSequence_t(new JSONItemSequence(result, env));
}

}}; // namespace zorba, jdbc
