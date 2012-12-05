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

#include "metadata.h"
#include "jdbc.h"
#include <zorba/singleton_item_sequence.h>

namespace zorba
{
namespace jdbc
{


ItemSequence_t
MetadataFunction::evaluate(const ExternalFunction::Arguments_t& args,
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

    std::vector<std::pair<zorba::Item, zorba::Item>> vResult;
    if (rowsAffected==-1) { // NON UPDATE QUERY
      jobject oResultSet = env->CallObjectMethod(oStatement, env->GetMethodID(cStatement, "getResultSet", "()Ljava/sql/ResultSet;"));
      CHECK_EXCEPTION(env);

      jclass cResultSet = env->FindClass("java/sql/ResultSet");
      CHECK_EXCEPTION(env);

      jobject oMetadata = env->CallObjectMethod(oResultSet, env->GetMethodID(cResultSet, "getMetaData", "()Ljava/sql/ResultSetMetaData;"));
      CHECK_EXCEPTION(env);

      jclass cResultSetMetaData = env->FindClass("java/sql/ResultSetMetaData");
      CHECK_EXCEPTION(env);

      int columns = env->CallIntMethod(oResultSet, env->GetMethodID(cResultSet, "getColumnCount", "()I"));
      CHECK_EXCEPTION(env);

      jmethodID mType = env->GetMethodID(cResultSetMetaData, "getColumnTypeName", "(I)Ljava/lang/String;");
      jmethodID mName = env->GetMethodID(cResultSetMetaData, "getColumnName",     "(I)Ljava/lang/String;");
      std::vector<zorba::Item> elements;

      for (int i=1; i<=columns; i++) {
          std::vector<std::pair<zorba::Item, zorba::Item>> column;

          jstring oName = (jstring) env->CallObjectMethod(oMetadata, mName, i);
          CHECK_EXCEPTION(env);
          String sName = env->GetStringUTFChars(oName, NULL);
          CHECK_EXCEPTION(env);
          zorba::Item iName = itemFactory->createString(sName);
          std::pair<zorba::Item, zorba::Item> pName(itemFactory->createString("name"), iName);
          column.push_back(pName);

          jstring oType = (jstring) env->CallObjectMethod(oMetadata, mType, i);
          CHECK_EXCEPTION(env);
          String  sType = env->GetStringUTFChars(oName, NULL);
          CHECK_EXCEPTION(env); 
          zorba::Item iType = itemFactory->createString(sName);
          std::pair<zorba::Item, zorba::Item> pType(itemFactory->createString("type"), iType);
          column.push_back(pType);
          elements.push_back(itemFactory->createJSONObject(column));
      }
      std::pair<zorba::Item, zorba::Item> allColumns(itemFactory->createString("columns"), itemFactory->createJSONArray(elements));
      vResult.push_back(allColumns);
    } else { // UPDATE QUERY
      std::pair<zorba::Item, zorba::Item> allColumns(itemFactory->createString("columns"), itemFactory->createInteger(rowsAffected));
      vResult.push_back(allColumns);
    }
    result = itemFactory->createJSONObject(vResult);
  JDBC_MODULE_CATCH
  
  return ItemSequence_t(new SingletonItemSequence(result));
}

}}; // namespace zorba, jdbc
