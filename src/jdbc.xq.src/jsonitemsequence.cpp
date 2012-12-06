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
      SQLTypes::init(env);
      cResultSet = env->FindClass("java/sql/ResultSet");
      CHECK_EXCEPTION(env);
      jclass cResultSetMetadata = env->FindClass("java/sql/ResultSetMetaData");
      CHECK_EXCEPTION(env);
      oMetadata = env->CallObjectMethod(oResultSet, env->GetMethodID(cResultSet, "ResultSetMetaData", "()Ljava/sql/ResultSetMetaData;"));
      CHECK_EXCEPTION(env);
      columnCount = env->CallIntMethod(oMetadata, env->GetMethodID(cResultSetMetadata, "getColumnCount", "()I"));
      CHECK_EXCEPTION(env);
      env->CallVoidMethod(oResultSet, env->GetMethodID(cResultSet, "beforeFirst", "()V"));
      CHECK_EXCEPTION(env);
      
      // Getting column names and types
      jmethodID mColumnType = env->GetMethodID(cResultSetMetadata, "getColumnType", "(I)I");
      jmethodID mColumnName = env->GetMethodID(cResultSetMetadata, "getColumnName", "(I)Ljava/lang/String;");
      columnNames = new std::string[columnCount];
      columnTypes = new long[columnCount];
      for(int i=0; i<columnCount; i++){
        jstring oName = (jstring) env->CallObjectMethod(oMetadata, mColumnName, i);
        CHECK_EXCEPTION(env);
        columnNames[i] = env->GetStringUTFChars(oName, NULL);
        CHECK_EXCEPTION(env);
        LOG("Getting column ["<< i << "] with name: " << columnNames[i])
        columnTypes[i] = env->CallIntMethod(oMetadata, mColumnType, i);
        CHECK_EXCEPTION(env);
      }

      mNext = env->GetMethodID(cResultSet, "next", "()Z");
      mGetInt = env->GetMethodID(cResultSet, "getInt", "(I)I");
      mGetDouble = env->GetMethodID(cResultSet, "getDouble", "(I)D");
      mGetString = env->GetMethodID(cResultSet, "getString", "(I)Ljava/lang/String;");
    }
    catch (zorba::jvm::VMOpenException&)
	  {
      JdbcModule::throwError("VM001", "Could not start the Java VM (is the classpath set?).");
	  }
	  catch (JavaException&)
	  {
      JdbcModule::throwJavaException(env, lException);
	  }
    itOpen=true;
  }

  bool JSONItemSequence::JSONIterator::next(zorba::Item& aItem){
    bool result = false;
    if(!itOpen)
      return result;

    JDBC_MODULE_TRY
      jboolean hasNext = env->CallBooleanMethod(oResultSet, mNext);
      CHECK_EXCEPTION(env);
      if (hasNext == JNI_FALSE)
        return result;

      std::vector<std::pair<zorba::Item, zorba::Item>> elements;

      for(int i=0; i<columnCount; i++){
        zorba::Item aKey = itemFactory->createString(columnNames[i]);
        zorba::Item aValue;

        if (SQLTypes::isInt(columnTypes[i])) {
          int value = env->CallIntMethod(oResultSet, mGetInt, i);
          CHECK_EXCEPTION(env);
          aValue = itemFactory->createInt(value);
        } else if (SQLTypes::isFloat(columnTypes[i])) {
          double value = env->CallDoubleMethod(oResultSet, mGetDouble, i);
          CHECK_EXCEPTION(env);
          aValue = itemFactory->createDouble(value);
          break;
        } else {
          jstring sValue = (jstring) env->CallObjectMethod(oResultSet, mGetString, i);
          CHECK_EXCEPTION(env);
          const char *value = env->GetStringUTFChars(sValue, NULL);
          CHECK_EXCEPTION(env);
          aValue = itemFactory->createString(zorba::String(value));
        }
        elements.push_back(std::pair<zorba::Item, zorba::Item>(aKey, aValue));
      }
      aItem = itemFactory->createJSONObject(elements);
      elements.clear();
      result = true;
    }
    catch (zorba::jvm::VMOpenException&)
	  {
      JdbcModule::throwError("VM001", "Could not start the Java VM (is the classpath set?).");
	  }
	  catch (JavaException&)
	  {
      JdbcModule::throwJavaException(env, lException);
	  }
    return result;
  }

  void JSONItemSequence::JSONIterator::close(){
    delete[] columnNames;
    delete[] columnTypes;
    itOpen = false;
    columnCount = 0;
  }

}}; // namespace zorba, jdbc
