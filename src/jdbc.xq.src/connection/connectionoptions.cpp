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

#include "connectionoptions.h"
#include "jdbc.h"

namespace zorba
{
namespace jdbc
{


ItemSequence_t
ConnectionOptionsFunction::evaluate(const ExternalFunction::Arguments_t& args,
                           const zorba::StaticContext* aStaticContext,
                           const zorba::DynamicContext* aDynamincContext) const
{
  CHECK_CONNECTION
  Item result;

  JDBC_MODULE_TRY
    String lStrUUID = JdbcModule::getStringArg(args, 0);

    jobject oConnection = JdbcModule::getObject(aDynamincContext, lStrUUID, INSTANCE_MAP_CONNECTIONS);

    std::vector<std::pair<Item, Item> > resultVector;

    jboolean isAutocommit = env->CallBooleanMethod(oConnection, jConnection.getAutoCommit);
    resultVector.push_back(std::pair<Item, Item>(theFactory->createString("autocommit"), theFactory->createBoolean(isAutocommit==JNI_TRUE)));
    CHECK_EXCEPTION
    jboolean isReadonly = env->CallBooleanMethod(oConnection, jConnection.isReadOnly);
    resultVector.push_back(std::pair<Item, Item>(theFactory->createString("readonly"), theFactory->createBoolean(isReadonly==JNI_TRUE)));
    CHECK_EXCEPTION

    int isolationLevel = env->CallIntMethod(oConnection, jConnection.getTransactionIsolation);
    CHECK_EXCEPTION
    String isolLevel;
    if (isolationLevel==jConnection.TRANSACTION_NONE) {
      isolLevel = "NOT-SUPPORTED";
    } else if (isolationLevel==jConnection.TRANSACTION_READ_UNCOMMITTED) {
      isolLevel = "READ-UNCOMMITTED";
    } else if (isolationLevel==jConnection.TRANSACTION_READ_COMMITTED) {
      isolLevel = "READ-COMMITTED";
    } else if (isolationLevel==jConnection.TRANSACTION_REPEATABLE_READ) {
      isolLevel = "REPEATABLE-READ";
    } else if (isolationLevel==jConnection.TRANSACTION_SERIALIZABLE) {
      isolLevel = "SERIALIZABLE";
    }
    resultVector.push_back(std::pair<Item, Item>(theFactory->createString("isolation-level"), theFactory->createString(isolLevel)));
    result = theFactory->createJSONObject(resultVector);

  JDBC_MODULE_CATCH
  
  return ItemSequence_t(new SingletonItemSequence(result));
}

}}; // namespace zorba, jdbc
