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

#include "parametermetadata.h"
#include "jdbc.h"
#include <zorba/singleton_item_sequence.h>

namespace zorba
{
namespace jdbc
{


ItemSequence_t
ParameterMetadataFunction::evaluate(const ExternalFunction::Arguments_t& args,
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

    jclass cPreparedStatement = env->FindClass("java/sql/PreparedStatement");
    CHECK_EXCEPTION(env);

    jobject oParameterMetaData = env->CallObjectMethod(oPreparedStatement, env->GetMethodID(cPreparedStatement, "getParameterMetaData", "()Ljava/sql/ParameterMetaData;"));
    CHECK_EXCEPTION(env);

    jclass cParameterMetaData = env->FindClass("java/sql/ParameterMetaData");
    CHECK_EXCEPTION(env);

    int columns = env->CallIntMethod(oParameterMetaData, env->GetMethodID(cParameterMetaData, "getParameterCount", "()I"));
    CHECK_EXCEPTION(env);

    zorba::ItemFactory* itemFactory = Zorba::getInstance(0)->getItemFactory();
    jmethodID mParameterType = env->GetMethodID(cParameterMetaData, "getParameterTypeName",  "(I)Ljava/lang/String;");
    jmethodID mParameterName = env->GetMethodID(cParameterMetaData, "getParameterClassName", "(I)Ljava/lang/String;");
    std::vector<zorba::Item> elements;

    for (int i=1; i<=columns; i++) {
        std::vector<std::pair<zorba::Item, zorba::Item>> column;

        jstring oName = (jstring) env->CallObjectMethod(oParameterMetaData, mParameterName, i);
        CHECK_EXCEPTION(env);
        String sName = env->GetStringUTFChars(oName, NULL);
        CHECK_EXCEPTION(env);
        zorba::Item iName = itemFactory->createString(sName);
        std::pair<zorba::Item, zorba::Item> pName(itemFactory->createString("name"), iName);
        column.push_back(pName);

        jstring oType = (jstring) env->CallObjectMethod(oParameterMetaData, mParameterType, i);
        CHECK_EXCEPTION(env);
        String  sType = env->GetStringUTFChars(oName, NULL);
        CHECK_EXCEPTION(env); 
        zorba::Item iType = itemFactory->createString(sName);
        std::pair<zorba::Item, zorba::Item> pType(itemFactory->createString("type"), iType);
        column.push_back(pType);
        elements.push_back(itemFactory->createJSONObject(column));
    }
    std::pair<zorba::Item, zorba::Item> allColumns(itemFactory->createString("columns"), itemFactory->createJSONArray(elements));
    std::vector<std::pair<zorba::Item, zorba::Item>> vResult;
    vResult.push_back(allColumns);
    result = itemFactory->createJSONObject(vResult);

  JDBC_MODULE_CATCH
  
  return ItemSequence_t(new SingletonItemSequence(result));
}

}}; // namespace zorba, jdbc
