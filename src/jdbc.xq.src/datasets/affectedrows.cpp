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
  JNIEnv *env = JdbcModule::getJavaEnv(aStaticContext);
  Item result;

  JDBC_MODULE_TRY
    String lStatementUUID = JdbcModule::getStringArg(args, 0);
    InstanceMap* lInstanceMap = JdbcModule::getCreateInstanceMap(aDynamincContext, INSTANCE_MAP_STATEMENTS);
    if (lInstanceMap==NULL)
    {
      JdbcModule::throwError("SQL003", "Statement does not exist.");
    }
    jobject oStatement = lInstanceMap->getInstance(lStatementUUID);
    if(oStatement==NULL)
    {
      JdbcModule::throwError("SQL003", "Statement does not exist.");
    }
    zorba::ItemFactory* itemFactory = Zorba::getInstance(0)->getItemFactory();
    jclass cStatement = env->FindClass("java/sql/Statement");
    CHECK_EXCEPTION(env);

    int rowsAffected = env->CallIntMethod(oStatement, env->GetMethodID(cStatement, "getUpdateCount", "()I"));
    CHECK_EXCEPTION(env);

    if (rowsAffected==-1) { // NON UPDATE QUERY
      jobject oResultSet = env->CallObjectMethod(oStatement, env->GetMethodID(cStatement, "getResultSet", "()Ljava/sql/ResultSet;"));
      CHECK_EXCEPTION(env);

      jclass cResultSet = env->FindClass("java/sql/ResultSet");
      CHECK_EXCEPTION(env);

      jboolean hasRows = env->CallBooleanMethod(oResultSet, env->GetMethodID(cResultSet, "last", "()Z"));
      CHECK_EXCEPTION(env);

      if (hasRows==JNI_TRUE) {
        rowsAffected = env->CallBooleanMethod(oResultSet, env->GetMethodID(cResultSet, "getRow", "()I"));
        CHECK_EXCEPTION(env);
      } else {
        rowsAffected=0;
      }
    }
    result = itemFactory->createInteger(rowsAffected);
  JDBC_MODULE_CATCH
  
  return ItemSequence_t(new SingletonItemSequence(result));
}

}}; // namespace zorba, jdbc
