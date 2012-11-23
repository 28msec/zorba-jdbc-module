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
#include "instancemap.h"

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
  jobject oConnection;
	try
  {
		// Local variables
    String lStrUUID = JdbcModule::getStringArg(args, 0);
    String lQuery = JdbcModule::getStringArg(args, 1);

    DynamicContext* lDctx = const_cast<DynamicContext*>(aDynamincContext);
    InstanceMap* lInstanceMap;
    if (!(lInstanceMap = dynamic_cast<InstanceMap*>(lDctx->getExternalFunctionParameter(JDBC_MODULE_INSTANCE_MAP_CONNECTIONS))))
    {
      JdbcModule::throwError("SQL08003", "Connection does not exist.");
    }
    if(!(oConnection = lInstanceMap->getInstance(lStrUUID)))
    {
      JdbcModule::throwError("SQL08003", "Connection does not exist.");
    }
    jclass cConnection = env->FindClass("java/sql/Connection");
    CHECK_EXCEPTION(env);
    jobject statement = env->CallObjectMethod(oConnection, env->GetMethodID(cConnection, "createStatement", "()Ljava/sql/Statement;"));
    CHECK_EXCEPTION(env);
    jclass cStatement = env->FindClass("java/sql/Statement");
    CHECK_EXCEPTION(env);
    jboolean executed = env->CallBooleanMethod(statement, env->GetMethodID(cStatement, "execute", "(Ljava/lang/String;)Z"), lQuery);
    CHECK_EXCEPTION(env);


	}
  catch (zorba::jvm::VMOpenException&)
	{
    JdbcModule::throwError("VM001", "Could not start the Java VM (is the classpath set?).");
	}
	catch (JavaException&)
	{
    JdbcModule::throwJavaException(env, lException);
	}
  
	return ItemSequence_t(new EmptySequence());
}

}}; // namespace zorba, jdbc
