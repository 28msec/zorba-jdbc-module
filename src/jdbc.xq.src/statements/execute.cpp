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

#include "execute.h"
#include "jdbc.h"
#include <zorba/singleton_item_sequence.h>

namespace zorba
{
namespace jdbc
{


ItemSequence_t
ExecuteFunction::evaluate(const ExternalFunction::Arguments_t& args,
                           const zorba::StaticContext* aStaticContext,
                           const zorba::DynamicContext* aDynamincContext) const
{
	jthrowable lException = 0;
  JNIEnv *env = JdbcModule::getJavaEnv(aStaticContext);
  Item result;
	try
  {
		// Local variables
    String lConnectionUUID = JdbcModule::getStringArg(args, 0);
    String lQuery = JdbcModule::getStringArg(args, 1);


    InstanceMap* lInstanceMap = JdbcModule::getCreateInstanceMap(aDynamincContext, INSTANCE_MAP_CONNECTIONS);
    if (lInstanceMap==NULL)
    {
      JdbcModule::throwError("SQL08003", "Connection does not exist.");
    }
    jobject oConnection = lInstanceMap->getInstance(lConnectionUUID);
    if(oConnection==NULL)
    {
      JdbcModule::throwError("SQL08003", "Connection does not exist.");
    }

    jclass cConnection = env->FindClass("java/sql/Connection");
    CHECK_EXCEPTION(env);
    jobject oStatement = env->CallObjectMethod(oConnection, env->GetMethodID(cConnection, "createStatement", "()Ljava/sql/Statement;"));
    CHECK_EXCEPTION(env);
    jclass cStatement = env->FindClass("java/sql/Statement");
    CHECK_EXCEPTION(env);
    env->CallBooleanMethod(oStatement, env->GetMethodID(cStatement, "execute", "(Ljava/lang/String;)Z"), lQuery);
    CHECK_EXCEPTION(env);

    lInstanceMap = JdbcModule::getCreateInstanceMap(aDynamincContext, INSTANCE_MAP_STATEMENTS);
    String resultUUID = JdbcModule::getUUID();
    lInstanceMap->storeInstance(resultUUID, oStatement);

    result = theFactory->createAnyURI(resultUUID);

	}
  catch (zorba::jvm::VMOpenException&)
	{
    JdbcModule::throwError("VM001", "Could not start the Java VM (is the classpath set?).");
	}
	catch (JavaException&)
	{
    JdbcModule::throwJavaException(env, lException);
	}
  
  return ItemSequence_t(new SingletonItemSequence(result));
}

}}; // namespace zorba, jdbc
