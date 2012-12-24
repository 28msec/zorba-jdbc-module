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

#include "connect.h"
#include "jdbc.h"

namespace zorba
{
namespace jdbc
{


ItemSequence_t
ConnectFunction::evaluate(const ExternalFunction::Arguments_t& args,
                           const zorba::StaticContext* aStaticContext,
                           const zorba::DynamicContext* aDynamincContext) const
{
  JdbcModule::init(aStaticContext);
  Item result;
  JDBC_MODULE_TRY
    jstring url(NULL), username(NULL), password(NULL);
    Item item = JdbcModule::getItemArg(args, 0);
    bool hasUsername=false;
    if (item.isJSONItem()) 
    {
      Iterator_t lKeys = item.getObjectKeys();
      lKeys->open();
      Item lKey;
      while (lKeys->next(lKey))
      {
        zorba::String keystring = lKey.getStringValue();
        zorba::String value = item.getObjectValue(keystring).getStringValue();
        if (keystring=="url") {
          url =  env->NewStringUTF(value.c_str());
          CHECK_EXCEPTION
        } else if (keystring=="user") {
          username =  env->NewStringUTF(value.c_str());
          CHECK_EXCEPTION
          hasUsername = true;
        } else if (keystring=="password") {
          password =  env->NewStringUTF(value.c_str());
          CHECK_EXCEPTION
        } else if (keystring.compare("type")) {
        } else if (keystring.compare("driver")) {
        }
      }
      lKeys->close();
    }

    jobject oConnection;
    if (hasUsername) {
      oConnection = env->CallStaticObjectMethod(jDriverManager.classID, jDriverManager.getConnectionWithUser, url, username, password);
      CHECK_EXCEPTION
    } else {
      oConnection = env->CallStaticObjectMethod(jDriverManager.classID, jDriverManager.getConnection, url);
      CHECK_EXCEPTION
    }

    item = JdbcModule::getItemArg(args, 1);
    if ((!item.isNull()) && (item.isJSONItem()))
    {
      Iterator_t lKeys = item.getObjectKeys();
      lKeys->open();
      Item lKey;
      while (lKeys->next(lKey))
      {
        zorba::String keystring = lKey.getStringValue();
        if (keystring=="autocommit") {
          jboolean value = JNI_FALSE;
          if (item.getObjectValue(keystring).getBooleanValue()) {
            value = JNI_TRUE;
          }
          env->CallVoidMethod(oConnection, jConnection.setAutoCommit, value);
          CHECK_EXCEPTION
        } else if (keystring=="readonly") {
          jboolean value = JNI_FALSE;
          if (item.getObjectValue(keystring).getBooleanValue()) {
            value = JNI_TRUE;
          }
          env->CallVoidMethod(oConnection, jConnection.setReadOnly, value);
          CHECK_EXCEPTION
        } else if (keystring=="isolation-level") {
          jint isolationLevel = (int) item.getObjectValue(keystring).getLongValue();
          env->CallVoidMethod(oConnection, jConnection.setTransactionIsolation, isolationLevel);
          CHECK_EXCEPTION
        }
      }
      lKeys->close();
    }

    InstanceMap* lInstanceMap = JdbcModule::getCreateInstanceMap(aDynamincContext, INSTANCE_MAP_CONNECTIONS);
    String lStrUUID = JdbcModule::getUUID();
    lInstanceMap->storeInstance(lStrUUID, oConnection);

    result = theFactory->createAnyURI(lStrUUID);
  JDBC_MODULE_CATCH
  return ItemSequence_t(new SingletonItemSequence(result));
}



}}; // namespace zorba, jdbc
