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

#include "rollback.h"
#include "jdbc.h"

namespace zorba
{
namespace jdbc
{


ItemSequence_t
RollbackFunction::evaluate(const ExternalFunction::Arguments_t& args,
                           const zorba::StaticContext* aStaticContext,
                           const zorba::DynamicContext* aDynamincContext) const
{
  String lConnectionUUID = JdbcModule::getStringArg(args, 0);

  CHECK_CONNECTION

  JDBC_MODULE_TRY
    jobject oConnection = JdbcModule::getObject(aDynamincContext, lConnectionUUID, INSTANCE_MAP_CONNECTIONS);

    jclass cConnection = jConnection.classID;
    env->CallVoidMethod(oConnection, jConnection.rollback);
    CHECK_EXCEPTION

  JDBC_MODULE_CATCH
  
  return ItemSequence_t(new EmptySequence());
}

}}; // namespace zorba, jdbc
