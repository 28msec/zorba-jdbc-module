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

#include "commit.h"
#include "jdbc.h"

namespace zorba
{
namespace jdbc
{


ItemSequence_t
CommitFunction::evaluate(const ExternalFunction::Arguments_t& args,
                           const zorba::StaticContext* aStaticContext,
                           const zorba::DynamicContext* aDynamincContext) const
{
  JNIEnv *env = JdbcModule::getJavaEnv(aStaticContext);

  JDBC_MODULE_TRY
    String lStrUUID = JdbcModule::getStringArg(args, 0);

    InstanceMap* lInstanceMap = JdbcModule::getInstanceMap(aDynamincContext, INSTANCE_MAP_CONNECTIONS);
    if (lInstanceMap==NULL)
    {
      JdbcModule::throwError("SQL08003", "Connection does not exist.");
    }
    jobject oConnection = lInstanceMap->getInstance(lStrUUID);
    if(oConnection==NULL)
    {
      JdbcModule::throwError("SQL08003", "Connection does not exist.");
    }
    jclass cConnection = env->FindClass("java/sql/Connection");
    CHECK_EXCEPTION(env);
    env->CallVoidMethod(oConnection, env->GetMethodID(cConnection, "commit", "()V"));
    CHECK_EXCEPTION(env);

  JDBC_MODULE_CATCH
  
	return ItemSequence_t(new EmptySequence());
}

}}; // namespace zorba, jdbc
