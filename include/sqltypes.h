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
#ifndef SQL_TYPES
#define SQL_TYPES

#include <map>
#include <string>
#include <zorba/zorba.h>
#include "jni.h"

namespace zorba
{
namespace jdbc
{

class SQLTypes
{

  public:
    static long ARRAY;
    static long BIGINT;
    static long BINARY;
    static long BIT;
    static long BLOB;
    static long BOOLEAN;
    static long CHAR;
    static long CLOB;
    static long DATALINK;
    static long DATE;
    static long DECIMAL;
    static long DISTINCT;
    static long DOUBLE;
    static long FLOAT;
    static long INTEGER;
    static long JAVA_OBJECT;
    static long LONGVARCHAR;
    static long LONGNVARCHAR;
    static long LONGVARBINARY;
    static long NCHAR;
    static long NCLOB;
    static long _NULL;
    static long NUMERIC;
    static long NVARCHAR;
    static long OTHER;
    static long REAL;
    static long REF;
    static long ROWID;
    static long SMALLINT;
    static long SQLXML;
    static long TIME;
    static long TIMESTAMP;
    static long TINYINT;
    static long VARBINARY;
    static long VARCHAR;
    
    
    static void init();
    static bool isInt(long lType);
    static bool isFloat(long lType);

};


}}; // namespace zorba, jdbc

#endif