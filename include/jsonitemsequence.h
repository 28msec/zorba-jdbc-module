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
#ifndef JSON_ITEM_SEQUENCE
#define JSON_ITEM_SEQUENCE

#include <map>
#include <string>
#include <zorba/zorba.h>
#include "jni.h"

namespace zorba
{
namespace jdbc
{

  class JSONItemSequence : public ItemSequence
  {
    public:
      class JSONIterator : public Iterator
      {
        protected:
          jclass cResultSet;
          jobject oResultSet;
          JNIEnv* env;
          String* columnNames;
          long* columnTypes;
          long columnCount;
          zorba::ItemFactory* itemFactory;
          jobject oMetadata;
          bool itOpen;
          jmethodID mNext;
          jmethodID mGetInt;
          jmethodID mGetDouble;
          jmethodID mGetString;

        public:
          JSONIterator(jobject resultSet, JNIEnv* env):
            itOpen(false), oResultSet(resultSet),env(env),columnNames(NULL),columnCount(0), oMetadata(NULL) {}

          virtual ~JSONIterator() {
          }

          void
          open();

          bool
          next(zorba::Item& aItem);

          void
          close();

          bool
          isOpen() const { return itOpen; }
      };

    protected:
      jobject resultSet;
      JNIEnv* env;

    public:
      JSONItemSequence(jobject resultSet, JNIEnv* env)
        : resultSet(resultSet), env(env)
      {}

      virtual ~JSONItemSequence() {}

      zorba::Iterator_t 
        getIterator() { return new JSONIterator(resultSet, env); }
  };

}}; // namespace zorba, jdbc

#endif