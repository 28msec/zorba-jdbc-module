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
#include "javaids.h"
#include "JavaVMSingleton.h"
#include "sqltypes.h"

#define JDBC_MODULE_NAMESPACE "http://www.zorba-xquery.com/modules/jdbc"

#define INSTANCE_MAP_CONNECTIONS "JdbcInstanceMapConnections"
#define INSTANCE_MAP_STATEMENTS "JdbcInstanceMapStatements"
#define INSTANCE_MAP_PREPAREDSTATEMENTS "JdbcInstanceMapPreparedStatements"

class JavaException {};


#define LOG_ACTIVE
#ifdef LOG_ACTIVE
#define LOG(message) std::cout << message << std::endl; std::cout.flush();
#else
#define LOG(message) // No logging activated
#endif

namespace zorba
{
namespace jdbc
{

extern JNIEnv* env;
extern JavaDriverManager     jDriverManager;
extern JavaConnection        jConnection;
extern JavaStatement         jStatement;
extern JavaResultSet         jResultSet;
extern JavaResultSetMetadata jResultSetMetadata;
extern JavaDatabaseMetadata  jDatabaseMetadata;
extern JavaPreparedStatement jPreparedStatement;
extern JavaParameterMetadata jParameterMetadata;
extern JavaBlob              jBlob;
extern bool isOutputJSON;

#define CHECK_EXCEPTION  if ((lException = env->ExceptionOccurred())) throw JavaException();
#define JDBC_MODULE_TRY  jthrowable lException = 0;  try   {
#define JDBC_MODULE_CATCH   \
  }  catch (zorba::jvm::VMOpenException&)  { \
    JdbcModule::throwError("VM001", "Could not start the Java VM (is the classpath set?)."); \
  }  catch (JavaException&)  { \
    JdbcModule::throwJavaException(env, lException); \
  }
#define CHECK_CONNECTION \
  if (env==NULL) { \
    JdbcModule::throwError("SQL08003", "There is no connection to any valid source."); \
  }
    



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
    FuncMap_t lFunctions;

  public:
    JdbcModule() 
    {};

    ~JdbcModule()
    {};

    static void initGlobals(const zorba::StaticContext* aStaticContext);

    virtual String getURI() const
    { return JDBC_MODULE_NAMESPACE; }

    virtual ExternalFunction* getExternalFunction(const String& localName);

    virtual void destroy()
    {

      delete this;
    }

    static String 
      getStringArg(const ExternalFunction::Arguments_t& args, int index);

    static bool 
      getOptionalStringArg(const ExternalFunction::Arguments_t& args, int index, String& aRes);

    static Item 
      getItemArg(const ExternalFunction::Arguments_t& args, int index);

    static void 
      throwJavaException(JNIEnv *env, jthrowable& lException);
    static void
      throwMapError(String aMap);
    static void
      throwError (const char *aLocalName, const char* aErrorMessage);
    static void
      throwError(const char *aLocalName, String aErrorMessage);

    static String
      getUUID();

    static InstanceMap* 
      getCreateInstanceMap(const zorba::DynamicContext* aDynamincContext, String mapName);
    static jobject 
      getObject(const zorba::DynamicContext* aDynamincContext, String aObjectUUID, String aMap);
    static void 
      deleteObject(const zorba::DynamicContext* aDynamincContext, String aObjectUUID, String aMap);

};

}}; // namespace zorba, jdbc

