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

#include "setstring.h"
#include "jdbc.h"

using namespace zorba::store;

namespace zorba
{
namespace jdbc
{


ItemSequence_t
  SetStringFunction::evaluate(const ExternalFunction::Arguments_t& args,
                           const zorba::StaticContext* aStaticContext,
                           const zorba::DynamicContext* aDynamincContext) const
{
  JNIEnv *env = JdbcModule::getJavaEnv(aStaticContext);
  Item result;

  JDBC_MODULE_TRY
    String lStatementUUID = JdbcModule::getStringArg(args, 0);

    InstanceMap* lInstanceMap = JdbcModule::getCreateInstanceMap(aDynamincContext, INSTANCE_MAP_PREPAREDSTATEMENTS);
    if (lInstanceMap==NULL)
    {
      JdbcModule::throwError("SQL003", "Prepared statement does not exist.");
    }
    jobject oPreparedStatement = lInstanceMap->getInstance(lStatementUUID);
    if(oPreparedStatement==NULL)
    {
      JdbcModule::throwError("SQL003", "Prepared statement does not exist.");
    }

    int index = JdbcModule::getItemArg(args, 1).getLongValue();
    Item value = JdbcModule::getItemArg(args, 2);
    int type = value.getTypeCode();

    jclass cPreparedStatement = env->FindClass("java/sql/PreparedStatement");
    CHECK_EXCEPTION(env);
    if (type == XS_STRING) {
      jstring val =  env->NewStringUTF(value.getStringValue().c_str());
      env->CallVoidMethod(oPreparedStatement, env->GetMethodID(cPreparedStatement, "setString", "(ILjava/lang/String;)V"), index, val);
    } else {
      JdbcModule::throwError("SQL004", "Error setting string value.");
    }
    CHECK_EXCEPTION(env);

  JDBC_MODULE_CATCH
  
  return ItemSequence_t(new EmptySequence());
}

}}; // namespace zorba, jdbc
