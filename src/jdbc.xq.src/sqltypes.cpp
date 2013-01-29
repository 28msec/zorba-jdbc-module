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
    jclass cTypes = env->FindClass("java/sql/Types");
    ARRAY    = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "ARRAY", "I"));
    BIGINT   = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "BIGINT", "I"));  //***
    BINARY   = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "BINARY", "I"));  //***
    BIT      = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "BIT", "I"));  //***
    BLOB     = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "BLOB", "I"));  ///***
    BOOLEAN  = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "BOOLEAN", "I"));
    CHAR     = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "CHAR", "I"));  //**
    CLOB     = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "CLOB", "I"));  //***
    DATALINK = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "DATALINK", "I"));
    DATE     = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "DATE", "I"));  //----
    DECIMAL  = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "DECIMAL", "I"));  //***
    DOUBLE   = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "DOUBLE", "I"));   ///****
    FLOAT    = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "FLOAT", "I"));   ///***
    INTEGER  = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "INTEGER", "I"));  //***
    JAVA_OBJECT   = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "JAVA_OBJECT", "I"));
    LONGVARCHAR   = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "LONGVARCHAR", "I"));  //**
    LONGNVARCHAR  = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "LONGNVARCHAR", "I"));  //**
    LONGVARBINARY = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "LONGVARBINARY", "I"));  //***
    NCHAR     = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "NCHAR", "I"));  //***
    NCLOB     = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "NCLOB", "I"));  //**
    _NULL     = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "NULL", "I"));   //***
    NUMERIC   = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "NUMERIC", "I"));  //**
    NVARCHAR  = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "NVARCHAR", "I"));  //***
    OTHER     = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "OTHER", "I"));
    REAL      = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "REAL", "I"));   //***
    REF       = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "REF", "I"));
    ROWID     = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "ROWID", "I"));
    SMALLINT  = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "SMALLINT", "I"));  //***
    SQLXML    = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "SQLXML", "I"));  //--
    TIME      = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "TIME", "I"));  //---
    TIMESTAMP = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "TIMESTAMP", "I"));  //---
    TINYINT   = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "TINYINT", "I")); //***
    VARBINARY = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "VARBINARY", "I"));  //**
    VARCHAR   = env->GetStaticIntField(cTypes, env->GetStaticFieldID(cTypes, "VARCHAR", "I"));  //***
}

bool SQLTypes::isBLOB(long lType) {
  return (
          (lType == BINARY) || (lType == BLOB) || (lType==LONGVARBINARY) || (lType==VARBINARY) ||
          (lType == ARRAY) || (lType == DATALINK) || (lType==JAVA_OBJECT) || (lType==OTHER) ||
          (lType == REF)
         );
}

bool SQLTypes::isString(long lType) {
  return (
          (lType == CHAR) || (lType == CLOB) || (lType == LONGVARCHAR) || (lType==LONGNVARCHAR) || 
          (lType==NCHAR) || (lType==NCLOB) || (lType==NVARCHAR) || (lType==VARCHAR) ||
          (lType==DATE) || (lType==TIME) || (lType==TIMESTAMP) || (lType==SQLXML) 
          );
}

bool SQLTypes::isInt(long lType){
  return ( (lType == INTEGER) || (lType==BIGINT) || (lType==TINYINT) || (lType==SMALLINT) || (lType==BIT) );
}

bool SQLTypes::isFloat(long lType){
  return ((lType == DECIMAL) || (lType==DOUBLE) || (lType==FLOAT) || (lType==NUMERIC) || (lType==REAL) );
}

}}; // namespace zorba, jdbc
