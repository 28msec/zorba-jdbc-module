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

#include "executeupdateprepared.h"
#include "jdbc.h"
#include <zorba/singleton_item_sequence.h>

namespace zorba
{
namespace jdbc
{


ItemSequence_t
ExecuteUpdatePreparedFunction::evaluate(const ExternalFunction::Arguments_t& args,
                           const zorba::StaticContext* aStaticContext,
                           const zorba::DynamicContext* aDynamincContext) const
{
  String lStatementUUID = JdbcModule::getStringArg(args, 0);

  CHECK_CONNECTION
  Item result;

  JDBC_MODULE_TRY
    jobject oPreparedStatement = JdbcModule::getObject(aDynamincContext, lStatementUUID, INSTANCE_MAP_PREPAREDSTATEMENTS);

    int rowCount = env->CallIntMethod(oPreparedStatement, jPreparedStatement.executeUpdate);
    CHECK_EXCEPTION

    result = theFactory->createInteger(rowCount);
  JDBC_MODULE_CATCH
  
  return ItemSequence_t(new SingletonItemSequence(result));
}

}}; // namespace zorba, jdbc
