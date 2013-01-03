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

#include "setvalue.h"
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
  CHECK_CONNECTION
  Item result;

  JDBC_MODULE_TRY
    String lStatementUUID = JdbcModule::getStringArg(args, 0);

    jobject oPreparedStatement = JdbcModule::getObject(aDynamincContext, lStatementUUID, INSTANCE_MAP_PREPAREDSTATEMENTS);

    long index = (long)JdbcModule::getItemArg(args, 1).getLongValue();
    Item value = JdbcModule::getItemArg(args, 2);
    int type = value.getTypeCode();

    switch (type) {
      case XS_DOUBLE:
        env->CallVoidMethod(oPreparedStatement, jPreparedStatement.setDouble, index, value.getDoubleValue());
      break;
      case XS_FLOAT:
        env->CallVoidMethod(oPreparedStatement, jPreparedStatement.setFloat, index, value.getDoubleValue());
      break;
      case XS_INTEGER:
        env->CallVoidMethod(oPreparedStatement, jPreparedStatement.setLong, index, value.getLongValue());
      break;
      case XS_DECIMAL:
        double dVal;
        sscanf(value.getStringValue().c_str(), "%lf", &dVal);
        env->CallVoidMethod(oPreparedStatement, jPreparedStatement.setDouble, index,  dVal);
      break;
      case XS_BOOLEAN:
        jboolean val = JNI_FALSE;
        if (value.getBooleanValue())
          val = JNI_TRUE;
        env->CallVoidMethod(oPreparedStatement, jPreparedStatement.setBoolean, index, val);
      case JS_NULL:
        jobject oParameterMetadata = env->CallObjectMethod(oPreparedStatement, jPreparedStatement.getParameterMetaData);
        CHECK_EXCEPTION
        int parameterType = env->CallIntMethod(oParameterMetadata, jParameterMetadata.getParameterType, index);
        CHECK_EXCEPTION
        env->CallVoidMethod(oPreparedStatement, jPreparedStatement.setNull, index, parameterType);
        CHECK_EXCEPTION
      default: // STRING
        jstring val =  env->NewStringUTF(value.getStringValue().c_str());
        env->CallVoidMethod(oPreparedStatement, jPreparedStatement.setString, index, val);
      break;
    }
    CHECK_EXCEPTION
  JDBC_MODULE_CATCH
  
  return ItemSequence_t(new EmptySequence());
}

}}; // namespace zorba, jdbc
