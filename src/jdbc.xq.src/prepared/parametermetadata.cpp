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

#include "parametermetadata.h"
#include "jdbc.h"
#include "instancemap.h"

namespace zorba
{
namespace jdbc
{


ItemSequence_t
ParameterMetadataFunction::evaluate(const ExternalFunction::Arguments_t& args,
                           const zorba::StaticContext* aStaticContext,
                           const zorba::DynamicContext* aDynamincContext) const
{
	jthrowable lException = 0;
	static JNIEnv* env;
  
	try
  {
    env = zorba::jvm::JavaVMSingleton::getInstance(aStaticContext)->getEnv();
    CHECK_EXCEPTION(env);

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
          std::cout << "Key: '" << keystring << "'" << std::endl; std::cout.flush();
          if (keystring=="") {
          }
        }
        lKeys->close();
      }
		}

		lIter->close();
    jclass cDriverManager = env->FindClass("java/sql/DriverManager");
    CHECK_EXCEPTION(env);
    jmethodID mConnection = env->GetStaticMethodID(cDriverManager, "getConnection", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/sql/Connection;");
    CHECK_EXCEPTION(env);
    jobject oConnection = env->CallStaticObjectMethod(cDriverManager, mConnection, "", "", "");
    CHECK_EXCEPTION(env);

    uuid lUUID;
    String lStrUUID;  
    uuid::create(&lUUID);
    std::stringstream lStream;
    lStream << lUUID;
    lStrUUID = lStream.str();

    DynamicContext* lDctx = const_cast<DynamicContext*>(aDynamincContext);
  
    InstanceMap* lInstanceMap;
    if (!(lInstanceMap = dynamic_cast<InstanceMap*>(lDctx->getExternalFunctionParameter(JDBC_MODULE_INSTANCE_MAP))))
    {
      lInstanceMap = new InstanceMap();
      lDctx->addExternalFunctionParameter(JDBC_MODULE_INSTANCE_MAP, lInstanceMap);
    }
    lInstanceMap->storeInstance(lStrUUID, oConnection);

	}
  catch (zorba::jvm::VMOpenException&)
	{
    JdbcModule::throwError("VM001", "Could not start the Java VM (is the classpath set?).");
	}
	catch (JavaException&)
	{
		jclass stringWriterClass = env->FindClass("java/io/StringWriter");
		jclass printWriterClass = env->FindClass("java/io/PrintWriter");
		jclass throwableClass = env->FindClass("java/lang/Throwable");
		jobject stringWriter = env->NewObject(
				stringWriterClass,
				env->GetMethodID(stringWriterClass, "<init>", "()V"));

		jobject printWriter = env->NewObject(
				printWriterClass,
				env->GetMethodID(printWriterClass, "<init>", "(Ljava/io/Writer;)V"),
				stringWriter);

		env->CallObjectMethod(lException,
				env->GetMethodID(throwableClass, "printStackTrace",
						"(Ljava/io/PrintWriter;)V"),
				printWriter);

		//env->CallObjectMethod(printWriter, env->GetMethodID(printWriterClass, "flush", "()V"));
		jmethodID toStringMethod =
			env->GetMethodID(stringWriterClass, "toString", "()Ljava/lang/String;");
		jobject errorMessageObj = env->CallObjectMethod(
				stringWriter, toStringMethod);
		jstring errorMessage = (jstring) errorMessageObj;
		const char *errMsg = env->GetStringUTFChars(errorMessage, 0);
		std::stringstream s;
		s << "A Java Exception was thrown:" << std::endl << errMsg;
		env->ReleaseStringUTFChars(errorMessage, errMsg);
		std::string err("");
		err += s.str();
		env->ExceptionClear();
    JdbcModule::throwError("JAVA-EXCEPTION", err);
	}
  
	return ItemSequence_t(new EmptySequence());
}

}}; // namespace zorba, jdbc
