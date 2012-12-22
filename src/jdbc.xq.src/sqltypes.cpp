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

#include "sqltypes.h"
#include "jdbc.h"
#include "jni.h"


namespace zorba
{
namespace jdbc
{
long SQLTypes::ARRAY;
long SQLTypes::BIGINT;
long SQLTypes::BINARY;
long SQLTypes::BIT;
long SQLTypes::BLOB;
long SQLTypes::BOOLEAN;
long SQLTypes::CHAR;
long SQLTypes::CLOB;
long SQLTypes::DATALINK;
long SQLTypes::DATE;
long SQLTypes::DECIMAL;
long SQLTypes::DISTINCT;
long SQLTypes::DOUBLE;
long SQLTypes::FLOAT;
long SQLTypes::INTEGER;
long SQLTypes::JAVA_OBJECT;
long SQLTypes::LONGVARCHAR;
long SQLTypes::LONGNVARCHAR;
long SQLTypes::LONGVARBINARY;
long SQLTypes::NCHAR;
long SQLTypes::NCLOB;
long SQLTypes::_NULL;
long SQLTypes::NUMERIC;
long SQLTypes::NVARCHAR;
long SQLTypes::OTHER;
long SQLTypes::REAL;
long SQLTypes::REF;
long SQLTypes::ROWID;
long SQLTypes::SMALLINT;
long SQLTypes::SQLXML;
long SQLTypes::TIME;
long SQLTypes::TIMESTAMP;
long SQLTypes::TINYINT;
long SQLTypes::VARBINARY;
long SQLTypes::VARCHAR;

void SQLTypes::init() {
  static bool TypesLoaded=false;
  if (TypesLoaded) 
    return;
  TypesLoaded = true;
    jclass cTypes = JdbcModule::env->FindClass("java/sql/Types");
    ARRAY    = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "ARRAY", "I"));
    BIGINT   = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "BIGINT", "I"));
    BINARY   = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "BINARY", "I"));
    BIT      = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "BIT", "I"));
    BLOB     = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "BLOB", "I"));
    BOOLEAN  = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "BOOLEAN", "I"));
    CHAR     = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "CHAR", "I"));
    CLOB     = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "CLOB", "I"));
    DATALINK = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "DATALINK", "I"));
    DATE     = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "DATE", "I"));
    DECIMAL  = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "DECIMAL", "I"));
    DOUBLE   = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "DOUBLE", "I"));
    FLOAT    = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "FLOAT", "I"));
    INTEGER  = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "INTEGER", "I"));
    JAVA_OBJECT   = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "JAVA_OBJECT", "I"));
    LONGVARCHAR   = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "LONGVARCHAR", "I"));
    LONGNVARCHAR  = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "LONGNVARCHAR", "I"));
    LONGVARBINARY = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "LONGVARBINARY", "I"));
    NCHAR     = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "NCHAR", "I"));
    NCLOB     = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "NCLOB", "I"));
    _NULL     = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "NULL", "I"));
    NUMERIC   = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "NUMERIC", "I"));
    NVARCHAR  = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "NVARCHAR", "I"));
    OTHER     = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "OTHER", "I"));
    REAL      = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "REAL", "I"));
    REF       = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "REF", "I"));
    ROWID     = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "ROWID", "I"));
    SMALLINT  = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "SMALLINT", "I"));
    SQLXML    = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "SQLXML", "I"));
    TIME      = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "TIME", "I"));
    TIMESTAMP = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "TIMESTAMP", "I"));
    TINYINT   = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "TINYINT", "I"));
    VARBINARY = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "VARBINARY", "I"));
    VARCHAR   = JdbcModule::env->GetStaticIntField(cTypes, JdbcModule::env->GetStaticFieldID(cTypes, "VARCHAR", "I"));
}

bool SQLTypes::isInt(long lType){
  return ( (lType == INTEGER) || (lType==BIGINT) || (lType==TINYINT) || (lType==BIT) );
}

bool SQLTypes::isFloat(long lType){
  return ((lType == DECIMAL) || (lType==DOUBLE) || (lType==FLOAT) || (lType==NUMERIC) || (lType==REAL) );
}


}}; // namespace zorba, jdbc
