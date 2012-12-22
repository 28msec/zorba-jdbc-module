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

#include "jsonitemsequence.h"
#include "sqltypes.h"
#include "jdbc.h"


namespace zorba
{
namespace jdbc
{
  void JSONItemSequence::JSONIterator::open(){
    if(oResultSet == NULL)
      return;

    JDBC_MODULE_TRY
      itemFactory = Zorba::getInstance(0)->getItemFactory();

      oMetadata = JdbcModule::env->CallObjectMethod(oResultSet, JdbcModule::jResultSet.getMetaData);
      CHECK_EXCEPTION
      columnCount = JdbcModule::env->CallIntMethod(oMetadata, JdbcModule::jResultSetMetadata.getColumnCount);
      CHECK_EXCEPTION
      JdbcModule::env->CallVoidMethod(oResultSet, JdbcModule::jResultSet.beforeFirst);
      CHECK_EXCEPTION

      // Getting column names and types
      columnNames = new String[columnCount];
      columnTypes = new long[columnCount];
      for(int i=0; i<columnCount; i++){
        jstring oName = (jstring) JdbcModule::env->CallObjectMethod(oMetadata, JdbcModule::jResultSetMetadata.getColumnName, i+1);
        CHECK_EXCEPTION
        columnNames[i] = JdbcModule::env->GetStringUTFChars(oName, NULL);
        CHECK_EXCEPTION
        columnTypes[i] = JdbcModule::env->CallIntMethod(oMetadata, JdbcModule::jResultSetMetadata.getColumnType, i+1);
        CHECK_EXCEPTION
      }
    JDBC_MODULE_CATCH
    itOpen=true;
  }

  bool JSONItemSequence::JSONIterator::next(zorba::Item& aItem){
    bool result = false;
    if(!itOpen)
      return result;

    JDBC_MODULE_TRY
      jboolean hasNext = JdbcModule::env->CallBooleanMethod(oResultSet, JdbcModule::jResultSet.next);
      CHECK_EXCEPTION
      if (hasNext == JNI_FALSE)
        return result;

      std::vector<std::pair<zorba::Item, zorba::Item> > elements;
      for(int i=0; i<columnCount; i++){
        zorba::Item aKey = itemFactory->createString(columnNames[i]);
        zorba::Item aValue;
        if (SQLTypes::isInt(columnTypes[i])) {
          int value = JdbcModule::env->CallIntMethod(oResultSet, JdbcModule::jResultSet.getInt, i+1);
          CHECK_EXCEPTION
          aValue = itemFactory->createInt(value);
        } else if (SQLTypes::isFloat(columnTypes[i])) {
          double value = JdbcModule::env->CallDoubleMethod(oResultSet, JdbcModule::jResultSet.getDouble, i+1);
          CHECK_EXCEPTION
          aValue = itemFactory->createDouble(value);
        } else {
          jstring sValue = (jstring) JdbcModule::env->CallObjectMethod(oResultSet, JdbcModule::jResultSet.getString, i+1);
          CHECK_EXCEPTION
          const char *value = JdbcModule::env->GetStringUTFChars(sValue, NULL);
          CHECK_EXCEPTION
          aValue = itemFactory->createString(zorba::String(value));
        }
        elements.push_back(std::pair<zorba::Item, zorba::Item>(aKey, aValue));
      }
      aItem = itemFactory->createJSONObject(elements);
      elements.clear();
      result = true;
    JDBC_MODULE_CATCH
    return result;
  }

  void JSONItemSequence::JSONIterator::close(){
    delete[] columnNames;
    delete[] columnTypes;
    itOpen = false;
    columnCount = 0;
  }

}}; // namespace zorba, jdbc
