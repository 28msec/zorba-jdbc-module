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

#include "javaids.h"
#include "jdbc.h"


namespace zorba
{
namespace jdbc
{

  bool JavaDriverManager::init() {
    classID = env->FindClass("java/sql/DriverManager");
    getConnection = env->GetStaticMethodID(classID, "getConnection", "(Ljava/lang/String;)Ljava/sql/Connection;");
    getConnectionWithUser = env->GetStaticMethodID(classID, "getConnection", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/sql/Connection;");
    return true;
  }
  bool JavaConnection::init() {
    classID = env->FindClass("java/sql/Connection");
    commit = env->GetMethodID(classID, "commit", "()V");
    rollback = env->GetMethodID(classID, "rollback", "()V");
    setAutoCommit = env->GetMethodID(classID, "setAutoCommit", "(Z)V");
    setReadOnly = env->GetMethodID(classID, "setReadOnly", "(Z)V");
    setTransactionIsolation = env->GetMethodID(classID, "setTransactionIsolation", "(I)V");
    getAutoCommit = env->GetMethodID(classID, "getAutoCommit", "()Z");
    isReadOnly = env->GetMethodID(classID, "isReadOnly", "()Z");
    getTransactionIsolation =  env->GetMethodID(classID, "getTransactionIsolation", "()I");
    isClosed = env->GetMethodID(classID, "isClosed", "()Z");
    close = env->GetMethodID(classID, "close", "()V");
    createStatement = env->GetMethodID(classID, "createStatement", "()Ljava/sql/Statement;");
    prepareStatement = env->GetMethodID(classID, "prepareStatement", "(Ljava/lang/String;)Ljava/sql/PreparedStatement;");

    TRANSACTION_NONE = env->GetStaticIntField(classID, env->GetStaticFieldID(classID, "TRANSACTION_NONE", "I"));
    TRANSACTION_READ_UNCOMMITTED = env->GetStaticIntField(classID, env->GetStaticFieldID(classID, "TRANSACTION_READ_UNCOMMITTED", "I"));
    TRANSACTION_READ_COMMITTED = env->GetStaticIntField(classID, env->GetStaticFieldID(classID, "TRANSACTION_READ_COMMITTED", "I"));
    TRANSACTION_REPEATABLE_READ = env->GetStaticIntField(classID, env->GetStaticFieldID(classID, "TRANSACTION_REPEATABLE_READ", "I"));
    TRANSACTION_SERIALIZABLE = env->GetStaticIntField(classID, env->GetStaticFieldID(classID, "TRANSACTION_SERIALIZABLE", "I"));
    return true;
  }
  bool JavaStatement::init() {
    classID = env->FindClass("java/sql/Statement");
    getUpdateCount = env->GetMethodID(classID, "getUpdateCount", "()I");
    getResultSet = env->GetMethodID(classID, "getResultSet", "()Ljava/sql/ResultSet;");
    execute = env->GetMethodID(classID, "execute", "(Ljava/lang/String;)Z");
    executeQuery = env->GetMethodID(classID, "executeQuery", "(Ljava/lang/String;)Ljava/sql/ResultSet;");
    executeUpdate = env->GetMethodID(classID, "executeUpdate", "(Ljava/lang/String;)I");
    return true;
  }
  bool JavaResultSet::init() {
    classID = env->FindClass("java/sql/ResultSet");
    last = env->GetMethodID(classID, "last", "()Z");
    getRow = env->GetMethodID(classID, "getRow", "()I");
    getMetaData = env->GetMethodID(classID, "getMetaData", "()Ljava/sql/ResultSetMetaData;");
    beforeFirst = env->GetMethodID(classID, "beforeFirst", "()V");
    next = env->GetMethodID(classID, "next", "()Z");
    getInt = env->GetMethodID(classID, "getInt", "(I)I");
    getDouble = env->GetMethodID(classID, "getDouble", "(I)D");
    getString = env->GetMethodID(classID, "getString", "(I)Ljava/lang/String;");
    return true;
  }
  bool JavaResultSetMetadata::init() {
    classID = env->FindClass("java/sql/ResultSetMetaData");
    getColumnCount = env->GetMethodID(classID, "getColumnCount", "()I");
    getColumnType = env->GetMethodID(classID, "getColumnType", "(I)I");
    getColumnName = env->GetMethodID(classID, "getColumnName", "(I)Ljava/lang/String;");
    getColumnTypeName = env->GetMethodID(classID, "getColumnTypeName", "(I)Ljava/lang/String;");
    isAutoIncrement = env->GetMethodID(classID, "isAutoIncrement", "(I)Z");
    isNullable = env->GetMethodID(classID, "isNullable", "(I)I");

    COLUMN_NO_NULLS = env->GetStaticIntField(classID, env->GetStaticFieldID(classID, "columnNoNulls", "I"));
    COLUMN_NULLABLE = env->GetStaticIntField(classID, env->GetStaticFieldID(classID, "columnNullable", "I"));
    COLUMN_NULLABLE_UNKNOWN = env->GetStaticIntField(classID, env->GetStaticFieldID(classID, "columnNullableUnknown", "I"));
    return true;
  }
  bool JavaPreparedStatement::init() {
    classID = env->FindClass("java/sql/PreparedStatement");
    clearParameters = env->GetMethodID(classID, "clearParameters", "()V");
    execute = env->GetMethodID(classID, "execute", "()Z");
    executeQuery = env->GetMethodID(classID, "executeQuery", "()Ljava/sql/ResultSet;");
    executeUpdate = env->GetMethodID(classID, "executeUpdate", "()I");
    getParameterMetaData = env->GetMethodID(classID, "getParameterMetaData", "()Ljava/sql/ParameterMetaData;");
    getUpdateCount = env->GetMethodID(classID, "getUpdateCount", "()I");
    getResultSet = env->GetMethodID(classID, "getResultSet", "()Ljava/sql/ResultSet;");
    setBoolean = env->GetMethodID(classID, "setBoolean", "(IZ)V");
    setNull = env->GetMethodID(classID, "setNull", "(II)V");
    setDouble = env->GetMethodID(classID, "setDouble", "(ID)V");
    setFloat = env->GetMethodID(classID, "setFloat", "(IF)V");
    setLong = env->GetMethodID(classID, "setLong", "(IJ)V");
    setString = env->GetMethodID(classID, "setString", "(ILjava/lang/String;)V");
    return true;
  }
  bool JavaParameterMetadata::init() {
    classID = env->FindClass("java/sql/ParameterMetaData");
    getParameterCount = env->GetMethodID(classID, "getParameterCount", "()I");
    getParameterTypeName = env->GetMethodID(classID, "getParameterTypeName",  "(I)Ljava/lang/String;");
    getParameterClassName = env->GetMethodID(classID, "getParameterClassName", "(I)Ljava/lang/String;");
    getParameterType = env->GetMethodID(classID, "getParameterType", "(I)I");
    return true;
  }


}}; // namespace zorba, jdbc

