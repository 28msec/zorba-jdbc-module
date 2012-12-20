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

#include "setnumeric.h"
#include "jdbc.h"

using namespace zorba::store;

namespace zorba
{
namespace jdbc
{


ItemSequence_t
SetNumericFunction::evaluate(const ExternalFunction::Arguments_t& args,
                           const zorba::StaticContext* aStaticContext,
                           const zorba::DynamicContext* aDynamincContext) const
{
  JNIEnv *env = JdbcModule::getJavaEnv(aStaticContext);
  Item result;

  JDBC_MODULE_TRY
    String lStatementUUID = JdbcModule::getStringArg(args, 0);

    jobject oPreparedStatement = JdbcModule::getObject(aDynamincContext, lStatementUUID, INSTANCE_MAP_PREPAREDSTATEMENTS);

    long index = (long)JdbcModule::getItemArg(args, 1).getLongValue();
    Item value = JdbcModule::getItemArg(args, 2);
    int type = value.getTypeCode();

    jclass cPreparedStatement = JdbcModule::getJavaClass(JC_PREPARED_STATEMEMT, env);

    switch (type) {
      case XS_DOUBLE:
        env->CallVoidMethod(oPreparedStatement, env->GetMethodID(cPreparedStatement, "setDouble", "(ID)V"), index, value.getDoubleValue());
      break;
      case XS_FLOAT:
        env->CallVoidMethod(oPreparedStatement, env->GetMethodID(cPreparedStatement, "setFloat", "(IF)V"), index, value.getDoubleValue());
      break;
      case XS_INTEGER:
        env->CallVoidMethod(oPreparedStatement, env->GetMethodID(cPreparedStatement, "setLong", "(IJ)V"), index, value.getLongValue());
      break;
      case XS_DECIMAL:
        double dVal;
        sscanf(value.getStringValue().c_str(), "%lf", &dVal);
        env->CallVoidMethod(oPreparedStatement, env->GetMethodID(cPreparedStatement, "setDouble", "(ID)V"), index,  dVal);
      break;
      default:
        JdbcModule::throwError("SQL004", "Error setting numeric value.");
      break;
    }
    CHECK_EXCEPTION(env);
  JDBC_MODULE_CATCH
  
  return ItemSequence_t(new EmptySequence());
}

}}; // namespace zorba, jdbc
