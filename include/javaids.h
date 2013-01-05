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
#pragma once
#ifndef JAVA_IDS
#define JAVA_IDS

#include "JavaVMSingleton.h"

namespace zorba
{
namespace jdbc
{

class JavaDriverManager {
public:
  bool init();
  jclass classID;
  jmethodID getConnection;
  jmethodID getConnectionWithUser;

};
class JavaConnection {
public:
  bool init();
  jclass classID;
  jmethodID commit;
  jmethodID rollback;
  jmethodID setAutoCommit;
  jmethodID setReadOnly;
  jmethodID setTransactionIsolation;
  jmethodID getAutoCommit;
  jmethodID isReadOnly;
  int TRANSACTION_NONE;
  int TRANSACTION_READ_UNCOMMITTED;
  int TRANSACTION_READ_COMMITTED;
  int TRANSACTION_REPEATABLE_READ;
  int TRANSACTION_SERIALIZABLE;
  jmethodID getTransactionIsolation;
  jmethodID isClosed;
  jmethodID close;
  jmethodID createStatement;
  jmethodID prepareStatement;

};
class JavaStatement {
public:
  bool init();
  jclass classID;
  jmethodID getUpdateCount;
  jmethodID getResultSet;
  jmethodID execute;
  jmethodID executeQuery;
  jmethodID executeUpdate;
};
class JavaResultSet {
public:
  bool init();
  jclass classID;
  jmethodID last;
  jmethodID getRow;
  jmethodID getMetaData;
  jmethodID beforeFirst;
  jmethodID next;
  jmethodID getInt;
  jmethodID getDouble;
  jmethodID getString;
};
class JavaResultSetMetadata {
public:
  bool init();
  jclass classID;
  jmethodID getColumnCount;
  jmethodID getColumnType;
  jmethodID getColumnName;
  jmethodID getColumnTypeName;
  jmethodID isAutoIncrement;
  jmethodID isNullable;

  int COLUMN_NO_NULLS;
  int COLUMN_NULLABLE;
  int COLUMN_NULLABLE_UNKNOWN;
};
class JavaPreparedStatement {
public:
  bool init();
  jclass classID;
  jmethodID clearParameters;
  jmethodID execute;
  jmethodID executeQuery;
  jmethodID executeUpdate;
  jmethodID getParameterMetaData;
  jmethodID getUpdateCount;
  jmethodID getResultSet;
  jmethodID setBoolean;
  jmethodID setNull;
  jmethodID setDouble;
  jmethodID setFloat;
  jmethodID setLong;
  jmethodID setString;
};
class JavaParameterMetadata {
public:
  bool init();
  jclass classID;
  jmethodID getParameterCount;
  jmethodID getParameterTypeName;
  jmethodID getParameterClassName;
  jmethodID getParameterType;
};


}}; // namespace zorba, jdbc

#endif