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

#include "executeupdate.h"
#include "jdbc.h"
#include "jsonitemsequence.h"
#include <zorba/singleton_item_sequence.h>

namespace zorba
{
namespace jdbc
{


ItemSequence_t
ExecuteUpdateFunction::evaluate(const ExternalFunction::Arguments_t& args,
                           const zorba::StaticContext* aStaticContext,
                           const zorba::DynamicContext* aDynamincContext) const
{
  JNIEnv *env = JdbcModule::getJavaEnv(aStaticContext);
  Item result;

  JDBC_MODULE_TRY
    String lConnectionUUID = JdbcModule::getStringArg(args, 0);
    String lQuery = JdbcModule::getStringArg(args, 1);
    
    jobject oConnection = JdbcModule::getObject(aDynamincContext, lConnectionUUID, INSTANCE_MAP_CONNECTIONS);

    jclass cConnection = JdbcModule::getJavaClass(JC_CONNECTION, env);
    jobject oStatement = env->CallObjectMethod(oConnection, env->GetMethodID(cConnection, "createStatement", "()Ljava/sql/Statement;"));
    CHECK_EXCEPTION(env);

    jclass cStatement = JdbcModule::getJavaClass(JC_STATEMENT, env);
    jstring query =  env->NewStringUTF(lQuery.c_str());
    int executionResult = env->CallIntMethod(oStatement, env->GetMethodID(cStatement, "executeUpdate", "(Ljava/lang/String;)I"), query );
    CHECK_EXCEPTION(env);
    result = theFactory->createInt(executionResult);

  JDBC_MODULE_CATCH
  
  return ItemSequence_t(new SingletonItemSequence(result));
}

}}; // namespace zorba, jdbc
