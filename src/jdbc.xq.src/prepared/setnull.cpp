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

#include "setnull.h"
#include "jdbc.h"

namespace zorba
{
namespace jdbc
{


ItemSequence_t
SetNullFunction::evaluate(const ExternalFunction::Arguments_t& args,
                           const zorba::StaticContext* aStaticContext,
                           const zorba::DynamicContext* aDynamincContext) const
{
  JNIEnv *env = JdbcModule::getJavaEnv(aStaticContext);
  Item result;

  JDBC_MODULE_TRY
    String lStatementUUID = JdbcModule::getStringArg(args, 0);

    jobject oPreparedStatement = JdbcModule::getObject(aDynamincContext, lStatementUUID, INSTANCE_MAP_PREPAREDSTATEMENTS);

    jclass cPreparedStatement = JdbcModule::getJavaClass(JC_PREPARED_STATEMEMT, env);

    jobject oParameterMetadata = env->CallObjectMethod(oPreparedStatement, env->GetMethodID(cPreparedStatement, "getParameterMetaData", "()Ljava/sql/ParameterMetaData;"));
    CHECK_EXCEPTION(env);

    jclass cParameterMetaData = JdbcModule::getJavaClass(JC_PARAMETER_META_DATA, env);

    long index = (long)JdbcModule::getItemArg(args, 1).getLongValue();
    int parameterType = env->CallIntMethod(oParameterMetadata, env->GetMethodID(cParameterMetaData, "getParameterType", "(I)I"), index);
    CHECK_EXCEPTION(env);

    env->CallVoidMethod(oPreparedStatement, env->GetMethodID(cPreparedStatement, "setNull", "(II)V"), index, parameterType);
    CHECK_EXCEPTION(env);

  JDBC_MODULE_CATCH
  
  return ItemSequence_t(new EmptySequence());
}

}}; // namespace zorba, jdbc
