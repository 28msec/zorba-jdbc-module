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
#include "affectedrows.h"
#include "jdbc.h"
#include <zorba/singleton_item_sequence.h>

namespace zorba
{
namespace jdbc
{


ItemSequence_t
AffectedRowsFunction::evaluate(const ExternalFunction::Arguments_t& args,
                           const zorba::StaticContext* aStaticContext,
                           const zorba::DynamicContext* aDynamincContext) const
{
  JdbcModule::init(aStaticContext);
  Item result;

  JDBC_MODULE_TRY
    String lStatementUUID = JdbcModule::getStringArg(args, 0);

    zorba::ItemFactory* itemFactory = Zorba::getInstance(0)->getItemFactory();

    jobject oStatement = JdbcModule::getObject(aDynamincContext, lStatementUUID, INSTANCE_MAP_STATEMENTS);

    int rowsAffected = JdbcModule::env->CallIntMethod(oStatement, JdbcModule::jStatement.getUpdateCount);
    CHECK_EXCEPTION

    if (rowsAffected==-1) { // NON UPDATE QUERY
      jobject oResultSet = JdbcModule::env->CallObjectMethod(oStatement, JdbcModule::jStatement.getResultSet);
      CHECK_EXCEPTION

      jboolean hasRows = JdbcModule::env->CallBooleanMethod(oResultSet, JdbcModule::jResultSet.last);
      CHECK_EXCEPTION

      if (hasRows==JNI_TRUE) {
        rowsAffected = JdbcModule::env->CallBooleanMethod(oResultSet, JdbcModule::jResultSet.getRow);
        CHECK_EXCEPTION
      } else {
        rowsAffected=0;
      }
    }
    result = itemFactory->createInteger(rowsAffected);
  JDBC_MODULE_CATCH
  
  return ItemSequence_t(new SingletonItemSequence(result));
}


}}; // namespace zorba, jdbc
