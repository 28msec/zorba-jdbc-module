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
#include "instancemap.h"

namespace zorba
{
namespace jdbc
{


ItemSequence_t
ConnectFunction::evaluate(const ExternalFunction::Arguments_t& args,
                           const zorba::StaticContext* aStaticContext,
                           const zorba::DynamicContext* aDynamincContext) const
{
	jthrowable lException = 0;
  JNIEnv *env = JdbcModule::getJavaEnv(aStaticContext);
  jclass cDriverManager;
  jstring url, username, password;
  uuid lUUID;
  String lStrUUID;
  
  try
  {
		// read input param 0
    Iterator_t lIter = args[0]->getIterator();
		lIter->open();
		Item item;
		std::vector<jstring> params;
    bool hasUsername=false;
		if (lIter->next(item))
		{
      if (item.isJSONItem()) 
      {
        Iterator_t lKeys = item.getObjectKeys();
        
        lKeys->open();
        Item lKey;
        while (lKeys->next(lKey))
        {
          zorba::String keystring = lKey.getStringValue();
          //std::cout << "Key: '" << keystring << "'" << std::endl; std::cout.flush();
          if (keystring=="url") {
            url =  env->NewStringUTF(item.getObjectValue(keystring).getStringValue().c_str());
          } else if (keystring=="user") {
            username =  env->NewStringUTF(item.getObjectValue(keystring).getStringValue().c_str());
            hasUsername = true;
          } else if (keystring=="password") {
            password =  env->NewStringUTF(item.getObjectValue(keystring).getStringValue().c_str());
          } else if (keystring.compare("type")) {
          } else if (keystring.compare("driver")) {
          }
        }
        lKeys->close();
      }
		}

		lIter->close();
    jmethodID mConnection;
    jobject oConnection;
    cDriverManager = env->FindClass("java/sql/DriverManager");
    CHECK_EXCEPTION(env);
    if (hasUsername) {
      mConnection = env->GetStaticMethodID(cDriverManager, "getConnection", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/sql/Connection;");
      CHECK_EXCEPTION(env);
      oConnection = env->CallStaticObjectMethod(cDriverManager, mConnection, url, username, password);
      CHECK_EXCEPTION(env);
    } else {
      mConnection = env->GetStaticMethodID(cDriverManager, "getConnection", "(Ljava/lang/String;)Ljava/sql/Connection;");
      CHECK_EXCEPTION(env);
      oConnection = env->CallStaticObjectMethod(cDriverManager, mConnection, url);
      CHECK_EXCEPTION(env);
    }
  
    uuid::create(&lUUID);
    std::stringstream lStream;
    lStream << lUUID;
    lStrUUID = lStream.str();

    DynamicContext* lDctx = const_cast<DynamicContext*>(aDynamincContext);
  
    InstanceMap* lInstanceMap;
    if (!(lInstanceMap = dynamic_cast<InstanceMap*>(lDctx->getExternalFunctionParameter(JDBC_MODULE_INSTANCE_MAP_CONNECTIONS))))
    {
      lInstanceMap = new InstanceMap();
      lDctx->addExternalFunctionParameter(JDBC_MODULE_INSTANCE_MAP_CONNECTIONS, lInstanceMap);
    }
    lInstanceMap->storeInstance(lStrUUID, oConnection);

	}
  catch (zorba::jvm::VMOpenException&)
	{
    JdbcModule::throwError("VM001", "Could not start the Java VM (is the classpath set?).");
	}
	catch (JavaException&)
	{
    JdbcModule::throwJavaException(env, lException);
	}
  
  return ItemSequence_t(new SingletonItemSequence(theFactory->createAnyURI(lStrUUID)));   
}

}}; // namespace zorba, jdbc
