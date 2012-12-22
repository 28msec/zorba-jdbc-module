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
    classID = JdbcModule::env->FindClass("java/sql/DriverManager");
    getConnection = JdbcModule::env->GetStaticMethodID(classID, "getConnection", "(Ljava/lang/String;)Ljava/sql/Connection;");
    getConnectionWithUser = JdbcModule::env->GetStaticMethodID(classID, "getConnection", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/sql/Connection;");
    return true;
  }
  bool JavaConnection::init() {
    TRANSACTION_NONE = JdbcModule::env->GetStaticIntField(classID, JdbcModule::env->GetStaticFieldID(classID, "TRANSACTION_NONE", "I"));
    TRANSACTION_READ_UNCOMMITTED = JdbcModule::env->GetStaticIntField(classID, JdbcModule::env->GetStaticFieldID(classID, "TRANSACTION_READ_UNCOMMITTED", "I"));
    TRANSACTION_READ_COMMITTED = JdbcModule::env->GetStaticIntField(classID, JdbcModule::env->GetStaticFieldID(classID, "TRANSACTION_READ_COMMITTED", "I"));
    TRANSACTION_REPEATABLE_READ = JdbcModule::env->GetStaticIntField(classID, JdbcModule::env->GetStaticFieldID(classID, "TRANSACTION_REPEATABLE_READ", "I"));
    TRANSACTION_SERIALIZABLE = JdbcModule::env->GetStaticIntField(classID, JdbcModule::env->GetStaticFieldID(classID, "TRANSACTION_SERIALIZABLE", "I"));

    classID = JdbcModule::env->FindClass("java/sql/Connection");
    commit = JdbcModule::env->GetMethodID(classID, "commit", "()V");
    rollback = JdbcModule::env->GetMethodID(classID, "rollback", "()V");
    setAutoCommit = JdbcModule::env->GetMethodID(classID, "setAutoCommit", "(Z)V");
    setReadOnly = JdbcModule::env->GetMethodID(classID, "setReadOnly", "(Z)V");
    setTransactionIsolation = JdbcModule::env->GetMethodID(classID, "setTransactionIsolation", "(I)V");
    getAutoCommit = JdbcModule::env->GetMethodID(classID, "getAutoCommit", "()Z");
    isReadOnly = JdbcModule::env->GetMethodID(classID, "isReadOnly", "()Z");
    getTransactionIsolation =  JdbcModule::env->GetMethodID(classID, "getTransactionIsolation", "()I");
    isClosed = JdbcModule::env->GetMethodID(classID, "isClosed", "()Z");
    close = JdbcModule::env->GetMethodID(classID, "close", "()V");
    createStatement = JdbcModule::env->GetMethodID(classID, "createStatement", "()Ljava/sql/Statement;");
    prepareStatement = JdbcModule::env->GetMethodID(classID, "prepareStatement", "(Ljava/lang/String;)Ljava/sql/PreparedStatement;");
    return true;
  }
  bool JavaStatement::init() {
    classID = JdbcModule::env->FindClass("java/sql/Statement");
    getUpdateCount = JdbcModule::env->GetMethodID(classID, "getUpdateCount", "()I");
    getResultSet = JdbcModule::env->GetMethodID(classID, "getResultSet", "()Ljava/sql/ResultSet;");
    execute = JdbcModule::env->GetMethodID(classID, "execute", "(Ljava/lang/String;)Z");
    executeQuery = JdbcModule::env->GetMethodID(classID, "executeQuery", "(Ljava/lang/String;)Ljava/sql/ResultSet;");
    executeUpdate = JdbcModule::env->GetMethodID(classID, "executeUpdate", "(Ljava/lang/String;)I");
    return true;
  }
  bool JavaResultSet::init() {
    classID = JdbcModule::env->FindClass("java/sql/ResultSet");
    last = JdbcModule::env->GetMethodID(classID, "last", "()Z");
    getRow = JdbcModule::env->GetMethodID(classID, "getRow", "()I");
    getMetaData = JdbcModule::env->GetMethodID(classID, "getMetaData", "()Ljava/sql/ResultSetMetaData;");
    beforeFirst = JdbcModule::env->GetMethodID(classID, "beforeFirst", "()V");
    next = JdbcModule::env->GetMethodID(classID, "next", "()Z");
    getInt = JdbcModule::env->GetMethodID(classID, "getInt", "(I)I");
    getDouble = JdbcModule::env->GetMethodID(classID, "getDouble", "(I)D");
    getString = JdbcModule::env->GetMethodID(classID, "getString", "(I)Ljava/lang/String;");
    return true;
  }
  bool JavaResultSetMetadata::init() {
    classID = JdbcModule::env->FindClass("java/sql/ResultSetMetaData");
    getColumnCount = JdbcModule::env->GetMethodID(classID, "getColumnCount", "()I");
    getColumnType = JdbcModule::env->GetMethodID(classID, "getColumnType", "(I)I");
    getColumnName = JdbcModule::env->GetMethodID(classID, "getColumnName", "(I)Ljava/lang/String;");
    getColumnTypeName = JdbcModule::env->GetMethodID(classID, "getColumnTypeName", "(I)Ljava/lang/String;");
    return true;
  }
  bool JavaPreparedStatement::init() {
    classID = JdbcModule::env->FindClass("java/sql/PreparedStatement");
    clearParameters = JdbcModule::env->GetMethodID(classID, "clearParameters", "()V");
    execute = JdbcModule::env->GetMethodID(classID, "execute", "()Z");
    executeQuery = JdbcModule::env->GetMethodID(classID, "executeQuery", "()Ljava/sql/ResultSet;");
    executeUpdate = JdbcModule::env->GetMethodID(classID, "executeUpdate", "()I");
    getParameterMetaData = JdbcModule::env->GetMethodID(classID, "getParameterMetaData", "()Ljava/sql/ParameterMetaData;");
    getUpdateCount = JdbcModule::env->GetMethodID(classID, "getUpdateCount", "()I");
    getResultSet = JdbcModule::env->GetMethodID(classID, "getResultSet", "()Ljava/sql/ResultSet;");
    setBoolean = JdbcModule::env->GetMethodID(classID, "setBoolean", "(IZ)V");
    setNull = JdbcModule::env->GetMethodID(classID, "setNull", "(II)V");
    setDouble = JdbcModule::env->GetMethodID(classID, "setDouble", "(ID)V");
    setFloat = JdbcModule::env->GetMethodID(classID, "setFloat", "(IF)V");
    setLong = JdbcModule::env->GetMethodID(classID, "setLong", "(IJ)V");
    setString = JdbcModule::env->GetMethodID(classID, "setString", "(ILjava/lang/String;)V");
    return true;
  }
  bool JavaParameterMetadata::init() {
    classID = JdbcModule::env->FindClass("java/sql/ParameterMetaData");
    getParameterCount = JdbcModule::env->GetMethodID(classID, "getParameterCount", "()I");
    getParameterTypeName = JdbcModule::env->GetMethodID(classID, "getParameterTypeName",  "(I)Ljava/lang/String;");
    getParameterClassName = JdbcModule::env->GetMethodID(classID, "getParameterClassName", "(I)Ljava/lang/String;");
    getParameterType = JdbcModule::env->GetMethodID(classID, "getParameterType", "(I)I");
    return true;
  }


}}; // namespace zorba, jdbc

