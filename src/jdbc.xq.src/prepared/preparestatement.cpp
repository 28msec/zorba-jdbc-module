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

#include "preparestatement.h"
#include "jdbc.h"
#include <zorba/singleton_item_sequence.h>

namespace zorba
{
namespace jdbc
{


ItemSequence_t
PrepareStatementFunction::evaluate(const ExternalFunction::Arguments_t& args,
                           const zorba::StaticContext* aStaticContext,
                           const zorba::DynamicContext* aDynamincContext) const
{
  Item result;
  
  JDBC_MODULE_TRY
    String lConnectionUUID = JdbcModule::getStringArg(args, 0);
    String lQuery = JdbcModule::getStringArg(args, 1);

    jobject oConnection = JdbcModule::getObject(aDynamincContext, lConnectionUUID, INSTANCE_MAP_CONNECTIONS);

    jstring query =  env->NewStringUTF(lQuery.c_str());
    jobject oPrepared = env->CallObjectMethod(oConnection, jConnection.prepareStatement, query);
    CHECK_EXCEPTION

    InstanceMap* lInstanceMap = JdbcModule::getCreateInstanceMap(aDynamincContext, INSTANCE_MAP_PREPAREDSTATEMENTS);
    String resultUUID = JdbcModule::getUUID();
    lInstanceMap->storeInstance(resultUUID, oPrepared);

    result = theFactory->createAnyURI(resultUUID);

  JDBC_MODULE_CATCH
  
  return ItemSequence_t(new SingletonItemSequence(result));
}

}}; // namespace zorba, jdbc
