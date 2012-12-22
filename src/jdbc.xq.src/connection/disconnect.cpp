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

#include "disconnect.h"
#include "jdbc.h"

namespace zorba
{
namespace jdbc
{


ItemSequence_t
DisconnectFunction::evaluate(const ExternalFunction::Arguments_t& args,
                           const zorba::StaticContext* aStaticContext,
                           const zorba::DynamicContext* aDynamincContext) const
{
  JdbcModule::init(aStaticContext);

  JDBC_MODULE_TRY
    String lStrUUID = JdbcModule::getStringArg(args, 0);

    jobject oConnection = JdbcModule::getObject(aDynamincContext, lStrUUID, INSTANCE_MAP_CONNECTIONS);

    jboolean isClosed = JdbcModule::env->CallBooleanMethod(oConnection, JdbcModule::jConnection.isClosed);
    CHECK_EXCEPTION
    if (isClosed==JNI_TRUE) {
      JdbcModule::throwError("SQL08008", "Connection already closed.");
    }

    JdbcModule::env->CallVoidMethod(oConnection, JdbcModule::jConnection.close);
    CHECK_EXCEPTION

  JDBC_MODULE_CATCH
  
  return ItemSequence_t(new EmptySequence());
}

}}; // namespace zorba, jdbc
