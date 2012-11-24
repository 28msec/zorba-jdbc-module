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

#include <zorba/zorba.h>
#include <zorba/external_module.h>
#include <zorba/util/uuid.h>
#include "instancemap.h"

#include "JavaVMSingleton.h"

#define JDBC_MODULE_NAMESPACE "http://www.zorba-xquery.com/modules/jdbc"

#define INSTANCE_MAP_CONNECTIONS "JdbcInstanceMapConnections"
#define INSTANCE_MAP_STATEMENTS "JdbcInstanceMapStatements"

class JavaException {};

#define CHECK_EXCEPTION(env)  if ((lException = env->ExceptionOccurred())) throw JavaException()


namespace zorba
{
namespace jdbc
{

class JdbcModule : public ExternalModule {
  protected:
    class ltstr
    {
    public:
      bool operator()(const String& s1, const String& s2) const
      {
        return s1.compare(s2) < 0;
      }
    };

    typedef std::map<String, ExternalFunction*, ltstr> FuncMap_t;

    FuncMap_t theFunctions;

	public:
    JdbcModule() 
		{
    }

		~JdbcModule()
		{}

		virtual String getURI() const
		{ return JDBC_MODULE_NAMESPACE; }

    virtual ExternalFunction* getExternalFunction(const String& localName);

		virtual void destroy()
		{
      delete this;
    }

    static JNIEnv* getJavaEnv(const zorba::StaticContext* aStaticContext);

    static String 
      getStringArg(const ExternalFunction::Arguments_t& args, int index);

    static Item 
      getItemArg(const ExternalFunction::Arguments_t& args, int index);

    static void 
      throwJavaException(JNIEnv *env, jthrowable& lException);
    static void
      throwError (const char *aLocalName, const char* aErrorMessage);
    static void
      throwError(const char *aLocalName, String aErrorMessage);

    static String
      getUUID();

    static InstanceMap* 
      getCreateInstanceMap(const zorba::DynamicContext* aDynamincContext, String mapName);
    static InstanceMap* 
      getInstanceMap(const zorba::DynamicContext* aDynamincContext, String mapName);

};


}}; // namespace zorba, jdbc

