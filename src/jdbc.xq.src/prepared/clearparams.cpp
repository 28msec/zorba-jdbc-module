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

#include "clearparams.h"
#include "jdbc.h"

namespace zorba
{
namespace jdbc
{


ItemSequence_t
ClearParamsFunction::evaluate(const ExternalFunction::Arguments_t& args,
                           const zorba::StaticContext* aStaticContext,
                           const zorba::DynamicContext* aDynamincContext) const
{
  JNIEnv *env = JdbcModule::getJavaEnv(aStaticContext);
  Item result;

  JDBC_MODULE_TRY
    String lStatementUUID = JdbcModule::getStringArg(args, 0);

    jobject oPreparedStatement = JdbcModule::getObject(aDynamincContext, lStatementUUID, INSTANCE_MAP_PREPAREDSTATEMENTS);

    jclass cPreparedStatement = env->FindClass("java/sql/PreparedStatement");
    CHECK_EXCEPTION(env);
    env->CallVoidMethod(oPreparedStatement, env->GetMethodID(cPreparedStatement, "clearParameters", "()V"));
    CHECK_EXCEPTION(env);

  JDBC_MODULE_CATCH
  
  return ItemSequence_t(new EmptySequence());
}

}}; // namespace zorba, jdbc
