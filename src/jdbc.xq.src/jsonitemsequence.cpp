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

      oMetadata = env->CallObjectMethod(oResultSet, jResultSet.getMetaData);
      CHECK_EXCEPTION
      columnCount = env->CallIntMethod(oMetadata, jResultSetMetadata.getColumnCount);
      CHECK_EXCEPTION
      env->CallVoidMethod(oResultSet, jResultSet.beforeFirst);
      CHECK_EXCEPTION

      // Getting column names and types
      columnNames = new String[columnCount];
      columnTypes = new long[columnCount];
      for(int i=0; i<columnCount; i++){
        jstring oName = (jstring) env->CallObjectMethod(oMetadata, jResultSetMetadata.getColumnName, i+1);
        CHECK_EXCEPTION
        const char * cName = env->GetStringUTFChars(oName, 0);
        CHECK_EXCEPTION
        columnNames[i] = String(cName);
        env->ReleaseStringUTFChars(oName, cName);
        CHECK_EXCEPTION 
        columnTypes[i] = env->CallIntMethod(oMetadata, jResultSetMetadata.getColumnType, i+1);
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
      jboolean hasNext = env->CallBooleanMethod(oResultSet, jResultSet.next);
      CHECK_EXCEPTION
      if (hasNext == JNI_FALSE)
        return result;

      std::vector<std::pair<zorba::Item, zorba::Item> > elements;
      for(int i=0; i<columnCount; i++){
        zorba::Item aKey = itemFactory->createString(columnNames[i]);
        zorba::Item aValue;
        if (SQLTypes::isInt(columnTypes[i])) {
          int value = env->CallIntMethod(oResultSet, jResultSet.getInt, i+1);
          CHECK_EXCEPTION
          aValue = itemFactory->createInt(value);
        } else if (SQLTypes::isFloat(columnTypes[i])) {
          double value = env->CallDoubleMethod(oResultSet, jResultSet.getDouble, i+1);
          CHECK_EXCEPTION
          aValue = itemFactory->createDouble(value);
        } else if (SQLTypes::isString(columnTypes[i])) {
          jstring sValue = (jstring) env->CallObjectMethod(oResultSet, jResultSet.getString, i+1);
          CHECK_EXCEPTION
          if (sValue!=NULL) {
            const char *value = env->GetStringUTFChars(sValue, 0);
            CHECK_EXCEPTION
            aValue = itemFactory->createString(String(value));
            env->ReleaseStringUTFChars(sValue, value);
          } else {
            aValue = itemFactory->createJSONNull();
          }
        } else if (SQLTypes::isString(columnTypes[i])) {
          jstring sValue = (jstring) env->CallObjectMethod(oResultSet, jResultSet.getString, i+1);
          CHECK_EXCEPTION
          if (sValue!=NULL) {
            const char *value = env->GetStringUTFChars(sValue, 0);
            CHECK_EXCEPTION
            aValue = itemFactory->createString(String(value));
            env->ReleaseStringUTFChars(sValue, value);
          } else {
            aValue = itemFactory->createJSONNull();
          }
        } else if (SQLTypes::isBLOB(columnTypes[i])) {
          jobject oBlob = env->CallObjectMethod(oResultSet, jResultSet.getBLOB, i+1);
          CHECK_EXCEPTION
          if (oBlob!=NULL) {
            jint length = env->CallIntMethod(oBlob, jBlob.length);
            CHECK_EXCEPTION
            jbyteArray bytes  = (jbyteArray) env->CallObjectMethod(oBlob, jBlob.getBytes, 1, length);
            CHECK_EXCEPTION
            const char* byteString = (const char*)(env->GetByteArrayElements(bytes, 0));
            aValue = itemFactory->createBase64Binary(byteString, length);
          } else {
            aValue = itemFactory->createJSONNull();
          } 
        } else if (columnTypes[i]==SQLTypes::_NULL) {
            aValue = itemFactory->createJSONNull();
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
