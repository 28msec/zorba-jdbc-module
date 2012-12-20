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
#include "affectedrows.h"
#include "jdbc.h"
#include <zorba/singleton_item_sequence.h>

namespace zorba
{
namespace jdbc
{


ItemSequence_t
AffectedRowsFunction::evaluate(const ExternalFunction::Arguments_t& args,
                           const zorba::StaticContext* aStaticContext,
                           const zorba::DynamicContext* aDynamincContext) const
{
  JNIEnv *env = JdbcModule::getJavaEnv(aStaticContext);
  Item result;

  JDBC_MODULE_TRY
    String lStatementUUID = JdbcModule::getStringArg(args, 0);

    zorba::ItemFactory* itemFactory = Zorba::getInstance(0)->getItemFactory();

    jobject oStatement = JdbcModule::getObject(aDynamincContext, lStatementUUID, INSTANCE_MAP_STATEMENTS);

    if (cStatement==NULL) {
      cStatement = JdbcModule::getJavaClass(JC_STATEMENT, env);
    }
    int rowsAffected = env->CallIntMethod(oStatement, getUpdateCountMethod(env));
    CHECK_EXCEPTION(env);

    if (rowsAffected==-1) { // NON UPDATE QUERY
      jobject oResultSet = env->CallObjectMethod(oStatement, getResultSetMethod(env));
      CHECK_EXCEPTION(env);

      if (cResultSet==NULL) {
        cResultSet = JdbcModule::getJavaClass(JC_STATEMENT, env);
      }
      jboolean hasRows = env->CallBooleanMethod(oResultSet, getLastMethod(env));
      CHECK_EXCEPTION(env);

      if (hasRows==JNI_TRUE) {
        rowsAffected = env->CallBooleanMethod(oResultSet, getGetRowMethod(env));
        CHECK_EXCEPTION(env);
      } else {
        rowsAffected=0;
      }
    }
    result = itemFactory->createInteger(rowsAffected);
  JDBC_MODULE_CATCH
  
  return ItemSequence_t(new SingletonItemSequence(result));
}

jclass AffectedRowsFunction::cStatement=NULL;
jmethodID AffectedRowsFunction::getUpdateCountMethod(JNIEnv *env) {
  static jmethodID mGetUpdateCount=NULL;
  if (mGetUpdateCount==NULL) {
    mGetUpdateCount = env->GetMethodID(cStatement, "getUpdateCount", "()I");
  }
  return mGetUpdateCount;
}
jmethodID AffectedRowsFunction::getResultSetMethod(JNIEnv *env) {
  static jmethodID mResultSet=NULL;
  if (mResultSet==NULL) {
    mResultSet = env->GetMethodID(cStatement, "getResultSet", "()Ljava/sql/ResultSet;");
  }
  return mResultSet;
}

jclass AffectedRowsFunction::cResultSet=NULL;
jmethodID AffectedRowsFunction::getLastMethod(JNIEnv *env) {
  static jmethodID mLast=NULL;
  if (mLast==NULL) {
    mLast = env->GetMethodID(cResultSet, "last", "()Z");
  }
  return mLast;
}
jmethodID AffectedRowsFunction::getGetRowMethod(JNIEnv *env) {
  static jmethodID mGetRow=NULL;
  if (mGetRow==NULL) {
    mGetRow = env->GetMethodID(cResultSet, "getRow", "()I");
  }
  return mGetRow;
}
}}; // namespace zorba, jdbc
